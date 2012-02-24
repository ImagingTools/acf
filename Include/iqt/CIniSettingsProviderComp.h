#ifndef iqt_CIniSettingsProviderComp_included
#define iqt_CIniSettingsProviderComp_included


#include <QString>

#include "icomp/CComponentBase.h"

#include "iqt/ISettingsProvider.h"


namespace iqt
{


/**
	Provides settings from INI file.
*/
class CIniSettingsProviderComp: 
			public icomp::CComponentBase,
			virtual public iqt::ISettingsProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CIniSettingsProviderComp);
		I_REGISTER_INTERFACE(iqt::ISettingsProvider);
		I_ASSIGN(m_iniFilePathAttrPtr, "IniFilePath", "Path to INI file", true, "Default.ini");
	I_END_COMPONENT;

	// reimplemented (iqt::ISettingsProvider)
	virtual QSettings& GetSettings() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	
private:
	I_ATTR(QString, m_iniFilePathAttrPtr);

	istd::TDelPtr<QSettings> m_settingsPtr;
};


} // namespace iqt


#endif // !iqt_CIniSettingsProviderComp_included


