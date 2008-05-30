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

	CSettingsArchiveBase(	const QString& organizationName,
							const QString& applicationName);

protected:
	QString CreateKey(bool replaceMultiple = true) const;

protected:
	struct TagInfo
	{
		TagInfo(const std::string& tagId, int count)
		{
			this->tagId = tagId;
			this->count = count;
		}
	
		std::string tagId;
		int count;
	};

	typedef std::vector<TagInfo> OpenTagsList;
	mutable OpenTagsList m_openTagsList;
};


} // namespace iqt


#endif // !iqt_CSettingsArchiveBase_included

