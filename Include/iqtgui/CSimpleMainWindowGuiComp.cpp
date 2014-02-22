#include "iqtgui/CSimpleMainWindowGuiComp.h"


// Qt includes
#include <QtCore/QSettings>

// ACF includes
#include "iqt/CSignalBlocker.h"


namespace iqtgui
{


// public methods

CSimpleMainWindowGuiComp::CSimpleMainWindowGuiComp()
	:m_commandsObserver(*this),
	m_menuCommands("Global"),
	m_showToolBarsCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_ONOFF),
	m_settingsCommand("", 200, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	m_showOtherWindows("", 300)
{
	connect(&m_showToolBarsCommand, SIGNAL(triggered()), this, SLOT(OnShowToolbars()));
	connect(&m_settingsCommand, SIGNAL(triggered()), this, SLOT(OnSettings()));
	connect(&m_aboutCommand, SIGNAL(triggered()), this, SLOT(OnAbout()));

	m_showOtherWindows.SetGroupId(0x320);
}


// reimplemented (iqtgui::IGuiObject)

void CSimpleMainWindowGuiComp::OnTryClose(bool* ignoredPtr)
{
	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->OnTryClose(ignoredPtr);

		return;
	}

	BaseClass::OnTryClose(ignoredPtr);
}


// protected methods

void CSimpleMainWindowGuiComp::UpdateMenuActions()
{
	if (!IsGuiCreated()){
		return;
	}

	if (!m_menuBarPtr.IsValid() && !m_standardToolBarPtr.IsValid()){
		return;
	}

	m_menuCommands.ResetChilds();

	m_viewCommand.SetPriority(90);
	m_toolsCommand.SetPriority(120);
	m_helpCommand.SetPriority(150);

	UpdateFixedCommands(m_fixedCommands);

	UpdateMenuActions(m_menuCommands);

	if (m_menuBarPtr.IsValid()){
		m_menuBarPtr->clear();
		CCommandTools::CreateMenu(m_menuCommands, *m_menuBarPtr);
	}

	if (m_standardToolBarPtr.IsValid()){
		m_standardToolBarPtr->clear();
		
		CCommandTools::SetupToolbar(m_menuCommands, *m_standardToolBarPtr);
		
		m_standardToolBarPtr->setVisible(true);
	}
}


void CSimpleMainWindowGuiComp::SetupMainWindowComponents(QMainWindow& mainWindow)
{
	if (m_standardToolBarPtr.IsValid()){
		Qt::ToolBarArea toolBarArea = Qt::TopToolBarArea;
		if (m_toolBarAreaAttrPtr.IsValid()){
			switch (*m_toolBarAreaAttrPtr){
				case 0:
					toolBarArea = Qt::LeftToolBarArea;
					break;

				case 1:
					toolBarArea = Qt::RightToolBarArea;
					break;
				case 2:
					toolBarArea = Qt::TopToolBarArea;
					break;

				case 3:
					toolBarArea = Qt::BottomToolBarArea;
					break;
				default:
					break;
			}
		}

		mainWindow.addToolBar(toolBarArea, m_standardToolBarPtr.GetPtr());

		m_toolBarsList.PushBack(m_standardToolBarPtr.GetPtr(), false);
	}

	if (m_menuBarPtr.IsValid()){
		mainWindow.setMenuBar(m_menuBarPtr.GetPtr());
	}

	for (int componentIndex = 0; componentIndex < m_mainWindowComponentsCompPtr.GetCount(); componentIndex++){
		iqtgui::IMainWindowComponent* mainWindowComponentPtr =  m_mainWindowComponentsCompPtr[componentIndex];
		if (mainWindowComponentPtr != NULL){
			AddMainComponent(mainWindowComponentPtr);
		}
	}
}


