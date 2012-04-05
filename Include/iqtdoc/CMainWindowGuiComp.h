#ifndef iqtdoc_CMainWindowComp_included
#define iqtdoc_CMainWindowComp_included


// Qt includes
#include <QtGui/QMainWindow>
#include <QtGui/QDockWidget>

// ACF includes
#include "imod/IModel.h"
#include "imod/IUndoManager.h"
#include "imod/TSingleModelObserverBase.h"

#include "ibase/IApplicationInfo.h"
#include "ibase/ICommandsProvider.h"

#include "idoc/IDocumentManager.h"

#include "iqtgui/CSimpleMainWindowGuiComp.h"


namespace iqtdoc
{


/**
	Standard main window component for any document based application.
*/
class CMainWindowGuiComp:
			public iqtgui::CSimpleMainWindowGuiComp,
			protected imod::TSingleModelObserverBase<idoc::IDocumentManager>
{
	Q_OBJECT

public:
	typedef iqtgui::CSimpleMainWindowGuiComp BaseClass;
	typedef imod::TSingleModelObserverBase<idoc::IDocumentManager> BaseClass2;

	I_BEGIN_COMPONENT(CMainWindowGuiComp);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info", true, "ApplicationInfo");
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Document manager", true, "DocumentManager");
		I_ASSIGN_TO(m_documentManagerModelCompPtr, m_documentManagerCompPtr, true);
		I_ASSIGN_TO(m_documentManagerCommandsCompPtr, m_documentManagerCompPtr, false);
		I_ASSIGN(m_isCopyPathVisibleAttrPtr, "IsCopyPathVisible", "If true, operation Tools/CopyDocumentPath will be visible", true, false);
		I_ASSIGN(m_isOpenContainingFolderVisibleAttrPtr, "IsOpenContainingFolderVisible", "If true, operation Tools/Open Containing Folder will be visible", true, false);
		I_ASSIGN(m_maxRecentFilesCountAttrPtr, "MaxRecentFiles", "Maximal size of recent file list for one document type", true, 10);
	I_END_COMPONENT;

	enum GroupId
	{
		GI_APPLICATION = 0x200,
		GI_DOCUMENT,
		GI_UNDO,
		GI_RECENT_FILE
	};

	enum CommandProviderId
	{
		CPI_DOCUMENT_MANAGER = 100,
		CPI_ACTIVE_VIEW,
		CPI_ACTIVE_DOCUMENT
	};

	CMainWindowGuiComp();

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

protected:
	virtual bool OpenFile(const QString& fileName);

	virtual void OnActiveViewChanged();
	virtual void OnActiveDocumentChanged();
	virtual void OnDragEnterEvent(QDragEnterEvent* dragEnterEventPtr);
	virtual void OnDropEvent(QDropEvent* dropEventPtr);

	void SetupNewCommand();
	bool HasDocumentTemplate() const;
	void UpdateUndoMenu();

	void OnNewDocument(const std::string& documentTypeId);

	virtual bool SerializeRecentFileList(iser::IArchive& archive);
	virtual void UpdateRecentFileList(const idoc::IDocumentManager::FileToTypeMap& fileToTypeMap);
	virtual void RemoveFromRecentFileList(const QString& filePath);

	// reimplemented (iqtgui::CSimpleMainWindowGuiComp)
	virtual void UpdateFixedCommands(iqtgui::CHierarchicalCommand& fixedCommands);
	virtual void UpdateViewCommands(iqtgui::CHierarchicalCommand& viewCommand);
	virtual void UpdateToolsCommands(iqtgui::CHierarchicalCommand& toolsCommand);
	virtual void UpdateMenuActions(iqtgui::CHierarchicalCommand& menuCommands);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnRetranslate();

	// reimplemented (imod::TSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected Q_SLOTS:
	void OnNew();
	void OnOpen();
	void OnSave();
	void OnSaveAs();
	void OnOpenDocument(const std::string* documentTypeIdPtr = NULL);
	void OnQuit();
	void OnUndo();
	void OnRedo();
	void OnFullScreen();
	void OnCopyPathToClipboard();
	void OnOpenDocumentFolder();

private:
	class NewDocumentCommand: public iqtgui::CHierarchicalCommand
	{
	public:
		typedef iqtgui::CHierarchicalCommand BaseClass;

		NewDocumentCommand(CMainWindowGuiComp* parentPtr, const std::string& documentTypeId): m_parent(*parentPtr), m_documentTypeId(documentTypeId){}

		// reimplemented (ibase::ICommand)
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

		RecentFileCommand(
					CMainWindowGuiComp* parentPtr,
					const QString& name,
					const QString& actionString,
					bool isOpenCommand);

		const QString& GetActionString() const;
		bool IsOpenCommand() const;

		// reimplemented (ibase::ICommand)
		virtual bool Execute(istd::IPolymorphic* /*contextPtr*/);

	private:
		CMainWindowGuiComp& m_parent;
		QString m_actionString;
		bool m_isOpenCommand;
	};

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

	iqtgui::CHierarchicalCommand m_fileCommand;
	iqtgui::CHierarchicalCommand m_editCommand;

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
	// tools menu group
	iqtgui::CHierarchicalCommand m_copyPathToClipboardCommand;
	iqtgui::CHierarchicalCommand m_openDocumentFolderCommand;

	typedef istd::TDelPtr<iqtgui::CHierarchicalCommand> RecentGroupCommandPtr;
	typedef QMap<std::string, RecentGroupCommandPtr> RecentFilesMap;
	RecentFilesMap m_recentFilesMap;

	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(imod::IModel, m_documentManagerModelCompPtr);
	I_REF(ibase::ICommandsProvider, m_documentManagerCommandsCompPtr);
	I_ATTR(bool, m_isCopyPathVisibleAttrPtr);
	I_ATTR(bool, m_isOpenContainingFolderVisibleAttrPtr);
	I_ATTR(int, m_maxRecentFilesCountAttrPtr);

	QByteArray m_beforeFullScreenGeometry;
	QByteArray m_beforeFullScreenState;
};


} // namespace iqtdoc


#endif // !iqtdoc_CMainWindowComp_included

