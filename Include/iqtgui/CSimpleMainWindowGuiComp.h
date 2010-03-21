#ifndef iqtgui_CMainWindowComp_included
#define iqtgui_CMainWindowComp_included


// Qt includes
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>


// ACF includes
#include "ibase/ICommandsProvider.h"

#include "iqtgui/IMainWindowComponent.h"
#include "iqtgui/IDialog.h"
#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TRestorableGuiWrap.h"
#include "iqtgui/CHierarchicalCommand.h"
#include "iqtgui/CGuiComponentDialog.h"


namespace iqtgui
{


class CSimpleMainWindowGuiComp: public iqtgui::TRestorableGuiWrap<iqtgui::TGuiComponentBase<QMainWindow> >
{
	Q_OBJECT
public:
	typedef iqtgui::TRestorableGuiWrap<iqtgui::TGuiComponentBase<QMainWindow> > BaseClass;

	I_BEGIN_COMPONENT(CSimpleMainWindowGuiComp);
		I_ASSIGN(m_workspaceCompPtr, "Workspace", "Document workspace", true, "Workspace");
		I_ASSIGN(m_workspaceCommandsCompPtr, "Workspace", "Document workspace", false, "Workspace");
		I_ASSIGN(m_aboutGuiCompPtr, "AboutGui", "Gui displayed if 'About' action is triggered", false, "AboutGui");
		I_ASSIGN(m_settingsGuiCompPtr, "SettingsGui", "Gui displayed if 'Settings' action is triggered", false, "SettingsGui");
		I_ASSIGN_MULTI_0(m_mainWindowComponentsPtr, "MainWindowComponents", "Additional GUI components", false);
		I_ASSIGN(m_isNestingEnabledAttrPtr, "IsNestingEnabled", "If true, docking nesting is enabled,\nmore than one docking widget is allowed on the same size", true, false);
		I_ASSIGN(m_isMenuVisibleAttrPtr, "IsMenuVisible", "If true, menu bar will be visible", true, true);
		I_ASSIGN(m_isToolbarVisibleAttrPtr, "IsToolbarVisible", "If true, tool bar will be visible", true, true);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size of icons using in the main window", false, 16);
		I_ASSIGN(m_useIconTextAttrPtr, "UseIconText", "Enable text under the tool bar icons", false, false);
	I_END_COMPONENT;

	enum GroupId
	{
		GI_SETTINGS = 0x400
	};

	CSimpleMainWindowGuiComp();

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL);

protected:
	virtual void UpdateMenuActions();
	virtual void UpdateFixedCommands();

	virtual void SetupMainWindowComponents(QMainWindow& mainWindow);

	virtual void AddMainComponent(iqtgui::IMainWindowComponent* componentPtr);
	virtual void RemoveMainComponent(iqtgui::IMainWindowComponent* componentPtr);

	virtual void CreateMenuBar();
	virtual void CreateDefaultToolBar();
	virtual int SetupToolbar(const iqtgui::CHierarchicalCommand& command, QToolBar& result, int prevGroupId = ibase::ICommand::GI_NONE) const;
	virtual void SetToolBarsVisible(bool isVisible = true);
	virtual void SetupMenu();

	virtual void UpdateFixedCommands(iqtgui::CHierarchicalCommand& fixedCommands);
	virtual void UpdateViewCommands(iqtgui::CHierarchicalCommand& viewCommand);
	virtual void UpdateToolsCommands(iqtgui::CHierarchicalCommand& toolsCommand);
	virtual void UpdateHelpCommands(iqtgui::CHierarchicalCommand& helpCommand);
	virtual void UpdateMenuActions(iqtgui::CHierarchicalCommand& menuCommands);

	// reimplemented (TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings);
	virtual void OnSaveSettings(QSettings& settings) const;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnRetranslate();

