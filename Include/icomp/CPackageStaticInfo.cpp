#include <icomp/CPackageStaticInfo.h>


#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace icomp
{


// static constants
static const iser::CArchiveTag s_descriptionTag("Description", "Human readable description", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_keywordsTag("Keywords", "Human readable keywords", iser::CArchiveTag::TT_LEAF);


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

	retVal = retVal && archive.BeginTag(s_descriptionTag);
	retVal = retVal && archive.Process(m_description);
	retVal = retVal && archive.EndTag(s_descriptionTag);

	retVal = retVal && archive.BeginTag(s_keywordsTag);
	retVal = retVal && archive.Process(m_keywords);
	retVal = retVal && archive.EndTag(s_keywordsTag);

	return retVal;
}


void CPackageStaticInfo::RegisterEmbeddedComponentInfo(const QByteArray& embeddedId, const IComponentStaticInfo* componentInfoPtr)
{
	m_embeddedComponentInfos[embeddedId] = componentInfoPtr;
}


// reimplemented (icomp::IElementStaticInfo)

IElementStaticInfo::Ids CPackageStaticInfo::GetMetaIds(int metaGroupId) const
{
	Ids retVal;

	if (metaGroupId == MGI_EMBEDDED_COMPONENTS){
		for (		EmbeddedComponentInfos::const_iterator iter = m_embeddedComponentInfos.begin();
					iter != m_embeddedComponentInfos.end();
					++iter){
			retVal.insert(iter.key());
		}
	}

	return retVal;
}


const IComponentStaticInfo* CPackageStaticInfo::GetEmbeddedComponentInfo(const QByteArray& embeddedId) const
{
	EmbeddedComponentInfos::ConstIterator foundIter = m_embeddedComponentInfos.constFind(embeddedId);
	if (foundIter != m_embeddedComponentInfos.constEnd()){
		return foundIter.value();
	}
	else{
		return NULL;
	}
}


// reimplemented (icomp::IPackageStaticInfo)

const QString& CPackageStaticInfo::GetDescription() const
{
	return m_description;
}


const QString& CPackageStaticInfo::GetKeywords() const
{
	return m_keywords;
}


} // namespace icomp


