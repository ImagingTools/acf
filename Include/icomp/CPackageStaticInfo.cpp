#include "icomp/CPackageStaticInfo.h"


#include "iser/CArchiveTag.h"


namespace icomp
{


CPackageStaticInfo::CPackageStaticInfo()
{
}


CPackageStaticInfo::CPackageStaticInfo(
			const istd::CString& description,
			const istd::CString& keywords)
:	m_description(description),
	m_keywords(keywords)
{
}


bool CPackageStaticInfo::SerializeMeta(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag descriptionTag("Description", "Human readable description");
	retVal = retVal && archive.BeginTag(descriptionTag);
	retVal = retVal && archive.Process(m_description);
	retVal = retVal && archive.EndTag(descriptionTag);

	static iser::CArchiveTag keywordsTag("Keywords", "Human readable keywords");
	retVal = retVal && archive.BeginTag(keywordsTag);
	retVal = retVal && archive.Process(m_keywords);
	retVal = retVal && archive.EndTag(keywordsTag);

	return retVal;
}


// reimplemented (icomp::IPackageStaticInfo)

const CPackageStaticInfo::InterfaceExtractors& CPackageStaticInfo::GetInterfaceExtractors() const
{
	static CascInterfaceExtractors empty;

	return empty;
}


const CPackageStaticInfo::AttributeInfos& CPackageStaticInfo::GetAttributeInfos() const
{
	static CascAttributeInfos empty;

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


