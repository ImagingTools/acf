#ifndef iqt_CSettingsWriteArchive_included
#define iqt_CSettingsWriteArchive_included


#include "iser/CTextWriteArchiveBase.h"

#include "iqt/CSettingsArchiveBase.h"


namespace iqt
{


/**
	Archive class for serialization based on QSettings implementation
*/
class CSettingsWriteArchive: public iser::CTextWriteArchiveBase, public iqt::CSettingsArchiveBase
{
public:
	typedef iser::CTextWriteArchiveBase BaseClass;
	typedef iqt::CSettingsArchiveBase BaseClass2;

	CSettingsWriteArchive(
				const QString& organizationName,
				const QString& applicationName,
				const QString& rootKey,
				const iser::IVersionInfo* versionInfoPtr = NULL);

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(QByteArray& value);
	virtual bool Process(QString& value);
};


} // namespace iqt


#endif // !iqt_CSettingsWriteArchive_included


