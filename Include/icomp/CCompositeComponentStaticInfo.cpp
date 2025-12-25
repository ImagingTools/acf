#include <icomp/CCompositeComponentStaticInfo.h>


#include <icomp/IRegistry.h>
#include <icomp/IComponentEnvironmentManager.h>
#include <icomp/TComponentWrap.h>
#include <icomp/CCompositeComponent.h>


namespace icomp
{


CCompositeComponentStaticInfo::CCompositeComponentStaticInfo(
			const IRegistry& registry,
			const icomp::IComponentEnvironmentManager& manager,
			const IComponentStaticInfo* parentPtr)
	:m_registry(registry)
{
	// register exported interfaces
	// Make a copy to avoid holding the lock during iteration
	const IRegistry::ExportedInterfacesMap interfacesMap = registry.GetExportedInterfacesMap();
	for (		IRegistry::ExportedInterfacesMap::const_iterator interfaceIter = interfacesMap.begin();
				interfaceIter != interfacesMap.end();
				++interfaceIter){
		const QByteArray& interfaceName = interfaceIter.key();

		RegisterInterfaceExtractor(interfaceName, NULL);
	}

	// register embedded components
	const IRegistry::Ids& embeddedComponentIds = registry.GetEmbeddedRegistryIds();
	for (		IRegistry::Ids::const_iterator embeddedIter = embeddedComponentIds.begin();
				embeddedIter != embeddedComponentIds.end();
				++embeddedIter){
		const QByteArray& embeddedComponentId = *embeddedIter;

		const IRegistry* embeddedRegistryPtr = registry.GetEmbeddedRegistry(embeddedComponentId);
		if (embeddedRegistryPtr == NULL){
			continue;
		}

		m_embeddedComponentInfos[embeddedComponentId].SetPtr(new icomp::CCompositeComponentStaticInfo(*embeddedRegistryPtr, manager, this));
	}

	// register exported subcomponents
	// Make a copy to avoid holding the lock during iteration
	const IRegistry::ExportedElementsMap exportedComponentsMap = registry.GetExportedElementsMap();
	for (		IRegistry::ExportedElementsMap::const_iterator subcomponentIter = exportedComponentsMap.begin();
				subcomponentIter != exportedComponentsMap.end();
				++subcomponentIter){
		const QByteArray& subcomponentId = subcomponentIter.key();
		const QByteArray& elementId = subcomponentIter.value();

		QByteArray baseId;
		QByteArray subId;
		istd::CIdManipBase::SplitId(elementId, baseId, subId);

		const IRegistry::ElementInfo* elementInfoPtr = GetElementInfoFromRegistry(registry, baseId, manager);
		if (elementInfoPtr == NULL){
			continue;
		}

		const IElementStaticInfo* subMetaInfoPtr = NULL;

		const QByteArray& packageId = elementInfoPtr->address.GetPackageId();
		if (!packageId.isEmpty()){
			// get non-embedded component
			subMetaInfoPtr = manager.GetComponentMetaInfo(elementInfoPtr->address);
		}
		else{
			//  get embedded component
			EmbeddedComponentInfos::ConstIterator findEmbeddIter = m_embeddedComponentInfos.constFind(elementInfoPtr->address.GetComponentId());
			if (findEmbeddIter != m_embeddedComponentInfos.constEnd()){
				subMetaInfoPtr = findEmbeddIter.value().GetPtr();
			}
		}

		if (subMetaInfoPtr == NULL){
			continue;
		}

		if (!subId.isEmpty()){
			subMetaInfoPtr = subMetaInfoPtr->GetSubelementInfo(subId);
			if (subMetaInfoPtr == NULL){
				continue;
			}
		}

		RegisterSubelementInfo(subcomponentId, subMetaInfoPtr);
	}

	// register exported attributes
	IRegistry::Ids elementIds = registry.GetElementIds();
	for (		IRegistry::Ids::const_iterator elementIter = elementIds.begin();
				elementIter != elementIds.end();
				++elementIter){
		const QByteArray& elementId = *elementIter;
		const IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(elementId);
		if (elementInfoPtr == NULL){
			continue;
		}

		const IComponentStaticInfo* subMetaInfoPtr = NULL;
		
		QByteArray packageId = elementInfoPtr->address.GetPackageId();
		if (!packageId.isEmpty()){
			subMetaInfoPtr = manager.GetComponentMetaInfo(elementInfoPtr->address);
		}
		else{
			EmbeddedComponentInfos::ConstIterator findEmbeddedIter = m_embeddedComponentInfos.constFind(elementInfoPtr->address.GetComponentId());
			if (findEmbeddedIter != m_embeddedComponentInfos.constEnd()){
				subMetaInfoPtr = findEmbeddedIter.value().GetPtr();
			}
		}
		if (subMetaInfoPtr == NULL){
			continue;
		}

		const IRegistryElement& element = *elementInfoPtr->elementPtr;
		iattr::IAttributesProvider::AttributeIds attributeIds = element.GetAttributeIds();
		for (		iattr::IAttributesProvider::AttributeIds::ConstIterator attrIter = attributeIds.constBegin();
					attrIter != attributeIds.constEnd();
					++attrIter){
			const QByteArray& attrId = *attrIter;
			const IRegistryElement::AttributeInfo* attrInfoPtr = element.GetAttributeInfo(attrId);
			if ((attrInfoPtr == NULL) || attrInfoPtr->exportId.isEmpty()){
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


const IRegistry& CCompositeComponentStaticInfo::GetRegistry() const
{
	return m_registry;
}


// reimplemented (icomp::IRealComponentStaticInfo)

IComponentUniquePtr CCompositeComponentStaticInfo::CreateComponent() const
{
	return IComponentUniquePtr(new CCompositeComponent(true));
}


//	reimplemented (icomp::IComponentStaticInfo)

int CCompositeComponentStaticInfo::GetComponentType() const
{
	return CT_COMPOSITE;
}


const IComponentStaticInfo* CCompositeComponentStaticInfo::GetEmbeddedComponentInfo(const QByteArray& embeddedId) const
{
	EmbeddedComponentInfos::iterator infoIter = m_embeddedComponentInfos.find(embeddedId);
	if (infoIter != m_embeddedComponentInfos.end()){
		return infoIter.value().GetPtr();
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
			const QByteArray& elementId,
			const icomp::IRegistriesManager& manager) const
{
	QByteArray baseId;
	QByteArray subId;
	if (istd::CIdManipBase::SplitId(elementId, baseId, subId)){
		const IRegistry::ElementInfo* subElementInfoPtr = registry.GetElementInfo(baseId);
		if (subElementInfoPtr != NULL){
			const icomp::IRegistry* subRegistryPtr = manager.GetRegistry(subElementInfoPtr->address, &registry);
			if (subRegistryPtr != NULL){
				// get right component path for exported components:
				// Make a copy to avoid holding the lock during iteration
				const IRegistry::ExportedElementsMap exportedComponentsMap = subRegistryPtr->GetExportedElementsMap();
				const IRegistry::ExportedElementsMap::ConstIterator foundComponentExportIter = exportedComponentsMap.constFind(subId);
				if (foundComponentExportIter != exportedComponentsMap.constEnd()){
					subId = foundComponentExportIter.value();
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
	Q_ASSERT(slavePtr != NULL);
	Q_ASSERT(defaultValuePtr != NULL);
}


// reimplemented (icomp::IAttributeStaticInfo)

IElementStaticInfo::Ids CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetRelatedMetaIds(int metaGroupId, int flags, int flagsMask) const
{
	return m_slave.GetRelatedMetaIds(metaGroupId, flags, flagsMask);
}


// reimplemented (iattr::IAttributeMetaInfo)

QString CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetAttributeDescription() const
{
	return m_slave.GetAttributeDescription();
}


const iser::IObject* CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetAttributeDefaultValue() const
{
	return m_defaultValuePtr;
}


QByteArray CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetAttributeTypeId() const
{
	return m_slave.GetAttributeTypeId();
}


int CCompositeComponentStaticInfo::AttrAsOptionalDelegator::GetAttributeFlags() const
{
	return (m_slave.GetAttributeFlags() & ~AF_OBLIGATORY) | AF_NULLABLE;
}


} // namespace icomp


