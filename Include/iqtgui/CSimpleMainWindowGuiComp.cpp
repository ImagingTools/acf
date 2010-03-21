#include "iqtgui/CSimpleMainWindowGuiComp.h"


// Qt includes
#include <QSettings>


// ACF includes
#include "iqt/CSignalBlocker.h"


namespace iqtgui
{


// public methods

CSimpleMainWindowGuiComp::CSimpleMainWindowGuiComp()
:	m_menuCommands("Global"),
	m_showToolBarsCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_ONOFF),
	m_settingsCommand("", 200, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR)
{
	connect(&m_showToolBarsCommand, SIGNAL(activated()), this, SLOT(OnShowToolbars()));
	connect(&m_settingsCommand, SIGNAL(activated()), this, SLOT(OnSettings()));
	connect(&m_aboutCommand, SIGNAL(activated()), this, SLOT(OnAbout()));
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
		CreateMenu(m_menuCommands, *m_menuBarPtr);
	}

	if (m_standardToolBarPtr.IsValid()){
		m_standardToolBarPtr->clear();
		
		SetupToolbar(m_menuCommands, *m_standardToolBarPtr);
		
		m_standardToolBarPtr->setVisible(true);
	}
}


void CSimpleMainWindowGuiComp::UpdateFixedCommands()
{
	UpdateFixedCommands(m_fixedCommands);
}