void CSimpleMainWindowGuiComp::AddMainComponent(iqtgui::IMainWindowComponent* componentPtr)
{
	QMainWindow* mainWindowPtr = GetQtWidget();
	if (mainWindowPtr != NULL){
		componentPtr->AddToMainWindow(*mainWindowPtr);

		iqtgui::IGuiObject* guiPtr =  dynamic_cast<iqtgui::IGuiObject*>(componentPtr);
		if (guiPtr != NULL){
			QToolBar* toolBarComponentPtr = dynamic_cast<QToolBar*>(guiPtr->GetWidget());
			if (toolBarComponentPtr != NULL){
				m_toolBarsList.PushBack(toolBarComponentPtr, false);

				if (m_standardToolBarPtr.IsValid()){
					toolBarComponentPtr->setToolButtonStyle(m_standardToolBarPtr->toolButtonStyle());
				}
			}
		}
	}
}


void CSimpleMainWindowGuiComp::RemoveMainComponent(iqtgui::IMainWindowComponent* componentPtr)
{
	QMainWindow* mainWindowPtr = GetQtWidget();
	if (mainWindowPtr != NULL){
		componentPtr->RemoveFromMainWindow(*mainWindowPtr);
	}
}


void CSimpleMainWindowGuiComp::CreateMenuBar()
{
	Q_ASSERT(m_isMenuVisibleAttrPtr.IsValid());	// is obligatory attribute
	if (*m_isMenuVisibleAttrPtr){
		QMainWindow* mainWindowPtr = GetQtWidget();
		if (mainWindowPtr != NULL){
			m_menuBarPtr.SetPtr(new QMenuBar(mainWindowPtr));
		}
	}
}


void CSimpleMainWindowGuiComp::CreateDefaultToolBar()
{
	Q_ASSERT(m_isToolbarVisibleAttrPtr.IsValid());	// is obligatory attribute
	if (*m_isToolbarVisibleAttrPtr){
		QMainWindow* mainWindowPtr = GetQtWidget();
		if (mainWindowPtr != NULL){
			m_standardToolBarPtr.SetPtr(new QToolBar(mainWindowPtr));

			m_standardToolBarPtr->setWindowTitle(tr("Standard"));
			m_standardToolBarPtr->setObjectName("StandardToolBar");
			m_standardToolBarPtr->toggleViewAction()->setVisible(false);

			if (m_useIconTextAttrPtr.IsValid() && m_useIconTextAttrPtr->GetValue()){
				m_standardToolBarPtr->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
			}

			m_standardToolBarPtr->setFloatable(*m_toolBarDockFeaturesAttrPtr & DF_FLOATABLE);
			m_standardToolBarPtr->setMovable(*m_toolBarDockFeaturesAttrPtr & DF_MOVEABLE);
		}
	}
}


void CSimpleMainWindowGuiComp::SetToolBarsVisible(bool isVisible)
{
	QMainWindow* mainWindowPtr = GetQtWidget();
	Q_ASSERT(mainWindowPtr != NULL);

	if (mainWindowPtr != NULL){
		for (int toolbarIndex = 0; toolbarIndex < m_toolBarsList.GetCount(); toolbarIndex++){
			QToolBar* toolbarPtr = m_toolBarsList.GetAt(toolbarIndex);

			toolbarPtr->setVisible(isVisible);
		}
	}

	iqt::CSignalBlocker block(&m_showToolBarsCommand);

	m_showToolBarsCommand.setChecked(isVisible);
}


void CSimpleMainWindowGuiComp::SetupMenu()
{
	m_menuCommands.ResetChilds();

	m_menuCommands.InsertChild(&m_fixedCommands, false);

	UpdateFixedCommands(m_fixedCommands);
}


void CSimpleMainWindowGuiComp::UpdateFixedCommands(iqtgui::CHierarchicalCommand& fixedCommands)
{
	fixedCommands.ResetChilds();

	UpdateViewCommands(m_viewCommand);
	if (m_viewCommand.GetChildsCount() > 0){
		fixedCommands.InsertChild(&m_viewCommand, false);
	}


	UpdateToolsCommands(m_toolsCommand); 
	if (m_toolsCommand.GetChildsCount() > 0){
		fixedCommands.InsertChild(&m_toolsCommand, false);
	}

	UpdateHelpCommands(m_helpCommand);
	if (m_helpCommand.GetChildsCount() > 0){
		fixedCommands.InsertChild(&m_helpCommand, false);
	}
}


