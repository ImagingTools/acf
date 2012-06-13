#include "iqt/CSettingsArchiveBase.h"


// ACF includes
#include "iqt/iqt.h"


namespace iqt
{


// public methods

CSettingsArchiveBase::CSettingsArchiveBase(
			const QString& organizationName,
			const QString& applicationName,
			const QString& rootKey,
			QSettings::Scope settingsScope)
	:BaseClass(settingsScope, organizationName, applicationName),
	m_rootKey(rootKey),
	m_valuesCount(0)
{
}


// protected methods

bool CSettingsArchiveBase::EnterTag(const QByteArray& tagId)
{
	m_openTagsList.push_back(TagInfo(tagId, m_valuesCount));

	m_valuesCount = 0;

	return true;
}


bool CSettingsArchiveBase::LeaveTag(const QByteArray& tagId)
{
	if (m_openTagsList.isEmpty()){
		return false;
	}

	if (m_openTagsList.back().tagId != tagId){
		return false;
	}

	TagInfo& tagInfo = m_openTagsList.back();
	m_valuesCount = tagInfo.siblingsCount + 1;

	m_openTagsList.pop_back();

	return true;
}


QString CSettingsArchiveBase::GetCurrentCountKey() const
{
	return GetBaseKey() + "count";
}


QString CSettingsArchiveBase::CreateNextValueKey()
{
	static const QString valueSuffix("_value");

	QString retVal = GetBaseKey() + QString::number(++m_valuesCount) + valueSuffix;

	return retVal;
}


QString CSettingsArchiveBase::GetBaseKey() const
{
	QString registryKey;

	if (!m_rootKey.isEmpty()){
		registryKey = m_rootKey + "/";
	}

	for (		OpenTagsList::const_iterator tagIter = m_openTagsList.begin();
				tagIter != m_openTagsList.end();
				tagIter++){
		const TagInfo& tagInfo = *tagIter;

		static const QString separator("_");

		registryKey += QString::number(tagInfo.siblingsCount + 1) + separator + tagInfo.tagId;

		registryKey += "/";
	}

	return registryKey;
}


} // namespace iqt


