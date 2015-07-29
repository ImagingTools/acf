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
#include "ifile/IFileNameParam.h"
#include "iprm/IOptionsManager.h"
#include "iqtgui/IMainWindowComponent.h"
#include "iqtgui/IDialog.h"
#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TRestorableGuiWrap.h"
#include "iqtgui/CCommandTools.h"
#include "ilog/IMessageConsumer.h"


namespace iqtgui
{


/**
	\internal
	Base class as work-around of static variable limit in ACF macros
*/
class CSimpleMainWindowGuiCompBase: public iqtgui::TRestorableGuiWrap< iqtgui::TGuiComponentBase<QMainWindow> >
{
public:
	typedef iqtgui::TRestorableGuiWrap< iqtgui::TGuiComponentBase<QMainWindow> > BaseClass;

	I_BEGIN_BASE_COMPONENT(CSimpleMainWindowGuiCompBase);
		I_ASSIGN(m_workspaceCompPtr, "Workspace", "Main widget", true, "Workspace");
		I_ASSIGN_TO(m_workspaceCommandsCompPtr, m_workspaceCompPtr, false);
		I_ASSIGN_TO(m_workspaceCommandsModelCompPtr, m_workspaceCompPtr, false);
		I_ASSIGN(m_aboutDialogCompPtr, "AboutDialog", "Gui displayed if 'About' action is triggered", false, "AboutDialog");
		I_ASSIGN(m_settingsDialogCompPtr, "SettingsDialog", "Dialog window displayed if 'Settings' action is triggered", false, "SettingsDialog");
		I_ASSIGN_MULTI_0(m_mainWindowComponentsCompPtr, "MainWindowComponents", "Additional GUI components", false);
		I_ASSIGN_TO(m_mainWindowCommandsCompPtr, m_mainWindowComponentsCompPtr, false);
		I_ASSIGN_TO(m_mainWindowCommandsModelCompPtr, m_mainWindowComponentsCompPtr, false);
		I_ASSIGN(m_isNestingEnabledAttrPtr, "IsNestingEnabled", "If true, docking nesting is enabled,\nmore than one docking widget is allowed on the same size", true, false);
		I_ASSIGN(m_isAllowTabbedDockAttrPtr, "IsTabbedDockAllowed", "If true, stacking docking windows on each other is enabled", true, false);
		I_ASSIGN(m_isMenuVisibleAttrPtr, "IsMenuVisible", "If true, menu bar will be visible", true, true);
		I_ASSIGN(m_isToolbarVisibleAttrPtr, "IsToolbarVisible", "If true, tool bar will be visible", true, true);
		I_ASSIGN(m_toolBarAreaAttrPtr, "ToolBarArea", "Specify the area where the standard toolbar will be placed\n 0 - left\n 1 - right\n 2 - top\n 3 - bottom", false, 0);
		I_ASSIGN(m_toolBarDockFeaturesAttrPtr, "ToolBarDockFeatures", "Specify the dock features for the standard tool bar\nIt is combination of options:\n2 - moveable\n4 - floatable", true, IMainWindowComponent::WCF_MOVEABLE | IMainWindowComponent::WCF_FLOATABLE);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size of icons using in the main window", false, 16);
		I_ASSIGN(m_useIconTextAttrPtr, "UseIconText", "Enable text under the tool bar icons", false, false);
		I_ASSIGN(m_helpFilePathPtr, "HelpFile", "Path to the help (manual) file opened by F1", false, "HelpFile");
	I_END_COMPONENT;

protected:
	I_REF(iqtgui::IGuiObject, m_workspaceCompPtr);
	I_REF(ibase::ICommandsProvider, m_workspaceCommandsCompPtr);
	I_REF(imod::IModel, m_workspaceCommandsModelCompPtr);
	I_MULTIREF(ibase::ICommandsProvider, m_mainWindowCommandsCompPtr);
	I_MULTIREF(imod::IModel, m_mainWindowCommandsModelCompPtr);
	I_REF(iqtgui::IDialog, m_aboutDialogCompPtr);
	I_REF(iqtgui::IDialog, m_settingsDialogCompPtr);
	I_MULTIREF(iqtgui::IMainWindowComponent, m_mainWindowComponentsCompPtr);
	I_ATTR(bool, m_isNestingEnabledAttrPtr);
	I_ATTR(bool, m_isAllowTabbedDockAttrPtr);
	I_ATTR(bool, m_isMenuVisibleAttrPtr);
	I_ATTR(bool, m_isToolbarVisibleAttrPtr);
	I_ATTR(int, m_toolBarAreaAttrPtr);
	I_ATTR(int, m_toolBarDockFeaturesAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_useIconTextAttrPtr);
	I_REF(ifile::IFileNameParam, m_helpFilePathPtr);
};


class CSimpleMainWindowGuiComp: public CSimpleMainWindowGuiCompBase
{
	Q_OBJECT

public:
	typedef CSimpleMainWindowGuiCompBase BaseClass;

