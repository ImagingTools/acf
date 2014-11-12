#include "icomp/CBaseComponentStaticInfo.h"


// ACF includes
#include "istd/CIdManipBase.h"
#include "icomp/IComponent.h"
#include "icomp/IAttributeStaticInfo.h"


namespace icomp
{


CBaseComponentStaticInfo::CBaseComponentStaticInfo(const IRealComponentStaticInfo* baseComponentPtr)
:	m_baseComponentPtr(baseComponentPtr)
{
}


void CBaseComponentStaticInfo::RegisterInterfaceExtractor(const QByteArray& interfaceName, InterfaceExtractorPtr extractorPtr)
{
	m_interfaceExtractors[interfaceName] = extractorPtr;
}


void CBaseComponentStaticInfo::RegisterAttributeInfo(const QByteArray& attributeId, const IAttributeStaticInfo* attributeInfoPtr)
{
	m_attributeInfos[attributeId] = attributeInfoPtr;
}


void CBaseComponentStaticInfo::RegisterSubelementInfo(const QByteArray& embeddedId, const IElementStaticInfo* staticInfoPtr)
{
	m_subelementInfos[embeddedId] = staticInfoPtr;
}


//	reimplemented (icomp::IRealComponentStaticInfo)

IComponent* CBaseComponentStaticInfo::CreateComponent() const
{
	qFatal("trying to create abstract base component. Check if I_BEGIN_COMPONENT in constructed component wasn't forgotten");

	return NULL;
}


//	reimplemented (icomp::IComponentInterfaceExtractor)

void* CBaseComponentStaticInfo::GetComponentInterface(
			const istd::CClassInfo& interfaceType,
			IComponent& component,
			const QByteArray& subId) const
{
	if (!subId.isEmpty()){
		QByteArray componentId;
		QByteArray restId;
		istd::CIdManipBase::SplitId(subId, componentId, restId);
		const IComponentInterfaceExtractor* subelementInfoPtr = dynamic_cast<const IComponentInterfaceExtractor*>(GetSubelementInfo(componentId));
		if (subelementInfoPtr != NULL){
			return subelementInfoPtr->GetComponentInterface(interfaceType, component, restId);
		}
	}
	else{
		static istd::CClassInfo compInterfaceType(typeid(icomp::IComponent));

		if (!interfaceType.IsValid() || interfaceType.IsVoid() || (interfaceType == compInterfaceType)){
			return &component;
		}

		InterfaceExtractors::ConstIterator foundIter = m_interfaceExtractors.constFind(interfaceType.GetName());
		if (foundIter != m_interfaceExtractors.constEnd()){
			InterfaceExtractorPtr extractorPtr = foundIter.value();

			return extractorPtr(component);
		}

		if (interfaceType.IsConst()){
			istd::CClassInfo nonConstType = interfaceType.GetConstCasted(false);

			InterfaceExtractors::ConstIterator foundIter = m_interfaceExtractors.constFind(nonConstType.GetName());
			if (foundIter != m_interfaceExtractors.constEnd()){
				InterfaceExtractorPtr extractorPtr = foundIter.value();

				return extractorPtr(component);
			}
		}
	}

	if (m_baseComponentPtr != NULL){
		return m_baseComponentPtr->GetComponentInterface(interfaceType, component, subId);
	}

	return NULL;
}


//	reimplemented (icomp::IComponentStaticInfo)

const IAttributeStaticInfo* CBaseComponentStaticInfo::GetAttributeInfo(const QByteArray& attributeId) const
{
	AttributeInfos::ConstIterator foundIter = m_attributeInfos.constFind(attributeId);
	if (foundIter != m_attributeInfos.constEnd()){
		return foundIter.value();
	}
	else if (m_baseComponentPtr != NULL){
		return m_baseComponentPtr->GetAttributeInfo(attributeId);
	}
	else{
		return NULL;
	}
}


//	reimplemented (icomp::IElementStaticInfo)

IElementStaticInfo::Ids CBaseComponentStaticInfo::GetMetaIds(int metaGroupId) const
{
	Ids retVal;

	if (m_baseComponentPtr != NULL){
		retVal = m_baseComponentPtr->GetMetaIds(metaGroupId);
	}

	if (metaGroupId == MGI_INTERFACES){
		for (		InterfaceExtractors::const_iterator iter = m_interfaceExtractors.begin();
					iter != m_interfaceExtractors.end();
					++iter){
			retVal.insert(iter.key());
		}
	}
	else if (metaGroupId == MGI_SUBELEMENTS){
		for (		SubelementInfos::const_iterator iter = m_subelementInfos.begin();
					iter != m_subelementInfos.end();
					++iter){
			retVal.insert(iter.key());
		}
	}

	return retVal;
}


const IElementStaticInfo* CBaseComponentStaticInfo::GetSubelementInfo(const QByteArray& subcomponentId) const
{
	SubelementInfos::ConstIterator foundIter = m_subelementInfos.constFind(subcomponentId);
	if (foundIter != m_subelementInfos.constEnd()){
		return foundIter.value();
	}
	else if (m_baseComponentPtr != NULL){
		return m_baseComponentPtr->GetSubelementInfo(subcomponentId);
	}
	else{
		return NULL;
	}
}


//	reimplemented (iattr::IAttributesMetaInfoProvider)

iattr::IAttributesProvider::AttributeIds CBaseComponentStaticInfo::GetAttributeMetaIds() const
{
	iattr::IAttributesProvider::AttributeIds retVal;

	if (m_baseComponentPtr != NULL){
		retVal = m_baseComponentPtr->GetAttributeMetaIds();
	}

	for (		AttributeInfos::const_iterator iter = m_attributeInfos.begin();
				iter != m_attributeInfos.end();
				++iter){
		retVal.insert(iter.key());
	}

	return retVal;
}


const iattr::IAttributeMetaInfo* CBaseComponentStaticInfo::GetAttributeMetaInfo(const QByteArray& attributeId) const
{
	return CBaseComponentStaticInfo::GetAttributeInfo(attributeId);
}


} // namespace icomp


