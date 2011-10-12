#include "icomp/CRegistry.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iser/CMemoryReadArchive.h"

#include "icomp/IComponentStaticInfo.h"
#include "icomp/TAttribute.h"
#include "icomp/TMultiAttribute.h"
#include "icomp/CReferenceAttribute.h"
#include "icomp/CFactoryAttribute.h"
#include "icomp/CInterfaceManipBase.h"
#include "icomp/CMultiReferenceAttribute.h"
#include "icomp/CMultiFactoryAttribute.h"


namespace icomp
{


// public methods

// reimplemented (icomp::IRegistry)

IRegistry::Ids CRegistry::GetElementIds() const
{
	Ids retVal;
	for (		ComponentsMap::const_iterator iter = m_componentsMap.begin();
				iter != m_componentsMap.end();
				++iter){
		retVal.insert(iter->first);
	}

	return retVal;
}


const IRegistry::ElementInfo* CRegistry::GetElementInfo(const std::string& elementId) const
{
	ComponentsMap::const_iterator iter = m_componentsMap.find(elementId);
	if (iter != m_componentsMap.end()){
		return &(iter->second);
	}

	return NULL;
}


IRegistry::ElementInfo* CRegistry::InsertElementInfo(
			const std::string& elementId,
			const CComponentAddress& address,
			bool ensureElementCreated)
{
	ComponentsMap::const_iterator iter = m_componentsMap.find(elementId);
	if (iter != m_componentsMap.end()){
		return NULL;
	}

	istd::TDelPtr<IRegistryElement> registryElementPtr;
	if (ensureElementCreated){
		registryElementPtr.SetPtr(CreateRegistryElement(elementId, address));

		if (!registryElementPtr.IsValid()){
			return NULL;
		}
	}

	istd::TChangeNotifier<IRegistry> changePtr(this, CF_COMPONENT_ADDED);

	ElementInfo& newElement = m_componentsMap[elementId];
	newElement.address = address;
	newElement.elementPtr.TakeOver(registryElementPtr);

	return &newElement;
}


bool CRegistry::RemoveElementInfo(const std::string& elementId)
{
	istd::TChangeNotifier<IRegistry> changePtr(this, CF_COMPONENT_REMOVED | CF_MODEL);

	// remove interfaces exported by this component:
	bool isDone = false;
	while(!isDone){
		isDone = true;      
		for (		ExportedInterfacesMap::iterator iter = m_exportedInterfacesMap.begin();
					iter != m_exportedInterfacesMap.end();){
			if (iter->second == elementId){                         
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
		for (		ExportedComponentsMap::iterator iter = m_exportedComponentsMap.begin();
					iter != m_exportedComponentsMap.end();){
			if (iter->second == elementId){
				m_exportedComponentsMap.erase(iter);
				isDone = false;
				break;
			}

			++iter;
		}
	}

	if (m_componentsMap.erase(elementId) <= 0){
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
		retVal.insert(iter->first);
	}

	return retVal;
}


IRegistry* CRegistry::GetEmbeddedRegistry(const std::string& registryId) const
{
	EmbeddedRegistriesMap::const_iterator iter = m_embeddedRegistriesMap.find(registryId);
	if (iter != m_embeddedRegistriesMap.end()){
		I_ASSERT(iter->second.IsValid());

		return iter->second.GetPtr();
	}

	return NULL;
}


IRegistry* CRegistry::InsertEmbeddedRegistry(const std::string& registryId)
{
	RegistryPtr newRegistryPtr(new CRegistry());

	RegistryPtr& registryPtr = m_embeddedRegistriesMap[registryId];
	if (registryPtr.IsValid()){
		return NULL;	// such ID exists yet!
	}

	registryPtr.TakeOver(newRegistryPtr);

	return registryPtr.GetPtr();
}


bool CRegistry::RemoveEmbeddedRegistry(const std::string& registryId)
{
	if (m_embeddedRegistriesMap.erase(registryId) <= 0){
		return false;
	}

	return true;
}


bool CRegistry::RenameEmbeddedRegistry(const std::string& oldRegistryId, const std::string& newRegistryId)
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
		return NULL;	// such ID exists yet!
	}

	newRegistryPtr.TakeOver(oldIter->second);

	m_embeddedRegistriesMap.erase(oldIter);

	return true;
}


const IRegistry::ExportedInterfacesMap& CRegistry::GetExportedInterfacesMap() const
{
	return m_exportedInterfacesMap;
}


const IRegistry::ExportedComponentsMap& CRegistry::GetExportedComponentsMap() const
{
	return m_exportedComponentsMap;
}


void CRegistry::SetElementInterfaceExported(
			const std::string& elementId,
			const std::string& interfaceName,
			bool state)
{
	I_ASSERT(!elementId.empty());
	I_ASSERT(!interfaceName.empty());

	if (state){
		m_exportedInterfacesMap[interfaceName] = elementId;
	}
	else{
		ExportedInterfacesMap::iterator foundIter = m_exportedInterfacesMap.find(interfaceName);
		if ((foundIter != m_exportedInterfacesMap.end()) && (foundIter->second == elementId)){
			m_exportedInterfacesMap.erase(foundIter);
		}
	}
}


void CRegistry::SetElementExported(
			const std::string& exportId,
			const std::string& elementId)
{
	if (!elementId.empty()){
		m_exportedComponentsMap[exportId] = elementId;
	}
	else{
		m_exportedComponentsMap.erase(exportId);
	}
}


bool CRegistry::RenameElement(const std::string& oldElementId, const std::string& newElementId)
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

