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


bool CMinimalVersionInfo::AddId(int versionId, const QString& description)
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

bool CMinimalVersionInfo::GetVersionNumber(int versionId, quint32& result) const
{
	Versions::const_iterator iter = m_versions.find(versionId);
	if (m_versions.find(versionId) != m_versions.end()){
		const Info& info = iter.value();

		result = info.version;

		return true;
	}

	result = 0xffffffff;

	return false;
}


QString CMinimalVersionInfo::GetVersionIdDescription(int versionId) const
{
	Versions::const_iterator iter = m_versions.find(versionId);
	if (m_versions.find(versionId) != m_versions.end()){
		const Info& info = iter.value();

		return info.description;
	}

	static QString description("<unregistered>");

	return description;
}


IVersionInfo::VersionIds CMinimalVersionInfo::GetVersionIds() const
{
	VersionIds retVal;

	for (		Versions::const_iterator iter = m_versions.begin();
				iter != m_versions.end();
				++iter){
		retVal.insert(iter.key());
	}

	return retVal;
}


QString CMinimalVersionInfo::GetEncodedVersionName(int /*versionId*/, quint32 /*versionNumber*/) const
{
	return "";
}


} // namespace iser


