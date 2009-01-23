#include "iqtgui/CAboutWidgetGuiComp.h"


namespace iqtgui
{


// public methods

// reimplemented (CGuiComponentBase)

void CAboutWidgetGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
	
	if (m_applicationInfoCompPtr.IsValid()){
		I_DWORD version;
		if (m_applicationInfoCompPtr->GetVersionNumber(iser::IVersionInfo::UserVersionId, version)){
			QString applicationVersion = iqt::GetQString(m_applicationInfoCompPtr->GetEncodedVersionName(iser::IVersionInfo::UserVersionId, version));
			VersionLabel->setText(applicationVersion);
		}
	
		ApplicationNameLabel->setText(iqt::GetQString(m_applicationInfoCompPtr->GetApplicationName()));
	}
}


void CAboutWidgetGuiComp::OnRetranslate()
{
}


} // namespace iqtgui