	istd::TChangeNotifier<IRegistry> changePtr(this, CF_MODEL | CF_COMPONENT_RENAMED);

	// calculate new component exports:
	IRegistry::ExportedComponentsMap newExportedComponentsMap;
	for (		IRegistry::ExportedComponentsMap::const_iterator index = m_exportedComponentsMap.begin();
				index != m_exportedComponentsMap.end();
				index++){
		std::string exportComponentId;
		std::string subId;
		std::string newExportId = index->second;
		istd::CIdManipBase::SplitId(newExportId, exportComponentId, subId);
		if (exportComponentId == oldElementId){
			newExportId = istd::CIdManipBase::JoinId(newElementId, subId);
		}

		newExportedComponentsMap[index->first] = newExportId;
	}

	// calculate new interface exports:
	IRegistry::ExportedInterfacesMap newExportedInterfacesMap;
	for (		IRegistry::ExportedInterfacesMap::const_iterator index = m_exportedInterfacesMap.begin();
				index != m_exportedInterfacesMap.end();
				index++){
		std::string elementId = index->second;
		if (elementId == oldElementId){
			elementId = newElementId;
		}

		newExportedInterfacesMap[index->first] = elementId;
	}

	// execute rename transaction:
	ComponentsMap::iterator oldElementIter = m_componentsMap.find(oldElementId);

	CComponentAddress oldAdress = oldElementIter->second.address;
	IRegistryElement* oldElementPtr = oldElementIter->second.elementPtr.GetPtr();

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
				const std::string& componentId = *compIdIter;
				const IRegistry::ElementInfo* infoPtr = GetElementInfo(componentId);
				if (infoPtr == NULL){
					continue;
				}
				const IRegistryElement* elementPtr = infoPtr->elementPtr.GetPtr();
				if (elementPtr == NULL){
					continue;
				}

				IRegistryElement::Ids attrIds = elementPtr->GetAttributeIds();

