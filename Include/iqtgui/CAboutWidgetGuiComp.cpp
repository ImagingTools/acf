#include "iqtgui/CAboutWidgetGuiComp.h"


namespace iqtgui
{


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CAboutWidgetGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
	
	if (m_logoPathAttrPtr.IsValid()){
		LogoLabel->setPixmap(QPixmap(iqt::GetQString(*m_logoPathAttrPtr)));
		LogoLabel->setVisible(true);
	}
	else{
		LogoLabel->setVisible(false);
	}
}


void CAboutWidgetGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	QString productName;
	QString applicationName;
	QString applicationSubname;
	QString applicationType;
	QString legalCopyright;
	QString mainVersionText;

	if (m_applicationInfoCompPtr.IsValid()){
		const iser::IVersionInfo& versionInfo = m_applicationInfoCompPtr->GetVersionInfo();

		I_DWORD mainVersionNumber;

		int mainVersionId = m_applicationInfoCompPtr->GetMainVersionId();
		if (mainVersionId >= 0){
			if (versionInfo.GetVersionNumber(mainVersionId, mainVersionNumber)){
				mainVersionText = iqt::GetQString(versionInfo.GetEncodedVersionName(mainVersionId, mainVersionNumber));
			}
		}

		// set version information
		QGridLayout* layoutPtr = dynamic_cast<QGridLayout*>(VersionsFrame->layout());
		if (layoutPtr != NULL){
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
						layoutPtr->addWidget(descriptionLabelPtr, i, 0);

						QLabel* versionLabelPtr = new QLabel(iqt::GetQString(versionText), VersionsFrame);
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
					if ((versionId != mainVersionId) && versionInfo.GetVersionNumber(versionId, version)){
						istd::CString description = versionInfo.GetVersionIdDescription(versionId);
						istd::CString versionText = versionInfo.GetEncodedVersionName(versionId, version);

						QLabel* descriptionLabelPtr = new QLabel(iqt::GetQString(description), VersionsFrame);
						layoutPtr->addWidget(descriptionLabelPtr, rowCount, 0);

						QLabel* versionLabelPtr = new QLabel(iqt::GetQString(versionText), VersionsFrame);
						layoutPtr->addWidget(versionLabelPtr, rowCount++, 1);
					}
				}
			}
		}

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
	ProductNameLabel->setVisible(!productName.isEmpty());
	ApplicationNameLabel->setText(applicationName);
	ApplicationNameLabel->setVisible(!applicationName.isEmpty());

	if (!applicationSubname.isEmpty() && !applicationType.isEmpty()){
		ApplicationTypeLabel->setText(applicationSubname + "(" + applicationType + ")");
	}
	else{
		ApplicationTypeLabel->setText(applicationSubname + applicationType);
	}
	ApplicationTypeLabel->setVisible(!applicationSubname.isEmpty() || !applicationType.isEmpty());

	if (!mainVersionText.isEmpty()){
		MainVersionLabel->setText(mainVersionText);
		MainVersionFrame->setVisible(true);
	}
	else{
		MainVersionFrame->setVisible(false);
	}

	CopyrightLabel->setText(legalCopyright);
	CopyrightLabel->setVisible(!legalCopyright.isEmpty());
}


} // namespace iqtgui


