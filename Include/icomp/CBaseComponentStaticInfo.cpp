#include "icomp/CBaseComponentStaticInfo.h"


// ACF includes
#include "istd/CIdManipBase.h"

#include "icomp/IComponent.h"


namespace icomp
{


CBaseComponentStaticInfo::CBaseComponentStaticInfo(const IRealComponentStaticInfo* baseComponentPtr)
:	m_baseComponentPtr(baseComponentPtr)
{
}


void CBaseComponentStaticInfo::RegisterInterfaceExtractor(const std::string& interfaceName, InterfaceExtractorPtr extractorPtr)
{
	m_interfaceExtractors[interfaceName] = extractorPtr;
}


void CBaseComponentStaticInfo::RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr)
{
	m_attributeInfos[attributeId] = attributeInfoPtr;
}


void CBaseComponentStaticInfo::RegisterSubelementInfo(const std::string& embeddedId, const IElementStaticInfo* staticInfoPtr)
{
	m_subelementInfos[embeddedId] = staticInfoPtr;
}


//	reimplemented (icomp::IRealComponentStaticInfo)

IComponent* CBaseComponentStaticInfo::CreateComponent() const
{
	I_CRITICAL();	// trying to create abstract base component. Check if I_BEGIN_COMPONENT in constructed component wasn't forgotten

	return NULL;
}


//	reimplemented (icomp::IComponentInterfaceExtractor)

void* CBaseComponentStaticInfo::GetComponentInterface(
			const istd::CClassInfo& interfaceType,
			IComponent& component,
			const std::string& subId) const
{
	if (!subId.empty()){
		std::string componentId;
		std::string restId;
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

		InterfaceExtractors::const_iterator foundIter = m_interfaceExtractors.find(interfaceType.GetName());
		if (foundIter != m_interfaceExtractors.end()){
			InterfaceExtractorPtr extractorPtr = foundIter->second;

			return extractorPtr(component);
		}

		if (interfaceType.IsConst()){
			istd::CClassInfo nonConstType = interfaceType.GetConstCasted(false);

			InterfaceExtractors::const_iterator foundIter = m_interfaceExtractors.find(nonConstType.GetName());
			if (foundIter != m_interfaceExtractors.end()){
				InterfaceExtractorPtr extractorPtr = foundIter->second;

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

const IAttributeStaticInfo* CBaseComponentStaticInfo::GetAttributeInfo(const std::string& attributeId) const
{
	AttributeInfos::const_iterator fondIter = m_attributeInfos.find(attributeId);
	if (fondIter != m_attributeInfos.end()){
		return fondIter->second;
	}
	else if (m_baseComponentPtr != NULL){
		return m_baseComponentPtr->GetAttributeInfo(attributeId);
	}
	else{
		return NULL;
	}
}


//	reimplemented (icomp::IElementStaticInfo)

const IElementStaticInfo* CBaseComponentStaticInfo::GetSubelementInfo(const std::string& subcomponentId) const
{
	SubelementInfos::const_iterator fondIter = m_subelementInfos.find(subcomponentId);
	if (fondIter != m_subelementInfos.end()){
		return fondIter->second;
	}
	else if (m_baseComponentPtr != NULL){
		return m_baseComponentPtr->GetSubelementInfo(subcomponentId);
	}
	else{
		return NULL;
	}
}


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
			retVal.insert(iter->first);
		}
	}
	else if (metaGroupId == MGI_ATTRIBUTES){
		for (		AttributeInfos::const_iterator iter = m_attributeInfos.begin();
					iter != m_attributeInfos.end();
					++iter){
			retVal.insert(iter->first);
		}
	}
	else if (metaGroupId == MGI_SUBELEMENTS){
		for (		SubelementInfos::const_iterator iter = m_subelementInfos.begin();
					iter != m_subelementInfos.end();
					++iter){
			retVal.insert(iter->first);
		}
	}

	return retVal;
}


} // namespace icomp


