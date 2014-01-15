#ifndef iqtgui_CSimpleMainWindowGuiComp_included
#define iqtgui_CSimpleMainWindowGuiComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#else
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#endif

// ACF includes
#include "imod/CMultiModelDispatcherBase.h"
#include "ibase/ICommandsProvider.h"
#include "iqtgui/IMainWindowComponent.h"
#include "iqtgui/IDialog.h"
#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TRestorableGuiWrap.h"
#include "iqtgui/CCommandTools.h"


namespace iqtgui
{


class CSimpleMainWindowGuiComp:
			public iqtgui::TRestorableGuiWrap<
						iqtgui::TGuiComponentBase<QMainWindow> >
{
	Q_OBJECT
public:
	typedef iqtgui::TRestorableGuiWrap<
					iqtgui::TGuiComponentBase<QMainWindow> > BaseClass;

	enum DockFeatures
	{
		DF_MOVEABLE = 1,
		DF_FLOATABLE = 2
	};

	I_BEGIN_COMPONENT(CSimpleMainWindowGuiComp);
		I_ASSIGN(m_workspaceCommandsCompPtr, "Workspace", "Main widget", false, "Workspace");
		I_ASSIGN_TO(m_workspaceCompPtr, m_workspaceCommandsCompPtr, true);
		I_ASSIGN(m_aboutDialogCompPtr, "AboutDialog", "Gui displayed if 'About' action is triggered", false, "AboutDialog");
		I_ASSIGN(m_settingsDialogCompPtr, "SettingsDialog", "Dialog window displayed if 'Settings' action is triggered", false, "SettingsDialog");
		I_ASSIGN_MULTI_0(m_mainWindowComponentsCompPtr, "MainWindowComponents", "Additional GUI components", false);
		I_ASSIGN_TO(m_mainWindowCommandsCompPtr, m_mainWindowComponentsCompPtr, false);
		I_ASSIGN(m_isNestingEnabledAttrPtr, "IsNestingEnabled", "If true, docking nesting is enabled,\nmore than one docking widget is allowed on the same size", true, false);
		I_ASSIGN(m_isAllowTabbedDockAttrPtr, "IsTabbedDockAllowed", "If true, stacking docking windows on each other is enabled", true, false);
		I_ASSIGN(m_isMenuVisibleAttrPtr, "IsMenuVisible", "If true, menu bar will be visible", true, true);
		I_ASSIGN(m_isToolbarVisibleAttrPtr, "IsToolbarVisible", "If true, tool bar will be visible", true, true);
		I_ASSIGN(m_toolBarAreaAttrPtr, "ToolBarArea", "Specify the area where the standard toolbar will be placed\n 0 - left\n 1 - right\n 2 - top\n 3 - bottom", false, 0);
		I_ASSIGN(m_toolBarDockFeaturesAttrPtr, "ToolBarDockFeatures", "Specify the dock features for the standard tool bar\nIt is combination of options:\n1 - moveable\n 2 - floatable", true, DF_MOVEABLE | DF_FLOATABLE);
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

	virtual void SetupMainWindowComponents(QMainWindow& mainWindow);

	virtual void AddMainComponent(iqtgui::IMainWindowComponent* componentPtr);
	virtual void RemoveMainComponent(iqtgui::IMainWindowComponent* componentPtr);

	virtual void CreateMenuBar();
	virtual void CreateDefaultToolBar();
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

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

protected Q_SLOTS:
	void OnShowToolbars();
	void OnAbout();
	void OnSettings();
	void OnShowOtherCommandTriggered(bool enabled);

private:
	class CommandsObserver: public imod::CMultiModelDispatcherBase
	{
	public:
		typedef imod::CMultiModelDispatcherBase BaseClass;

		CommandsObserver(CSimpleMainWindowGuiComp& parent);

		// reimplemented (imod::CMultiModelDispatcherBase)
		void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CSimpleMainWindowGuiComp& m_parent;
	};

protected:
	CommandsObserver m_commandsObserver;

	typedef QMap<iqtgui::IMainWindowComponent*, bool > MainComponentVisibilityMap;

	MainComponentVisibilityMap m_mainComponentVisibilityMap;

	I_MULTIREF(iqtgui::IMainWindowComponent, m_mainWindowComponentsCompPtr);

private:
	I_REF(iqtgui::IGuiObject, m_workspaceCompPtr);
	I_REF(ibase::ICommandsProvider, m_workspaceCommandsCompPtr);
	I_MULTIREF(ibase::ICommandsProvider, m_mainWindowCommandsCompPtr);
	I_REF(iqtgui::IDialog, m_aboutDialogCompPtr);
	I_REF(iqtgui::IDialog, m_settingsDialogCompPtr);
	I_ATTR(bool, m_isNestingEnabledAttrPtr);
	I_ATTR(bool, m_isAllowTabbedDockAttrPtr);
	I_ATTR(bool, m_isMenuVisibleAttrPtr);
	I_ATTR(bool, m_isToolbarVisibleAttrPtr);
	I_ATTR(int, m_toolBarAreaAttrPtr);
	I_ATTR(int, m_toolBarDockFeaturesAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_useIconTextAttrPtr);

	istd::TDelPtr<QMenuBar> m_menuBarPtr;
	istd::TDelPtr<QToolBar> m_standardToolBarPtr;

	istd::TOptPointerVector<QToolBar> m_toolBarsList;

	iqtgui::CHierarchicalCommand m_menuCommands;

	iqtgui::CHierarchicalCommand m_viewCommand;
	iqtgui::CHierarchicalCommand m_toolsCommand;
	iqtgui::CHierarchicalCommand m_helpCommand;

	// view menu group
	iqtgui::CHierarchicalCommand m_showToolBarsCommand;
	iqtgui::CHierarchicalCommand m_showOtherWindows;

	// tools menu group
	iqtgui::CHierarchicalCommand m_settingsCommand;

	// help menu group
	iqtgui::CHierarchicalCommand m_aboutCommand;

	iqtgui::CHierarchicalCommand m_fixedCommands;
};


} // namespace iqtgui


#endif // !iqtgui_CSimpleMainWindowGuiComp_included