void CSimpleMainWindowGuiComp::UpdateViewCommands(iqtgui::CHierarchicalCommand& viewCommand)
{
	viewCommand.ResetChilds();

	if (*m_isToolbarVisibleAttrPtr){
		viewCommand.InsertChild(&m_showToolBarsCommand, false);
	}

	viewCommand.InsertChild(&m_showOtherWindows, false);
}


void CSimpleMainWindowGuiComp::UpdateToolsCommands(iqtgui::CHierarchicalCommand& toolsCommand)
{
	toolsCommand.ResetChilds();

	if (m_settingsDialogCompPtr.IsValid()){
		m_settingsCommand.SetGroupId(GI_SETTINGS);
		toolsCommand.InsertChild(&m_settingsCommand, false);
	}
}


void CSimpleMainWindowGuiComp::UpdateHelpCommands(iqtgui::CHierarchicalCommand& helpCommand)
{
	if (m_aboutDialogCompPtr.IsValid()){
		helpCommand.InsertChild(&m_aboutCommand, false);
	}
}


void CSimpleMainWindowGuiComp::UpdateMenuActions(iqtgui::CHierarchicalCommand& menuCommands)
{
	menuCommands.JoinLinkFrom(&m_fixedCommands);

	if (m_workspaceCommandsCompPtr.IsValid()){
		const ibase::IHierarchicalCommand* commandsPtr = m_workspaceCommandsCompPtr->GetCommands();
		if (commandsPtr != NULL){
			menuCommands.JoinLinkFrom(commandsPtr);
		}
	}

	int mainWindowsCommandsCount = m_mainWindowCommandsCompPtr.GetCount();
	for (int mainCommIndex = 0; mainCommIndex < mainWindowsCommandsCount; ++mainCommIndex){
		const ibase::ICommandsProvider* providerPtr = m_mainWindowCommandsCompPtr[mainCommIndex];
		if (providerPtr != NULL){
			const ibase::IHierarchicalCommand* commandsPtr = providerPtr->GetCommands();
			if (commandsPtr != NULL){
				menuCommands.JoinLinkFrom(commandsPtr);
			}
		}
	}
}


// reimplemented (TRestorableGuiWrap)

void CSimpleMainWindowGuiComp::OnRestoreSettings(const QSettings& settings)
{
	Q_ASSERT(IsGuiCreated());
	QMainWindow* mainWindowPtr = GetQtWidget();
	Q_ASSERT(mainWindowPtr != NULL);

	QByteArray windowState = settings.value("MainWindow/State").toByteArray();
	QByteArray windowGeometry = settings.value("MainWindow/Geometry").toByteArray();

	mainWindowPtr->restoreState(windowState);
	mainWindowPtr->restoreGeometry(windowGeometry);

	for (int i = 0; i < m_mainWindowComponentsCompPtr.GetCount(); ++i){
		iqtgui::IMainWindowComponent* mainWindowComponentPtr = m_mainWindowComponentsCompPtr[i];
		if ((mainWindowComponentPtr != NULL) && ((mainWindowComponentPtr->GetFlags() & iqtgui::IMainWindowComponent::WCF_PERMANENT) == 0)){
			iqtgui::IGuiObject* guiPtr = dynamic_cast<iqtgui::IGuiObject*>(mainWindowComponentPtr);
			if ((guiPtr != NULL) && guiPtr->IsGuiCreated()){
				QWidget* widgetPtr = guiPtr->GetWidget();
				QWidget* parentPtr = widgetPtr->parentWidget();
				bool isVisibleToParent = false;
				if (parentPtr != NULL){
					isVisibleToParent = widgetPtr->isVisibleTo(parentPtr);
				}

				widgetPtr->installEventFilter(this);

				m_mainComponentVisibilityMap[mainWindowComponentPtr] = isVisibleToParent;

				iqtgui::CHierarchicalCommand* commandPtr = new iqtgui::CHierarchicalCommand(mainWindowComponentPtr->GetTitle());
				commandPtr->SetStaticFlags(iqtgui::CHierarchicalCommand::CF_ONOFF | iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU);

				commandPtr->setChecked(isVisibleToParent);

				m_showOtherWindows.InsertChild(commandPtr, true);

				connect(commandPtr, SIGNAL(toggled(bool)), this, SLOT(OnShowOtherCommandTriggered(bool)));
			}
		}
	}

	m_showOtherWindows.setVisible(m_showOtherWindows.GetChildsCount() > 0);

	CSimpleMainWindowGuiComp::UpdateMenuActions();
}


