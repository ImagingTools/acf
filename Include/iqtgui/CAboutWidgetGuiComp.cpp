#include "iqtgui/CAboutWidgetGuiComp.h"


namespace iqtgui
{


// public methods

// reimplemented (CGuiComponentBase)

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

	if (m_descriptionTextAttrPtr.IsValid()){
		DescriptionLabel->setText(iqt::GetQString(*m_descriptionTextAttrPtr));
	}

	if (m_applicationInfoCompPtr.IsValid()){
		ApplicationNameLabel->setText(iqt::GetQString(m_applicationInfoCompPtr->GetApplicationName()));

		QLayout* layoutPtr = VersionsWidget->layout();

		if (layoutPtr != NULL){
			iser::IVersionInfo::VersionIds ids = m_applicationInfoCompPtr->GetVersionIds();
			for (		iser::IVersionInfo::VersionIds::const_iterator idIter = ids.begin();
						idIter != ids.end();
						++idIter){
				int versionId = *idIter;

				I_DWORD versionNumber;
				if (m_applicationInfoCompPtr->GetVersionNumber(versionId, versionNumber)){
					QString versionDesciption = iqt::GetQString(m_applicationInfoCompPtr->GetVersionIdDescription(versionId));
					QString encodedVersion = iqt::GetQString(m_applicationInfoCompPtr->GetEncodedVersionName(versionId, versionNumber));

					QString infoText = QObject::tr("Version %1:\t%2").arg(versionDesciption).arg(encodedVersion);

					layoutPtr->addWidget(new QLabel(infoText, VersionsWidget));
				}
			}
		}
	}
}


void CAboutWidgetGuiComp::OnRetranslate()
{
}


} // namespace iqtgui


