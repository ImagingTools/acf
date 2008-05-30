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

#include "iqt/IMainWindowComponent.h"
#include "iqt/IToolBarManager.h"
#include "iqt/IDockManager.h"
#include "iqt/ITranslationManager.h"
#include "iqt/TGuiComponentBase.h"
#include "iqt/CHierarchicalCommand.h"

#include "iqtdoc/IWorkspaceController.h"


namespace iqtdoc
{


class CMainWindowGuiComp:
			public iqt::TGuiComponentBase<QMainWindow>,
			public imod::TSingleModelObserverBase<idoc::IDocumentManager>,
			public iqt::IDockManager,
			public iqt::IToolBarManager
{
	Q_OBJECT

public:
	typedef iqt::TGuiComponentBase<QMainWindow> BaseClass;
	typedef imod::TSingleModelObserverBase<idoc::IDocumentManager> BaseClass2;

	I_BEGIN_COMPONENT(CMainWindowGuiComp)
		I_REGISTER_INTERFACE(imod::IObserver)
		I_REGISTER_INTERFACE(iqt::IDockManager)
		I_REGISTER_INTERFACE(iqt::IToolBarManager)
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

	// reimplemented (iqt::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL);

	// reimplemented (iqt::IToolBarManager)
	virtual void SetToolBarsVisible(bool isVisible = true);
	virtual int GetToolBarCount() const ;
	virtual	void AddToolBar(QToolBar* widgetPtr);
	virtual void RemoveToolBar(QToolBar* widgetPtr);

	// reimplemented (iqt::IDockManager)
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

	template <class MenuType>
	void CreateMenu(const iqt::CHierarchicalCommand& command, MenuType& result) const;
	int CreateToolbar(const iqt::CHierarchicalCommand& command, QToolBar& result, int prevGroupId = idoc::ICommand::GI_NONE) const;

	void SetupMainWindow(QMainWindow& mainWindow);
	void SetupFileMenu();
	void SetupEditMenu();
	void SetupMainWindowComponents(QMainWindow& mainWindow);
	bool HasDocumentTemplate() const;
	void UpdateUndoMenu();
	void UpdateMenuActions();
	void UpdateRecentFileMenu();
	void CreateRecentMenu();

	void OnNewDocument(const std::string& documentTypeId);
	void OnOpenFile(const QString& fileName);

	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnRetranslate();

	// reimplemented (imod::TSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

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
	void OnAbout();
	void OnLanguageSelected(QAction* a); 
	void OnStyleSelected(QAction* a);

	// Workspace controller slots
	void OnCloseAllWindows(); 
	void OnCascade(); 
	void OnTileHorizontally(); 
	void OnTile(); 

private:
	QMenuBar* m_menuBar;
	QToolBar* m_standardToolBar;

	iqt::CHierarchicalCommand m_fileCommand;
	iqt::CHierarchicalCommand m_editCommand;
	iqt::CHierarchicalCommand m_viewCommand;
	iqt::CHierarchicalCommand m_windowCommand;
	iqt::CHierarchicalCommand m_helpCommand;

	// file menu group
	iqt::CHierarchicalCommand m_newCommand;
	iqt::CHierarchicalCommand m_openCommand;
	iqt::CHierarchicalCommand m_saveCommand;
	iqt::CHierarchicalCommand m_saveAsCommand;
	iqt::CHierarchicalCommand m_quitCommand;
	// edit menu group
	iqt::CHierarchicalCommand m_undoCommand;
	iqt::CHierarchicalCommand m_redoCommand;
	// view menu group
	iqt::CHierarchicalCommand m_fullScreenCommand;
	// window menu group
	iqt::CHierarchicalCommand m_cascadeCommand;
	iqt::CHierarchicalCommand m_tileHorizontallyCommand;
	iqt::CHierarchicalCommand m_tileVerticallyCommand;
	iqt::CHierarchicalCommand m_closeAllDocumentsCommand;
	// help menu group
	iqt::CHierarchicalCommand m_aboutCommand;

	class NewDocumentCommand: public iqt::CHierarchicalCommand
	{
	public:
		typedef iqt::CHierarchicalCommand BaseClass;

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

	class RecentFileCommand: public iqt::CHierarchicalCommand
	{
	public:
		typedef iqt::CHierarchicalCommand BaseClass;

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

	iqt::CHierarchicalCommand m_fixedCommands;

	iqt::CHierarchicalCommand m_menuCommands;

	typedef std::map<std::string, istd::TDelPtr<iqt::CHierarchicalCommand> >  RecentFileCommandMap;
	RecentFileCommandMap m_recentFilesCommands;

	I_REF(iqt::IGuiObject, m_workspaceCompPtr);
	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(imod::IModel, m_documentManagerModelCompPtr);
	I_REF(iqtdoc::IWorkspaceController, m_workspaceControllerCompPtr);
	I_REF(iqt::ITranslationManager, m_translationManagerCompPtr);
	I_MULTIREF(iqt::IMainWindowComponent, m_mainWindowComponentsPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_useIconTextAttrPtr);
};


// public template methods

template <class MenuType>
void CMainWindowGuiComp::CreateMenu(const iqt::CHierarchicalCommand& command, typename MenuType& result) const
{
	int prevGroupId = idoc::ICommand::GI_NONE;

	int childsCount = command.GetChildsCount();

	std::map<int, istd::TPointer<QActionGroup> > groups;

	for (int i = 0; i < childsCount; ++i){
		iqt::CHierarchicalCommand* hierarchicalPtr = const_cast<iqt::CHierarchicalCommand*>(
					dynamic_cast<const iqt::CHierarchicalCommand*>(command.GetChild(i)));

		if (hierarchicalPtr != NULL){
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

