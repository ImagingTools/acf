#ifndef iqtdoc_CMainWindowComp_included
#define iqtdoc_CMainWindowComp_included


// Qt includes
#include <QMainWindow>
#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QStatusBar>

#include "imod/IModel.h"
#include "imod/IUndoManager.h"
#include "imod/TSingleModelObserverBase.h"

#include "idoc/IDocumentManager.h"

#include "iqt/ITranslationManager.h"

#include "iqtgui/IMainWindowComponent.h"
#include "iqtgui/IToolBarManager.h"
#include "iqtgui/IDockManager.h"
#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "iqtdoc/IWorkspaceController.h"


namespace iqtdoc
{


class CMainWindowGuiComp:
			public iqtgui::TGuiComponentBase<QMainWindow>,
			public imod::TSingleModelObserverBase<idoc::IDocumentManager>,
			public iqtgui::IDockManager,
			public iqtgui::IToolBarManager
{
	Q_OBJECT

public:
	typedef iqtgui::TGuiComponentBase<QMainWindow> BaseClass;
	typedef imod::TSingleModelObserverBase<idoc::IDocumentManager> BaseClass2;

	I_BEGIN_COMPONENT(CMainWindowGuiComp)
		I_REGISTER_INTERFACE(imod::IObserver)
		I_REGISTER_INTERFACE(iqtgui::IDockManager)
		I_REGISTER_INTERFACE(iqtgui::IToolBarManager)
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Document manager", true, "DocumentManager")
		I_ASSIGN(m_documentManagerModelCompPtr, "DocumentManager", "Document manager", true, "DocumentManager")
		I_ASSIGN(m_workspaceCompPtr, "Workspace", "Document workspace", true, "Workspace")
		I_ASSIGN(m_workspaceControllerCompPtr, "Workspace", "Workspace controller", true, "Workspace")
		I_ASSIGN_MULTI_0(m_mainWindowComponentsPtr, "MainWindowComponents", "Additional GUI components", false)
		I_ASSIGN(m_translationManagerCompPtr, "TranslationManager", "Translation manager", false, "TranslationManager")
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size of icons using in the main window", false, 16)
		I_ASSIGN(m_useIconTextAttrPtr, "UseIconText", "Enable text under the tool bar icons", false, false)
	I_END_COMPONENT

	enum GroupId
	{
		GI_APPLICATION = 0x200,
		GI_WINDOW,
		GI_DOCUMENT,
		GI_UNDO
	};

	CMainWindowGuiComp();

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL);

	// reimplemented (iqtgui::IToolBarManager)
	virtual void SetToolBarsVisible(bool isVisible = true);
	virtual int GetToolBarCount() const ;
	virtual	void AddToolBar(int flags, QToolBar* widgetPtr);
	virtual void RemoveToolBar(QToolBar* widgetPtr);

	// reimplemented (iqtgui::IDockManager)
	virtual	void AddDockWidget(int flags, QDockWidget* widget);
	virtual void RemoveDockWidget(QDockWidget* widget);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	virtual void OnDocumentCountChanged();
	virtual void OnActiveViewChanged();
	virtual void OnActiveDocumentChanged();
	virtual void OnRecentFileListChanged();
	virtual void OnDragEnterEvent(QDragEnterEvent* dragEnterEventPtr);
	virtual void OnDropEvent(QDropEvent* dropEventPtr);

	template <class MenuType>
	void CreateMenu(const iqtgui::CHierarchicalCommand& command, MenuType& result) const;
	int CreateToolbar(const iqtgui::CHierarchicalCommand& command, QToolBar& result, int prevGroupId = idoc::ICommand::GI_NONE) const;

	void SetupMainWindow(QMainWindow& mainWindow);
	void SetupNewCommand();
	void SetupMainWindowComponents(QMainWindow& mainWindow);
	bool HasDocumentTemplate() const;
	void UpdateFixedCommands();
	void UpdateUndoMenu();
	void UpdateMenuActions();
	void UpdateRecentFileMenu();
	void CreateRecentMenu();

	void OnNewDocument(const std::string& documentTypeId);
	void OnOpenFile(const QString& fileName);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnRetranslate();

	// reimplemented (imod::TSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

	// static methods
	static QIcon GetIcon(const std::string& name);

protected slots:
	void OnNew();
	void OnOpen();
	void OnSave();
	void OnSaveAs();
	void OnOpenDocument(const std::string* documentTypeIdPtr = NULL);
	void OnQuit();
	void OnUndo();
	void OnRedo();
	void OnFullScreen();
	void OnShowToolbars();
	void OnAbout();
	void OnLanguageSelected(QAction* a); 
	void OnStyleSelected(QAction* a);

	// Workspace controller slots
	void OnCloseAllWindows(); 
	void OnCascade(); 
	void OnTileHorizontally(); 
	void OnTile(); 

private:
	istd::TDelPtr<QMenuBar> m_menuBarPtr;
	istd::TDelPtr<QToolBar> m_standardToolBarPtr;

	iqtgui::CHierarchicalCommand m_fileCommand;
	iqtgui::CHierarchicalCommand m_editCommand;
	iqtgui::CHierarchicalCommand m_viewCommand;
	iqtgui::CHierarchicalCommand m_windowCommand;
	iqtgui::CHierarchicalCommand m_helpCommand;

