#ifndef iqt_CSettingsWriteArchive_included
#define iqt_CSettingsWriteArchive_included


// ACF includes
#include <iser/CTextWriteArchiveBase.h>
#include <iqt/CSettingsArchiveBase.h>


namespace iqt
{


/**
	Write archive implementation for serialization based on QSettings.

	\ingroup Persistence
*/
class CSettingsWriteArchive: public iser::CTextWriteArchiveBase, public iqt::CSettingsArchiveBase
{
public:
	typedef iser::CTextWriteArchiveBase BaseClass;
	typedef iqt::CSettingsArchiveBase BaseClass2;

	/**
		Constructor.
		
		\param	organizationName	Name of the organization.
		\param	applicationName 	Name of the application.
		\param	rootKey				The root key.
		\param	settingsScope   	(Optional) the settings scope.
		\param	versionInfoPtr  	(Optional) pointer to the version information instance.
	*/
	CSettingsWriteArchive(
				const QString& organizationName,
				const QString& applicationName,
				const QString& rootKey,
				QSettings::Scope settingsScope = QSettings::UserScope,
				const iser::IVersionInfo* versionInfoPtr = NULL);

	// reimplemented (iser::IArchive)
	using BaseClass::Process;
	virtual bool BeginTag(const iser::CArchiveTag& tag) override;
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count) override;
	virtual bool EndTag(const iser::CArchiveTag& tag) override;
	virtual bool Process(QString& value) override;

protected:
	// reimplemented (iser::CTextWriteArchiveBase)
	virtual bool WriteTextNode(const QByteArray& text) override;
};


} // namespace iqt


#endif // !iqt_CSettingsWriteArchive_included


