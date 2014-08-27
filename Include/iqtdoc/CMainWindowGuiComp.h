#ifndef iqtdoc_CMainWindowGuiComp_included
#define iqtdoc_CMainWindowGuiComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#else
#include <QtGui/QMainWindow>
#include <QtGui/QDockWidget>
#endif

// ACF includes
#include "imod/IModel.h"
#include "imod/TSingleModelObserverBase.h"
#include "ibase/IApplication.h"
#include "ibase/IApplicationInfo.h"
#include "ibase/ICommandsProvider.h"
#include "idoc/IUndoManager.h"
#include "idoc/IDocumentManager.h"
#include "iqtgui/CSimpleMainWindowGuiComp.h"


namespace iqtdoc
{


// Base class as work-around of static variable limit in ACF macros
class CMainWindowGuiCompBase:
			public iqtgui::CSimpleMainWindowGuiComp
{
public:
	typedef iqtgui::CSimpleMainWindowGuiComp BaseClass;

	I_BEGIN_BASE_COMPONENT(CMainWindowGuiCompBase);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info", true, "ApplicationInfo");
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Document manager", true, "DocumentManager");
		I_ASSIGN_TO(m_documentManagerModelCompPtr, m_documentManagerCompPtr, true);
		I_ASSIGN_TO(m_documentManagerCommandsCompPtr, m_documentManagerCompPtr, false);
		I_ASSIGN(m_applicationCompPtr, "Application", "Access to the application's command line", true, "Application");
		I_ASSIGN(m_isCopyPathVisibleAttrPtr, "IsCopyPathVisible", "If true, operation Tools/CopyDocumentPath will be visible", true, false);
		I_ASSIGN(m_isOpenContainingFolderVisibleAttrPtr, "IsOpenContainingFolderVisible", "If true, operation Tools/Open Containing Folder will be visible", true, false);
		I_ASSIGN(m_maxRecentFilesCountAttrPtr, "MaxRecentFiles", "Maximal size of recent file list for one document type", true, 10);
		I_ASSIGN(m_isOpenCommandVisibleAttrPtr, "OpenCommandVisible", "If enabled the open document command will be shown", false, true);
		I_ASSIGN(m_isSaveCommandVisibleAttrPtr, "SaveCommandVisible", "If enabled the save document commands will be shown", false, true);
		I_ASSIGN_MULTI_0(m_mainWindowCompTypeIdsAttrPtr, "MainWindowCompTypeIds", "Set of document type IDs (comma seperated) to enable main window component, empty string enables component independly from document selection", true);
	I_END_COMPONENT;

protected:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(imod::IModel, m_documentManagerModelCompPtr);
	I_REF(ibase::ICommandsProvider, m_documentManagerCommandsCompPtr);
	I_REF(ibase::IApplication, m_applicationCompPtr);
	I_ATTR(bool, m_isCopyPathVisibleAttrPtr);
	I_ATTR(bool, m_isOpenContainingFolderVisibleAttrPtr);
	I_ATTR(int, m_maxRecentFilesCountAttrPtr);
	I_ATTR(bool, m_isOpenCommandVisibleAttrPtr);
	I_ATTR(bool, m_isSaveCommandVisibleAttrPtr);
	I_MULTIATTR(QByteArray, m_mainWindowCompTypeIdsAttrPtr);
};


/**
	Standard main window component for any document based application.
*/
class CMainWindowGuiComp:
			public CMainWindowGuiCompBase,
			protected imod::TSingleModelObserverBase<idoc::IDocumentManager>
{
	Q_OBJECT

public:
	typedef CMainWindowGuiCompBase BaseClass;
	typedef imod::TSingleModelObserverBase<idoc::IDocumentManager> BaseClass2;

	I_BEGIN_COMPONENT(CMainWindowGuiComp);
		I_REGISTER_SUBELEMENT(FileCommands);
		I_REGISTER_SUBELEMENT_INTERFACE(FileCommands, ibase::IHierarchicalCommand, GetFileCommands);
		I_REGISTER_SUBELEMENT_INTERFACE(FileCommands, ibase::ICommand, GetFileCommands);
		I_REGISTER_SUBELEMENT_INTERFACE(FileCommands, iprm::INameParam, GetFileCommands);
		I_REGISTER_SUBELEMENT_INTERFACE(FileCommands, iprm::IEnableableParam, GetFileCommands);
		I_REGISTER_SUBELEMENT_INTERFACE(FileCommands, istd::IChangeable, GetFileCommands);
		I_REGISTER_SUBELEMENT(EditCommands);
		I_REGISTER_SUBELEMENT_INTERFACE(FileCommands, ibase::IHierarchicalCommand, GetEditCommands);
		I_REGISTER_SUBELEMENT_INTERFACE(FileCommands, ibase::ICommand, GetEditCommands);
		I_REGISTER_SUBELEMENT_INTERFACE(FileCommands, iprm::INameParam, GetEditCommands);
		I_REGISTER_SUBELEMENT_INTERFACE(FileCommands, iprm::IEnableableParam, GetEditCommands);
		I_REGISTER_SUBELEMENT_INTERFACE(FileCommands, istd::IChangeable, GetEditCommands);
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
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);
	virtual bool OnModelDetached(imod::IModel* modelPtr);

protected:
	virtual bool OpenFile(const QString& fileName);

