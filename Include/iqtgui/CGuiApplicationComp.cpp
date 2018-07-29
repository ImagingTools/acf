#include <iqtgui/CGuiApplicationComp.h>


// Qt includes
#include <QtCore/QString>
#include <QtCore/QtGlobal>
#include <QtGui/QIcon>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#else
#include <QtGui/QApplication>
#include <QtGui/QVBoxLayout>
#endif

// ACF includes
#include <istd/CSystem.h>
#include <iqtgui/CCommandTools.h>
#include <iqtgui/CHierarchicalCommand.h>


#if defined(Q_OS_MACX) && (QT_VERSION >= 0x040000) && (QT_VERSION < 0x050000)
void qt_mac_set_menubar_icons(bool enable);
#endif


namespace iqtgui
{


// public methods

CGuiApplicationComp::CGuiApplicationComp()
	:m_defaultWidgetFlags(0),
	m_trayMessages(*this)
{
}


// reimplemented (iqtgui::IGuiApplication)

const iqtgui::IGuiObject* CGuiApplicationComp::GetApplicationGui() const
{
	if (m_mainGuiCompPtr.IsValid()){
		return m_mainGuiCompPtr.GetPtr();
	}

	return NULL;
}


// reimplemented (ibase::IApplication)

bool CGuiApplicationComp::InitializeApplication(int argc, char** argv)
{
#if QT_VERSION > 0x050000
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	qputenv("QT_HIDPI_AWARE", "1");
	qputenv("QT_HIGHDPI_AWARE", "1");
#endif

#if QT_VERSION > 0x050600
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

#if defined(Q_OS_MACX) && (QT_VERSION >= 0x040000) && (QT_VERSION < 0x050000)
	QCoreApplication::setAttribute(Qt::AA_DontShowIconsInMenus, !*m_useMenuIconsOnMacAttrPtr);
	qt_mac_set_menubar_icons(*m_useMenuIconsOnMacAttrPtr);
#endif

	if (BaseClass::InitializeApplication(argc, argv)){
		return true;
	}

	return false;
}


int CGuiApplicationComp::Execute(int argc, char** argv)
{
	int retVal = -1;

	if (BaseClass::InitializeApplication(argc, argv)){
		m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_STARTING);

		TryShowSplashScreen();

		if (m_allowApplicationCloseModelCompPtr.IsValid()){
			m_allowApplicationCloseModelCompPtr->AttachObserver(this);
		}

		if (m_mainGuiCompPtr.IsValid()){
			if (m_frameSpaceSizeAttrPtr.IsValid()){
				m_mainWidgetPtr.SetPtr(new QWidget());

				QVBoxLayout* frameLayout = new QVBoxLayout(m_mainWidgetPtr.GetPtr());

				frameLayout->setMargin(*m_frameSpaceSizeAttrPtr);

				// Create application's main widget:
				m_mainGuiCompPtr->CreateGui(m_mainWidgetPtr.GetPtr());
			}
			else{
				m_mainGuiCompPtr->CreateGui(NULL);

				m_mainWidgetPtr.SetPtr(m_mainGuiCompPtr->GetWidget());
			}


			QString operationSystemName = istd::CSystem::GetOperationSystemName();
			m_mainWidgetPtr->setProperty("OperatingSystem", operationSystemName);

#if defined(Q_OS_OSX)
			m_mainWidgetPtr->setAttribute(Qt::WA_MacNormalSize, true);
#endif

			m_mainWidgetPtr->setWindowTitle(QCoreApplication::applicationName());
			m_mainWidgetPtr->setWindowIcon(QApplication::windowIcon());
		}

		HideSplashScreen();

		if (*m_useTrayIconAttrPtr){
			m_trayIconPtr.SetPtr(new QSystemTrayIcon);
#if QT_VERSION >= 0x050000
			m_trayIconPtr->setIcon(QGuiApplication::windowIcon());
#else
			m_trayIconPtr->setIcon(QApplication::windowIcon());
#endif

			QMenu* trayMenuPtr = new QMenu;

			if (m_trayIconCommandsCompPtr.IsValid()){
				const iqtgui::CHierarchicalCommand* commandPtr = dynamic_cast<const iqtgui::CHierarchicalCommand*>(m_trayIconCommandsCompPtr->GetCommands());
				if (commandPtr != NULL){
					iqtgui::CCommandTools::CreateMenu<QMenu>(*commandPtr, *trayMenuPtr);
				}
			}

			trayMenuPtr->addSeparator();

			trayMenuPtr->addAction(tr("Quit"), this, SLOT(OnQuit()));

			m_trayIconPtr->setContextMenu(trayMenuPtr);

			m_trayIconPtr->setVisible(true);
		}

		if (m_mainWidgetPtr.IsValid()){
			m_defaultWidgetFlags = m_mainWidgetPtr->windowFlags();

			UpdateMainWidgetDecorations();

			ShowWindow();

			m_lastWidgetGeometry = m_mainWidgetPtr->geometry();

			m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_RUNNING);

			// Start application loop:
			retVal = QApplication::exec();

			m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_SHUTDOWN);

