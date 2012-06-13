#ifndef iqt_CSettingsArchiveBase_included
#define iqt_CSettingsArchiveBase_included


// Qt includes
#include <QtCore/QList>

// Qt includes
#include <QtCore/QSettings>
#include <QtCore/QString>


namespace iqt
{


class CSettingsArchiveBase: protected QSettings
{
public:
	typedef QSettings BaseClass;

	CSettingsArchiveBase(
				const QString& organizationName,
				const QString& applicationName,
				const QString& rootKey,
				QSettings::Scope settingsScope = QSettings::UserScope);

protected:
	bool EnterTag(const QByteArray& tagId);
	bool LeaveTag(const QByteArray& tagId);
	QString GetCurrentCountKey() const;
	QString CreateNextValueKey();
	QString GetBaseKey() const;

	struct TagInfo
	{
		TagInfo(const QByteArray& tagId, int siblingsCount)
		{
			this->tagId = tagId;
			this->siblingsCount = siblingsCount;
		}
	
		QByteArray tagId;
		int siblingsCount;
	};

	typedef QList<TagInfo> OpenTagsList;
	OpenTagsList m_openTagsList;

	QString m_rootKey;

	int m_valuesCount;
};


} // namespace iqt


#endif // !iqt_CSettingsArchiveBase_included

