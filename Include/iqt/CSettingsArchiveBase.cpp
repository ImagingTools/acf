#include "iqt/CSettingsArchiveBase.h"


// ACF includes
#include "iqt/iqt.h"


namespace iqt
{


// public methods

CSettingsArchiveBase::CSettingsArchiveBase(	const QString& organizationName,
											const QString& applicationName)
	:BaseClass(organizationName, applicationName)
{
}


// protected methods

QString CSettingsArchiveBase::CreateKey(bool replaceMultiple) const
{
	QString registryKey;

	for (		OpenTagsList::iterator index = m_openTagsList.begin();
				index != m_openTagsList.end();
				index++){
		TagInfo& tagInfo = *index;
		if (tagInfo.count > 0 && replaceMultiple){
			tagInfo.tagId = QString::number(tagInfo.count).toStdString();
		}

		registryKey += iqt::GetQString(tagInfo.tagId);
	
		registryKey += "/";
	}

	return registryKey;
}


} // namespace iqt

