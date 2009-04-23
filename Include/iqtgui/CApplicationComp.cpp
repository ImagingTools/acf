#include "iqtgui/CApplicationComp.h"


#include <QApplication>
#include <QString>
#include <QIcon>
#include <QStyle>
#include <QTextStream>
#include <QFile>

#include "iqt/CTimer.h"


namespace iqtgui
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
		
		QIcon icon;
		icon.addFile(":/Icons/acfLogoSmall");
		icon.addFile(":/Icons/acfLogo3d");
		m_applicationPtr->setWindowIcon(icon);
	}

	return true;
}


// reimplemented (ibase::IApplication)

int CApplicationComp::Execute(int argc, char** argv)
{
	int retVal = -1;

	if (EnsureInitialized(argc, argv)){
		iqt::CTimer timer;

		m_applicationPtr->setPalette(QApplication::style()->standardPalette());

		// set style sheet for the application:
		if (m_styleSheetAttrPtr.IsValid()){
			SetStyleSheet(iqt::GetQString(*m_styleSheetAttrPtr));
		}

		// show splash screen:
		bool useSplashScreen = m_splashScreenCompPtr.IsValid() && m_splashScreenCompPtr->CreateGui(NULL);
		if (useSplashScreen){
			QWidget* splashWidgetPtr = m_splashScreenCompPtr->GetWidget();
			I_ASSERT(splashWidgetPtr != NULL);

			splashWidgetPtr->show();

			m_applicationPtr->processEvents();
		}

		QWidget* mainWidgetPtr = NULL;
		if (m_mainGuiCompPtr.IsValid()){
			// create application's main widget:
			m_mainGuiCompPtr->CreateGui(NULL);
			mainWidgetPtr = m_mainGuiCompPtr->GetWidget();

			if (m_applicationInfoCompPtr.IsValid()){
				mainWidgetPtr->setWindowTitle(iqt::GetQString(m_applicationInfoCompPtr->GetApplicationName()));
			}
			else{
				mainWidgetPtr->setWindowTitle(QObject::tr("ACF application"));
			}
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
			mainWidgetPtr->show();
		}

		m_freeComponentCompPtr.IsValid();

		if (mainWidgetPtr != NULL){
			// start application loop:
			retVal = m_applicationPtr->exec();

			I_ASSERT(m_mainGuiCompPtr.IsValid());
			m_mainGuiCompPtr->DestroyGui();
		}
	}

	return retVal;
}


istd::CString CApplicationComp::GetHelpText() const
{
	return "-style QtStyle\tname of Qt-specified style";
}


// private methods

void CApplicationComp::SetStyleSheet(const QString& styleSheetFileName)
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



