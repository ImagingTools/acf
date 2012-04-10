#ifndef iqt_CSettingsReadArchive_included
#define iqt_CSettingsReadArchive_included


#include "iser/CTextReadArchiveBase.h"

#include "iqt/CSettingsArchiveBase.h"


namespace iqt
{


class CSettingsReadArchive: public iser::CTextReadArchiveBase, public iqt::CSettingsArchiveBase
{
public:
	typedef iser::CTextReadArchiveBase BaseClass;
	typedef CSettingsArchiveBase BaseClass2;

	CSettingsReadArchive(
				const QString& organizationName,
				const QString& applicationName,
				const QString& rootKey);

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(QByteArray& value);
	virtual bool Process(QString& value);
};


} // namespace iqt


#endif // !iqt_CSettingsReadArchive_included