				for (		IRegistryElement::Ids::iterator attrIdIter = attrIds.begin();
							attrIdIter != attrIds.end();
							++attrIdIter){
					const std::string& attributeId = *attrIdIter;
					const IRegistryElement::AttributeInfo* attrInfoPtr = elementPtr->GetAttributeInfo(attributeId);
					if (attrInfoPtr == NULL){
						continue;
					}

					iser::ISerializable* attributePtr = attrInfoPtr->attributePtr.GetPtr();

					CReferenceAttribute* referenceAttrPtr = dynamic_cast<CReferenceAttribute*>(attributePtr);
					if (referenceAttrPtr != NULL){
						std::string baseId;
						std::string subId;
						CInterfaceManipBase::SplitId(referenceAttrPtr->GetValue(), baseId, subId);
						if (baseId == oldElementId){
							referenceAttrPtr->SetValue(CInterfaceManipBase::JoinId(newElementId, subId));
						}
					}

					CMultiReferenceAttribute* multiRefAttrPtr = dynamic_cast<CMultiReferenceAttribute*>(attributePtr);
					if (multiRefAttrPtr != NULL){
						int valuesCount = multiRefAttrPtr->GetValuesCount();
						for (int i = 0; i < valuesCount; ++i){
							std::string baseId;
							std::string subId;
							CInterfaceManipBase::SplitId(multiRefAttrPtr->GetValueAt(i), baseId, subId);
							if (baseId == oldElementId){
								multiRefAttrPtr->SetValueAt(i, CInterfaceManipBase::JoinId(newElementId, subId));
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

	changePtr.Abort();

	return false;
}


const istd::CString& CRegistry::GetDescription() const
{
	return m_description;
}


void CRegistry::SetDescription(const istd::CString& description)
{
	if (description != m_description){
		istd::CChangeNotifier notifier(this);

		m_description = description;
	}
}


const istd::CString& CRegistry::GetKeywords() const
{
	return m_keywords;
}


void CRegistry::SetKeywords(const istd::CString& keywords)
{
	if (keywords != m_keywords){
		istd::CChangeNotifier notifier(this);

		m_keywords = keywords;
	}
}


// reimplemented (iser::ISerializable)

bool CRegistry::Serialize(iser::IArchive& archive)
{
	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();
	I_DWORD frameworkVersion = 0;
	versionInfo.GetVersionNumber(iser::IVersionInfo::AcfVersionId, frameworkVersion);

	istd::CChangeNotifier changePtr(archive.IsStoring()? NULL: this);

	bool retVal = true;

	retVal = retVal && SerializeComponents(archive);
	retVal = retVal && SerializeEmbeddedRegistries(archive);
	retVal = retVal && SerializeExportedInterfaces(archive);
	retVal = retVal && SerializeExportedComponents(archive);

	if (frameworkVersion >= 807){
		static iser::CArchiveTag descriptionTag("Description", "Human readable description");
		retVal = retVal && archive.BeginTag(descriptionTag);
		retVal = retVal && archive.Process(m_description);
		retVal = retVal && archive.EndTag(descriptionTag);

		static iser::CArchiveTag keywordsTag("Keywords", "Human readable keywords");
		retVal = retVal && archive.BeginTag(keywordsTag);
		retVal = retVal && archive.Process(m_keywords);
		retVal = retVal && archive.EndTag(keywordsTag);

		if(frameworkVersion >= 1422 && frameworkVersion < 1431){
			static iser::CArchiveTag categoryTag("Category", "Logical category of the registry");
			retVal = retVal && archive.BeginTag(categoryTag);
			int dummy;
			retVal = retVal && archive.Process(dummy);
			retVal = retVal && archive.EndTag(categoryTag);	
		}
	}
	else if (!archive.IsStoring()){
		m_description = "";
		m_keywords = "";
	}

	return retVal;
}


I_DWORD CRegistry::GetMinimalVersion(int versionId) const
{
	if (versionId == iser::IVersionInfo::AcfVersionId){
		if (!m_embeddedRegistriesMap.empty()){
			return 1637;
		}

		if (!m_description.IsEmpty() || !m_keywords.IsEmpty()){
			return 807;
		}
	}

	return 0;
}


// protected methods

IRegistryElement* CRegistry::CreateRegistryElement(
			const std::string& /*elementId*/,
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
	static iser::CArchiveTag elementsListTag("ElementsList", "List of registry elements");
	static iser::CArchiveTag elementTag("Element", "Description of single element");
	static iser::CArchiveTag elementIdTag("Id", "ID of element in registry");
	static iser::CArchiveTag dataTag("Data", "Data of single element", true);
	static iser::CArchiveTag isEnabledTag("IsEnabled", "It is true if this element is valid");

	bool isStoring = archive.IsStoring();

	bool retVal = true;

	int count = int(m_componentsMap.size());

	retVal = retVal && archive.BeginMultiTag(elementsListTag, elementTag, count);

	if (isStoring){
		for (		ComponentsMap::iterator iter = m_componentsMap.begin();
					iter != m_componentsMap.end();
					++iter){
			ElementInfo& element = iter->second;

			retVal = retVal && archive.BeginTag(elementTag);

			retVal = retVal && archive.BeginTag(elementIdTag);
			retVal = retVal && archive.Process(const_cast< std::string&>(iter->first));
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

			std::string elementId;
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
	static iser::CArchiveTag registriesListTag("EmbeddedRegistriesList", "List of embedded registries");
	static iser::CArchiveTag registryTag("EmbeddedRegistry", "Description of single embedded registry");
	static iser::CArchiveTag registryIdTag("Id", "ID of embedded registry");
	static iser::CArchiveTag dataTag("Data", "Data of single embedded registry");

	bool isStoring = archive.IsStoring();

	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();
	I_DWORD frameworkVersion = 0;
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
			RegistryPtr& registryPtr = iter->second;
			I_ASSERT(registryPtr.IsValid());

			retVal = retVal && archive.BeginTag(registryTag);

			retVal = retVal && archive.BeginTag(registryIdTag);
			retVal = retVal && archive.Process(const_cast< std::string&>(iter->first));
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

			std::string elementId;
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
	static iser::CArchiveTag exportedInterfacesTag("ExportedInterfaces", "List of exported interfaces");
	static iser::CArchiveTag interfaceTag("Interface", "Single exported interface");
	static iser::CArchiveTag interfaceIdTag("InterfaceId", "ID of exported interface");
	static iser::CArchiveTag componentIdTag("ComponentId", "ID of component in registry implementing exported interface");

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
			std::string interfaceName = iter->first;
			retVal = retVal && archive.Process(interfaceName);
			retVal = retVal && archive.EndTag(interfaceIdTag);

			retVal = retVal && archive.BeginTag(componentIdTag);
			retVal = retVal && archive.Process(iter->second);
			retVal = retVal && archive.EndTag(componentIdTag);

			retVal = retVal && archive.EndTag(interfaceTag);
		}
	}
	else{
		m_exportedInterfacesMap.clear();

		for (int i = 0; i < count; ++i){
			retVal = retVal && archive.BeginTag(interfaceTag);

			std::string interfaceName;
			retVal = retVal && archive.BeginTag(interfaceIdTag);
			retVal = retVal && archive.Process(interfaceName);
			retVal = retVal && archive.EndTag(interfaceIdTag);

			std::string componentId;
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
	static iser::CArchiveTag exportedComponentsTag("ExportedComponents", "List of exported components");
	static iser::CArchiveTag componentTag("Component", "Exported component info");
	static iser::CArchiveTag exportedIdTag("ExportedId", "Exported component ID");
	static iser::CArchiveTag componentIdTag("ComponentId", "ID of component in registry");

	bool isStoring = archive.IsStoring();

	bool retVal = true;

	int count = int(m_exportedComponentsMap.size());

	retVal = retVal && archive.BeginMultiTag(exportedComponentsTag, componentTag, count);

	if (isStoring){
		for (		ExportedComponentsMap::iterator iter = m_exportedComponentsMap.begin();
					iter != m_exportedComponentsMap.end();
					++iter){
			retVal = retVal && archive.BeginTag(componentTag);

			retVal = retVal && archive.BeginTag(exportedIdTag);
			retVal = retVal && archive.Process(const_cast< std::string&>(iter->first));
			retVal = retVal && archive.EndTag(exportedIdTag);

			retVal = retVal && archive.BeginTag(componentIdTag);
			retVal = retVal && archive.Process(iter->second);
			retVal = retVal && archive.EndTag(componentIdTag);

			retVal = retVal && archive.EndTag(componentTag);
		}
	}
	else{
		m_exportedComponentsMap.clear();

		for (int i = 0; i < count; ++i){
			retVal = retVal && archive.BeginTag(componentTag);

			std::string exportedId;
			retVal = retVal && archive.BeginTag(exportedIdTag);
			retVal = retVal && archive.Process(exportedId);
			retVal = retVal && archive.EndTag(exportedIdTag);

			std::string componentId;
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


}//namespace icomp