void CSimpleMainWindowGuiComp::SetupMainWindowComponents(QMainWindow& mainWindow)
{
	if (m_standardToolBarPtr.IsValid()){
		mainWindow.addToolBar(Qt::TopToolBarArea, m_standardToolBarPtr.GetPtr());

		m_toolBarsList.PushBack(m_standardToolBarPtr.GetPtr(), false);
	}

	if (m_menuBarPtr.IsValid()){
		mainWindow.setMenuBar(m_menuBarPtr.GetPtr());
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
	I_ASSERT(m_isMenuVisibleAttrPtr.IsValid());	// is obligatory attribute
	if (*m_isMenuVisibleAttrPtr){
		QMainWindow* mainWindowPtr = GetQtWidget();
		if (mainWindowPtr != NULL){
			m_menuBarPtr.SetPtr(new QMenuBar(mainWindowPtr));
		}
	}
}


void CSimpleMainWindowGuiComp::CreateDefaultToolBar()
{
	I_ASSERT(m_isToolbarVisibleAttrPtr.IsValid());	// is obligatory attribute
	if (*m_isToolbarVisibleAttrPtr){
		QMainWindow* mainWindowPtr = GetQtWidget();
		if (mainWindowPtr != NULL){
			m_standardToolBarPtr.SetPtr(new QToolBar(mainWindowPtr));
			m_standardToolBarPtr->setWindowTitle(tr("Standard"));
			m_standardToolBarPtr->setObjectName("Standard");

			if (m_useIconTextAttrPtr.IsValid() && m_useIconTextAttrPtr->GetValue()){
				m_standardToolBarPtr->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
			}
		}
	}
}


int CSimpleMainWindowGuiComp::SetupToolbar(const iqtgui::CHierarchicalCommand& command, QToolBar& result, int prevGroupId) const
{
	int childsCount = command.GetChildsCount();

	std::map<int, istd::TPointer<QActionGroup> > groups;

	for (int i = 0; i < childsCount; ++i){
		iqtgui::CHierarchicalCommand* hierarchicalPtr = const_cast<iqtgui::CHierarchicalCommand*>(
					dynamic_cast<const iqtgui::CHierarchicalCommand*>(command.GetChild(i)));

		if (hierarchicalPtr != NULL){
			int groupId = hierarchicalPtr->GetGroupId();
			int flags = hierarchicalPtr->GetStaticFlags();

			if ((flags & ibase::ICommand::CF_TOOLBAR) != 0){
				if (hierarchicalPtr->GetChildsCount() > 0){
					QMenu* newMenuPtr = new QMenu(&result);
					if (newMenuPtr != NULL){
						newMenuPtr->setTitle(iqt::GetQString(hierarchicalPtr->GetName()));

						CreateMenu<QMenu>(*hierarchicalPtr, *newMenuPtr);

						newMenuPtr->setIcon(hierarchicalPtr->icon());
						result.addAction(newMenuPtr->menuAction());
					}
				}
				else{
					if ((groupId != prevGroupId) && (prevGroupId != ibase::ICommand::GI_NONE)){
						result.addSeparator();
					}

					result.addAction(hierarchicalPtr);
				}

				if (groupId != ibase::ICommand::GI_NONE){
					prevGroupId = groupId;
				}
			}
			else if (hierarchicalPtr->GetChildsCount() > 0){
				prevGroupId = SetupToolbar(*hierarchicalPtr, result, prevGroupId);
			}
		}
	}

	return prevGroupId;
}


void CSimpleMainWindowGuiComp::SetToolBarsVisible(bool isVisible)
{
	QMainWindow* mainWindowPtr = GetQtWidget();
	I_ASSERT(mainWindowPtr != NULL);

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
}


void CSimpleMainWindowGuiComp::UpdateToolsCommands(iqtgui::CHierarchicalCommand& toolsCommand)
{
	toolsCommand.ResetChilds();

	if (m_settingsGuiCompPtr.IsValid()){
		m_settingsCommand.SetGroupId(GI_SETTINGS);
		toolsCommand.InsertChild(&m_settingsCommand, false);
	}
}


void CSimpleMainWindowGuiComp::UpdateHelpCommands(iqtgui::CHierarchicalCommand& helpCommand)
{
	if (m_aboutGuiCompPtr.IsValid()){
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
}


// reimplemented (TRestorableGuiWrap)

void CSimpleMainWindowGuiComp::OnRestoreSettings(const QSettings& settings)
{
	I_ASSERT(IsGuiCreated());
	QMainWindow* mainWindowPtr = GetQtWidget();
	I_ASSERT(mainWindowPtr != NULL);

	QByteArray windowState = settings.value("MainWindow/State").toByteArray();
	QByteArray windowGeometry = settings.value("MainWindow/Geometry").toByteArray();

	mainWindowPtr->restoreState(windowState);
	mainWindowPtr->restoreGeometry(windowGeometry);
}


void CSimpleMainWindowGuiComp::OnSaveSettings(QSettings& settings) const
{
	I_ASSERT(IsGuiCreated());
	QMainWindow* mainWindowPtr = GetQtWidget();
	I_ASSERT(mainWindowPtr != NULL);

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

	I_ASSERT(m_isNestingEnabledAttrPtr.IsValid());
	mainWindowPtr->setDockOptions(*m_isNestingEnabledAttrPtr? QMainWindow::AllowNestedDocks: QMainWindow::DockOption(0));

	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->CreateGui(NULL);
		QWidget* workspacePtr = m_workspaceCompPtr->GetWidget();
		if (workspacePtr != NULL){
			mainWindowPtr->setCentralWidget(workspacePtr);
		}
	}

	for (int componentIndex = 0; componentIndex < m_mainWindowComponentsPtr.GetCount(); componentIndex++){
		iqtgui::IMainWindowComponent* mainWindowComponentPtr =  m_mainWindowComponentsPtr[componentIndex];
		if (mainWindowComponentPtr != NULL){
			AddMainComponent(mainWindowComponentPtr);
		}
	}

	m_showToolBarsCommand.setChecked(true);

	if (m_aboutGuiCompPtr.IsValid()){
		m_aboutDialogPtr.SetPtr(new iqtgui::CGuiComponentDialog(m_aboutGuiCompPtr.GetPtr(), 0, true, mainWindowPtr)); 
	}

	OnRetranslate();

	CreateMenuBar();

	CreateDefaultToolBar();

	SetupMenu();

	UpdateMenuActions();

	SetupMainWindowComponents(*mainWindowPtr);

}


void CSimpleMainWindowGuiComp::OnGuiDestroyed()
{
	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->DestroyGui();
	}

	for (int componentIndex = 0; componentIndex < m_mainWindowComponentsPtr.GetCount(); componentIndex++){
		iqtgui::IMainWindowComponent* mainWindowComponentPtr =  m_mainWindowComponentsPtr[componentIndex];
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

	m_aboutDialogPtr.Reset(); 

	BaseClass::OnGuiDestroyed();
}


void CSimpleMainWindowGuiComp::OnRetranslate()
{
	BaseClass::OnRetranslate();

	I_ASSERT(GetWidget() != NULL);

	QWidget* parentWidgetPtr = GetWidget()->parentWidget();
	if (parentWidgetPtr == NULL){
		parentWidgetPtr = GetWidget();
	}

	// Main commands
	m_viewCommand.SetName(iqt::GetCString(tr("&View")));
	m_toolsCommand.SetName(iqt::GetCString(tr("&Tools")));
	m_helpCommand.SetName(iqt::GetCString(tr("&Help")));
	
	// View commands
	m_showToolBarsCommand.SetVisuals(tr("&Show Toolbars"), tr("Show Toolbars"), tr("Show and hide toolbars"));

	// Tools commands
	m_settingsCommand.SetVisuals(tr("&Settings"), tr("Settings"), tr("Show global application settings"), QIcon(":/Icons/Settings.svg"));

	// Help commands
	m_aboutCommand.SetVisuals(tr("&About..."), tr("About"), tr("Shows information about this application"), QIcon(":/Icons/About"));
}


// protected slots

void CSimpleMainWindowGuiComp::OnShowToolbars()
{
	SetToolBarsVisible(m_showToolBarsCommand.isChecked());
}


void CSimpleMainWindowGuiComp::OnAbout()
{
	if (m_aboutDialogPtr.IsValid()){
		m_aboutDialogPtr->exec();
	}
}


void CSimpleMainWindowGuiComp::OnSettings()
{
	if (m_settingsGuiCompPtr.IsValid()){
		m_settingsGuiCompPtr->Execute();
	}
}


} // namespace iqtgui


