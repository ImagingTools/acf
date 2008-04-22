#include "iqt/CSplashScreenGuiComp.h"


namespace iqt
{


// reimplemented (iqt::CGuiComponentBase)

void CSplashScreenGuiComp::OnGuiShown()
{
	istd::CString acfVersionText;
	istd::CString productVersionText;

	if (m_applicationInfoCompPtr.IsValid()){
		if (m_showAcfVersionAttrPtr.IsValid() && *m_showAcfVersionAttrPtr){
			I_DWORD acfVersion = m_applicationInfoCompPtr->GetVersion(iser::IVersionInfo::FrameworkVersionId);
			acfVersionText = m_applicationInfoCompPtr->EncodeVersionName(acfVersion, iser::IVersionInfo::FrameworkVersionId);
		}
		if (m_showProductVersionAttrPtr.IsValid()){
			I_DWORD acfVersion = m_applicationInfoCompPtr->GetVersion(iser::IVersionInfo::UserVersionId);
			productVersionText = m_applicationInfoCompPtr->EncodeVersionName(acfVersion, iser::IVersionInfo::UserVersionId);
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

	if (m_productNameAttrPtr.IsValid()){
		ProductNameLabel->setText(iqt::GetQString(*m_productNameAttrPtr));
	}
	else{
		ProductNameLabel->setText("");
	}

	if (m_copyrightTextAttrPtr.IsValid()){
		CopyrightLabel->setText(iqt::GetQString(*m_copyrightTextAttrPtr));
	}
	CopyrightLabel->setVisible(m_copyrightTextAttrPtr.IsValid());

	QSplashScreen* splashScreenPtr = dynamic_cast<QSplashScreen*>(GetWidget());
	if (m_imagePathAttrPtr.IsValid() && (splashScreenPtr != NULL)){
		QPixmap image(GetQString(*m_imagePathAttrPtr));
		splashScreenPtr->setPixmap(image);
	}
}


} // namespace iqt