	// protected template methods
	template <class MenuType>
	void CreateMenu(const iqtgui::CHierarchicalCommand& command, MenuType& result) const;

protected Q_SLOTS:
	void OnShowToolbars();
	void OnSettings();
	void OnAbout();

private:
	I_REF(iqtgui::IGuiObject, m_workspaceCompPtr);
	I_REF(ibase::ICommandsProvider, m_workspaceCommandsCompPtr);
	I_MULTIREF(iqtgui::IMainWindowComponent, m_mainWindowComponentsPtr);
	I_REF(iqtgui::IGuiObject, m_aboutGuiCompPtr);
	I_REF(iqtgui::IDialog, m_settingsGuiCompPtr);
	I_ATTR(bool, m_isNestingEnabledAttrPtr);
	I_ATTR(bool, m_isMenuVisibleAttrPtr);
	I_ATTR(bool, m_isToolbarVisibleAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_useIconTextAttrPtr);

	istd::TDelPtr<iqtgui::CGuiComponentDialog> m_aboutDialogPtr;

	istd::TDelPtr<QMenuBar> m_menuBarPtr;
	istd::TDelPtr<QToolBar> m_standardToolBarPtr;

	istd::TOptPointerVector<QToolBar> m_toolBarsList;

	iqtgui::CHierarchicalCommand m_menuCommands;

	iqtgui::CHierarchicalCommand m_viewCommand;
	iqtgui::CHierarchicalCommand m_toolsCommand;
	iqtgui::CHierarchicalCommand m_helpCommand;

	// view menu group
	iqtgui::CHierarchicalCommand m_showToolBarsCommand;

	// tools menu group
	iqtgui::CHierarchicalCommand m_settingsCommand;

	// help menu group
	iqtgui::CHierarchicalCommand m_aboutCommand;

	iqtgui::CHierarchicalCommand m_fixedCommands;
};


// protected template methods

template <class MenuType>
void CSimpleMainWindowGuiComp::CreateMenu(const iqtgui::CHierarchicalCommand& command, MenuType& result) const
{
	int prevGroupId = ibase::ICommand::GI_NONE;

	int childsCount = command.GetChildsCount();

	std::map<int, istd::TPointer<QActionGroup> > groups;

	for (int i = 0; i < childsCount; ++i){
		QString text = command.text();
		iqtgui::CHierarchicalCommand* hierarchicalPtr = const_cast<iqtgui::CHierarchicalCommand*>(
					dynamic_cast<const iqtgui::CHierarchicalCommand*>(command.GetChild(i)));

		if (hierarchicalPtr != NULL){
			QString text2 = hierarchicalPtr->text();
			int groupId = hierarchicalPtr->GetGroupId();
			int flags = hierarchicalPtr->GetStaticFlags();

			if ((groupId != prevGroupId) && (prevGroupId != ibase::ICommand::GI_NONE)){
				result.addSeparator();
			}

			if (groupId != ibase::ICommand::GI_NONE){
				prevGroupId = groupId;
			}

			if (hierarchicalPtr->GetChildsCount() > 0){
				QMenu* newMenuPtr = new QMenu(&result);
				newMenuPtr->setTitle(iqt::GetQString(hierarchicalPtr->GetName()));

				CreateMenu<QMenu>(*hierarchicalPtr, *newMenuPtr);

				result.addMenu(newMenuPtr);
			}
			else if ((flags & ibase::ICommand::CF_GLOBAL_MENU) != 0){
				if ((flags & ibase::ICommand::CF_EXCLUSIVE) != 0){
					istd::TPointer<QActionGroup>& groupPtr = groups[hierarchicalPtr->GetGroupId()];
					if (!groupPtr.IsValid()){
						groupPtr.SetPtr(new QActionGroup(&result));
						groupPtr->setExclusive(true);
					}

					groupPtr->addAction(hierarchicalPtr);
					hierarchicalPtr->setCheckable(true);
				}

				result.addAction(hierarchicalPtr);
			}
		}
	}
}


} // namespace iqtgui


#endif // !iqtgui_CMainWindowComp_included


