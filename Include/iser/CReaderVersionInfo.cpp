#include "iser/CReaderVersionInfo.h"


namespace iser
{


void CReaderVersionInfo::Reset()
{
	m_versionIdList.clear();
}


bool CReaderVersionInfo::InsertVersionId(int id, I_DWORD version, const std::string& description)
{
	VersionIdElement element(version, description);

	std::pair<VersionElements::iterator, bool> status = m_versionIdList.insert(VersionElements::value_type(id, element));

	return status.second;
}


bool CReaderVersionInfo::RemoveVersionId(int id)
{
	return m_versionIdList.erase(id) > 0;
}


// reimplemented (iser::IVersionInfo)

I_DWORD CReaderVersionInfo::GetVersion(int versionId) const
{
	const VersionIdElement& element = GetVersionIdElement(versionId);

	return element.version;
}


istd::CString CReaderVersionInfo::GetVersionIdDescription(int versionId) const
{
	const VersionIdElement& element = GetVersionIdElement(versionId);

	return element.description;
}


CReaderVersionInfo::VersionIds CReaderVersionInfo::GetVersionIds() const
{
	VersionIds ids;

	for (		VersionElements::const_iterator iter = m_versionIdList.begin();
				iter != m_versionIdList.end();
				++iter){
		ids.insert(iter->first);
	}

	return ids;
}


// protected methods

const CReaderVersionInfo::VersionIdElement& CReaderVersionInfo::GetVersionIdElement(int versionId) const
{
	VersionElements::const_iterator iter = m_versionIdList.find(versionId);

	if (iter != m_versionIdList.end()){
		return iter->second;
	}

	return s_element;
}


// static attributes

CReaderVersionInfo::VersionIdElement CReaderVersionInfo::s_element(I_DWORD(UnknownVersion), "");


} // namespace iser


