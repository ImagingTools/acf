#include "iabc/CServiceApplicationComp.h"


// Qt includes
#include <QVector>
#include <QMessageBox>
#include <QApplication>
#include <QSystemTrayIcon>


// ACF includes
#include "iqtgui/IGuiApplication.h"


namespace iabc
{


// public methods

QStringList CServiceApplicationComp::GetApplicationArguments(int argc, char** argv) const
{
	QStringList applicationArguments;

	for (int argIndex = 0; argIndex < argc; argIndex++){
		applicationArguments.push_back(QString().fromLocal8Bit(argv[argIndex]));
	}
	
	return applicationArguments;
}


// reimplemented (ibase::IApplication)

bool CServiceApplicationComp::InitializeApplication(int/* argc*/, char** /* argv*/)
{
	return true;
}


int CServiceApplicationComp::Execute(int argc, char** argv)
{
	I_ASSERT(m_serviceNameAttrPtr.IsValid());
	I_ASSERT(m_serviceDescriptionAttrPtr.IsValid());

	if (!m_serviceDescriptionAttrPtr.IsValid() || !m_serviceNameAttrPtr.IsValid()){
		return -1;
	}

	QString serviceName = iqt::GetQString(*m_serviceNameAttrPtr);

	if (m_applicationCompPtr.IsValid()){
		// check whether the service already installed, if not, then install it:
		QtServiceController controller(serviceName);
		if (!controller.isInstalled()){
			// for installation we need 3 additional arguments: -i for install, account and password (last both we leave always empty)
			// standard Qt mechanisms don't allow definition of a service with command line parameters, so we need some workaround:
			QVector<char*> instalArguments(argc + 3); 
			QList<QByteArray> argvData;
			for (int i = 0; i < argc; ++i){
				argvData.append(argv[i]);
			}

			// copy first argument:
			instalArguments[0] = argv[0];

			// insert install arguments needed by Qt for service creation:
			instalArguments[1] = "-i";
			instalArguments[2] = "";
			instalArguments[3] = "";
			
			// append user defined application's arguments:
			for (int i = 4; i < argc + 3; ++i){
				instalArguments[i] = argvData[i - 3].data();
			}

			m_servicePtr.SetPtr(new CService(
						*this,
						*m_applicationCompPtr.GetPtr(),
						argc + 3,
						instalArguments.data(),
						serviceName));
			
			m_servicePtr->setServiceDescription(iqt::GetQString(*m_serviceDescriptionAttrPtr));

			QtServiceController::StartupType serviceStartUp = QtServiceController::AutoStartup;
			if (m_manualStartupAttrPtr.IsValid() && *m_manualStartupAttrPtr){
				serviceStartUp = QtServiceController::ManualStartup;
			}
			m_servicePtr->setStartupType(serviceStartUp);

			// start service installing:
			int retVal = m_servicePtr->exec();
			if (retVal == 0){
				QApplication* appPtr = dynamic_cast<QApplication*>(QCoreApplication::instance());
				QString installMessage = QString(QCoreApplication::tr("Service: %1 was succesfully installed.\nNow you can start the service from the Service Manager.")).arg(serviceName);
				if (appPtr != NULL){				
					QMessageBox::information(
								NULL,
								QCoreApplication::tr("ACF Service Application"), installMessage);
				}
				else{
					SendInfoMessage(0, iqt::GetCString(installMessage));
				}
			}

			return retVal;
		}

		SendInfoMessage(0, istd::CString("Application arguments: ") + iqt::GetCString(GetApplicationArguments(argc, argv).join(" ")));

		m_servicePtr.SetPtr(new CService(
					*this,
					*m_applicationCompPtr.GetPtr(),
					argc,
					argv,
					serviceName));

		return m_servicePtr->exec();
	}

	return -1;
}


istd::CString CServiceApplicationComp::GetHelpText() const
{
	// TODO: Combine service and application related  usages.
	if (m_applicationCompPtr.IsValid()){
		return m_applicationCompPtr->GetHelpText();
	}

	return istd::CString();
}


// reimplemented (QObject)
	
bool CServiceApplicationComp::eventFilter(QObject* sourceObject, QEvent* eventPtr)
{
	I_ASSERT(m_applicationCompPtr.IsValid());
	iqtgui::IGuiApplication* guiAppPtr = dynamic_cast<iqtgui::IGuiApplication*>(m_applicationCompPtr.GetPtr());
	if (guiAppPtr != NULL){
		const iqtgui::IGuiObject* mainWdigetPtr = guiAppPtr->GetApplicationGui();
		if (mainWdigetPtr != NULL && mainWdigetPtr->IsGuiCreated()){
			QWidget* widgetPtr = mainWdigetPtr->GetWidget();
			I_ASSERT(widgetPtr != NULL);

			// prevent closing of the main widget:
			if ((widgetPtr == sourceObject) && eventPtr->type() == QEvent::Close){
				eventPtr->ignore();

				widgetPtr->hide();

				return true;	
			}
		}
	}

	return false;
}



// protected slots:

void CServiceApplicationComp::OnTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	I_ASSERT(m_applicationCompPtr.IsValid());

