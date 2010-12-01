#include "icomp/CBaseComponentStaticInfo.h"


namespace icomp
{


CBaseComponentStaticInfo::CBaseComponentStaticInfo(const IRealComponentStaticInfo* baseComponentPtr)
:	m_baseComponentPtr(baseComponentPtr)
{
	if (baseComponentPtr != NULL){
		m_interfaceExtractors.SetParent(&baseComponentPtr->GetInterfaceExtractors());
		m_attributeInfos.SetParent(&baseComponentPtr->GetAttributeInfos());
	}
}


bool CBaseComponentStaticInfo::RegisterInterfaceExtractor(const std::string& interfaceName, InterfaceExtractorPtr extractorPtr)
{
	return m_interfaceExtractors.InsertLocal(interfaceName, extractorPtr);
}


bool CBaseComponentStaticInfo::RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr)
{
	return m_attributeInfos.InsertLocal(attributeId, attributeInfoPtr);
}


//	reimplemented (icomp::IRealComponentStaticInfo)

const IRealComponentStaticInfo::InterfaceExtractors& CBaseComponentStaticInfo::GetInterfaceExtractors() const
{
	return m_interfaceExtractors;
}


//	reimplemented (icomp::IComponentStaticInfo)

const IComponentStaticInfo::AttributeInfos& CBaseComponentStaticInfo::GetAttributeInfos() const
{
	return m_attributeInfos;
}


IComponentStaticInfo::Ids CBaseComponentStaticInfo::GetMetaIds(int metaGroupId) const
{
	Ids retVal;
	if (metaGroupId == MGI_INTERFACES){
		m_interfaceExtractors.GetKeys(retVal);
	}

	return retVal;
}


IComponentStaticInfo::Ids CBaseComponentStaticInfo::GetSubcomponentIds() const
{
	Ids retVal = BaseClass::GetSubcomponentIds();

	if (m_baseComponentPtr != NULL){
		Ids baseIds = m_baseComponentPtr->GetSubcomponentIds();

		retVal.insert(baseIds.begin(), baseIds.end());
	}

	return retVal;
}


const IComponentStaticInfo* CBaseComponentStaticInfo::GetSubcomponentInfo(const std::string& subcomponentId) const
{
	const IComponentStaticInfo* retVal = BaseClass::GetSubcomponentInfo(subcomponentId);
	if (retVal != NULL){
		return retVal;
	}
	else if (m_baseComponentPtr != NULL){
		return m_baseComponentPtr->GetSubcomponentInfo(subcomponentId);
	}
	else{
		return NULL;
	}
}


}//namespace icomp