			Q_ASSERT(m_mainGuiCompPtr.IsValid());
			m_mainGuiCompPtr->DestroyGui();

			m_mainWidgetPtr.Reset();
		}
		else{
			if (m_trayIconPtr.IsValid()){
				m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_RUNNING);

				// Start application loop:
				retVal = QApplication::exec();

				m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_SHUTDOWN);
			}
		}
	}

	if (m_trayIconPtr.IsValid()){
		QMenu* trayMenuPtr = m_trayIconPtr->contextMenu();
		if (trayMenuPtr != NULL){
			delete trayMenuPtr;
		}
	}

	m_trayIconPtr.Reset();

	return retVal;
}


QString CGuiApplicationComp::GetHelpText() const
{
	return "-style QtStyle\tname of Qt-specified style";
}


// protected methods

// reimplemented (imod::TSingleModelObserverBase)

void CGuiApplicationComp::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	if (m_mainWidgetPtr.IsValid()){
		m_lastWidgetGeometry = m_mainWidgetPtr->geometry();

		UpdateMainWidgetDecorations();

		ShowWindow();

		if (		!m_mainWidgetPtr->isMaximized() &&
					!m_mainWidgetPtr->isMinimized() &&
					!m_mainWidgetPtr->isFullScreen()){
			m_mainWidgetPtr->setGeometry(m_lastWidgetGeometry);
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CGuiApplicationComp::OnComponentDestroyed()
{
	BaseClass2::EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// private methods

void CGuiApplicationComp::UpdateMainWidgetDecorations()
{
	if (m_allowApplicationCloseCompPtr.IsValid() && m_mainWidgetPtr.IsValid()){
		Qt::WindowFlags windowFlags = m_defaultWidgetFlags;

		if (!m_allowApplicationCloseCompPtr->IsEnabled()){
			windowFlags = (m_mainWidgetPtr->windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint;
		}

		if (windowFlags != m_mainWidgetPtr->windowFlags()){
			m_mainWidgetPtr->setWindowFlags(windowFlags);

#if QT_VERSION >= 0x050000
			// workaround to go full screen after start (Windows, Qt 5.6 - 5.11)
			m_mainWidgetPtr->showMaximized();
#endif
		}
	}
}


void CGuiApplicationComp::ShowWindow()
{
	Q_ASSERT(m_mainWidgetPtr.IsValid());

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


// private slots

void CGuiApplicationComp::OnQuit()
{
	if (m_mainWidgetPtr.IsValid()){
		m_mainWidgetPtr->close();
	}
	else{
		QCoreApplication::quit();
	}
}


// public methods of the embedded class TrayMessages

CGuiApplicationComp::TrayMessages::TrayMessages(CGuiApplicationComp& parent)
	:m_parent(parent)
{
}


// reimplemented (ilog::IMessageConsumer)

bool CGuiApplicationComp::TrayMessages::IsMessageSupported(
			int /*messageCategory*/,
			int /*messageId*/,
			const istd::IInformationProvider* /*messagePtr*/) const
{
	return m_parent.m_trayIconPtr.IsValid();
}


void CGuiApplicationComp::TrayMessages::AddMessage(const MessagePtr& messagePtr)
{
	if (m_parent.m_trayIconPtr.IsValid()){
		QSystemTrayIcon::MessageIcon severityIcon = QSystemTrayIcon::NoIcon;

		switch (messagePtr->GetInformationCategory()){
		case istd::IInformationProvider::IC_INFO:
			severityIcon = QSystemTrayIcon::Information;
			break;

		case istd::IInformationProvider::IC_WARNING:
			severityIcon = QSystemTrayIcon::Warning;
			break;

		case istd::IInformationProvider::IC_ERROR:
		case istd::IInformationProvider::IC_CRITICAL:
			severityIcon = QSystemTrayIcon::Critical;
			break;

		default:
			break;
		}

		m_parent.m_trayIconPtr->showMessage(QCoreApplication::applicationName(), messagePtr->GetInformationDescription(), severityIcon);
	}
}


} // namespace iqtgui