void CSimpleMainWindowGuiComp::OnSaveSettings(QSettings& settings) const
{
	Q_ASSERT(IsGuiCreated());
	QMainWindow* mainWindowPtr = GetQtWidget();
	Q_ASSERT(mainWindowPtr != NULL);

	QByteArray windowState = mainWindowPtr->saveState();
	QByteArray windowGeometry = mainWindowPtr->saveGeometry();

	settings.setValue("MainWindow/State", windowState);
	settings.setValue("MainWindow/Geometry", windowGeometry);
}


// reimplemented (iqtgui::TGuiComponentBase)

void CSimpleMainWindowGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QMainWindow* mainWindowPtr = GetQtWidget();
	if (mainWindowPtr == NULL){
		return;
	}

	if (m_iconSizeAttrPtr.IsValid() && m_iconSizeAttrPtr->GetValue() != 0){
		mainWindowPtr->setIconSize(QSize(m_iconSizeAttrPtr->GetValue(), m_iconSizeAttrPtr->GetValue()));
	}	

	int dockOptions = 0;

	Q_ASSERT(m_isNestingEnabledAttrPtr.IsValid());
	if (*m_isNestingEnabledAttrPtr){
		dockOptions |= QMainWindow::AllowNestedDocks;
	}
	Q_ASSERT(m_isAllowTabbedDockAttrPtr.IsValid());
	if (*m_isAllowTabbedDockAttrPtr){
		dockOptions |= QMainWindow::AllowTabbedDocks;
	}
	mainWindowPtr->setDockOptions(QMainWindow::DockOption(dockOptions));

	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->CreateGui(NULL);
		QWidget* workspacePtr = m_workspaceCompPtr->GetWidget();
		if (workspacePtr != NULL){
			mainWindowPtr->setCentralWidget(workspacePtr);
		}
	}

	m_showToolBarsCommand.setChecked(true);

	CreateMenuBar();

	CreateDefaultToolBar();

	SetupMenu();

	UpdateMenuActions();

	SetupMainWindowComponents(*mainWindowPtr);

	if (m_workspaceCommandsCompPtr.IsValid()){
		imod::IModel* modelPtr = CompCastPtr<imod::IModel>(m_workspaceCommandsCompPtr.GetPtr());
		if (modelPtr != NULL){
			m_commandsObserver.RegisterModel(modelPtr, 0, ibase::ICommandsProvider::CF_COMMANDS);
		}
	}

	if (m_mainWindowCommandsCompPtr.IsValid()){
		int commandsProviderCount = m_mainWindowCommandsCompPtr.GetCount();

		for (int index = 0; index < commandsProviderCount; index++){
			imod::IModel* modelPtr = CompCastPtr<imod::IModel>(m_mainWindowCommandsCompPtr[index]);
			if (modelPtr != NULL){
				m_commandsObserver.RegisterModel(modelPtr, index + 1, ibase::ICommandsProvider::CF_COMMANDS);
			}
		}
	}
}


void CSimpleMainWindowGuiComp::OnGuiDestroyed()
{
	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->DestroyGui();
	}

	for (int componentIndex = 0; componentIndex < m_mainWindowComponentsCompPtr.GetCount(); componentIndex++){
		iqtgui::IMainWindowComponent* mainWindowComponentPtr =  m_mainWindowComponentsCompPtr[componentIndex];
		if (mainWindowComponentPtr != NULL){
			RemoveMainComponent(mainWindowComponentPtr);
		}
	}

	m_viewCommand.ResetChilds();
	m_toolsCommand.ResetChilds();
	m_helpCommand.ResetChilds();
	m_fixedCommands.ResetChilds();

	m_menuBarPtr.Reset();
	m_standardToolBarPtr.Reset();

	BaseClass::OnGuiDestroyed();
}


