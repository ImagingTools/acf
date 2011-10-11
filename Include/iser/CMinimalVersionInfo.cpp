#include "iser/CMinimalVersionInfo.h"


// ACF includes
#include "iser/ISerializable.h"


namespace iser
{


CMinimalVersionInfo::CMinimalVersionInfo(const ISerializable* serializablePtr, bool addStandardIds)
:	m_serializable(*serializablePtr)
{
	I_ASSERT(serializablePtr != NULL);

	if (addStandardIds){
		AddId(AcfVersionId, "ACF Framework");
	}
}


bool CMinimalVersionInfo::AddId(int versionId, const istd::CString& description)
{
	Versions::const_iterator iter = m_versions.find(versionId);
	if (iter != m_versions.end()){
		return false;
	}

	Info& info = m_versions[versionId];

	info.description = description;
	info.version = m_serializable.GetMinimalVersion(versionId);

	return true;
}


// reimplemented (iser::IVersionInfo)

bool CMinimalVersionInfo::GetVersionNumber(int versionId, I_DWORD& result) const
{
	Versions::const_iterator iter = m_versions.find(versionId);
	if (m_versions.find(versionId) != m_versions.end()){
		const Info& info = iter->second;

		result = info.version;

		return true;
	}

	result = 0xffffffff;

	return false;
}


istd::CString CMinimalVersionInfo::GetVersionIdDescription(int versionId) const
{
	Versions::const_iterator iter = m_versions.find(versionId);
	if (m_versions.find(versionId) != m_versions.end()){
		const Info& info = iter->second;

		return info.description;
	}

	static istd::CString description("<unregistered>");

	return description;
}


IVersionInfo::VersionIds CMinimalVersionInfo::GetVersionIds() const
{
	VersionIds retVal;

	for (		Versions::const_iterator iter = m_versions.begin();
				iter != m_versions.end();
				++iter){
		retVal.insert(iter->first);
	}

	return retVal;
}


istd::CString CMinimalVersionInfo::GetEncodedVersionName(int /*versionId*/, I_DWORD /*versionNumber*/) const
{
	return "";
}


} // namespace iser


