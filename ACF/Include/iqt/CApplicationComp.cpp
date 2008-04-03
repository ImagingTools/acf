#include "iqt/CApplicationComp.h"


#include <QApplication>

#include "iqt/CTimer.h"


namespace iqt
{


// reimplemented (ibase::IApplication)

int CApplicationComp::Execute(int argc, char** argv)
{
	int retVal = -1;

	if (m_mainGuiCompPtr.IsValid()){
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

		QApplication application(argc, argv);
		application.setStyle(appStyle.c_str());

		iqt::CTimer timer;

		bool useSplashScreen = m_splashScreenCompPtr.IsValid() && m_splashScreenCompPtr->CreateGui(NULL);
		if (useSplashScreen){
			QWidget* splashWidgetPtr = m_splashScreenCompPtr->GetWidget();
			I_ASSERT(splashWidgetPtr != NULL);

			splashWidgetPtr->show();
		}

		m_mainGuiCompPtr->CreateGui(NULL);
		QWidget* mainWidgetPtr = m_mainGuiCompPtr->GetWidget();

		if (useSplashScreen){
			I_ASSERT(m_splashTimeAttrPtr.IsValid());
			timer.WaitTo(m_splashTimeAttrPtr->GetValue());

			QWidget* splashWidgetPtr = m_mainGuiCompPtr->GetWidget();
			I_ASSERT(splashWidgetPtr != NULL);

			splashWidgetPtr->hide();

			m_splashScreenCompPtr->DestroyGui();
		}

		if (mainWidgetPtr != NULL){
			mainWidgetPtr->show();

			// start application loop:
			retVal = application.exec();
		}

		m_mainGuiCompPtr->DestroyGui();
	}

	return retVal;
}


istd::CString CApplicationComp::GetHelpText() const
{
	return "-style QtStyle\tname of Qt-specified style";
}


} // namespace iqt