void CSimpleMainWindowGuiComp::OnRetranslate()
{
	BaseClass::OnRetranslate();

	// Main commands
	m_viewCommand.SetName(tr("&View"));
	m_toolsCommand.SetName(tr("&Tools"));
	m_helpCommand.SetName(tr("&Help"));
	
	// View commands
	m_showToolBarsCommand.SetVisuals(tr("&Show Toolbars"), tr("Show Toolbars"), tr("Show and hide toolbars"));
	m_showOtherWindows.SetVisuals(tr("Other Windows"), tr("Other Windows"), tr("Show additional windows"));

	// Tools commands
	m_settingsCommand.SetVisuals(tr("&Settings"), tr("Settings"), tr("Show global application settings"), QIcon(":/Icons/Settings.svg"));
	m_settingsCommand.setMenuRole(QAction::PreferencesRole);

	// Help commands
	m_aboutCommand.SetVisuals(tr("&About..."), tr("About"), tr("Shows information about this application"), QIcon(":/Icons/About"));
	m_aboutCommand.setMenuRole(QAction::AboutRole);
}


// reimplemented (QObject)

bool CSimpleMainWindowGuiComp::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	if (eventPtr->type() == QEvent::HideToParent){
		QString objectName = sourcePtr->objectName();

		if (!objectName.isEmpty()){
			for (		MainComponentVisibilityMap::Iterator iter = m_mainComponentVisibilityMap.begin();
						iter != m_mainComponentVisibilityMap.end();
						++iter){
				IMainWindowComponent* mainWindowComponentPtr = iter.key();
				Q_ASSERT(mainWindowComponentPtr != NULL);

				if (mainWindowComponentPtr->GetTitle() == objectName){
					*iter = false;
				}

				for (int i = 0; i < m_showOtherWindows.GetChildsCount(); ++i){
					CHierarchicalCommand* commandPtr = dynamic_cast<CHierarchicalCommand*>(m_showOtherWindows.GetChild(i));
					Q_ASSERT(commandPtr != NULL);

					QString commandName = commandPtr->GetName();

					if (commandName == objectName){
						iqt::CSignalBlocker block(commandPtr);

						commandPtr->setChecked(false);
					}
				}
			}
		}
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


// protected slots

void CSimpleMainWindowGuiComp::OnShowToolbars()
{
	SetToolBarsVisible(m_showToolBarsCommand.isChecked());
}


void CSimpleMainWindowGuiComp::OnAbout()
{
	if (m_aboutDialogCompPtr.IsValid()){
		m_aboutDialogCompPtr->ExecuteDialog(this);
	}
}


void CSimpleMainWindowGuiComp::OnSettings()
{
	if (m_settingsDialogCompPtr.IsValid()){
		m_settingsDialogCompPtr->ExecuteDialog(this);
	}
}


void CSimpleMainWindowGuiComp::OnShowOtherCommandTriggered(bool enabled)
{
	QAction* actionPtr = dynamic_cast<QAction*>(sender());
	Q_ASSERT(actionPtr != NULL);

	QString actionName = actionPtr->text();

	for (		MainComponentVisibilityMap::Iterator iter = m_mainComponentVisibilityMap.begin();
				iter != m_mainComponentVisibilityMap.end();
				++iter){
		IMainWindowComponent* mainWindowComponentPtr = iter.key();
		Q_ASSERT(mainWindowComponentPtr != NULL);

		if (mainWindowComponentPtr->GetTitle() == actionName){
			*iter = enabled;

			iqtgui::IGuiObject* guiPtr = dynamic_cast<iqtgui::IGuiObject*>(mainWindowComponentPtr);
			if ((guiPtr != NULL) && guiPtr->IsGuiCreated()){
				Q_ASSERT(guiPtr->GetWidget() != NULL);

				guiPtr->GetWidget()->setVisible(enabled);
			}
		}
	}
}


// public methods of embedded class ActiveUndoManager

CSimpleMainWindowGuiComp::CommandsObserver::CommandsObserver(CSimpleMainWindowGuiComp& parent)
:	m_parent(parent)
{
}
		

// protected methods of embedded class ActiveUndoManager

// reimplemented (imod::CMultiModelDispatcherBase)

void CSimpleMainWindowGuiComp::CommandsObserver::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.UpdateMenuActions();
}


} // namespace iqtgui