	I_BEGIN_COMPONENT(CSimpleMainWindowGuiComp);
		I_REGISTER_SUBELEMENT(VisibleWindowsManager);
		I_REGISTER_SUBELEMENT_INTERFACE(VisibleWindowsManager, iprm::IOptionsManager, GetVisibleWindowsManager);
		I_REGISTER_SUBELEMENT_INTERFACE(VisibleWindowsManager, iprm::IOptionsList, GetVisibleWindowsManager);
		I_REGISTER_SUBELEMENT_INTERFACE(VisibleWindowsManager, imod::IModel, GetVisibleWindowsManager);
		I_REGISTER_SUBELEMENT_INTERFACE(VisibleWindowsManager, istd::IChangeable, GetVisibleWindowsManager);
		I_ASSIGN(m_progressMessagesComsumerCompPtr, "ProgressMessagesConsumer", "Consumer of the progress messages", false, "");
	I_END_COMPONENT;

	enum GroupId
	{
		GI_SETTINGS = 0x400
	};

	enum ModelId
	{
		MI_PREFERENCE_COMMAND = 0x1000
	};

	CSimpleMainWindowGuiComp();

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL);

protected:
	virtual void UpdateMenuActions();

	virtual void SetupMainWindowComponents(QMainWindow& mainWindow);

	virtual void AddMainComponent(int componentIndex, iqtgui::IMainWindowComponent* componentPtr);
	virtual void RemoveMainComponent(iqtgui::IMainWindowComponent* componentPtr);

	virtual void CreateMenuBar();
	virtual void CreateDefaultToolBar();
	virtual void SetToolBarsVisible(bool isVisible = true);
	virtual void SetupMenu();

	virtual void UpdateFixedCommands(iqtgui::CHierarchicalCommand& fixedCommands);
	virtual void UpdateViewCommands(iqtgui::CHierarchicalCommand& viewCommand);
	virtual void UpdateToolsCommands(iqtgui::CHierarchicalCommand& toolsCommand);
	virtual void UpdateHelpCommands(iqtgui::CHierarchicalCommand& helpCommand);
	virtual void AppendMenuActions(iqtgui::CHierarchicalCommand& menuCommands);
	/**
		Set main window visible or invisible, depending on menu commands.
	*/
	virtual void UpdateMainWindowComponentsVisibility();
	virtual bool IsMainWindowActive(int index) const;

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
	void OnFullScreen();
	void OnAbout();
    void OnHelpManual();
	void OnSettings();
	void OnShowOtherCommandTriggered(bool enabled);

protected:
	class CommandsObserver: public imod::CMultiModelDispatcherBase
	{
	public:
		typedef imod::CMultiModelDispatcherBase BaseClass;

		CommandsObserver(CSimpleMainWindowGuiComp& parent);

		// reimplemented (imod::CMultiModelDispatcherBase)
		void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

	private:
		CSimpleMainWindowGuiComp& m_parent;
	};

	CommandsObserver m_commandsObserver;

private:
	class VisibleWindowsManager: public iprm::IOptionsManager
	{
	public:
		VisibleWindowsManager();

		void SetParent(CSimpleMainWindowGuiComp* parentPtr);

		// reimplemented (iprm::IOptionsManager)
		virtual int GetOptionOperationFlags(int index = -1) const;
		virtual bool SetOptionEnabled(int index, bool isEnabled = true);
		virtual bool RemoveOption(int index);
		virtual bool InsertOption(
					const QString& optionName,
					const QByteArray& optionId,
					const QString& optionDescription = QString(),
					int index = -1);
		virtual bool SwapOptions(int index1, int index2);
		virtual bool SetOptionName(int optionIndex, const QString& optionName);
		virtual bool SetOptionDescription(int optionIndex, const QString& optionDescription);

		// reimplemented (iprm::IOptionsList)
		virtual int GetOptionsFlags() const;
		virtual int GetOptionsCount() const;
		virtual QString GetOptionName(int index) const;
		virtual QString GetOptionDescription(int index) const;
		virtual QByteArray GetOptionId(int index) const;
		virtual bool IsOptionEnabled(int index) const;

		// reimplemented (iprm::ISelectionParam)
		virtual const iprm::IOptionsList* GetSelectionConstraints() const;
		virtual int GetSelectedOptionIndex() const;
		virtual bool SetSelectedOptionIndex(int index);
		virtual iprm::ISelectionParam* GetSubselection(int index) const;

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

	private:
		CSimpleMainWindowGuiComp* m_parentPtr;
	};

	template <class InterfaceType>
	static InterfaceType* GetVisibleWindowsManager(CSimpleMainWindowGuiComp& parent)
	{
		return &parent.m_visibleWindowsManager;
	}

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
#if !defined(Q_OS_MAC)
	iqtgui::CHierarchicalCommand m_fullScreenCommand;
#endif

	// tools menu group
	iqtgui::CHierarchicalCommand m_settingsCommand;

	// help menu group
	iqtgui::CHierarchicalCommand m_aboutCommand;
    iqtgui::CHierarchicalCommand m_manualCommand;

	iqtgui::CHierarchicalCommand m_fixedCommands;

	QByteArray m_beforeFullScreenGeometry;
	QByteArray m_beforeFullScreenState;

	imod::TModelWrap<VisibleWindowsManager> m_visibleWindowsManager;

	typedef QMap<int, int> IndexToIndexMap;

	IndexToIndexMap m_commandIndexToMainCompMap;

	I_REF(ilog::IMessageConsumer, m_progressMessagesComsumerCompPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CSimpleMainWindowGuiComp_included