	// file menu group
	iqtgui::CHierarchicalCommand m_newCommand;
	iqtgui::CHierarchicalCommand m_openCommand;
	iqtgui::CHierarchicalCommand m_saveCommand;
	iqtgui::CHierarchicalCommand m_saveAsCommand;
	iqtgui::CHierarchicalCommand m_quitCommand;
	// edit menu group
	iqtgui::CHierarchicalCommand m_undoCommand;
	iqtgui::CHierarchicalCommand m_redoCommand;
	// view menu group
	iqtgui::CHierarchicalCommand m_fullScreenCommand;
	iqtgui::CHierarchicalCommand m_showToolBarsCommand;
	// window menu group
	iqtgui::CHierarchicalCommand m_cascadeCommand;
	iqtgui::CHierarchicalCommand m_tileHorizontallyCommand;
	iqtgui::CHierarchicalCommand m_tileVerticallyCommand;
	iqtgui::CHierarchicalCommand m_closeAllDocumentsCommand;
	// help menu group
	iqtgui::CHierarchicalCommand m_aboutCommand;

	class NewDocumentCommand: public iqtgui::CHierarchicalCommand
	{
	public:
		typedef iqtgui::CHierarchicalCommand BaseClass;

		NewDocumentCommand(CMainWindowGuiComp* parentPtr, const std::string& documentTypeId): m_parent(*parentPtr), m_documentTypeId(documentTypeId){}

		// reimplemented (idoc::ICommand)
		virtual bool Execute(istd::IPolymorphic* /*contextPtr*/)
		{
			m_parent.OnNewDocument(m_documentTypeId);

			return true;
		}

	private:
		CMainWindowGuiComp& m_parent;
		std::string m_documentTypeId;
	};

	class RecentFileCommand: public iqtgui::CHierarchicalCommand
	{
	public:
		typedef iqtgui::CHierarchicalCommand BaseClass;

		RecentFileCommand(CMainWindowGuiComp* parentPtr, const QString& fileName)
			:m_parent(*parentPtr), m_fileName(fileName)
		{
			setText(m_fileName);
		}

		// reimplemented (idoc::ICommand)
		virtual bool Execute(istd::IPolymorphic* /*contextPtr*/)
		{
			m_parent.OnOpenFile(m_fileName);

			return true;
		}

	private:
		CMainWindowGuiComp& m_parent;
		QString m_fileName;
	};

	friend class NewDocumentCommand;

	class ActiveUndoManager: public imod::TSingleModelObserverBase<imod::IUndoManager>
	{
	public:
		typedef imod::TSingleModelObserverBase<imod::IUndoManager> BaseClass;

		ActiveUndoManager(CMainWindowGuiComp& parent);
		
		// reimplemented (imod::IObserver)
		virtual bool OnAttached(imod::IModel* modelPtr);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CMainWindowGuiComp& m_parent;
	};

	ActiveUndoManager m_activeUndoManager;

	istd::IPolymorphic* m_activeViewPtr;
	istd::IChangeable* m_activeDocumentPtr;

	iqtgui::CHierarchicalCommand m_fixedCommands;

	iqtgui::CHierarchicalCommand m_menuCommands;

	typedef std::map<std::string, istd::TDelPtr<iqtgui::CHierarchicalCommand> >  RecentFileCommandMap;
	RecentFileCommandMap m_recentFilesCommands;

	I_REF(iqtgui::IGuiObject, m_workspaceCompPtr);
	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(imod::IModel, m_documentManagerModelCompPtr);
	I_REF(iqtdoc::IWorkspaceController, m_workspaceControllerCompPtr);
	I_REF(iqt::ITranslationManager, m_translationManagerCompPtr);
	I_MULTIREF(iqtgui::IMainWindowComponent, m_mainWindowComponentsPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_useIconTextAttrPtr);

	istd::TOptPointerVector<QToolBar> m_toolBarsList;
};


// public template methods

template <class MenuType>
void CMainWindowGuiComp::CreateMenu(const iqtgui::CHierarchicalCommand& command, typename MenuType& result) const
{
	int prevGroupId = idoc::ICommand::GI_NONE;

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

			if ((groupId != prevGroupId) && (prevGroupId != idoc::ICommand::GI_NONE)){
				result.addSeparator();
			}

			if (groupId != idoc::ICommand::GI_NONE){
				prevGroupId = groupId;
			}

			if (hierarchicalPtr->GetChildsCount() > 0){
				QMenu* newMenuPtr = new QMenu(&result);
				newMenuPtr->setTitle(iqt::GetQString(hierarchicalPtr->GetName()));

				CreateMenu<QMenu>(*hierarchicalPtr, *newMenuPtr);

				result.addMenu(newMenuPtr);
			}
			else if ((flags & idoc::ICommand::CF_GLOBAL_MENU) != 0){
				if ((flags & idoc::ICommand::CF_EXCLUSIVE) != 0){
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


} // namespace iqtdoc


#endif // !iqtdoc_CMainWindowComp_included

