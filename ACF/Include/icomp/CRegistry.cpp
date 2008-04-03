#include "icomp/CRegistry.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "icomp/IComponentStaticInfo.h"
#include "icomp/CRegistryElement.h"


namespace icomp
{


CRegistry::CRegistry(const IComponentStaticInfo* factoryPtr)
:	m_componentsFactory(*factoryPtr)
{
	I_ASSERT(factoryPtr != NULL);
}


// reimplemented (icomp::IRegistry)

IRegistry::Ids CRegistry::GetElementIds() const
{
	Ids retVal;
	for (		ComponentsMap::const_iterator iter = m_componentsMap.begin();
				iter != m_componentsMap.begin();
				++iter){
		retVal.insert(iter->first);
	}

	return retVal;
}


IRegistry::ElementInfo* CRegistry::InsertElementInfo(
			const std::string& elementId,
			int elementType,
			const std::string& packageId,
			const std::string& componentId,
			bool ensureElementCreated)
{
	ComponentsMap::const_iterator iter = m_componentsMap.find(elementId);
	if (iter != m_componentsMap.end()){
		return NULL;
	}

	istd::TDelPtr<IRegistryElement> registryPtr;

	switch (elementType){
	case IRegistry::ET_COMPONENT:
		{
			const IComponentStaticInfo::SubcomponentInfos& subInfos = m_componentsFactory.GetSubcomponentInfos();
			const IComponentStaticInfo::SubcomponentInfos::ValueType* packageInfoPtr2 = subInfos.FindElement(packageId);
			if (packageInfoPtr2 != NULL){
				I_ASSERT(*packageInfoPtr2 != NULL);

				const IComponentStaticInfo::SubcomponentInfos& packageSubInfos = (*packageInfoPtr2)->GetSubcomponentInfos();
				const IComponentStaticInfo::SubcomponentInfos::ValueType* componentInfoPtr2 = packageSubInfos.FindElement(componentId);
				if (componentInfoPtr2 != NULL){
					I_ASSERT(*componentInfoPtr2 != NULL);

					registryPtr.SetPtr(new CRegistryElement(*componentInfoPtr2));
				}
			}
		}
		break;

	default:
		I_CRITICAL();		// TODO: implement creation of registry element for compositions.
	}

	if (ensureElementCreated && !registryPtr.IsValid()){
		return NULL;
	}

	ElementInfo& newElement = m_componentsMap[elementId];
	newElement.elementType = elementType;
	newElement.packageId = packageId;
	newElement.componentId = componentId;
	newElement.elementPtr.TakeOver(registryPtr);

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
	return m_componentsMap.erase(elementId) > 0;
}


const IRegistry::ExportedInterfacesMap& CRegistry::GetExportedExportedInterfacesMap() const
{
	return m_exportedInterfacesMap;
}


const IRegistry::ExportedComponentsMap& CRegistry::GetExportedComponentsMap() const
{
	return m_exportedComponentsMap;
}


// reimplemented (iser::ISerializable)

bool CRegistry::Serialize(iser::IArchive& archive)
{
	return		SerializeComponents(archive) &&
				SerializeExportedInterfaces(archive) &&
				SerializeExportedComponents(archive);
}


// protected methods

bool CRegistry::SerializeComponents(iser::IArchive& archive)
{
	static iser::CArchiveTag elementsListTag("ElementsList", "List of registry elements");
	static iser::CArchiveTag elementTag("Element", "Description of single element");
	static iser::CArchiveTag elementIdTag("Id", "ID of element in registry");
	static iser::CArchiveTag elementTypeTag("Type", "Type of element");
	static iser::CArchiveTag packageIdTag("PackageId", "ID of package");
	static iser::CArchiveTag componentIdTag("ComponentId", "ID of factory");
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

			retVal = retVal && archive.BeginTag(elementTypeTag);
			retVal = retVal && archive.Process(element.elementType);
			retVal = retVal && archive.EndTag(elementTypeTag);

			retVal = retVal && archive.BeginTag(packageIdTag);
			retVal = retVal && archive.Process(element.packageId);
			retVal = retVal && archive.EndTag(packageIdTag);

			retVal = retVal && archive.BeginTag(componentIdTag);
			retVal = retVal && archive.Process(element.componentId);
			retVal = retVal && archive.EndTag(componentIdTag);

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
		for (int i = 0; i < count; ++i){
			retVal = retVal && archive.BeginTag(elementTag);

			ElementInfo newInfo;

			std::string elementId;
			retVal = retVal && archive.BeginTag(elementIdTag);
			retVal = retVal && archive.Process(elementId);
			retVal = retVal && archive.EndTag(elementIdTag);

			int elementType = ET_NONE;
			retVal = retVal && archive.BeginTag(elementTypeTag);
			retVal = retVal && archive.Process(elementType);
			retVal = retVal && archive.EndTag(elementTypeTag);

			std::string packageId;
			retVal = retVal && archive.BeginTag(packageIdTag);
			retVal = retVal && archive.Process(packageId);
			retVal = retVal && archive.EndTag(packageIdTag);

			std::string componentId;
			retVal = retVal && archive.BeginTag(componentIdTag);
			retVal = retVal && archive.Process(componentId);
			retVal = retVal && archive.EndTag(componentIdTag);

			retVal = retVal && archive.BeginTag(dataTag);

			if (elementType != ET_NONE){
				ElementInfo* newInfoPtr = InsertElementInfo(elementId, elementType, packageId, componentId, false);
				if (newInfoPtr == NULL){
					return false;
				}

				bool isEnabled = false;
				retVal = retVal && archive.BeginTag(isEnabledTag);
				retVal = retVal && archive.Process(isEnabled);
				retVal = retVal && archive.EndTag(isEnabledTag);

				if (isEnabled && newInfoPtr->elementPtr.IsValid()){
					retVal = retVal && newInfoPtr->elementPtr->Serialize(archive);
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
			retVal = retVal && archive.Process(const_cast< std::string&>(iter->first));
			retVal = retVal && archive.EndTag(interfaceIdTag);

			retVal = retVal && archive.BeginTag(componentIdTag);
			retVal = retVal && archive.Process(iter->second);
			retVal = retVal && archive.EndTag(componentIdTag);

			retVal = retVal && archive.EndTag(interfaceTag);
		}
	}
	else{
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

			m_exportedInterfacesMap[interfaceId] = componentId;

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


