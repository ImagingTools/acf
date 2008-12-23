#include "iqtgui/CSplashScreenGuiComp.h"


namespace iqtgui
{


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CSplashScreenGuiComp::OnGuiCreated()
{
	istd::CString acfVersionText;
	istd::CString productVersionText;

	if (m_applicationInfoCompPtr.IsValid()){
		if (m_showAcfVersionAttrPtr.IsValid() && *m_showAcfVersionAttrPtr){
			I_DWORD version;
			if (m_applicationInfoCompPtr->GetVersionNumber(iser::IVersionInfo::FrameworkVersionId, version)){
				acfVersionText = m_applicationInfoCompPtr->GetEncodedVersionName(iser::IVersionInfo::FrameworkVersionId, version);
			}
		}

		if (m_showProductVersionAttrPtr.IsValid()){
			I_DWORD version;
			if (m_applicationInfoCompPtr->GetVersionNumber(iser::IVersionInfo::UserVersionId, version)){
				productVersionText = m_applicationInfoCompPtr->GetEncodedVersionName(iser::IVersionInfo::UserVersionId, version);
			}
		}
	}

	if (!acfVersionText.empty()){
		AcfVersionLabel->setText(iqt::GetQString(acfVersionText));
	}
	AcfVersionFrame->setVisible(!acfVersionText.empty());

	if (!productVersionText.empty()){
		VersionLabel->setText(iqt::GetQString(productVersionText));
	}
	VersionFrame->setVisible(!productVersionText.empty());

	if (m_productTypeAttrPtr.IsValid()){
		ProductTypeLabel->setText(iqt::GetQString(*m_productTypeAttrPtr));
	}

	ProductTypeLabel->setVisible(m_productTypeAttrPtr.IsValid());

	QString productName;
	if (m_productNameAttrPtr.IsValid()){
		productName = iqt::GetQString(*m_productNameAttrPtr);
	}
	else if (m_applicationInfoCompPtr.IsValid()){
		productName = iqt::GetQString(m_applicationInfoCompPtr->GetApplicationName());
	}
	ProductNameLabel->setText(productName);

	if (m_copyrightTextAttrPtr.IsValid()){
		CopyrightLabel->setText(iqt::GetQString(*m_copyrightTextAttrPtr));
	}
	CopyrightLabel->setVisible(m_copyrightTextAttrPtr.IsValid());

	QSplashScreen* splashScreenPtr = dynamic_cast<QSplashScreen*>(GetWidget());
	if (m_imagePathAttrPtr.IsValid() && (splashScreenPtr != NULL)){
		QPixmap image(iqt::GetQString(*m_imagePathAttrPtr));
		splashScreenPtr->setPixmap(image);
	}
}


} // namespace iqtgui

