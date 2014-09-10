#include "iqtgui/CSplashScreenGuiComp.h"


// ACF includes
#include "istd/CSystem.h"


namespace iqtgui
{


CSplashScreenGuiComp::CSplashScreenGuiComp()
:	m_mainVersionId(0)
{
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CSplashScreenGuiComp::OnGuiCreated()
{
	Q_ASSERT(IsGuiCreated());
	QPalette palette = GetQtWidget()->palette();

	if (m_applicationInfoCompPtr.IsValid()){
		m_mainVersionId = m_applicationInfoCompPtr->GetMainVersionId();

		QGridLayout* layoutPtr = dynamic_cast<QGridLayout*>(VersionsFrame->layout());
		if (layoutPtr != NULL){
			int gridVerticalOffset = (m_mainVersionId >= 0)? 2: 0;

			const iser::IVersionInfo& versionInfo = m_applicationInfoCompPtr->GetVersionInfo();

			if (m_versionIdsAttrPtr.IsValid()){
				int versionsCount = m_versionIdsAttrPtr.GetCount();

				for (int i = 0; i < versionsCount; ++i){
					int versionId = m_versionIdsAttrPtr[i];

					quint32 version;
					if (versionInfo.GetVersionNumber(versionId, version)){
						QString description = (i < m_versionNamesAttrPtr.GetCount())?
									m_versionNamesAttrPtr[i]:
									versionInfo.GetVersionIdDescription(versionId);
						QString versionText = versionInfo.GetEncodedVersionName(versionId, version);

						QLabel* descriptionLabelPtr = new QLabel(description, VersionsFrame);
						descriptionLabelPtr->setPalette(palette);
						layoutPtr->addWidget(descriptionLabelPtr, gridVerticalOffset + i, 0);

						QLabel* versionLabelPtr = new QLabel(versionText, VersionsFrame);
						versionLabelPtr->setPalette(palette);
						layoutPtr->addWidget(versionLabelPtr, gridVerticalOffset + i, 1);
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

					quint32 version;
					if ((versionId != m_mainVersionId) && versionInfo.GetVersionNumber(versionId, version)){
						QString description = versionInfo.GetVersionIdDescription(versionId);
						QString versionText = versionInfo.GetEncodedVersionName(versionId, version);

						QLabel* descriptionLabelPtr = new QLabel(description, VersionsFrame);
						descriptionLabelPtr->setPalette(palette);
						layoutPtr->addWidget(descriptionLabelPtr, gridVerticalOffset + rowCount, 0);

						QLabel* versionLabelPtr = new QLabel(versionText, VersionsFrame);
						versionLabelPtr->setPalette(palette);
						layoutPtr->addWidget(versionLabelPtr, gridVerticalOffset + rowCount++, 1);
					}
				}
			}
		}
	}

	QSplashScreen* splashScreenPtr = GetQtWidget();
	if (m_imagePathAttrPtr.IsValid() && (splashScreenPtr != NULL)){
		QString imagePath = istd::CSystem::GetEnrolledPath(*m_imagePathAttrPtr);

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
	QString mainVersionText;

	if (m_applicationInfoCompPtr.IsValid()){
		const iser::IVersionInfo& versionInfo = m_applicationInfoCompPtr->GetVersionInfo();

		if (m_mainVersionId >= 0){
			quint32 mainVersionNumber;
			if (versionInfo.GetVersionNumber(m_mainVersionId, mainVersionNumber)){
				mainVersionText = versionInfo.GetEncodedVersionName(m_mainVersionId, mainVersionNumber);
			}
		}

		if (*m_showProductNameAttrPtr){
			productName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_PRODUCT_NAME);
		}

		if (*m_showApplicationNameAttrPtr){
			applicationName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME);
		}

		if (*m_showApplicationSubnameAttrPtr){
			applicationSubname = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_SUBNAME);
		}

		if (*m_showProductTypeAttrPtr){
			applicationType = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_TYPE);
		}

		if (*m_showLegalCopyrightAttrPtr){
			legalCopyright = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_LEGAL_COPYRIGHT);
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

	if (!mainVersionText.isEmpty()){
		MainVersionLabel->setText(mainVersionText);
		MainVersionLabel->setVisible(true);
		MainVersionTitleLabel->setVisible(true);
	}
	else{
		MainVersionLabel->setVisible(false);
		MainVersionTitleLabel->setVisible(false);
	}

	CopyrightLabel->setText(legalCopyright);
	CopyrightLabel->setVisible(!legalCopyright.isEmpty());
}


} // namespace iqtgui


