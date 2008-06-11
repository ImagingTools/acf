#include "icomp/CCompositeComponentStaticInfo.h"


#include "icomp/IRegistry.h"
#include "icomp/TComponentWrap.h"
#include "icomp/CCompositeComponent.h"


namespace icomp
{


CCompositeComponentStaticInfo::CCompositeComponentStaticInfo(const IRegistry& registry)
{
	// register exported interfaces
	const IRegistry::ExportedInterfacesMap& interfacesMap = registry.GetExportedInterfacesMap();
	for (		IRegistry::ExportedInterfacesMap::const_iterator interfaceIter = interfacesMap.begin();
				interfaceIter != interfacesMap.end();
				++interfaceIter){
		const std::string& interfaceId = interfaceIter->first;
		RegisterInterfaceExtractor(interfaceId, NULL);
	}

	// register exported subcomponents
	const IRegistry::ExportedComponentsMap& exportedComponentsMap = registry.GetExportedComponentsMap();
	for (		IRegistry::ExportedComponentsMap::const_iterator subcomponentIter = exportedComponentsMap.begin();
				subcomponentIter != exportedComponentsMap.end();
				++subcomponentIter){
		const std::string& subcomponentId = subcomponentIter->first;
		const std::string& elementId = subcomponentIter->second;
		const IRegistry::ElementInfo* elementInfoPtr =registry.GetElementInfo(elementId);
		if ((elementInfoPtr == NULL) || !elementInfoPtr->elementPtr.IsValid()){
			continue;
		}

		const IRegistryElement& element = *elementInfoPtr->elementPtr;
		const IComponentStaticInfo& subcomponentInfo = element.GetComponentStaticInfo();

		RegisterSubcomponentInfo(subcomponentId, &subcomponentInfo);
	}

	// register exported attributes
	IRegistry::Ids elementIds = registry.GetElementIds();
	for (		IRegistry::Ids::const_iterator elementIter = elementIds.begin();
				elementIter != elementIds.end();
				++elementIter){
		const std::string& elementId = *elementIter;
		const IRegistry::ElementInfo* elementInfoPtr =registry.GetElementInfo(elementId);
		if ((elementInfoPtr == NULL) || !elementInfoPtr->elementPtr.IsValid()){
			continue;
		}

		const IRegistryElement& element = *elementInfoPtr->elementPtr;
		const IComponentStaticInfo& componentInfo = element.GetComponentStaticInfo();

		IRegistryElement::Ids attributeIds = element.GetAttributeIds();
		for (		IRegistryElement::Ids::iterator attrIter = attributeIds.begin();
					attrIter != attributeIds.end();
					++attrIter){
			const std::string& attrId = *attrIter;
			const IRegistryElement::AttributeInfo* attrInfoPtr = element.GetAttributeInfo(attrId);
			if ((attrInfoPtr == NULL) || attrInfoPtr->exportId.empty()){
				continue;
			}

			const IComponentStaticInfo::AttributeInfos& attrInfos = componentInfo.GetAttributeInfos();
			const IComponentStaticInfo::AttributeInfos::ValueType* attrStaticInfoPtr2 =
						attrInfos.FindElement(attrId);
			if ((attrStaticInfoPtr2 != NULL) && (*attrStaticInfoPtr2 != NULL)){
				RegisterAttributeInfo(attrInfoPtr->exportId, *attrStaticInfoPtr2);
			}
		}
	}
}


//	reimplemented (icomp::IComponentStaticInfo)

IComponent* CCompositeComponentStaticInfo::CreateComponent(const IComponentContext* contextPtr) const
{
	TComponentWrap<CCompositeComponent>* componentPtr = new TComponentWrap<CCompositeComponent>(contextPtr);

	return componentPtr;
}


}//namespace icomp


