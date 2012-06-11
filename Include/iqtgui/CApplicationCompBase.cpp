#include "iqtgui/CApplicationCompBase.h"


// Qt includes
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QStyle>
#include <QtGui/QVBoxLayout>

// ACF includes
#include "icomp/CCompositeComponent.h"


namespace iqtgui
{


// reimplemented (ibase::IApplication)

bool CApplicationCompBase::InitializeApplication(int argc, char** argv)
{
	if (!m_applicationPtr.IsValid()){
		m_applicationPtr.SetPtr(new QApplication(argc, argv));
		if (!m_applicationPtr.IsValid()){
			return false;
		}

		// set up current language
		if (m_translationManagerCompPtr.IsValid() && (m_translationManagerCompPtr->GetCurrentLanguageIndex() < 0)){
			m_translationManagerCompPtr->SetSystemLanguage();
		}

		// set up application name
		QString applicationFullName;
		if (m_applicationInfoCompPtr.IsValid()){
			applicationFullName = *m_titleFormatAttrPtr;

			applicationFullName.replace("$(CompanyName)", m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME));
			applicationFullName.replace("$(ProductName)", m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_PRODUCT_NAME));
			applicationFullName.replace("$(AppName)", m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME));
			applicationFullName.replace("$(AppSubName)", m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_SUBNAME));
			applicationFullName.replace("$(AppType)", m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_TYPE));
			applicationFullName.replace("$(Version)", m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_MAIN_VERSION));
		}
		else{
			applicationFullName = QObject::tr("ACF application");
		}
		m_applicationPtr->setApplicationName(applicationFullName);

		// set up icon
		QIcon icon;
		if (m_iconPathAttrPtr.IsValid()){
			icon = QIcon(*m_iconPathAttrPtr);
		}
		else{	
			icon.addFile(":/Icons/AcfLogo.svg");
		}
		m_applicationPtr->setWindowIcon(icon);

		if (m_applicationInfoCompPtr.IsValid()){
			// set up application data
			QString mainVersionText = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_MAIN_VERSION);
			m_applicationPtr->setApplicationVersion(mainVersionText);

			QString companyNameText = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME);
			m_applicationPtr->setOrganizationName(companyNameText);
		}

		// set up palette
		m_applicationPtr->setPalette(QApplication::style()->standardPalette());

		// set up style sheet
		if (m_styleSheetAttrPtr.IsValid()){
			SetStyleSheet(*m_styleSheetAttrPtr);
		}

		icomp::ICompositeComponent* parentPtr = const_cast<icomp::ICompositeComponent*>(GetParentComponent(true));
		icomp::CCompositeComponent* compositePtr = dynamic_cast<icomp::CCompositeComponent*>(parentPtr);

		if (compositePtr != NULL){
			compositePtr->EndAutoInitBlock();
		}
	}

	return true;
}


// protected methods

QApplication* CApplicationCompBase::GetQtApplication() const
{
	return m_applicationPtr.GetPtr();
}


bool CApplicationCompBase::TryShowSplashScreen()
{
	m_splashScreenTimer.Start();
	m_useSplashScreen = m_splashScreenCompPtr.IsValid() && m_splashScreenCompPtr->CreateGui(NULL);
	if (m_useSplashScreen){
		QWidget* splashWidgetPtr = m_splashScreenCompPtr->GetWidget();
		I_ASSERT(splashWidgetPtr != NULL);

		splashWidgetPtr->show();

		m_applicationPtr->processEvents();

		return true;
	}

	return false;
}


void CApplicationCompBase::HideSplashScreen()
{
	if (m_useSplashScreen){
		I_ASSERT(m_splashScreenCompPtr.IsValid());

		I_ASSERT(m_splashTimeAttrPtr.IsValid());
		m_splashScreenTimer.WaitTo(m_splashTimeAttrPtr->GetValue());

		QWidget* splashWidgetPtr = m_splashScreenCompPtr->GetWidget();
		I_ASSERT(splashWidgetPtr != NULL);

		splashWidgetPtr->hide();

		m_splashScreenCompPtr->DestroyGui();
	}
}


// reimplemented (icomp::CComponentBase)

void CApplicationCompBase::OnComponentCreated()
{
	icomp::ICompositeComponent* parentPtr = const_cast<icomp::ICompositeComponent*>(GetParentComponent(true));
	icomp::CCompositeComponent* compositePtr = dynamic_cast<icomp::CCompositeComponent*>(parentPtr);

	if (compositePtr != NULL){
		compositePtr->BeginAutoInitBlock();
	}

	BaseClass::OnComponentCreated();
}


// private methods

void CApplicationCompBase::SetStyleSheet(const QString& styleSheetFileName)
{
	I_ASSERT(m_applicationPtr != NULL);

	QFile styleSheetFile(styleSheetFileName);
	if (styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		QTextStream stream(&styleSheetFile);
		QString styleSheetText;
		QString textLine;
		do{
			textLine = stream.readLine();
			styleSheetText += textLine;
		} while (!textLine.isNull());

		m_applicationPtr->setStyleSheet(styleSheetText);
	}
}


} // namespace iqtgui



