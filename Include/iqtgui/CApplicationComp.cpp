#include "iqtgui/CApplicationComp.h"


// Qt includes
#include <QApplication>
#include <QString>
#include <QIcon>
#include <QStyle>
#include <QTextStream>
#include <QFile>
#include <QVBoxLayout>


// ACF includes
#include "icomp/CCompositeComponent.h"
#include "iqt/CTimer.h"


namespace iqtgui
{


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
			icon.addFile(":/Icons/AcfLogo.svg");
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

			if (m_applicationInfoCompPtr.IsValid()){
				QString format = iqt::GetQString(*m_titleFormatAttrPtr);
				QString applicationName = iqt::GetQString(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME));
				QString companyName = iqt::GetQString(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME));
				m_mainWidgetPtr->setWindowTitle(format.arg(applicationName).arg(companyName));
			}
			else{
				m_mainWidgetPtr->setWindowTitle(QObject::tr("ACF application"));
			}

			m_mainWidgetPtr->setWindowIcon(m_applicationPtr->windowIcon());
		}

		if (useSplashScreen){
			I_ASSERT(m_splashTimeAttrPtr.IsValid());
			timer.WaitTo(m_splashTimeAttrPtr->GetValue());

			QWidget* splashWidgetPtr = m_mainGuiCompPtr->GetWidget();
			I_ASSERT(splashWidgetPtr != NULL);

			splashWidgetPtr->hide();

			m_splashScreenCompPtr->DestroyGui();
		}

		if (m_mainWidgetPtr.IsValid()){
			int uiStartMode = 0;
			if (m_uiStartModeAttrPtr.IsValid()){
				uiStartMode = *m_uiStartModeAttrPtr;
			}

			switch (uiStartMode){
				case 0:
					m_mainWidgetPtr->show();
					break;
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


// protected methods

// reimplemented (icomp::CComponentBase)

void CApplicationComp::OnComponentCreated()
{
	icomp::IComponent* parentPtr = const_cast<icomp::IComponent*>(GetParentComponent(true));
	icomp::CCompositeComponent* compositePtr = dynamic_cast<icomp::CCompositeComponent*>(parentPtr);

	if (compositePtr != NULL){
		compositePtr->BeginAutoInitBlock();
	}

	BaseClass::OnComponentCreated();
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



