#include "icomp/CPackageStaticInfo.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace icomp
{


CPackageStaticInfo::CPackageStaticInfo()
{
}


CPackageStaticInfo::CPackageStaticInfo(
			const QString& description,
			const QString& keywords)
:	m_description(description),
	m_keywords(keywords)
{
}


void CPackageStaticInfo::Reset()
{
	m_embeddedComponentInfos.clear();
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


void CPackageStaticInfo::RegisterEmbeddedComponentInfo(const std::string& embeddedId, const IComponentStaticInfo* componentInfoPtr)
{
	m_embeddedComponentInfos[embeddedId] = componentInfoPtr;
}


// reimplemented (icomp::IPackageStaticInfo)

const IComponentStaticInfo* CPackageStaticInfo::GetEmbeddedComponentInfo(const std::string& embeddedId) const
{
	EmbeddedComponentInfos::const_iterator foundIter = m_embeddedComponentInfos.find(embeddedId);
	if (foundIter != m_embeddedComponentInfos.end()){
		return foundIter->second;
	}
	else{
		return NULL;
	}
}


IElementStaticInfo::Ids CPackageStaticInfo::GetMetaIds(int metaGroupId) const
{
	Ids retVal;

	if (metaGroupId == MGI_EMBEDDED_COMPONENTS){
		for (		EmbeddedComponentInfos::const_iterator iter = m_embeddedComponentInfos.begin();
					iter != m_embeddedComponentInfos.end();
					++iter){
			retVal.insert(iter->first);
		}
	}

	return retVal;
}


const QString& CPackageStaticInfo::GetDescription() const
{
	return m_description;
}


const QString& CPackageStaticInfo::GetKeywords() const
{
	return m_keywords;
}


} // namespace icomp


