#include "iqtgui/CSplashScreenGuiComp.h"


// ACF includes
#include "iqt/CFileSystem.h"


namespace iqtgui
{


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CSplashScreenGuiComp::OnGuiCreated()
{
	I_ASSERT(IsGuiCreated());
	QPalette palette = GetQtWidget()->palette();

	if (m_applicationInfoCompPtr.IsValid()){
		QGridLayout* layoutPtr = dynamic_cast<QGridLayout*>(VersionsFrame->layout());
		if (layoutPtr != NULL){
			const iser::IVersionInfo& versionInfo = m_applicationInfoCompPtr->GetVersionInfo();

			if (m_versionIdsAttrPtr.IsValid()){
				int versionsCount = m_versionIdsAttrPtr.GetCount();

				for (int i = 0; i < versionsCount; ++i){
					int versionId = m_versionIdsAttrPtr[i];

					I_DWORD version;
					if (versionInfo.GetVersionNumber(versionId, version)){
						istd::CString description = (i < m_versionNamesAttrPtr.GetCount())?
									m_versionNamesAttrPtr[i]:
									versionInfo.GetVersionIdDescription(versionId);
						istd::CString versionText = versionInfo.GetEncodedVersionName(versionId, version);

						QLabel* descriptionLabelPtr = new QLabel(iqt::GetQString(description), VersionsFrame);
						descriptionLabelPtr->setPalette(palette);
						layoutPtr->addWidget(descriptionLabelPtr, i, 0);

						QLabel* versionLabelPtr = new QLabel(iqt::GetQString(versionText), VersionsFrame);
						versionLabelPtr->setPalette(palette);
						layoutPtr->addWidget(versionLabelPtr, i, 1);
					}
				}
			}
			else{
				int rowCount = 0;
				
				iser::IVersionInfo::VersionIds ids = versionInfo.GetVersionIds();
				for (		iser::IVersionInfo::VersionIds::const_iterator iter = ids.begin();
							iter != ids.end();
							++iter){
					int versionId = *iter;

					I_DWORD version;
					if (versionInfo.GetVersionNumber(versionId, version)){
						istd::CString description = versionInfo.GetVersionIdDescription(versionId);
						istd::CString versionText = versionInfo.GetEncodedVersionName(versionId, version);

						QLabel* descriptionLabelPtr = new QLabel(iqt::GetQString(description), VersionsFrame);
						descriptionLabelPtr->setPalette(palette);
						layoutPtr->addWidget(descriptionLabelPtr, rowCount, 0);

						QLabel* versionLabelPtr = new QLabel(iqt::GetQString(versionText), VersionsFrame);
						versionLabelPtr->setPalette(palette);
						layoutPtr->addWidget(versionLabelPtr, rowCount++, 1);
					}
				}
			}
		}
	}

	QSplashScreen* splashScreenPtr = GetQtWidget();
	if (m_imagePathAttrPtr.IsValid() && (splashScreenPtr != NULL)){
		QString imagePath = iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(*m_imagePathAttrPtr));

		splashScreenPtr->setPixmap(QPixmap(imagePath));
	}
}


void CSplashScreenGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	QString productName;
	QString applicationName;
	QString applicationSubname;
	QString applicationType;
	QString legalCopyright;

	if (m_applicationInfoCompPtr.IsValid()){
		if (*m_showProductNameAttrPtr){
			productName = iqt::GetQString(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_PRODUCT_NAME));
		}

		if (*m_showApplicationNameAttrPtr){
			applicationName = iqt::GetQString(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME));
		}

		if (*m_showApplicationSubnameAttrPtr){
			applicationSubname = iqt::GetQString(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_SUBNAME));
		}

		if (*m_showProductTypeAttrPtr){
			applicationType = iqt::GetQString(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_TYPE));
		}

		if (*m_showLegalCopyrightAttrPtr){
			legalCopyright = iqt::GetQString(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_LEGAL_COPYRIGHT));
		}
	}

	ProductNameLabel->setText(productName);
	ApplicationNameLabel->setText(applicationName);
	if (!applicationSubname.isEmpty() && !applicationType.isEmpty()){
		ApplicationTypeLabel->setText(applicationSubname + "(" + applicationType + ")");
	}
	else{
		ApplicationTypeLabel->setText(applicationSubname + applicationType);
	}

	ApplicationTypeLabel->setVisible(!applicationSubname.isEmpty() || !applicationType.isEmpty());

	CopyrightLabel->setText(legalCopyright);
	CopyrightLabel->setVisible(!legalCopyright.isEmpty());
}


} // namespace iqtgui


