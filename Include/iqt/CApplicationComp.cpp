#include "iqt/CApplicationComp.h"


#include <QApplication>
#include <QString>
#include <QIcon>

#include "iqt/CTimer.h"


namespace iqt
{


bool CApplicationComp::EnsureInitialized(int argc, char** argv)
{
	if (!m_applicationPtr.IsValid()){
		std::string appStyle;

		// parse arguments:
		for (int index = 1; index < argc; index++){
			std::string arg = argv[index];
			if (arg == "-style"){
				if (index+1 < argc){
					appStyle = argv[index+1];
				}
			}
		}

		m_applicationPtr.SetPtr(new QApplication(argc, argv));
		if (!m_applicationPtr.IsValid()){
			return false;
		}

		m_applicationPtr->setStyle(appStyle.c_str());
		m_applicationPtr->setWindowIcon(QIcon(":/Icons/acfLogo"));
	}

	return true;
}


// reimplemented (ibase::IApplication)

int CApplicationComp::Execute(int argc, char** argv)
{
	int retVal = -1;

	if (EnsureInitialized(argc, argv) && m_mainGuiCompPtr.IsValid()){
		iqt::CTimer timer;

		bool useSplashScreen = m_splashScreenCompPtr.IsValid() && m_splashScreenCompPtr->CreateGui(NULL);
		if (useSplashScreen){
			QWidget* splashWidgetPtr = m_splashScreenCompPtr->GetWidget();
			I_ASSERT(splashWidgetPtr != NULL);

			splashWidgetPtr->show();
		}

		m_mainGuiCompPtr->CreateGui(NULL);
		QWidget* mainWidgetPtr = m_mainGuiCompPtr->GetWidget();

		if (m_applicationInfoCompPtr.IsValid()){
			mainWidgetPtr->setWindowTitle(iqt::GetQString(m_applicationInfoCompPtr->GetApplicationName()));
		}
		else{
			mainWidgetPtr->setWindowTitle(QObject::tr("ACF application"));
		}

		if (useSplashScreen){
			I_ASSERT(m_splashTimeAttrPtr.IsValid());
			timer.WaitTo(m_splashTimeAttrPtr->GetValue());

			QWidget* splashWidgetPtr = m_mainGuiCompPtr->GetWidget();
			I_ASSERT(splashWidgetPtr != NULL);

			splashWidgetPtr->hide();

			m_splashScreenCompPtr->DestroyGui();
		}

		if (mainWidgetPtr != NULL){
			if (m_showMaximizedAttrPtr.IsValid() && m_showMaximizedAttrPtr->GetValue()){
				mainWidgetPtr->showMaximized();
			}
			else{
				mainWidgetPtr->show();
			}

			// start application loop:
			retVal = m_applicationPtr->exec();
		}

		m_mainGuiCompPtr->DestroyGui();
	}

	return retVal;
}


istd::CString CApplicationComp::GetHelpText() const
{
	return "-style QtStyle\tname of Qt-specified style";
}


// protected methods

} // namespace iqt



