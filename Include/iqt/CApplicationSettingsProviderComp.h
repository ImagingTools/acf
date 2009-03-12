#ifndef iqt_CApplicationSettingsProviderComp_included
#define iqt_CApplicationSettingsProviderComp_included


#include "icomp/CComponentBase.h"

#include "ibase/IApplicationInfo.h"

#include "iqt/IApplicationSettingsProvider.h"


namespace iqt
{


class CApplicationSettingsProviderComp: 
			public icomp::CComponentBase,
			public iqt::IApplicationSettingsProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CApplicationSettingsProviderComp)
		I_REGISTER_INTERFACE(iqt::IApplicationSettingsProvider)
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info", true, "ApplicationInfo");
	I_END_COMPONENT

	CApplicationSettingsProviderComp();

	// reimplemented (iqt::IApplicationSettingsProvider)
	virtual QSettings& GetApplicationSettings() const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	
private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);

	QString m_companyName;
	QString m_applicationName;
	istd::TDelPtr<QSettings> m_settingsPtr;
};


} // namespace iqt


#endif // !iqt_CApplicationSettingsProviderComp_included


