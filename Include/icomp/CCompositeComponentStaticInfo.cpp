#include "icomp/CCompositeComponentStaticInfo.h"


#include "icomp/IRegistry.h"
#include "icomp/IComponentEnvironmentManager.h"
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
		const std::string& interfaceName = interfaceIter->first;

		RegisterInterfaceExtractor(interfaceName, NULL);
	}

	// register exported subcomponents
	const IRegistry::ExportedComponentsMap& exportedComponentsMap = registry.GetExportedComponentsMap();
	for (		IRegistry::ExportedComponentsMap::const_iterator subcomponentIter = exportedComponentsMap.begin();
				subcomponentIter != exportedComponentsMap.end();
				++subcomponentIter){
		const std::string& subcomponentId = subcomponentIter->first;
		const std::string& elementId = subcomponentIter->second;

		std::string baseId;
		std::string subId;
		istd::CIdManipBase::SplitId(elementId, baseId, subId);

		const IRegistry::ElementInfo* elementInfoPtr = GetElementInfoFromRegistry(registry, baseId, manager);
		if (elementInfoPtr == NULL){
			continue;
		}

		const IElementStaticInfo* subMetaInfoPtr = manager.GetComponentMetaInfo(elementInfoPtr->address);
		if (subMetaInfoPtr == NULL){
			continue;
		}

		if (!subId.empty()){
			subMetaInfoPtr = subMetaInfoPtr->GetSubelementInfo(subId);
			if (subMetaInfoPtr == NULL){
				continue;
			}
		}

		RegisterSubelementInfo(subcomponentId, subMetaInfoPtr);
	}

	// register embedded components
	const IRegistry::Ids& embeddedComponentIds = registry.GetEmbeddedRegistryIds();
	for (		IRegistry::Ids::const_iterator embeddedIter = embeddedComponentIds.begin();
				embeddedIter != embeddedComponentIds.end();
				++embeddedIter){
		const std::string& embeddedComponentId = *embeddedIter;

		const IRegistry* embeddedRegistryPtr = registry.GetEmbeddedRegistry(embeddedComponentId);
		if (embeddedRegistryPtr == NULL){
			continue;
		}

		m_embeddedComponentInfos[embeddedComponentId].SetPtr(new icomp::CCompositeComponentStaticInfo(*embeddedRegistryPtr, manager, this));
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

			const IAttributeStaticInfo* attributeInfoPtr = subMetaInfoPtr->GetAttributeInfo(attrId);
			if (attributeInfoPtr != NULL){
				if (attrInfoPtr->attributePtr.IsValid()){
					// attribute was obligatory, but it was defined -> now it is optional
					AttrMetaInfoPtr& replaceAttrPtr = m_attrReplacers[attributeInfoPtr];
					if (!replaceAttrPtr.IsValid()){
						replaceAttrPtr.SetPtr(new AttrAsOptionalDelegator(attributeInfoPtr, attrInfoPtr->attributePtr.GetPtr()));
					}

					RegisterAttributeInfo(attrInfoPtr->exportId, replaceAttrPtr.GetPtr());
				}
				else{
					RegisterAttributeInfo(attrInfoPtr->exportId, attributeInfoPtr);
				}
			}
		}
	}

	m_description = registry.GetDescription();
	m_keywords = registry.GetKeywords();

	if (parentPtr != NULL){
		const QString& parentKeywords = parentPtr->GetKeywords();
		if (!parentKeywords.isEmpty()){
			if (m_keywords.isEmpty()){
				m_keywords = parentKeywords;
			}
			else{
				m_keywords = m_keywords + " " + parentKeywords;
			}
		}
	}
}


// reimplemented (icomp::IRealComponentStaticInfo)

IComponent* CCompositeComponentStaticInfo::CreateComponent() const
{
	return new CCompositeComponent();
}


//	reimplemented (icomp::IComponentStaticInfo)

int CCompositeComponentStaticInfo::GetComponentType() const
{
	return CT_COMPOSITE;
}


const IComponentStaticInfo* CCompositeComponentStaticInfo::GetEmbeddedComponentInfo(const std::string& embeddedId) const
{
	EmbeddedComponentInfos::iterator infoIter = m_embeddedComponentInfos.find(embeddedId);
	if (infoIter != m_embeddedComponentInfos.end()){
		return infoIter->second.GetPtr();
	}

	return NULL;
}


const QString& CCompositeComponentStaticInfo::GetDescription() const
{
	return m_description;
}


const QString& CCompositeComponentStaticInfo::GetKeywords() const
{
	return m_keywords;
}


// protected methods

const IRegistry::ElementInfo* CCompositeComponentStaticInfo::GetElementInfoFromRegistry(
			const IRegistry& registry,
			const std::string& elementId,
			const icomp::IRegistriesManager& manager) const
{
	std::string baseId;
	std::string subId;
	if (istd::CIdManipBase::SplitId(elementId, baseId, subId)){
		const IRegistry::ElementInfo* subElementInfoPtr = registry.GetElementInfo(baseId);
		if (subElementInfoPtr != NULL){
			const icomp::IRegistry* subRegistryPtr = manager.GetRegistry(subElementInfoPtr->address, &registry);
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


std::string CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetAttributeTypeName() const
{
	return m_slave.GetAttributeTypeName();
}


IElementStaticInfo::Ids CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetRelatedMetaIds(int metaGroupId, int flags, int flagsMask) const
{
	return m_slave.GetRelatedMetaIds(metaGroupId, flags, flagsMask);
}


int CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetAttributeFlags() const
{
	return AF_NULLABLE;
}


} // namespace icomp


