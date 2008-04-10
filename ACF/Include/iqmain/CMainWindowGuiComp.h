#ifndef iqmain_CMainWindowComp_included
#define iqmain_CMainWindowComp_included


#include "iqmain/iqmain.h"


#include <QMainWindow>
#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QStatusBar>
#include <QApplication>


#include "imod/TSingleModelObserverBase.h"
#include "imod/IUndoManager.h"

#include "idoc/IDocumentManager.h"

#include "iqt/IMainWindowComponent.h"
#include "iqt/IToolBarManager.h"
#include "iqt/IDockManager.h"
#include "iqt/ITranslationManager.h"
#include "iqt/TGuiComponentBase.h"


namespace iqmain
{


class CMainWindowGuiComp:	public iqt::TGuiComponentBase<QMainWindow>, 
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
		I_ASSIGN(m_workspaceCompPtr, "Workspace", "Document workspace", true, "Workspace")
		I_ASSIGN_MULTI_0(m_mainWindowComponentsPtr, "MainWindowComponents", "Additional GUI components", false)
		I_ASSIGN(m_translationManagerCompPtr, "TranslationManager", "Translation manager", false, "TranslationManager")
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size of icons using in the main window", false, 16)
	I_END_COMPONENT


	CMainWindowGuiComp();
	virtual ~CMainWindowGuiComp();

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
	virtual void OnDocumentCountChanged(int documentCount);
	virtual void OnActiveDocumentChanged(imod::IModel* activeDocumentPtr);

	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnRetranslate();

	// reimplemented (imod::TSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected slots:
	void OnFileNewAction(QAction* action);
	void OnFileOpenAction(QAction* action);
	void OnNew();
	void OnOpen();
	void OnSave();
	void OnSaveAs();
	void OnNewDocument(const QString& documentTypeId);
	void OnOpenDocument(const std::string* documentTypeIdPtr = NULL);
	void OnQuit();
	void OnUndo();
	void OnRedo();
	void OnFullScreen();
//	void OnTileHorizontally();
	void OnAbout();
	void OnLanguageSelected(QAction* a); 
	void OnStyleSelected(QAction* a);

protected:
	void SetupMainWindow(QMainWindow& mainWindow);
	void CreateMenuComponents(QMainWindow& mainWindow);
	void SetupMenuComponents(QMainWindow& mainWindow);
	void SetupFileMenu();
	void SetupEditMenu();
	void SetupTranslationMenu();
	void SetupStyleMenu();
	void SetupStatusBar(QMainWindow& mainWindow);
	void SetupWorkspace(QMainWindow& mainWindow);
	void SetupMainWindowComponents(QMainWindow& mainWindow);
	bool HasDocumentTemplate() const;
	void UpdateUndoMenu();

private:
	QMenuBar* m_menuBar;
	QMenu* m_fileMenu;
	QMenu* m_editMenu;
	QMenu* m_viewMenu;
	QMenu* m_windowMenu;
	QMenu* m_helpMenu;
	QAction* m_cascadeAction;
	QAction* m_tileHorizontallyAction;
	QAction* m_tileVerticallyAction;
	QAction* m_fullScreenAction;
	QAction* m_closeAllDocumentsAction;
	QAction* m_aboutAction;

	QMenu* m_newMenu;
	QMenu* m_openMenu;
	QAction* m_newAction;
	QAction* m_openAction;
	QAction* m_saveAction;
	QAction* m_saveAsAction;
	QAction* m_quitAction;
	QAction* m_undoAction;
	QAction* m_redoAction;
	QActionGroup* m_languageGroup;
	QToolBar* m_standardToolBar;

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

private:
	I_REF(iqt::IGuiObject, m_workspaceCompPtr);
	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(iqt::ITranslationManager, m_translationManagerCompPtr);
	I_MULTIREF(iqt::IMainWindowComponent, m_mainWindowComponentsPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
};


} // namespace iqmain


#endif // !iqmain_CMainWindowComp_included