	if (reason == QSystemTrayIcon::DoubleClick){
		iqtgui::IGuiApplication* guiAppPtr = dynamic_cast<iqtgui::IGuiApplication*>(m_applicationCompPtr.GetPtr());
		if (guiAppPtr != NULL){
			const iqtgui::IGuiObject* mainWdigetPtr = guiAppPtr->GetApplicationGui();
			if (mainWdigetPtr != NULL && mainWdigetPtr->IsGuiCreated()){
				QWidget* widgetPtr = mainWdigetPtr->GetWidget();
				I_ASSERT(widgetPtr != NULL);
				
				if (widgetPtr->isHidden()){
					widgetPtr->show();
				}
				else{
					widgetPtr->activateWindow();
				}
			}
		}	
	}
}


// public methods of embedded class CService

CServiceApplicationComp::CService::CService(
			CServiceApplicationComp& parent,
			ibase::IApplication& application,
			int serviceArgc,
			char** serviceArgv,
			const QString &name)
	:QtServiceBase(serviceArgc, serviceArgv, name),
	m_parent(parent),
	m_application(application)
{
	m_applicationArguments = m_parent.GetApplicationArguments(serviceArgc, serviceArgv);

	setServiceFlags(QtServiceBase::CanBeSuspended);
}


// protected methods of embedded class CService

// reimplemented (QtServiceBase)

void CServiceApplicationComp::CService::start()
{
	iqtgui::IGuiApplication* guiAppPtr = dynamic_cast<iqtgui::IGuiApplication*>(&m_application);
	if (guiAppPtr != NULL){
		const iqtgui::IGuiObject* mainWdigetPtr = guiAppPtr->GetApplicationGui();
		if (mainWdigetPtr != NULL && mainWdigetPtr->IsGuiCreated()){
			QWidget* widgetPtr = mainWdigetPtr->GetWidget();
			I_ASSERT(widgetPtr != NULL);
			QSystemTrayIcon* trayIcon = new QSystemTrayIcon(widgetPtr);
			connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), &m_parent, SLOT(OnTrayIconActivated(QSystemTrayIcon::ActivationReason)));

			trayIcon->setIcon(widgetPtr->windowIcon());

			trayIcon->setVisible(true);

			widgetPtr->installEventFilter(&m_parent);

			widgetPtr->hide();
		}
	}
}


void CServiceApplicationComp::CService::stop()
{
}


void CServiceApplicationComp::CService::pause()
{
	iqtgui::IGuiApplication* guiAppPtr = dynamic_cast<iqtgui::IGuiApplication*>(&m_application);
	if (guiAppPtr != NULL){
		const iqtgui::IGuiObject* mainWdigetPtr = guiAppPtr->GetApplicationGui();
		if (mainWdigetPtr != NULL && mainWdigetPtr->IsGuiCreated()){
			mainWdigetPtr->GetWidget()->hide();
		}
	}
}


void CServiceApplicationComp::CService::resume()
{
	iqtgui::IGuiApplication* guiAppPtr = dynamic_cast<iqtgui::IGuiApplication*>(&m_application);
	if (guiAppPtr != NULL){
		const iqtgui::IGuiObject* mainWdigetPtr = guiAppPtr->GetApplicationGui();
		if (mainWdigetPtr != NULL && mainWdigetPtr->IsGuiCreated()){
			mainWdigetPtr->GetWidget()->show();
		}
	}
}


void CServiceApplicationComp::CService::createApplication(int&/*argc*/, char** /* argv*/)
{
	QVector<char*> argv = GetApplicationArguments();

	m_application.InitializeApplication(argv.count(), argv.data());
}


int CServiceApplicationComp::CService::executeApplication()
{
	QVector<char*> argv = GetApplicationArguments();

	return m_application.Execute(argv.count(), argv.data());
}


// private methods of embedded class CService

QVector<char*> CServiceApplicationComp::CService::GetApplicationArguments() const
{
	QVector<char*> argv(m_applicationArguments.count());

	for (int argIndex = 0; argIndex < m_applicationArguments.count(); argIndex++){
		argv[argIndex] = m_applicationArguments[argIndex].toLocal8Bit().data();
	}

	return argv;
}


} // namespace iabc


