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


IRegistry::ElementInfo* CRegistry::InsertElementInfo(
			const std::string& elementId,
			const icomp::CComponentAddress& address,
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

	istd::TChangeNotifier<icomp::IRegistry> changePtr(this, CF_COMPONENT_ADDED);

	ElementInfo& newElement = m_componentsMap[elementId];
	newElement.address = address;
	newElement.elementPtr.TakeOver(registryElementPtr);

	return &newElement;
}


const IRegistry::ElementInfo* CRegistry::GetElementInfo(const std::string& elementId) const
{
	ComponentsMap::const_iterator iter = m_componentsMap.find(elementId);
	if (iter != m_componentsMap.end()){
		return &(iter->second);
	}

	return NULL;
}


bool CRegistry::RemoveElementInfo(const std::string& elementId)
{
	istd::TChangeNotifier<icomp::IRegistry> changePtr(this, CF_COMPONENT_REMOVED | CF_MODEL);

	// remove interfaces exported by this component:
	for (		ExportedInterfacesMap::iterator iter = m_exportedInterfacesMap.begin();
				iter != m_exportedInterfacesMap.end();){
		if (iter->second == elementId){
			iter = m_exportedInterfacesMap.erase(iter);
		}
		else{
			++iter;
		}
	}

	for (		ExportedComponentsMap::iterator iter = m_exportedComponentsMap.begin();
				iter != m_exportedComponentsMap.end();){
		if (iter->second == elementId){
			iter = m_exportedComponentsMap.erase(iter);
		}
		else{
			++iter;
		}
	}

	if (m_componentsMap.erase(elementId) <= 0){
		return false;
	}

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
			const istd::CClassInfo& exportInterfaceInfo,
			bool state)
{
	I_ASSERT(!elementId.empty());
	I_ASSERT(exportInterfaceInfo.IsValid());

	if (state){
		m_exportedInterfacesMap[exportInterfaceInfo] = elementId;
	}
	else{
		ExportedInterfacesMap::iterator foundIter = m_exportedInterfacesMap.find(exportInterfaceInfo);
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

	istd::TChangeNotifier<icomp::IRegistry> changePtr(this, CF_MODEL | CF_COMPONENT_RENAMED);

	// calculate new component exports:
	icomp::IRegistry::ExportedComponentsMap newExportedComponentsMap;
	for (		icomp::IRegistry::ExportedComponentsMap::const_iterator index = m_exportedComponentsMap.begin();
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
	icomp::IRegistry::ExportedInterfacesMap newExportedInterfacesMap;
	for (		icomp::IRegistry::ExportedInterfacesMap::const_iterator index = m_exportedInterfacesMap.begin();
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

	icomp::CComponentAddress oldAdress = oldElementIter->second.address;
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
				const icomp::IRegistry::ElementInfo* infoPtr = GetElementInfo(componentId);
				if (infoPtr == NULL){
					continue;
				}
				const icomp::IRegistryElement* elementPtr = infoPtr->elementPtr.GetPtr();
				if (elementPtr == NULL){
					continue;
				}

				icomp::IRegistryElement::Ids attrIds = elementPtr->GetAttributeIds();

				for (		icomp::IRegistryElement::Ids::iterator attrIdIter = attrIds.begin();
							attrIdIter != attrIds.end();
							++attrIdIter){
					const std::string& attributeId = *attrIdIter;
					const icomp::IRegistryElement::AttributeInfo* attrInfoPtr = elementPtr->GetAttributeInfo(attributeId);
					if (attrInfoPtr == NULL){
						continue;
					}

					iser::ISerializable* attributePtr = attrInfoPtr->attributePtr.GetPtr();
					icomp::TAttribute<std::string>* singleAttrPtr = dynamic_cast<icomp::TAttribute<std::string>*>(attributePtr);
					icomp::TMultiAttribute<std::string>* multiAttrPtr = dynamic_cast<icomp::TMultiAttribute<std::string>*>(attributePtr);

					if (		(dynamic_cast<icomp::CReferenceAttribute*>(attributePtr) != NULL) ||
								(dynamic_cast<icomp::CFactoryAttribute*>(attributePtr) != NULL)){
						std::string baseId;
						std::string subId;
						icomp::CInterfaceManipBase::SplitId(singleAttrPtr->GetValue(), baseId, subId);
						if (baseId == oldElementId){
							singleAttrPtr->SetValue(icomp::CInterfaceManipBase::JoinId(newElementId, subId));
						}
					}

					if (		(dynamic_cast<icomp::CMultiReferenceAttribute*>(attributePtr) != NULL) ||
								(dynamic_cast<icomp::CMultiFactoryAttribute*>(attributePtr) != NULL)){
						int valuesCount = multiAttrPtr->GetValuesCount();
						for (int i = 0; i < valuesCount; ++i){
							std::string baseId;
							std::string subId;
							icomp::CInterfaceManipBase::SplitId(multiAttrPtr->GetValueAt(i), baseId, subId);
							if (baseId == oldElementId){
								multiAttrPtr->SetValueAt(i, icomp::CInterfaceManipBase::JoinId(newElementId, subId));
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
	istd::CChangeNotifier changePtr(archive.IsStoring()? NULL: this);

	bool retVal =	SerializeComponents(archive) &&
					SerializeExportedInterfaces(archive) &&
					SerializeExportedComponents(archive);

	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();
	I_DWORD frameworkVersion;
	if (!versionInfo.GetVersionNumber(iser::IVersionInfo::FrameworkVersionId, frameworkVersion) || (frameworkVersion >= 807)){
		static iser::CArchiveTag descriptionTag("Description", "Human readable description");
		retVal = retVal && archive.BeginTag(descriptionTag);
		retVal = retVal && archive.Process(m_description);
		retVal = retVal && archive.EndTag(descriptionTag);

		static iser::CArchiveTag keywordsTag("Keywords", "Human readable keywords");
		retVal = retVal && archive.BeginTag(keywordsTag);
		retVal = retVal && archive.Process(m_keywords);
		retVal = retVal && archive.EndTag(keywordsTag);
	}
	else if (!archive.IsStoring()){
		m_description = "";
		m_keywords = "";
	}

	return retVal;
}


I_DWORD CRegistry::GetMinimalVersion(int versionId) const
{
	if (versionId == iser::IVersionInfo::FrameworkVersionId){
		if (!m_description.IsEmpty() || !m_keywords.IsEmpty()){
			return 807;
		}
	}

	return 0;
}


// protected methods

icomp::IRegistryElement* CRegistry::CreateRegistryElement(
			const std::string& /*elementId*/,
			const icomp::CComponentAddress& /*address*/) const
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
			std::string interfaceId = iter->first.GetName();
			retVal = retVal && archive.Process(interfaceId);
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

			std::string interfaceId;
			retVal = retVal && archive.BeginTag(interfaceIdTag);
			retVal = retVal && archive.Process(interfaceId);
			retVal = retVal && archive.EndTag(interfaceIdTag);

			std::string componentId;
			retVal = retVal && archive.BeginTag(componentIdTag);
			retVal = retVal && archive.Process(componentId);
			retVal = retVal && archive.EndTag(componentIdTag);

			istd::CClassInfo interfaceInfo(interfaceId);
			m_exportedInterfacesMap[interfaceInfo] = componentId;

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


