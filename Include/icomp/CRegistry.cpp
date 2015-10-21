#include "icomp/CRegistry.h"


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include "istd/CChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iser/CMemoryReadArchive.h"
#include "icomp/IComponentStaticInfo.h"
#include "icomp/CReferenceAttribute.h"
#include "icomp/CFactoryAttribute.h"
#include "icomp/CInterfaceManipBase.h"
#include "icomp/CMultiReferenceAttribute.h"
#include "icomp/CMultiFactoryAttribute.h"


namespace icomp
{


static istd::IChangeable::ChangeSet s_renameElementChangeSet(icomp::IRegistry::CF_ELEMENT_RENAMED, QObject::tr("Rename element"));
static istd::IChangeable::ChangeSet s_addElementChangeSet(icomp::IRegistry::CF_ELEMENT_ADDED, QObject::tr("Add element"));
static istd::IChangeable::ChangeSet s_removeElementChangeSet(icomp::IRegistry::CF_ELEMENT_REMOVED, QObject::tr("Remove element"));

	
	// public methods

// reimplemented (icomp::IRegistry)

IRegistry::Ids CRegistry::GetElementIds() const
{
	Ids retVal;
	for (		ComponentsMap::const_iterator iter = m_componentsMap.begin();
				iter != m_componentsMap.end();
				++iter){
		retVal.insert(iter.key());
	}

	return retVal;
}


const IRegistry::ElementInfo* CRegistry::GetElementInfo(const QByteArray& elementId) const
{
	ComponentsMap::ConstIterator iter = m_componentsMap.constFind(elementId);
	if (iter != m_componentsMap.constEnd()){
		return &(iter.value());
	}

	return NULL;
}


IRegistry::ElementInfo* CRegistry::InsertElementInfo(
			const QByteArray& elementId,
			const CComponentAddress& address,
			bool ensureElementCreated)
{
	if (m_componentsMap.contains(elementId)){
		return NULL;
	}

	istd::TDelPtr<IRegistryElement> registryElementPtr;
	if (ensureElementCreated){
		registryElementPtr.SetPtr(CreateRegistryElement(elementId, address));

		if (!registryElementPtr.IsValid()){
			return NULL;
		}
	}

	istd::CChangeNotifier notifier(this, &s_addElementChangeSet);
	Q_UNUSED(notifier);

	ElementInfo& newElement = m_componentsMap[elementId];
	newElement.address = address;
	newElement.elementPtr.TakeOver(registryElementPtr);

	return &newElement;
}


bool CRegistry::RemoveElementInfo(const QByteArray& elementId)
{
	istd::CChangeNotifier notifier(this, &s_removeElementChangeSet);
	Q_UNUSED(notifier);

	// remove interfaces exported by this component:
	bool isDone = false;
	while(!isDone){
		isDone = true;
		for (		ExportedInterfacesMap::iterator iter = m_exportedInterfacesMap.begin();
					iter != m_exportedInterfacesMap.end();){
			if (iter.value() == elementId){
				m_exportedInterfacesMap.erase(iter);
				isDone = false;
				break;
			}
			iter++;
		}
	}

	isDone = false;
	while(!isDone){
		isDone = true;
		for (		ExportedElementsMap::iterator iter = m_exportedComponentsMap.begin();
					iter != m_exportedComponentsMap.end();){
			if (iter.value() == elementId){
				m_exportedComponentsMap.erase(iter);
				isDone = false;
				break;
			}

			++iter;
		}
	}

	if (m_componentsMap.remove(elementId) <= 0){
		return false;
	}

	return true;
}


IRegistry::Ids CRegistry::GetEmbeddedRegistryIds() const
{
	Ids retVal;
	for (		EmbeddedRegistriesMap::const_iterator iter = m_embeddedRegistriesMap.begin();
				iter != m_embeddedRegistriesMap.end();
				++iter){
		retVal.insert(iter.key());
	}

	return retVal;
}


IRegistry* CRegistry::GetEmbeddedRegistry(const QByteArray& registryId) const
{
	EmbeddedRegistriesMap::ConstIterator iter = m_embeddedRegistriesMap.constFind(registryId);
	if (iter != m_embeddedRegistriesMap.constEnd()){
		Q_ASSERT(iter.value().IsValid());

		return iter.value().GetPtr();
	}

	return NULL;
}


IRegistry* CRegistry::InsertEmbeddedRegistry(const QByteArray& registryId)
{
	RegistryPtr newRegistryPtr(new CRegistry());

	RegistryPtr& registryPtr = m_embeddedRegistriesMap[registryId];
	if (registryPtr.IsValid()){
		return NULL;	// such ID exists yet!
	}

	registryPtr.TakeOver(newRegistryPtr);

	return registryPtr.GetPtr();
}


bool CRegistry::RemoveEmbeddedRegistry(const QByteArray& registryId)
{
	if (m_embeddedRegistriesMap.remove(registryId) <= 0){
		return false;
	}

	return true;
}


bool CRegistry::RenameEmbeddedRegistry(const QByteArray& oldRegistryId, const QByteArray& newRegistryId)
{
	if (oldRegistryId == newRegistryId){
		return true;
	}

	EmbeddedRegistriesMap::iterator oldIter = m_embeddedRegistriesMap.find(oldRegistryId);
	if (oldIter == m_embeddedRegistriesMap.end()){
		return false;
	}

	RegistryPtr& newRegistryPtr = m_embeddedRegistriesMap[newRegistryId];
	if (newRegistryPtr.IsValid()){
		return false;	// such ID exists yet!
	}

	newRegistryPtr.TakeOver(oldIter.value());

	m_embeddedRegistriesMap.erase(oldIter);

	return true;
}


const IRegistry::ExportedInterfacesMap& CRegistry::GetExportedInterfacesMap() const
{
	return m_exportedInterfacesMap;
}


const IRegistry::ExportedElementsMap& CRegistry::GetExportedElementsMap() const
{
	return m_exportedComponentsMap;
}


void CRegistry::SetElementInterfaceExported(
			const QByteArray& elementId,
			const QByteArray& interfaceName,
			bool state)
{
	Q_ASSERT(!elementId.isEmpty());
	Q_ASSERT(!interfaceName.isEmpty());

	if (state){
		m_exportedInterfacesMap[interfaceName] = elementId;
	}
	else{
		ExportedInterfacesMap::iterator foundIter = m_exportedInterfacesMap.find(interfaceName);
		if ((foundIter != m_exportedInterfacesMap.end()) && (foundIter.value() == elementId)){
			m_exportedInterfacesMap.erase(foundIter);
		}
	}
}


void CRegistry::SetElementExported(
			const QByteArray& exportId,
			const QByteArray& elementId)
{
	if (!elementId.isEmpty()){
		m_exportedComponentsMap[exportId] = elementId;
	}
	else{
		m_exportedComponentsMap.remove(exportId);
	}
}


bool CRegistry::RenameElement(const QByteArray& oldElementId, const QByteArray& newElementId)
{
	if (newElementId == oldElementId){
		return true;
	}

	ComponentsMap::iterator elementIter = m_componentsMap.find(oldElementId);
	if (elementIter == m_componentsMap.end()){
		return false;
	}

	// element with this ID already exists:
	elementIter = m_componentsMap.find(newElementId);
	if (elementIter != m_componentsMap.end()){
		return false;
	}

	istd::CChangeNotifier notifier(this, &s_renameElementChangeSet);
	Q_UNUSED(notifier);

	// calculate new component exports:
	IRegistry::ExportedElementsMap newExportedComponentsMap;
	for (		IRegistry::ExportedElementsMap::const_iterator index = m_exportedComponentsMap.begin();
				index != m_exportedComponentsMap.end();
				index++){
		QByteArray exportComponentId;
		QByteArray subId;
		QByteArray newExportId = index.value();
		istd::CIdManipBase::SplitId(newExportId, exportComponentId, subId);
		if (exportComponentId == oldElementId){
			newExportId = istd::CIdManipBase::JoinId(newElementId, subId);
		}

		newExportedComponentsMap[index.key()] = newExportId;
	}

	// calculate new interface exports:
	IRegistry::ExportedInterfacesMap newExportedInterfacesMap;
	for (		IRegistry::ExportedInterfacesMap::const_iterator index = m_exportedInterfacesMap.begin();
				index != m_exportedInterfacesMap.end();
				index++){
		QByteArray elementId = index.value();
		if (elementId == oldElementId){
			elementId = newElementId;
		}

		newExportedInterfacesMap[index.key()] = elementId;
	}

	// execute rename transaction:
	ComponentsMap::iterator oldElementIter = m_componentsMap.find(oldElementId);

	CComponentAddress oldAdress = oldElementIter.value().address;
	IRegistryElement* oldElementPtr = oldElementIter.value().elementPtr.GetPtr();

	if (InsertElementInfo(newElementId, oldAdress)){
		ElementInfo& newElement = m_componentsMap[newElementId];

		if (iser::CMemoryReadArchive::CloneObjectByArchive(*oldElementPtr, *newElement.elementPtr.GetPtr())){
			m_exportedComponentsMap = newExportedComponentsMap;
			m_exportedInterfacesMap = newExportedInterfacesMap;

			// udpate component dependencies:
			Ids elementIds = GetElementIds();
			for (		Ids::iterator compIdIter = elementIds.begin();
						compIdIter != elementIds.end();
						++compIdIter){
				const QByteArray& componentId = *compIdIter;
				const IRegistry::ElementInfo* infoPtr = GetElementInfo(componentId);
				if (infoPtr == NULL){
					continue;
				}
				const IRegistryElement* elementPtr = infoPtr->elementPtr.GetPtr();
				if (elementPtr == NULL){
					continue;
				}

				iattr::IAttributesProvider::AttributeIds attrIds = elementPtr->GetAttributeIds();
				for (		iattr::IAttributesProvider::AttributeIds::ConstIterator attrIdIter = attrIds.constBegin();
							attrIdIter != attrIds.constEnd();
							++attrIdIter){
					const QByteArray& attributeId = *attrIdIter;

					iser::IObject* attributePtr = elementPtr->GetAttribute(attributeId);

					CReferenceAttribute* referenceAttrPtr = dynamic_cast<CReferenceAttribute*>(attributePtr);
					if (referenceAttrPtr != NULL){
						QByteArray baseId;
						QByteArray subId;
						istd::CIdManipBase::SplitId(referenceAttrPtr->GetValue(), baseId, subId);
						if (baseId == oldElementId){
							referenceAttrPtr->SetValue(istd::CIdManipBase::JoinId(newElementId, subId));
						}
					}

					CMultiReferenceAttribute* multiRefAttrPtr = dynamic_cast<CMultiReferenceAttribute*>(attributePtr);
					if (multiRefAttrPtr != NULL){
						int valuesCount = multiRefAttrPtr->GetValuesCount();
						for (int i = 0; i < valuesCount; ++i){
							QByteArray baseId;
							QByteArray subId;
							istd::CIdManipBase::SplitId(multiRefAttrPtr->GetValueAt(i), baseId, subId);
							if (baseId == oldElementId){
								multiRefAttrPtr->SetValueAt(i, istd::CIdManipBase::JoinId(newElementId, subId));
							}
						}
					}
				}
			}

			// remove old element info:
			RemoveElementInfo(oldElementId);

			// trigger update for the element observer:
			istd::CChangeNotifier elementChangePtr(newElement.elementPtr.GetPtr());	
		}

		return true;
	}

	notifier.Abort();

	return false;
}


const QString& CRegistry::GetDescription() const
{
	return m_description;
}


void CRegistry::SetDescription(const QString& description)
{
	if (description != m_description){
		istd::CChangeNotifier notifier(this);

		m_description = description;
	}
}


const QString& CRegistry::GetKeywords() const
{
	return m_keywords;
}


void CRegistry::SetKeywords(const QString& keywords)
{
	if (keywords != m_keywords){
		istd::CChangeNotifier notifier(this);

		m_keywords = keywords;
	}
}


// reimplemented (iser::ISerializable)

bool CRegistry::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag descriptionTag("Description", "Human readable description", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag keywordsTag("Keywords", "Human readable keywords", iser::CArchiveTag::TT_LEAF);

	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();
	quint32 frameworkVersion = 0;
	versionInfo.GetVersionNumber(iser::IVersionInfo::AcfVersionId, frameworkVersion);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && SerializeComponents(archive);
	retVal = retVal && SerializeEmbeddedRegistries(archive);
	retVal = retVal && SerializeExportedInterfaces(archive);
	retVal = retVal && SerializeExportedComponents(archive);

	if (frameworkVersion >= 807){
		retVal = retVal && archive.BeginTag(descriptionTag);
		retVal = retVal && archive.Process(m_description);
		retVal = retVal && archive.EndTag(descriptionTag);

		retVal = retVal && archive.BeginTag(keywordsTag);
		retVal = retVal && archive.Process(m_keywords);
		retVal = retVal && archive.EndTag(keywordsTag);

		// TODO: remove it when back compatibility to specified versions range will be no more important
		if (frameworkVersion >= 1422 && frameworkVersion < 1431){
			static iser::CArchiveTag categoryTag("Category", "Logical category of the registry", iser::CArchiveTag::TT_LEAF);
			retVal = retVal && archive.BeginTag(categoryTag);
			int dummy;
			retVal = retVal && archive.Process(dummy);
			retVal = retVal && archive.EndTag(categoryTag);	
		}
	}
	else if (!archive.IsStoring()){
		m_description.clear();
		m_keywords.clear();
	}

	return retVal;
}


quint32 CRegistry::GetMinimalVersion(int versionId) const
{
	if (versionId == iser::IVersionInfo::AcfVersionId){
		if (!m_embeddedRegistriesMap.isEmpty()){
			return 1637;
		}

		if (!m_description.isEmpty() || !m_keywords.isEmpty()){
			return 807;
		}
	}

	return 0;
}


// protected methods

IRegistryElement* CRegistry::CreateRegistryElement(
			const QByteArray& /*elementId*/,
			const CComponentAddress& /*address*/) const
{
	Element* registryElementPtr = new Element;
	if (registryElementPtr != NULL){
		registryElementPtr->SetSlavePtr(const_cast<CRegistry*>(this));

		return registryElementPtr;
	}

	return NULL;
}


bool CRegistry::SerializeComponents(iser::IArchive& archive)
{
	static iser::CArchiveTag elementsListTag("ElementsList", "List of registry elements", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag elementTag("Element", "Description of single element", iser::CArchiveTag::TT_GROUP, &elementsListTag);
	static iser::CArchiveTag elementIdTag("Id", "ID of element in registry", iser::CArchiveTag::TT_LEAF, &elementTag);
	static iser::CArchiveTag dataTag("Data", "Data of single element", iser::CArchiveTag::TT_GROUP, &elementTag, true);
	static iser::CArchiveTag isEnabledTag("IsEnabled", "It is true if this element is valid", iser::CArchiveTag::TT_LEAF, &dataTag);

	bool isStoring = archive.IsStoring();

	bool retVal = true;

	int count = int(m_componentsMap.size());

	retVal = retVal && archive.BeginMultiTag(elementsListTag, elementTag, count);

	if (isStoring){
		for (		ComponentsMap::iterator iter = m_componentsMap.begin();
					iter != m_componentsMap.end();
					++iter){
			ElementInfo& element = iter.value();

			retVal = retVal && archive.BeginTag(elementTag);

			retVal = retVal && archive.BeginTag(elementIdTag);
			retVal = retVal && archive.Process(const_cast< QByteArray&>(iter.key()));
			retVal = retVal && archive.EndTag(elementIdTag);

			retVal = retVal && element.address.Serialize(archive);

			retVal = retVal && archive.BeginTag(dataTag);

			bool isEnabled = element.elementPtr.IsValid();
			retVal = retVal && archive.BeginTag(isEnabledTag);
			retVal = retVal && archive.Process(isEnabled);
			retVal = retVal && archive.EndTag(isEnabledTag);

			if (isEnabled){
				retVal = retVal && element.elementPtr->Serialize(archive);
			}

			retVal = retVal && archive.EndTag(dataTag);

			retVal = retVal && archive.EndTag(elementTag);
		}
	}
	else{
		m_componentsMap.clear();

		for (int i = 0; i < count; ++i){
			retVal = retVal && archive.BeginTag(elementTag);

			QByteArray elementId;
			retVal = retVal && archive.BeginTag(elementIdTag);
			retVal = retVal && archive.Process(elementId);
			retVal = retVal && archive.EndTag(elementIdTag);

			CComponentAddress address;
			retVal = retVal && address.Serialize(archive);

			retVal = retVal && archive.BeginTag(dataTag);

			if (!retVal){
				return false;
			}

			bool isEnabled = false;
			retVal = retVal && archive.BeginTag(isEnabledTag);
			retVal = retVal && archive.Process(isEnabled);
			retVal = retVal && archive.EndTag(isEnabledTag);

			ElementInfo* newInfoPtr = InsertElementInfo(elementId, address, isEnabled);
			if (newInfoPtr == NULL){
				return false;
			}

			if (isEnabled){
				if (newInfoPtr->elementPtr.IsValid()){
					retVal = retVal && newInfoPtr->elementPtr->Serialize(archive);
				}
				else{
					// TODO: add some error handling for skipped registry elements...
					if (!archive.IsTagSkippingSupported()){	// if element couldn't be created and tag couldn't be skipped, we cannot continue...
						return false;
					}
				}
			}

			retVal = retVal && archive.EndTag(dataTag);

			retVal = retVal && archive.EndTag(elementTag);
		}
	}

	retVal = retVal && archive.EndTag(elementsListTag);

	return retVal;
}


bool CRegistry::SerializeEmbeddedRegistries(iser::IArchive& archive)
{
	static iser::CArchiveTag registriesListTag("EmbeddedRegistriesList", "List of embedded registries", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag registryTag("EmbeddedRegistry", "Description of single embedded registry", iser::CArchiveTag::TT_GROUP, &registriesListTag);
	static iser::CArchiveTag registryIdTag("Id", "ID of embedded registry", iser::CArchiveTag::TT_LEAF, &registryTag);
	static iser::CArchiveTag dataTag("Data", "Data of single embedded registry", iser::CArchiveTag::TT_GROUP, &registryTag);

	bool isStoring = archive.IsStoring();

	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();
	quint32 frameworkVersion = 0;
	versionInfo.GetVersionNumber(iser::IVersionInfo::AcfVersionId, frameworkVersion);
	if (frameworkVersion < 1637){
		if (!isStoring){
			m_embeddedRegistriesMap.clear();
		}

		return true;
	}

	bool retVal = true;

	int count = int(m_embeddedRegistriesMap.size());

	retVal = retVal && archive.BeginMultiTag(registriesListTag, registryTag, count);

	if (isStoring){
		for (		EmbeddedRegistriesMap::iterator iter = m_embeddedRegistriesMap.begin();
					iter != m_embeddedRegistriesMap.end();
					++iter){
			RegistryPtr& registryPtr = iter.value();
			Q_ASSERT(registryPtr.IsValid());

			retVal = retVal && archive.BeginTag(registryTag);

			retVal = retVal && archive.BeginTag(registryIdTag);
			retVal = retVal && archive.Process(const_cast<QByteArray&>(iter.key()));
			retVal = retVal && archive.EndTag(registryIdTag);

			retVal = retVal && archive.BeginTag(dataTag);
			retVal = retVal && registryPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(dataTag);

			retVal = retVal && archive.EndTag(registryTag);
		}
	}
	else{
		m_embeddedRegistriesMap.clear();

		for (int i = 0; i < count; ++i){
			retVal = retVal && archive.BeginTag(registryTag);

			QByteArray elementId;
			retVal = retVal && archive.BeginTag(registryIdTag);
			retVal = retVal && archive.Process(elementId);
			retVal = retVal && archive.EndTag(registryIdTag);

			retVal = retVal && archive.BeginTag(dataTag);

			IRegistry* registryPtr = InsertEmbeddedRegistry(elementId);
			if (registryPtr == NULL){
				return false;
			}
			retVal = retVal && registryPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(dataTag);

			retVal = retVal && archive.EndTag(registryTag);
		}
	}

	retVal = retVal && archive.EndTag(registriesListTag);

	return retVal;
}


bool CRegistry::SerializeExportedInterfaces(iser::IArchive& archive)
{
	static iser::CArchiveTag exportedInterfacesTag("ExportedInterfaces", "List of exported interfaces", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag interfaceTag("Interface", "Single exported interface", iser::CArchiveTag::TT_GROUP, &exportedInterfacesTag);
	static iser::CArchiveTag interfaceIdTag("InterfaceId", "ID of exported interface", iser::CArchiveTag::TT_LEAF, &interfaceTag);
	static iser::CArchiveTag componentIdTag("ComponentId", "ID of component in registry implementing exported interface", iser::CArchiveTag::TT_LEAF, &interfaceTag);

	bool isStoring = archive.IsStoring();

	bool retVal = true;

	int count = int(m_exportedInterfacesMap.size());

	retVal = retVal && archive.BeginMultiTag(exportedInterfacesTag, interfaceTag, count);

	if (isStoring){
		for (		ExportedInterfacesMap::iterator iter = m_exportedInterfacesMap.begin();
					iter != m_exportedInterfacesMap.end();
					++iter){
			retVal = retVal && archive.BeginTag(interfaceTag);

			retVal = retVal && archive.BeginTag(interfaceIdTag);
			QByteArray interfaceName = iter.key();
			retVal = retVal && archive.Process(interfaceName);
			retVal = retVal && archive.EndTag(interfaceIdTag);

			retVal = retVal && archive.BeginTag(componentIdTag);
			retVal = retVal && archive.Process(iter.value());
			retVal = retVal && archive.EndTag(componentIdTag);

			retVal = retVal && archive.EndTag(interfaceTag);
		}
	}
	else{
		m_exportedInterfacesMap.clear();

		for (int i = 0; i < count; ++i){
			retVal = retVal && archive.BeginTag(interfaceTag);

			QByteArray interfaceName;
			retVal = retVal && archive.BeginTag(interfaceIdTag);
			retVal = retVal && archive.Process(interfaceName);
			retVal = retVal && archive.EndTag(interfaceIdTag);

			QByteArray componentId;
			retVal = retVal && archive.BeginTag(componentIdTag);
			retVal = retVal && archive.Process(componentId);
			retVal = retVal && archive.EndTag(componentIdTag);

			m_exportedInterfacesMap[interfaceName] = componentId;

			retVal = retVal && archive.EndTag(interfaceTag);
		}
	}

	retVal = retVal && archive.EndTag(exportedInterfacesTag);

	return retVal;
}


bool CRegistry::SerializeExportedComponents(iser::IArchive& archive)
{
	static iser::CArchiveTag exportedComponentsTag("ExportedComponents", "List of exported components", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag componentTag("Component", "Exported component info", iser::CArchiveTag::TT_GROUP, &exportedComponentsTag);
	static iser::CArchiveTag exportedIdTag("ExportedId", "Exported component ID", iser::CArchiveTag::TT_LEAF, &componentTag);
	static iser::CArchiveTag componentIdTag("ComponentId", "ID of component in registry", iser::CArchiveTag::TT_LEAF, &componentTag);

	bool isStoring = archive.IsStoring();

	bool retVal = true;

	int count = int(m_exportedComponentsMap.size());

	retVal = retVal && archive.BeginMultiTag(exportedComponentsTag, componentTag, count);

	if (isStoring){
		for (		ExportedElementsMap::iterator iter = m_exportedComponentsMap.begin();
					iter != m_exportedComponentsMap.end();
					++iter){
			retVal = retVal && archive.BeginTag(componentTag);

			retVal = retVal && archive.BeginTag(exportedIdTag);
			retVal = retVal && archive.Process(const_cast< QByteArray&>(iter.key()));
			retVal = retVal && archive.EndTag(exportedIdTag);

			retVal = retVal && archive.BeginTag(componentIdTag);
			retVal = retVal && archive.Process(iter.value());
			retVal = retVal && archive.EndTag(componentIdTag);

			retVal = retVal && archive.EndTag(componentTag);
		}
	}
	else{
		m_exportedComponentsMap.clear();

		for (int i = 0; i < count; ++i){
			retVal = retVal && archive.BeginTag(componentTag);

			QByteArray exportedId;
			retVal = retVal && archive.BeginTag(exportedIdTag);
			retVal = retVal && archive.Process(exportedId);
			retVal = retVal && archive.EndTag(exportedIdTag);

			QByteArray componentId;
			retVal = retVal && archive.BeginTag(componentIdTag);
			retVal = retVal && archive.Process(componentId);
			retVal = retVal && archive.EndTag(componentIdTag);

			m_exportedComponentsMap[exportedId] = componentId;

			retVal = retVal && archive.EndTag(componentTag);
		}
	}

	retVal = retVal && archive.EndTag(exportedComponentsTag);

	return retVal;
}


} // namespace icomp


