#include "iqtgui/CApplicationComp.h"


#include <QApplication>
#include <QString>
#include <QIcon>
#include <QStyle>
#include <QTextStream>
#include <QFile>
#include <QVBoxLayout>

#include "icomp/CCompositeComponent.h"
#include "iqt/CTimer.h"


namespace iqtgui
{


// reimplemented (icomp::IComponent)

void CApplicationComp::OnComponentCreated()
{
	icomp::IComponent* parentPtr = const_cast<icomp::IComponent*>(GetParentComponent(true));
	icomp::CCompositeComponent* compositePtr = dynamic_cast<icomp::CCompositeComponent*>(parentPtr);

	if (compositePtr != NULL){
		compositePtr->BeginAutoInitBlock();
	}

	BaseClass::OnComponentCreated();
}


// reimplemented (iqtgui::IGuiApplication)

const iqtgui::IGuiObject* CApplicationComp::GetApplicationGui() const
{
	if (m_mainGuiCompPtr.IsValid()){
		return m_mainGuiCompPtr.GetPtr();
	}

	return NULL;
}


// reimplemented (ibase::IApplication)

bool CApplicationComp::InitializeApplication(int argc, char** argv)
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
		if (m_iconPathAttrPtr.IsValid()){
			icon = QIcon(iqt::GetQString(*m_iconPathAttrPtr));
		}
		else{	
			icon.addFile(":/Icons/AcfLogoSmall");
			icon.addFile(":/Icons/acfLogo3d");
		}

		m_applicationPtr->setWindowIcon(icon);

		icomp::IComponent* parentPtr = const_cast<icomp::IComponent*>(GetParentComponent(true));
		icomp::CCompositeComponent* compositePtr = dynamic_cast<icomp::CCompositeComponent*>(parentPtr);

		if (compositePtr != NULL){
			compositePtr->EndAutoInitBlock();
		}
	}

	return true;
}


int CApplicationComp::Execute(int argc, char** argv)
{
	int retVal = -1;

	if (InitializeApplication(argc, argv)){
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
			if (m_frameSpaceSizeAttrPtr.IsValid()){
				mainWidgetPtr = new QWidget();
				QVBoxLayout* frameLayout = new QVBoxLayout(mainWidgetPtr);

				frameLayout->setMargin(*m_frameSpaceSizeAttrPtr);

				// create application's main widget:
				m_mainGuiCompPtr->CreateGui(mainWidgetPtr);
			}
			else{
				m_mainGuiCompPtr->CreateGui(NULL);
				mainWidgetPtr = m_mainGuiCompPtr->GetWidget();
			}

			if (m_applicationInfoCompPtr.IsValid()){
				QString format = iqt::GetQString(*m_titleFormatAttrPtr);
				QString applicationName = iqt::GetQString(m_applicationInfoCompPtr->GetApplicationName());
				QString companyName = iqt::GetQString(m_applicationInfoCompPtr->GetCompanyName());
				mainWidgetPtr->setWindowTitle(format.arg(applicationName).arg(companyName));
			}
			else{
				mainWidgetPtr->setWindowTitle(QObject::tr("ACF application"));
			}

			mainWidgetPtr->setWindowIcon(m_applicationPtr->windowIcon());
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
			int uiStartMode = 0;
			if (m_uiStartModeAttrPtr.IsValid()){
				uiStartMode = *m_uiStartModeAttrPtr;
			}

			switch (uiStartMode){
				case 0:
					mainWidgetPtr->show();
					break;
				case 1:
					mainWidgetPtr->showFullScreen();
					break;
				case 2:
					mainWidgetPtr->showMinimized();
					break;
				case 3:
					mainWidgetPtr->showMaximized();
					break;
				default:
					mainWidgetPtr->show();
			}
		}

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



