#include "iqtgui/CGuiApplicationComp.h"


// Qt includes
#include <QApplication>
#include <QString>
#include <QVBoxLayout>
#include <QIcon>


namespace iqtgui
{


// reimplemented (iqtgui::IGuiApplication)

const iqtgui::IGuiObject* CGuiApplicationComp::GetApplicationGui() const
{
	if (m_mainGuiCompPtr.IsValid()){
		return m_mainGuiCompPtr.GetPtr();
	}

	return NULL;
}


// reimplemented (ibase::IApplication)

int CGuiApplicationComp::Execute(int argc, char** argv)
{
	int retVal = -1;

	if (BaseClass::InitializeApplication(argc, argv)){
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
		QApplication::setStyle(appStyle.c_str());

		TryShowSplashScreen();

		if (m_mainGuiCompPtr.IsValid()){
			if (m_frameSpaceSizeAttrPtr.IsValid()){
				m_mainWidgetPtr.SetPtr(new QWidget());
				QVBoxLayout* frameLayout = new QVBoxLayout(m_mainWidgetPtr.GetPtr());

				frameLayout->setMargin(*m_frameSpaceSizeAttrPtr);

				// create application's main widget:
				m_mainGuiCompPtr->CreateGui(m_mainWidgetPtr.GetPtr());
			}
			else{
				m_mainGuiCompPtr->CreateGui(NULL);
				
				m_mainWidgetPtr.SetPtr(m_mainGuiCompPtr->GetWidget());
			}

			m_mainWidgetPtr->setWindowTitle(QCoreApplication::applicationName());
			m_mainWidgetPtr->setWindowIcon(QApplication::windowIcon());
		}

		HideSplashScreen();

		if (m_mainWidgetPtr.IsValid()){
			int uiStartMode = 0;
			if (m_uiStartModeAttrPtr.IsValid()){
				uiStartMode = *m_uiStartModeAttrPtr;
			}

			switch (uiStartMode){
			case 1:
				m_mainWidgetPtr->showFullScreen();
				break;

			case 2:
				m_mainWidgetPtr->showMinimized();
				break;

			case 3:
				m_mainWidgetPtr->showMaximized();
				break;

			default:
				m_mainWidgetPtr->show();
			}
		}

		if (m_mainWidgetPtr.IsValid()){
			QApplication* applicationPtr = GetQtApplication();
			I_ASSERT(applicationPtr != NULL);	// application initialization was succesfull, it must be valid

			// start application loop:
			retVal = applicationPtr->exec();

			I_ASSERT(m_mainGuiCompPtr.IsValid());
			m_mainGuiCompPtr->DestroyGui();
		}
	}

	return retVal;
}


QString CGuiApplicationComp::GetHelpText() const
{
	return "-style QtStyle\tname of Qt-specified style";
}


} // namespace iqtgui



