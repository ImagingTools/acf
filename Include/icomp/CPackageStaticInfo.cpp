#include "icomp/CPackageStaticInfo.h"


namespace icomp
{


CPackageStaticInfo::CPackageStaticInfo()
{
}


CPackageStaticInfo::CPackageStaticInfo(
			const std::string& /*logicalName*/,
			const istd::CString& description,
			const istd::CString& keywords)
:	m_description(description),
	m_keywords(keywords)
{
}


// reimplemented (icomp::IPackageStaticInfo)

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


bool CPackageStaticInfo::RegisterInterfaceExtractor(const istd::CClassInfo& /*interfaceId*/, InterfaceExtractorPtr /*extractorPtr*/)
{
	return false;
}


bool CPackageStaticInfo::RegisterAttributeInfo(const std::string& /*attributeId*/, const IAttributeStaticInfo* /*attributeInfoPtr*/)
{
	return false;
}


const istd::CString& CPackageStaticInfo::GetDescription() const
{
	return m_description;
}


const istd::CString& CPackageStaticInfo::GetKeywords() const
{
	return m_keywords;
}


}//namespace icomp


