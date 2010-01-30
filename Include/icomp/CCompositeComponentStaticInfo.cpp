#include "icomp/CCompositeComponentStaticInfo.h"


#include "icomp/IRegistry.h"
#include "icomp/TComponentWrap.h"
#include "icomp/CCompositeComponent.h"


namespace icomp
{


CCompositeComponentStaticInfo::CCompositeComponentStaticInfo(
			const IRegistry& registry,
			const icomp::IMetaInfoManager& manager)
{
	// register exported interfaces
	const IRegistry::ExportedInterfacesMap& interfacesMap = registry.GetExportedInterfacesMap();
	for (		IRegistry::ExportedInterfacesMap::const_iterator interfaceIter = interfacesMap.begin();
				interfaceIter != interfacesMap.end();
				++interfaceIter){
		const istd::CClassInfo& interfaceId = interfaceIter->first;

		RegisterInterfaceExtractor(interfaceId, NULL);
	}

	// register exported subcomponents
	const IRegistry::ExportedComponentsMap& exportedComponentsMap = registry.GetExportedComponentsMap();
	for (		IRegistry::ExportedComponentsMap::const_iterator subcomponentIter = exportedComponentsMap.begin();
				subcomponentIter != exportedComponentsMap.end();
				++subcomponentIter){
		const std::string& subcomponentId = subcomponentIter->first;
		const std::string& elementId = subcomponentIter->second;
		const IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(elementId);
		if (elementInfoPtr == NULL){
			continue;
		}

		const IComponentStaticInfo* subMetaInfoPtr = manager.GetComponentMetaInfo(elementInfoPtr->address);
		if (subMetaInfoPtr == NULL){
			continue;
		}

		RegisterSubcomponentInfo(subcomponentId, subMetaInfoPtr);
	}

	// register exported attributes
	IRegistry::Ids elementIds = registry.GetElementIds();
	for (		IRegistry::Ids::const_iterator elementIter = elementIds.begin();
				elementIter != elementIds.end();
				++elementIter){
		const std::string& elementId = *elementIter;
		const IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(elementId);
		if (elementInfoPtr == NULL){
			continue;
		}

		const IComponentStaticInfo* subMetaInfoPtr = manager.GetComponentMetaInfo(elementInfoPtr->address);
		if (subMetaInfoPtr == NULL){
			continue;
		}

		const IRegistryElement& element = *elementInfoPtr->elementPtr;
		IRegistryElement::Ids attributeIds = element.GetAttributeIds();

		for (		IRegistryElement::Ids::iterator attrIter = attributeIds.begin();
					attrIter != attributeIds.end();
					++attrIter){
			const std::string& attrId = *attrIter;
			const IRegistryElement::AttributeInfo* attrInfoPtr = element.GetAttributeInfo(attrId);
			if ((attrInfoPtr == NULL) || attrInfoPtr->exportId.empty()){
				continue;
			}

			const IComponentStaticInfo::AttributeInfos& attrInfos = subMetaInfoPtr->GetAttributeInfos();
			const IComponentStaticInfo::AttributeInfos::ValueType* attrStaticInfoPtr2 =
						attrInfos.FindElement(attrId);
			if ((attrStaticInfoPtr2 != NULL) && (*attrStaticInfoPtr2 != NULL)){
				if (attrInfoPtr->attributePtr.IsValid() && (*attrStaticInfoPtr2)->IsObligatory()){
					// attribute was obligatory, but it was defined -> now it is optional
					AttrMetaInfoPtr& replaceAttrPtr = m_attrReplacers[*attrStaticInfoPtr2];
					if (!replaceAttrPtr.IsValid()){
						replaceAttrPtr.SetPtr(new AttrAsOptionalDelegator(*attrStaticInfoPtr2));
					}

					RegisterAttributeInfo(attrInfoPtr->exportId, replaceAttrPtr.GetPtr());
				}
				else{
					RegisterAttributeInfo(attrInfoPtr->exportId, *attrStaticInfoPtr2);
				}
			}
		}
	}

	m_description = registry.GetDescription();
	m_keywords = registry.GetKeywords();
}


//	reimplemented (icomp::IComponentStaticInfo)

int CCompositeComponentStaticInfo::GetComponentType() const
{
	return CT_COMPOSITE;
}


IComponent* CCompositeComponentStaticInfo::CreateComponent() const
{
	return new TComponentWrap<CCompositeComponent>();
}


const istd::CString& CCompositeComponentStaticInfo::GetDescription() const
{
	return m_description;
}


const istd::CString& CCompositeComponentStaticInfo::GetKeywords() const
{
	return m_keywords;
}


// public methods of embedded class AttrAsOptionalDelegator

CCompositeComponentStaticInfo::AttrAsOptionalDelegator::AttrAsOptionalDelegator(const IAttributeStaticInfo* slavePtr)
:	m_slave(*slavePtr)
{
	I_ASSERT(slavePtr != NULL);
}


const std::string& CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetAttributeDescription() const
{
	return m_slave.GetAttributeDescription();
}


const iser::IObject* CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetAttributeDefaultValue() const
{
	return m_slave.GetAttributeDefaultValue();
}


const std::string& CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetAttributeTypeName() const
{
	return m_slave.GetAttributeTypeName();
}


const istd::CClassInfo& CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetRelatedInterfaceType() const
{
	return m_slave.GetRelatedInterfaceType();
}


bool CCompositeComponentStaticInfo::AttrAsOptionalDelegator::IsObligatory() const
{
	return false;
}


}//namespace icomp


