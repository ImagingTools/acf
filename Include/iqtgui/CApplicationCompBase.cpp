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
	m_applicationArguments.clear();

	for (int argIndex = 0; argIndex < argc; argIndex++){
		m_applicationArguments << QString::fromLocal8Bit(argv[argIndex]);
	}

	if (!m_applicationPtr.IsValid()){
		m_applicationPtr.SetPtr(new QApplication(argc, argv));
		if (!m_applicationPtr.IsValid()){
			return false;
		}

		if (IsComponentActive()){
			InitializeComponentApplication();
		}

		icomp::CCompositeComponent* rootComponentPtr = NULL;
		
		for (		icomp::ICompositeComponent* componentPtr = const_cast<icomp::ICompositeComponent*>(GetParentComponent(true));
					componentPtr != NULL;
					componentPtr = const_cast<icomp::ICompositeComponent*>(componentPtr->GetParentComponent(true))){
			rootComponentPtr = dynamic_cast<icomp::CCompositeComponent*>(componentPtr);
		}

		if (rootComponentPtr != NULL){
			rootComponentPtr->EnsureAutoInitComponentsCreated();
		}
	}

	return true;
}


QStringList CApplicationCompBase::GetApplicationArguments() const
{
	return m_applicationArguments;
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


void CApplicationCompBase::InitializeComponentApplication()
{
	Q_ASSERT(IsComponentActive());
	Q_ASSERT(m_applicationPtr.IsValid());

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
		QString mainVersionText = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_MAIN_VERSION, false);
		m_applicationPtr->setApplicationVersion(mainVersionText);

		QString companyNameText = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME, false);
		m_applicationPtr->setOrganizationName(companyNameText);
	}

	// set up palette
	m_applicationPtr->setPalette(QApplication::style()->standardPalette());

	// set up style sheet
	if (m_styleSheetAttrPtr.IsValid()){
		if (!iqtgui::SetStyleSheet(*m_applicationPtr, *m_styleSheetAttrPtr)){
			qDebug("Style sheet file could not be set: %s", (*m_styleSheetAttrPtr).toLocal8Bit().constData());
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CApplicationCompBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_applicationPtr.IsValid()){
		InitializeComponentApplication();
	}
}


} // namespace iqtgui



