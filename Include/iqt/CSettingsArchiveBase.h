#ifndef iqt_CSettingsArchiveBase_included
#define iqt_CSettingsArchiveBase_included


#include <QSettings>

#include "istd/CString.h"


namespace iqt
{


class CSettingsArchiveBase: protected QSettings
{
public:
	typedef QSettings BaseClass;

	CSettingsArchiveBase(
				const QString& organizationName,
				const QString& applicationName,
				const QString& rootKey);

protected:
	bool EnterTag(const std::string& tagId);
	bool LeaveTag(const std::string& tagId);
	QString GetCurrentCountKey() const;
	QString CreateNextValueKey();
	QString GetBaseKey() const;

	struct TagInfo
	{
		TagInfo(const std::string& tagId, int siblingsCount)
		{
			this->tagId = tagId;
			this->siblingsCount = siblingsCount;
		}
	
		std::string tagId;
		int siblingsCount;
	};

	typedef std::vector<TagInfo> OpenTagsList;
	OpenTagsList m_openTagsList;

	QString m_rootKey;

	int m_valuesCount;
};


} // namespace iqt


#endif // !iqt_CSettingsArchiveBase_included

