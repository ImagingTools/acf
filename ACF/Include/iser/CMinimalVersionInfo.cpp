#include "iser/CMinimalVersionInfo.h"


#include "iser/ISerializable.h"


namespace iser
{


CMinimalVersionInfo::CMinimalVersionInfo(const iser::ISerializable* serializablePtr, bool addStandardIds)
:	m_serializable(*serializablePtr)
{
	I_ASSERT(serializablePtr != NULL);

	if (addStandardIds){
		AddId(FrameworkVersionId, "ACF Framework");
		AddId(UserVersionId, "User");
	}
}


bool CMinimalVersionInfo::AddId(int versionId, const std::string& description)
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

I_DWORD CMinimalVersionInfo::GetVersion(int versionId) const
{
	Versions::const_iterator iter = m_versions.find(versionId);
	if (m_versions.find(versionId) != m_versions.end()){
		const Info& info = iter->second;

		return info.version;
	}

	return 0;
}


const std::string& CMinimalVersionInfo::GetVersionIdDescription(int versionId) const
{
	Versions::const_iterator iter = m_versions.find(versionId);
	if (m_versions.find(versionId) != m_versions.end()){
		const Info& info = iter->second;

		return info.description;
	}

	static std::string description("<unregistered>");

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


} // namespace iser


