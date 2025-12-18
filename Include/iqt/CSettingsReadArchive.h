#ifndef iqt_CSettingsReadArchive_included
#define iqt_CSettingsReadArchive_included


// ACF includes
#include <iser/CTextReadArchiveBase.h>
#include <iqt/CSettingsArchiveBase.h>


namespace iqt
{

/**
	The read archive used global system settings mechansim (by example Windows registry or INI file) as medium.

	\ingroup Persistence
*/
class CSettingsReadArchive: public iser::CTextReadArchiveBase, public iqt::CSettingsArchiveBase
{
public:
	typedef iser::CTextReadArchiveBase BaseClass;
	typedef CSettingsArchiveBase BaseClass2;

	/**
		Constructor.
		
		\param	organizationName	Name of the organization.
		\param	applicationName 	Name of the application.
		\param	rootKey				The root key.
		\param	settingsScope   	(Optional) the settings scope.
	*/
	CSettingsReadArchive(
				const QString& organizationName,
				const QString& applicationName,
				const QString& rootKey,
				QSettings::Scope settingsScope = QSettings::UserScope);

	// reimplemented (iser::IArchive)
	using BaseClass::Process;

	virtual bool BeginTag(const iser::CArchiveTag& tag) override;
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count) override;
	virtual bool EndTag(const iser::CArchiveTag& tag) override;
	virtual bool Process(QString& value) override;

protected:
	bool ReadStringNode(QString& text);

	// reimplemented (iser::CTextReadArchiveBase)
	virtual bool ReadTextNode(QByteArray& text) override;
};


} // namespace iqt


#endif // !iqt_CSettingsReadArchive_included


