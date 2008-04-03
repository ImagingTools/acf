#include "icomp/CPackageStaticInfo.h"


namespace icomp
{


// reimplemented (icomp::IPackageStaticInfo)

IComponent* CPackageStaticInfo::CreateComponent(const IComponentContext* /*contextPtr*/) const
{
	return NULL;
}


const CPackageStaticInfo::InterfaceExtractors& CPackageStaticInfo::GetInterfaceExtractors() const
{
	static InterfaceExtractors empty;

	return empty;
}


const CPackageStaticInfo::AttributeInfos& CPackageStaticInfo::GetAttributeInfos() const
{
	static AttributeInfos empty;

	return empty;
}


const CPackageStaticInfo::SubcomponentInfos& CPackageStaticInfo::GetSubcomponentInfos() const
{
	return m_subcomponentInfos;
}


bool CPackageStaticInfo::RegisterInterfaceExtractor(const std::string& /*interfaceId*/, InterfaceExtractorPtr /*extractorPtr*/)
{
	return false;
}


bool CPackageStaticInfo::RegisterAttributeInfo(const std::string& /*attributeId*/, const IAttributeStaticInfo* /*attributeInfoPtr*/)
{
	return false;
}


bool CPackageStaticInfo::RegisterSubcomponentInfo(const std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr)
{
	return m_subcomponentInfos.InsertLocal(subcomponentId, componentInfoPtr);
}


// protected methods

void CPackageStaticInfo::ResetSubcomponentList()
{
	m_subcomponentInfos.ResetLocal();
}


}//namespace icomp


