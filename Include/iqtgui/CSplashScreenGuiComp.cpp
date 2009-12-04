#include "iqtgui/CSplashScreenGuiComp.h"


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
			if (m_versionIdsAttrPtr.IsValid()){
				int versionsCount = m_versionIdsAttrPtr.GetCount();

				for (int i = 0; i < versionsCount; ++i){
					int versionId = m_versionIdsAttrPtr[i];

					I_DWORD version;
					if (m_applicationInfoCompPtr->GetVersionNumber(versionId, version)){
						istd::CString description = (i < m_versionNamesAttrPtr.GetCount())?
									m_versionNamesAttrPtr[i]:
									m_applicationInfoCompPtr->GetVersionIdDescription(versionId);
						istd::CString versionText = m_applicationInfoCompPtr->GetEncodedVersionName(versionId, version);

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
				
				iser::IVersionInfo::VersionIds ids = m_applicationInfoCompPtr->GetVersionIds();
				for (		iser::IVersionInfo::VersionIds::const_iterator iter = ids.begin();
							iter != ids.end();
							++iter){
					int versionId = *iter;

					I_DWORD version;
					if (m_applicationInfoCompPtr->GetVersionNumber(versionId, version)){
						istd::CString description = m_applicationInfoCompPtr->GetVersionIdDescription(versionId);;
						istd::CString versionText = m_applicationInfoCompPtr->GetEncodedVersionName(versionId, version);

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

	QSplashScreen* splashScreenPtr = GetQtWidget();
	if (m_imagePathAttrPtr.IsValid() && (splashScreenPtr != NULL)){
		QString imagePath = iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(*m_imagePathAttrPtr));

		splashScreenPtr->setPixmap(QPixmap(imagePath));
	}
}


} // namespace iqtgui

