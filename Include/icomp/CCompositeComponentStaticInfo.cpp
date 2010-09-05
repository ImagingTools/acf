#include "icomp/CCompositeComponentStaticInfo.h"


#include "icomp/IRegistry.h"
#include "icomp/TComponentWrap.h"
#include "icomp/CCompositeComponent.h"


namespace icomp
{


CCompositeComponentStaticInfo::CCompositeComponentStaticInfo(
			const IRegistry& registry,
			const icomp::IComponentEnvironmentManager& manager,
			const IComponentStaticInfo* parentPtr)
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

		const IRegistry::ElementInfo* elementInfoPtr = GetElementInfoFromRegistry(registry, elementId, manager);
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
				if (attrInfoPtr->attributePtr.IsValid()){
					// attribute was obligatory, but it was defined -> now it is optional
					AttrMetaInfoPtr& replaceAttrPtr = m_attrReplacers[*attrStaticInfoPtr2];
					if (!replaceAttrPtr.IsValid()){
						replaceAttrPtr.SetPtr(new AttrAsOptionalDelegator(*attrStaticInfoPtr2, attrInfoPtr->attributePtr.GetPtr()));
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

	if (parentPtr != NULL){
		const istd::CString& parentKeywords = parentPtr->GetKeywords();
		if (!parentKeywords.IsEmpty()){
			if (m_keywords.IsEmpty()){
				m_keywords = parentKeywords;
			}
			else{
				m_keywords = m_keywords + " " + parentKeywords;
			}
		}
	}
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


// protected methods

const IRegistry::ElementInfo* CCompositeComponentStaticInfo::GetElementInfoFromRegistry(
			const IRegistry& registry,
			const std::string& elementId,
			const icomp::IComponentEnvironmentManager& manager) const
{
	std::string baseId;
	std::string subId;
	if (istd::CIdManipBase::SplitId(elementId, baseId, subId)){
		const IRegistry::ElementInfo* subElementInfoPtr = registry.GetElementInfo(baseId);
		if (subElementInfoPtr != NULL){
			const icomp::IRegistry* subRegistryPtr = manager.GetRegistry(subElementInfoPtr->address);
			if (subRegistryPtr != NULL){
				// get right component path for exported components:
				const IRegistry::ExportedComponentsMap& exportedComponentsMap = subRegistryPtr->GetExportedComponentsMap();
				const IRegistry::ExportedComponentsMap::const_iterator foundComponentExportIter = exportedComponentsMap.find(subId);
				if (foundComponentExportIter != exportedComponentsMap.end()){
					subId = foundComponentExportIter->second;
				}
		
				return GetElementInfoFromRegistry(*subRegistryPtr, subId, manager);
			}
		}
	}
	else{
		return registry.GetElementInfo(elementId);
	}

	return NULL;
}


// public methods of embedded class AttrAsOptionalDelegator

CCompositeComponentStaticInfo::AttrAsOptionalDelegator::AttrAsOptionalDelegator(
			const IAttributeStaticInfo* slavePtr,
			const iser::IObject* defaultValuePtr)
:	m_slave(*slavePtr),
	m_defaultValuePtr(defaultValuePtr)
{
	I_ASSERT(slavePtr != NULL);
	I_ASSERT(defaultValuePtr != NULL);
}


const std::string& CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetAttributeDescription() const
{
	return m_slave.GetAttributeDescription();
}


const iser::IObject* CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetAttributeDefaultValue() const
{
	return m_defaultValuePtr;
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


