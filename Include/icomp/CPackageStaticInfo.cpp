#include "icomp/CPackageStaticInfo.h"


namespace icomp
{


// reimplemented (icomp::IPackageStaticInfo)

IComponent* CPackageStaticInfo::CreateComponent() const
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


bool CPackageStaticInfo::RegisterInterfaceExtractor(const std::string& /*interfaceId*/, InterfaceExtractorPtr /*extractorPtr*/)
{
	return false;
}


bool CPackageStaticInfo::RegisterAttributeInfo(const std::string& /*attributeId*/, const IAttributeStaticInfo* /*attributeInfoPtr*/)
{
	return false;
}


}//namespace icomp


