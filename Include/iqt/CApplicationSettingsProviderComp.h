#ifndef iqt_CApplicationSettingsProviderComp_included
#define iqt_CApplicationSettingsProviderComp_included


#include "icomp/CComponentBase.h"

#include "ibase/IApplicationInfo.h"

#include "iqt/ISettingsProvider.h"


namespace iqt
{


class CApplicationSettingsProviderComp: 
			public icomp::CComponentBase,
			virtual public iqt::ISettingsProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CApplicationSettingsProviderComp);
		I_REGISTER_INTERFACE(iqt::ISettingsProvider);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info", true, "ApplicationInfo");
	I_END_COMPONENT;

	CApplicationSettingsProviderComp();

	// reimplemented (iqt::ISettingsProvider)
	virtual QSettings& GetSettings() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	
private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);

	istd::TDelPtr<QSettings> m_settingsPtr;
};


} // namespace iqt


#endif // !iqt_CApplicationSettingsProviderComp_included