	virtual void OnActiveViewChanged();
	virtual void OnActiveDocumentChanged();
	virtual void OnDragEnterEvent(QDragEnterEvent* dragEnterEventPtr);
	virtual void OnDropEvent(QDropEvent* dropEventPtr);

	void SetupNewCommand();
	bool HasDocumentTemplate() const;
	void UpdateUndoMenu();

	void OnNewDocument(const QByteArray& documentTypeId);

	virtual bool SerializeRecentFileList(iser::IArchive& archive);
	/**
		Updates displayed list of recent used files.
		\param	fileToTypeMap	list of new files and corresponding types.
	*/
	virtual void UpdateRecentFileList(const idoc::IDocumentManager::FileToTypeMap& fileToTypeMap);
	virtual void RemoveFromRecentFileList(const QString& filePath);

	virtual bool IsMainWindowActive(int index) const;

	// reimplemented (iqtgui::CSimpleMainWindowGuiComp)
	virtual void UpdateFixedCommands(iqtgui::CHierarchicalCommand& fixedCommands);
	virtual void UpdateToolsCommands(iqtgui::CHierarchicalCommand& toolsCommand);
	virtual void AppendMenuActions(iqtgui::CHierarchicalCommand& menuCommands);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnRetranslate();

	// reimplemented (imod::TSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

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
	void OnOpenDocument(const QByteArray* documentTypeIdPtr = NULL);
	void OnQuit();
	void OnUndo();
	void OnRedo();
	void OnCopyPathToClipboard();
	void OnOpenDocumentFolder();

private:
	class NewDocumentCommand: public iqtgui::CHierarchicalCommand
	{
	public:
		typedef iqtgui::CHierarchicalCommand BaseClass;

		NewDocumentCommand(CMainWindowGuiComp* parentPtr, const QByteArray& documentTypeId): m_parent(*parentPtr), m_documentTypeId(documentTypeId){}

		// reimplemented (ibase::ICommand)
		virtual bool Execute(istd::IPolymorphic* /*contextPtr*/)
		{
			m_parent.OnNewDocument(m_documentTypeId);

			return true;
		}

	private:
		CMainWindowGuiComp& m_parent;
		QByteArray m_documentTypeId;
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

	class ActiveUndoManager: public imod::TSingleModelObserverBase<idoc::IUndoManager>
	{
	public:
		typedef imod::TSingleModelObserverBase<idoc::IUndoManager> BaseClass;

		ActiveUndoManager(CMainWindowGuiComp& parent);

		// reimplemented (imod::IObserver)
		virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	private:
		CMainWindowGuiComp& m_parent;
	};

	template <class InterfaceType>
	static InterfaceType* GetFileCommands(CMainWindowGuiComp& parent)
	{
		return &parent.m_fileCommand;
	}

	template <class InterfaceType>
	static InterfaceType* GetEditCommands(CMainWindowGuiComp& parent)
	{
		return &parent.m_editCommand;
	}

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
	// tools menu group
	iqtgui::CHierarchicalCommand m_copyPathToClipboardCommand;
	iqtgui::CHierarchicalCommand m_openDocumentFolderCommand;

	typedef istd::TDelPtr<iqtgui::CHierarchicalCommand> RecentGroupCommandPtr;
	typedef QMap<QByteArray, RecentGroupCommandPtr> RecentFilesMap;
	RecentFilesMap m_recentFilesMap;
};


} // namespace iqtdoc


#endif // !iqtdoc_CMainWindowGuiComp_included


