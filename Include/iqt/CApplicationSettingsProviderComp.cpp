#include <iqt/CApplicationSettingsProviderComp.h>



namespace iqt
{


// public methods

// reimplemented (iqt::ISettingsProvider)

QSettings& CApplicationSettingsProviderComp::GetSettings() const
{
	Q_ASSERT(m_settingsPtr.IsValid());

	return *m_settingsPtr.GetPtr();
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CApplicationSettingsProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	QString companyName("UnknownCompany");
	QString applicationName("ACF Application");

	if (m_applicationInfoCompPtr.IsValid()){
		companyName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME, false);
		applicationName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME, false);
		QString applicationSubname = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_SUBNAME, false);
		if (!applicationSubname.isEmpty()){
			applicationName = applicationName + "." + applicationSubname;
		}
	}

	m_settingsPtr.SetPtr(new QSettings(QSettings::UserScope, companyName, applicationName));
}


} // namespace iqt


