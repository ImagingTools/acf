#include "iqt/CApplicationSettingsProviderComp.h"



namespace iqt
{
	
	
CApplicationSettingsProviderComp::CApplicationSettingsProviderComp()
	:m_companyName("UnknownCompany"),
	m_applicationName("ACF Application")
{
}


// reimplemented (icomp::IRegistryLoaderProvider)

QSettings& CApplicationSettingsProviderComp::GetApplicationSettings() const
{
	I_ASSERT(m_settingsPtr.IsValid());

	return *m_settingsPtr.GetPtr();
}


// reimplemented (icomp::IComponent)

void CApplicationSettingsProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_applicationInfoCompPtr.IsValid()){
		m_companyName = iqt::GetQString(m_applicationInfoCompPtr->GetCompanyName());
		m_applicationName = iqt::GetQString(m_applicationInfoCompPtr->GetApplicationName());
	}

	m_settingsPtr.SetPtr(new QSettings(QSettings::UserScope, m_companyName, m_applicationName));
}


} // namespace iqt


