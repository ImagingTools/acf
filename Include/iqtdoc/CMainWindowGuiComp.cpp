#include "iqtdoc/CMainWindowGuiComp.h"


// Qt includes
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QStatusBar>
#include <QtGui/QClipboard>
#include <QtGui/QDesktopServices>

// ACF includes
#include "imod/IObserver.h"

#include "ibase/ICommandsProvider.h"

#include "iqt/CSettingsWriteArchive.h"
#include "iqt/CSettingsReadArchive.h"


namespace iqtdoc
{


// public methods

CMainWindowGuiComp::CMainWindowGuiComp()
:	m_activeUndoManager(*this),
	m_activeViewPtr(NULL),
	m_activeDocumentPtr(NULL),
	m_newCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	m_openCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	m_saveCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	m_undoCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	m_redoCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	m_fullScreenCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_ONOFF),
	m_copyPathToClipboardCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU),
	m_openDocumentFolderCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU)
{
	m_quitCommand.setMenuRole(QAction::QuitRole);

	connect(&m_newCommand, SIGNAL(triggered()), this, SLOT(OnNew()));
	connect(&m_openCommand, SIGNAL(triggered()), this, SLOT(OnOpen()));
	connect(&m_saveCommand, SIGNAL(triggered()), this, SLOT(OnSave()));
	connect(&m_saveAsCommand, SIGNAL(triggered()), this, SLOT(OnSaveAs()));
	connect(&m_quitCommand, SIGNAL(triggered()), this, SLOT(OnQuit()));
	connect(&m_undoCommand, SIGNAL(triggered()), this, SLOT(OnUndo()));
	connect(&m_redoCommand, SIGNAL(triggered()), this, SLOT(OnRedo()));
	connect(&m_fullScreenCommand, SIGNAL(triggered()), this, SLOT(OnFullScreen()));
	connect(&m_copyPathToClipboardCommand, SIGNAL(triggered()), this, SLOT(OnCopyPathToClipboard()));
	connect(&m_openDocumentFolderCommand, SIGNAL(triggered()), this, SLOT(OnOpenDocumentFolder()));
}


// reimplemented (imod::IObserver)

bool CMainWindowGuiComp::OnAttached(imod::IModel* modelPtr)
{
	bool retVal = BaseClass2::OnAttached(modelPtr);

	if (retVal){
		const idoc::IDocumentManager* managerPtr = GetObjectPtr();
		if (managerPtr != NULL){
			m_fileCommand.ResetChilds();
			m_editCommand.ResetChilds();

			SetupNewCommand();

			m_fileCommand.SetPriority(30);
			m_openCommand.SetGroupId(GI_DOCUMENT);
			m_fileCommand.InsertChild(&m_openCommand, false);
			m_saveCommand.SetGroupId(GI_DOCUMENT);
			m_fileCommand.InsertChild(&m_saveCommand, false);
			m_saveAsCommand.SetGroupId(GI_DOCUMENT);
			m_fileCommand.InsertChild(&m_saveAsCommand, false);

			const idoc::IDocumentManager* managerPtr = GetObjectPtr();
			if (managerPtr != NULL){
				idoc::IDocumentTypesInfo::Ids ids = managerPtr->GetDocumentTypeIds();
				for (		idoc::IDocumentTypesInfo::Ids::const_iterator iter = ids.begin();
							iter != ids.end();
							++iter){
					const QByteArray& documentTypeId = *iter;
					I_ASSERT(!documentTypeId.isEmpty());

					RecentGroupCommandPtr& groupCommandPtr = m_recentFilesMap[documentTypeId];

					QString documentTypeName = managerPtr->GetDocumentTypeName(documentTypeId);

					QString recentListTitle = (ids.size() > 1)?
								tr("Recent %1 Files").arg(documentTypeName):
								tr("Recent Files");
					iqtgui::CHierarchicalCommand* fileListCommandPtr = new iqtgui::CHierarchicalCommand(recentListTitle);

					if (fileListCommandPtr != NULL){
						fileListCommandPtr->SetPriority(130);

						groupCommandPtr.SetPtr(fileListCommandPtr);

						m_fileCommand.InsertChild(fileListCommandPtr, false);
					}
				}
			}

			m_quitCommand.SetPriority(999);
			m_quitCommand.SetGroupId(GI_APPLICATION);
			m_fileCommand.InsertChild(&m_quitCommand, false);

			m_editCommand.SetPriority(60);
			m_undoCommand.SetGroupId(GI_UNDO);
			m_editCommand.InsertChild(&m_undoCommand, false);
			m_redoCommand.SetGroupId(GI_UNDO);
			m_editCommand.InsertChild(&m_redoCommand, false);
		}
	}

	return retVal;
}


bool CMainWindowGuiComp::OnDetached(imod::IModel* modelPtr)
{
	bool retVal = BaseClass2::OnDetached(modelPtr);

	if (retVal){
		if (m_applicationInfoCompPtr.IsValid()){ 
			QString applicationName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME);
			QString companyName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME);

			iqt::CSettingsWriteArchive archive(
						companyName,
						applicationName,
						"RecentFileList",
						QSettings::UserScope,
						&m_applicationInfoCompPtr->GetVersionInfo());

			SerializeRecentFileList(archive);
		}

		m_recentFilesMap.clear();
	}

	return retVal;
}



// protected methods

bool CMainWindowGuiComp::OpenFile(const QString& fileName)
{
	bool retVal = false;

	if (m_documentManagerCompPtr.IsValid()){
		idoc::IDocumentManager::FileToTypeMap fileMap;

		retVal = m_documentManagerCompPtr->FileOpen(NULL, &fileName, true, "", NULL, &fileMap);
		if (retVal){
			UpdateRecentFileList(fileMap);
		}
		else{
			QMessageBox::warning(GetWidget(), "", tr("Document could not be opened"));

			RemoveFromRecentFileList(QString(fileName));
		}
	}

	return retVal;
}


void CMainWindowGuiComp::OnActiveViewChanged()
{
	BaseClass::UpdateMenuActions();
}


void CMainWindowGuiComp::OnActiveDocumentChanged()
{
	imod::IModel* oldModelPtr = m_activeUndoManager.GetModelPtr();
	if (oldModelPtr != NULL){
		oldModelPtr->DetachObserver(&m_activeUndoManager);
	}

	bool hasUndoManager = false;
	idoc::IDocumentManager* documentManagerPtr = GetObjectPtr();
	if (documentManagerPtr != NULL){
		imod::IModel* activeUndoManagerModelPtr = dynamic_cast<imod::IModel*>(documentManagerPtr->GetUndoManagerForDocument(m_activeDocumentPtr));
		if (activeUndoManagerModelPtr != NULL){
			activeUndoManagerModelPtr->AttachObserver(&m_activeUndoManager);

			hasUndoManager = true;
		}
	}

	m_undoCommand.setVisible(hasUndoManager);
	m_redoCommand.setVisible(hasUndoManager);

	UpdateUndoMenu();

	if (!IsGuiCreated()){
		return;
	}

	BaseClass::UpdateMenuActions();
}


void CMainWindowGuiComp::OnDragEnterEvent(QDragEnterEvent* dragEnterEventPtr)
{
	if (dragEnterEventPtr->mimeData()->hasFormat("text/uri-list")){
         dragEnterEventPtr->acceptProposedAction();
	}
}


void CMainWindowGuiComp::OnDropEvent(QDropEvent* dropEventPtr)
{
	const QMimeData* mimeData = dropEventPtr->mimeData();
	if (mimeData->hasUrls()){
		QList<QUrl> files = mimeData->urls();

		for (int fileIndex = 0; fileIndex < files.count(); fileIndex++){
			QString filePath = files.at(fileIndex).toLocalFile();

			if (m_documentManagerCompPtr.IsValid()){
				idoc::IDocumentTypesInfo::Ids availableDocumentIds = m_documentManagerCompPtr->GetDocumentTypeIdsForFile(filePath);
				if (!availableDocumentIds.isEmpty()){

					OpenFile(filePath);
				}
			}
		}
	}
}


void CMainWindowGuiComp::SetupNewCommand()
{
	if (!m_documentManagerCompPtr.IsValid()){
		return;
	}
	
	idoc::IDocumentTypesInfo::Ids ids = m_documentManagerCompPtr->GetDocumentTypeIds();
	if (ids.isEmpty()){
		return;
	}

	m_newCommand.SetGroupId(GI_DOCUMENT);

	bool isNewSupported = m_documentManagerCompPtr->IsFeatureSupported(idoc::IDocumentTypesInfo::SF_NEW_DOCUMENT, ids.front());

	if (ids.size() > 1){
		for (		idoc::IDocumentTypesInfo::Ids::const_iterator iter = ids.begin();
					iter != ids.end();
					++iter){
			const QByteArray& documentTypeId = *iter;

			if (m_documentManagerCompPtr->IsFeatureSupported(idoc::IDocumentTypesInfo::SF_NEW_DOCUMENT, documentTypeId)){
				NewDocumentCommand* newCommandPtr = new NewDocumentCommand(this, documentTypeId);
				if (newCommandPtr != NULL){
					QString commandName = m_documentManagerCompPtr->GetDocumentTypeName(documentTypeId);
					newCommandPtr->SetVisuals(commandName, commandName, tr("Creates new document %1").arg(commandName));
					m_newCommand.InsertChild(newCommandPtr, true);

					isNewSupported = true;
				}
			}
		}
	}

	if (isNewSupported){
		m_fileCommand.InsertChild(&m_newCommand, false);
	}
}


bool CMainWindowGuiComp::HasDocumentTemplate() const
{
	if (!m_documentManagerCompPtr.IsValid()){
		return false;
	}

	idoc::IDocumentTypesInfo::Ids documentTypeIds = m_documentManagerCompPtr->GetDocumentTypeIds();
	if (documentTypeIds.isEmpty()){
		return false;
	}

	return true;
}


void CMainWindowGuiComp::UpdateUndoMenu()
{
	idoc::IUndoManager* undoManagerPtr = m_activeUndoManager.GetObjectPtr();
	if (undoManagerPtr != NULL){
		m_undoCommand.SetEnabled(undoManagerPtr->IsUndoAvailable());
		m_redoCommand.SetEnabled(undoManagerPtr->IsRedoAvailable());
	}
}


void CMainWindowGuiComp::OnNewDocument(const QByteArray& documentFactoryId)
{
	if (m_documentManagerCompPtr.IsValid()){
		if (!m_documentManagerCompPtr->FileNew(documentFactoryId)){
			QMessageBox::warning(GetWidget(), "", tr("Document could not be created"));
			return;
		}
	}
}


bool CMainWindowGuiComp::SerializeRecentFileList(iser::IArchive& archive)
{
	int documentTypeIdsCount = m_recentFilesMap.size();

	static iser::CArchiveTag documentTypeIdsTag("DocumentIds", "List of document ID's");
	static iser::CArchiveTag documentTypeIdTag("DocumentTypeId", "Document Type ID");
	static iser::CArchiveTag fileListTag("FileList", "List of recent files");
	static iser::CArchiveTag filePathTag("FilePath", "File path");

	bool retVal = archive.BeginMultiTag(documentTypeIdsTag, documentTypeIdTag, documentTypeIdsCount);

	if (archive.IsStoring()){
		for (		RecentFilesMap::const_iterator index = m_recentFilesMap.begin();
					index != m_recentFilesMap.end();
					index++){
			QByteArray documentTypeId = index.key();
			I_ASSERT(!documentTypeId.isEmpty());

			retVal = retVal && archive.BeginTag(documentTypeIdTag);
			retVal = retVal && archive.Process(documentTypeId);
			retVal = retVal && archive.EndTag(documentTypeIdTag);

			const RecentGroupCommandPtr& groupCommandPtr = index.value();

			int elementsCount = groupCommandPtr.IsValid()? groupCommandPtr->GetChildsCount(): 0;

			int filesCount = 0;
			for (int i = 0; i < elementsCount; ++i){
				const RecentFileCommand* commandPtr = dynamic_cast<const RecentFileCommand*>(groupCommandPtr->GetChild(i));
				if ((commandPtr != NULL) && commandPtr->IsOpenCommand()){
					filesCount++;
				}
			}

			retVal = retVal && archive.BeginMultiTag(fileListTag, filePathTag, filesCount);

			for (int i = filesCount - 1; i >= 0; --i){
				I_ASSERT(groupCommandPtr.IsValid());

				RecentFileCommand* commandPtr = dynamic_cast<RecentFileCommand*>(groupCommandPtr->GetChild(i));
				if ((commandPtr != NULL) && commandPtr->IsOpenCommand()){
					QString filePath = commandPtr->GetActionString();

					retVal = retVal && archive.BeginTag(filePathTag);
					retVal = retVal && archive.Process(filePath);					
					retVal = retVal && archive.EndTag(filePathTag);
				}
			}

			retVal = retVal && archive.EndTag(fileListTag);
		}
	}
	else{
		for (int typeIndex = 0; typeIndex < documentTypeIdsCount; typeIndex++){
			QByteArray documentTypeId;

			retVal = retVal && archive.BeginTag(documentTypeIdTag);
			retVal = retVal && archive.Process(documentTypeId);
			retVal = retVal && archive.EndTag(documentTypeIdTag);

			if (documentTypeId.isEmpty()){
				return false;
			}

			RecentGroupCommandPtr& groupCommandPtr = m_recentFilesMap[documentTypeId];

			if (groupCommandPtr.IsValid()){
				groupCommandPtr->ResetChilds();
			}

			int filesCount = 0;
			retVal = retVal && archive.BeginMultiTag(fileListTag, filePathTag, filesCount);

			for (int fileIndex = 0; fileIndex < filesCount; fileIndex++){
				QString filePath;
				retVal = retVal && archive.BeginTag(filePathTag);
				retVal = retVal && archive.Process(filePath);					
				retVal = retVal && archive.EndTag(filePathTag);

				if (retVal && !filePath.isEmpty()){
					idoc::IDocumentManager::FileToTypeMap fileMap;

					fileMap[filePath] = documentTypeId;

					UpdateRecentFileList(fileMap);
				}
			}

			retVal = retVal && archive.EndTag(fileListTag);
		}

		BaseClass::UpdateMenuActions();
	}

	retVal = retVal && archive.EndTag(documentTypeIdsTag);

	return retVal;
}


void CMainWindowGuiComp::UpdateRecentFileList(const idoc::IDocumentManager::FileToTypeMap& fileToTypeMap)
{
	for (		idoc::IDocumentManager::FileToTypeMap::const_iterator iter = fileToTypeMap.begin();
				iter != fileToTypeMap.end();
				++iter){
		QFileInfo fileInfo(iter.key());

		QString filePath = fileInfo.absoluteFilePath();
		const QByteArray documentTypeId = iter.value();
		I_ASSERT(!documentTypeId.isEmpty());

		RemoveFromRecentFileList(filePath);

		RecentGroupCommandPtr& groupCommandPtr = m_recentFilesMap[documentTypeId];

		if (groupCommandPtr.IsValid()){
			if (groupCommandPtr->GetChildsCount() == 0){
				RecentFileCommand* commandPtr = new RecentFileCommand(this, tr("Clear List"), documentTypeId, false);
				groupCommandPtr->InsertChild(commandPtr, true);
			}

			RecentFileCommand* commandPtr = new RecentFileCommand(this, filePath, filePath, true);
			groupCommandPtr->InsertChild(commandPtr, true, 0);

			int childsCount = groupCommandPtr->GetChildsCount();
			if ((childsCount > 1) && (childsCount > *m_maxRecentFilesCountAttrPtr + 1)){
				groupCommandPtr->RemoveChild(childsCount - 2);
			}
		}
	}
}


void CMainWindowGuiComp::RemoveFromRecentFileList(const QString& filePath)
{
	for (		RecentFilesMap::iterator iter = m_recentFilesMap.begin();
				iter != m_recentFilesMap.end();
				++iter){
		RecentGroupCommandPtr& groupCommandPtr = iter.value();

		if (!groupCommandPtr.IsValid()){
			continue;
		}

		int childsCount = groupCommandPtr->GetChildsCount();

		for (int i = 0; i < childsCount; ++i){
			const ibase::ICommand* commandPtr = groupCommandPtr->GetChild(i);
			if ((commandPtr != NULL) && (commandPtr->GetName() == filePath)){
				groupCommandPtr->RemoveChild(i);
				--childsCount;
				--i;
			}
		}
	}
}


// reimplemented (iqtgui::CSimpleMainWindowGuiComp)

void CMainWindowGuiComp::UpdateFixedCommands(iqtgui::CHierarchicalCommand& fixedCommands)
{
	BaseClass::UpdateFixedCommands(fixedCommands);

	// fill menu bar with main commands
	if (m_documentManagerCompPtr.IsValid()){
		fixedCommands.InsertChild(&m_fileCommand, false, 0);

		if (m_activeDocumentPtr != NULL){
			if (m_documentManagerCompPtr.IsValid()){
				QByteArray documentTypeId = m_documentManagerCompPtr->GetDocumentTypeId(*m_activeDocumentPtr);
					
				if (m_documentManagerCompPtr->IsFeatureSupported(idoc::IDocumentTypesInfo::SF_EDIT_DOCUMENT, documentTypeId)){
					fixedCommands.InsertChild(&m_editCommand, false, 1);
				}
			}
		}
	}
}


void CMainWindowGuiComp::UpdateViewCommands(iqtgui::CHierarchicalCommand& viewCommand)
{
	BaseClass::UpdateViewCommands(viewCommand);

	viewCommand.InsertChild(&m_fullScreenCommand, false);
}


void CMainWindowGuiComp::UpdateToolsCommands(iqtgui::CHierarchicalCommand& toolsCommand)
{
	BaseClass::UpdateToolsCommands(toolsCommand);

	if (*m_isCopyPathVisibleAttrPtr){
		toolsCommand.InsertChild(&m_copyPathToClipboardCommand, false);
	}

	if (*m_isOpenContainingFolderVisibleAttrPtr){
		toolsCommand.InsertChild(&m_openDocumentFolderCommand, false);
	}
}


void CMainWindowGuiComp::UpdateMenuActions(iqtgui::CHierarchicalCommand& menuCommands)
{
	BaseClass::UpdateMenuActions(menuCommands);

	int allowedOperationFlags = 0;

	if (m_documentManagerCompPtr.IsValid()){
		allowedOperationFlags = m_documentManagerCompPtr->GetAllowedOperationFlags();
	}

	m_newCommand.SetEnabled((allowedOperationFlags & idoc::IDocumentManager::OF_FILE_NEW) != 0);
	m_openCommand.SetEnabled((allowedOperationFlags & idoc::IDocumentManager::OF_FILE_OPEN) != 0);
	m_saveCommand.SetEnabled((allowedOperationFlags & idoc::IDocumentManager::OF_FILE_SAVE) != 0);
	m_saveAsCommand.SetEnabled((allowedOperationFlags & idoc::IDocumentManager::OF_FILE_SAVE_AS) != 0);
	m_copyPathToClipboardCommand.SetEnabled((allowedOperationFlags & idoc::IDocumentManager::OF_KNOWN_PATH) != 0);
	m_openDocumentFolderCommand.SetEnabled((allowedOperationFlags & idoc::IDocumentManager::OF_KNOWN_PATH) != 0);

	if (m_documentManagerCommandsCompPtr.IsValid()){
		const ibase::IHierarchicalCommand* commandsPtr = m_documentManagerCommandsCompPtr->GetCommands();
		if (commandsPtr != NULL){
			menuCommands.JoinLinkFrom(commandsPtr);
		}
	}

	istd::IPolymorphic* activeViewPtr = m_activeViewPtr;
	if (m_documentManagerCompPtr.IsValid()){
		activeViewPtr = m_documentManagerCompPtr->GetActiveView();
	}

	const ibase::ICommandsProvider* viewProviderPtr = CompCastPtr<ibase::ICommandsProvider>(activeViewPtr);
	if (viewProviderPtr != NULL){
		const ibase::IHierarchicalCommand* commandsPtr = viewProviderPtr->GetCommands();
		if (commandsPtr != NULL){
			menuCommands.JoinLinkFrom(commandsPtr);
		}
	}

	istd::IChangeable* activeDocumentPtr = m_activeDocumentPtr;
	if (m_documentManagerCompPtr.IsValid()){
		if (activeViewPtr != NULL){
			activeDocumentPtr = m_documentManagerCompPtr->GetDocumentFromView(*activeViewPtr);
		}
		else{		
			activeDocumentPtr = NULL;
		}
	}

	const ibase::ICommandsProvider* documentProviderPtr = CompCastPtr<ibase::ICommandsProvider>(activeDocumentPtr);
	if (documentProviderPtr != NULL){
		const ibase::IHierarchicalCommand* commandsPtr = documentProviderPtr->GetCommands();
		if (commandsPtr != NULL){
			menuCommands.JoinLinkFrom(commandsPtr);
		}
	}
}


// reimplemented (iqtgui::TGuiComponentBase)

void CMainWindowGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QMainWindow* mainWindowPtr = GetQtWidget();
	if (mainWindowPtr == NULL){
		return;
	}

	UpdateUndoMenu();
	
 	mainWindowPtr->setAcceptDrops(true);

	mainWindowPtr->installEventFilter(this);

	// load the document from command line:
	QStringList applicationArguments = qApp->arguments();
	if (applicationArguments.count() > 1 && m_documentManagerCompPtr.IsValid()){
		QString documentFileName = applicationArguments[1];
		idoc::IDocumentManager::FileToTypeMap fileMap;

		if (m_documentManagerCompPtr->FileOpen(NULL, &documentFileName, true, "", NULL, &fileMap)){
			UpdateRecentFileList(fileMap);
		}
	}

	if (m_documentManagerCommandsCompPtr.IsValid()){
		imod::IModel* modelPtr = CompCastPtr<imod::IModel>(m_documentManagerCommandsCompPtr.GetPtr());
		if (modelPtr != NULL){
			m_commandsObserver.RegisterModel(modelPtr, CPI_DOCUMENT_MANAGER, ibase::ICommandsProvider::CF_COMMANDS);
		}
	}
}


void CMainWindowGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


void CMainWindowGuiComp::OnRetranslate()
{
	BaseClass::OnRetranslate();

	// Main commands
	m_fileCommand.SetName(tr("&File"));
	m_editCommand.SetName(tr("&Edit"));

	// File commands
	m_newCommand.SetVisuals(tr("&New"), tr("New"), tr("Creates new document"), QIcon(":/Icons/New"));
	m_newCommand.setShortcut(tr("Ctrl+N"));
	m_openCommand.SetVisuals(tr("&Open..."), tr("Open"), tr("Opens document from file"), QIcon(":/Icons/Open.svg"));
	m_openCommand.setShortcut(tr("Ctrl+O"));
	m_saveCommand.SetVisuals(tr("&Save"), tr("Save"), tr("Saves document to actual working file"), QIcon(":/Icons/Save.svg"));
	m_saveCommand.setShortcut(tr("Ctrl+S"));
	m_saveAsCommand.SetVisuals(tr("&Save As..."), tr("Save As"), tr("Saves document into selected file"));
	m_quitCommand.SetVisuals(tr("&Quit"), tr("Quit"), tr("Quits this application"), QIcon(":/Icons/Shutdown.svg"));
	m_undoCommand.SetVisuals(tr("&Undo"), tr("Undo"), tr("Undo last document changes"), QIcon(":/Icons/Undo.svg"));
	m_undoCommand.setShortcut(tr("Ctrl+Z"));
	m_redoCommand.SetVisuals(tr("&Redo"), tr("Redo"), tr("Redo last document changes"), QIcon(":/Icons/Redo.svg"));
	m_redoCommand.setShortcut(tr("Ctrl+Shift+Z"));
	
	// View commands
	m_fullScreenCommand.SetVisuals(tr("&Full Screen"), tr("Full Screen"), tr("Turn full screen mode on/off"));
	m_fullScreenCommand.setShortcut(tr("F11"));

	// Tools commands
	m_copyPathToClipboardCommand.SetVisuals(tr("&Copy Document Path"), tr("Copy Path"), tr("Copy current document path to system clipboard"));
	m_openDocumentFolderCommand.SetVisuals(tr("&Open Containing Folder"), tr("Open Containing Folder"), tr("Open folder containing the current document"));
}


// reimplemented (imod::TSingleModelObserverBase)

void CMainWindowGuiComp::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if ((updateFlags & idoc::IDocumentManager::CF_VIEW_ACTIVATION_CHANGED) != 0){
		idoc::IDocumentManager* documentManagerPtr = GetObjectPtr();
		if (documentManagerPtr != NULL){
			istd::IChangeable* documentPtr = NULL;

			istd::IPolymorphic* activeViewPtr = documentManagerPtr->GetActiveView();

			if (activeViewPtr != NULL){
				documentPtr = documentManagerPtr->GetDocumentFromView(*activeViewPtr);
			}

			bool isViewChanged = (activeViewPtr != m_activeViewPtr);
			bool isDocumentChanged = (documentPtr != m_activeDocumentPtr);

			if (isViewChanged){
				m_commandsObserver.UnregisterModel(CPI_ACTIVE_VIEW);
				ibase::ICommandsProvider* commandsProviderPtr = CompCastPtr<ibase::ICommandsProvider>(activeViewPtr);
				imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(commandsProviderPtr);
				if (modelPtr != NULL){
					m_commandsObserver.RegisterModel(modelPtr, CPI_ACTIVE_VIEW, ibase::ICommandsProvider::CF_COMMANDS);
				}
			}

			if (isDocumentChanged){
				m_commandsObserver.UnregisterModel(CPI_ACTIVE_DOCUMENT);
				ibase::ICommandsProvider* commandsProviderPtr = CompCastPtr<ibase::ICommandsProvider>(documentPtr);
				imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(commandsProviderPtr);
				if (modelPtr != NULL){
					m_commandsObserver.RegisterModel(modelPtr, CPI_ACTIVE_DOCUMENT, ibase::ICommandsProvider::CF_COMMANDS);
				}
			}

			m_activeViewPtr = activeViewPtr;
			m_activeDocumentPtr = documentPtr;

			if (isViewChanged){
				OnActiveViewChanged();
			}

			if (isDocumentChanged){
				OnActiveDocumentChanged();
			}
		}
	}
}


// reimplemented (QObject)

bool CMainWindowGuiComp::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	if (eventPtr->type() == QEvent::DragEnter){
		QDragEnterEvent* dragEnterEventPtr = dynamic_cast<QDragEnterEvent*>(eventPtr);
		I_ASSERT(dragEnterEventPtr != NULL);

		OnDragEnterEvent(dragEnterEventPtr);

		return true;
	}
	else if(eventPtr->type() == QEvent::Drop){
		QDropEvent* dropEventPtr = dynamic_cast<QDropEvent*>(eventPtr);
		I_ASSERT(dropEventPtr != NULL);

		OnDropEvent(dropEventPtr);

		return true;
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


// reimplemented (icomp::CComponentBase)

void CMainWindowGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_documentManagerModelCompPtr.IsValid()){
		m_documentManagerModelCompPtr->AttachObserver(this);
	}
	
	if (m_applicationInfoCompPtr.IsValid()){ 
		QString applicationName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME);
		QString companyName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME);

		iqt::CSettingsReadArchive archive(companyName, applicationName, "RecentFileList");

		SerializeRecentFileList(archive);
	}
}


void CMainWindowGuiComp::OnComponentDestroyed()
{
	m_fileCommand.ResetChilds();
	m_editCommand.ResetChilds();

	BaseClass2::EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// protected slots

void CMainWindowGuiComp::OnNew()
{
	if (!m_documentManagerCompPtr.IsValid()){
		return;
	}

	idoc::IDocumentTypesInfo::Ids ids = m_documentManagerCompPtr->GetDocumentTypeIds();
	for (		idoc::IDocumentTypesInfo::Ids::const_iterator iter = ids.begin();
				iter != ids.end();
				++iter){
		const QByteArray& documentTypeId = *iter;
		I_ASSERT(!documentTypeId.isEmpty());

		if (m_documentManagerCompPtr->IsFeatureSupported(idoc::IDocumentTypesInfo::SF_NEW_DOCUMENT, documentTypeId)){
			OnNewDocument(documentTypeId);

			return;
		}
	}
}


void CMainWindowGuiComp::OnOpen()
{
	OnOpenDocument(NULL);
}


void CMainWindowGuiComp::OnSave()
{
	if (m_documentManagerCompPtr.IsValid()){
		idoc::IDocumentManager::FileToTypeMap fileMap;

		if (m_documentManagerCompPtr->FileSave(-1, false, &fileMap)){
			UpdateRecentFileList(fileMap);
		}
		else{
			QMessageBox::critical(GetWidget(), "", tr("File could not be saved!"));
		}

		BaseClass::UpdateMenuActions();
	}
}


void CMainWindowGuiComp::OnSaveAs()
{
	if (m_documentManagerCompPtr.IsValid()){
		idoc::IDocumentManager::FileToTypeMap fileMap;

		if (m_documentManagerCompPtr->FileSave(-1, true, &fileMap)){
			UpdateRecentFileList(fileMap);
		}
		else{
			QMessageBox::critical(GetWidget(), "", tr("File could not be saved!"));
		}

		BaseClass::UpdateMenuActions();
	}
}


void CMainWindowGuiComp::OnOpenDocument(const QByteArray* documentTypeIdPtr)
{
	idoc::IDocumentManager::FileToTypeMap fileMap;

	if (m_documentManagerCompPtr.IsValid()){
		if (m_documentManagerCompPtr->FileOpen(documentTypeIdPtr, NULL, true, "", NULL, &fileMap)){
			UpdateRecentFileList(fileMap);
		}
		else{
			QMessageBox::warning(GetWidget(), "", tr("Document could not be opened"));
		}

		BaseClass::UpdateMenuActions();
	}
}


void CMainWindowGuiComp::OnQuit()
{
	QCoreApplication::quit();
}


void CMainWindowGuiComp::OnUndo()
{
	I_ASSERT(m_activeUndoManager.GetObjectPtr() != NULL);
	
	m_activeUndoManager.GetObjectPtr()->DoUndo();

	UpdateUndoMenu();
}


void CMainWindowGuiComp::OnRedo()
{
	I_ASSERT(m_activeUndoManager.GetObjectPtr() != NULL);

	m_activeUndoManager.GetObjectPtr()->DoRedo();
	
	UpdateUndoMenu();
}


void CMainWindowGuiComp::OnFullScreen()
{
	QMainWindow* mainWidgetPtr = GetQtWidget();

	I_ASSERT(mainWidgetPtr != NULL);

	QWidget* parentWidgetPtr = mainWidgetPtr->parentWidget();
	if (parentWidgetPtr == NULL){
		parentWidgetPtr = mainWidgetPtr;
	}

	if (parentWidgetPtr == NULL){
		return;
	}

	QStatusBar* statusBarPtr = mainWidgetPtr->statusBar();
	if (parentWidgetPtr->isFullScreen()){
		parentWidgetPtr->showNormal();
		statusBarPtr->show();

		mainWidgetPtr->restoreGeometry(m_beforeFullScreenGeometry);
		mainWidgetPtr->restoreState(m_beforeFullScreenState);
	}
	else{
		m_beforeFullScreenGeometry = mainWidgetPtr->saveGeometry();
		m_beforeFullScreenState = mainWidgetPtr->saveState();

		statusBarPtr->hide();
		SetToolBarsVisible(false);
		parentWidgetPtr->showFullScreen();
	}
}


void CMainWindowGuiComp::OnCopyPathToClipboard()
{
	if ((m_activeViewPtr != NULL) && m_documentManagerCompPtr.IsValid()){
		idoc::IDocumentManager::DocumentInfo info;
		if (m_documentManagerCompPtr->GetDocumentFromView(*m_activeViewPtr, &info) != NULL){
			QClipboard* clipboardPtr = QApplication::clipboard();
			if (clipboardPtr != NULL){
				QString filePath = QDir::toNativeSeparators(info.filePath);

				clipboardPtr->setText(filePath);
			}
		}
	}
}


void CMainWindowGuiComp::OnOpenDocumentFolder()
{
	if ((m_activeViewPtr != NULL) && m_documentManagerCompPtr.IsValid()){
		idoc::IDocumentManager::DocumentInfo info;
		if (m_documentManagerCompPtr->GetDocumentFromView(*m_activeViewPtr, &info) != NULL){
			QFileInfo fileInfo(info.filePath);
			if (fileInfo.absoluteDir().exists()){
				QDesktopServices::openUrl(fileInfo.absoluteDir().absolutePath());
			}
		}
	}
}


// public methods of embedded class RecentFileCommand

CMainWindowGuiComp::RecentFileCommand::RecentFileCommand(
			CMainWindowGuiComp* parentPtr,
			const QString& name,
			const QString& actionString,
			bool isOpenCommand)
:	m_parent(*parentPtr),
	m_actionString(actionString),
	m_isOpenCommand(isOpenCommand)
{
	SetName(name);
	SetGroupId(isOpenCommand? GI_RECENT_FILE: GI_APPLICATION);
}


const QString& CMainWindowGuiComp::RecentFileCommand::GetActionString() const
{
	return m_actionString;
}


bool CMainWindowGuiComp::RecentFileCommand::IsOpenCommand() const
{
	return m_isOpenCommand;
}


// reimplemented (ibase::ICommand)

bool CMainWindowGuiComp::RecentFileCommand::Execute(istd::IPolymorphic* /*contextPtr*/)
{
	if (m_isOpenCommand){
		m_parent.OpenFile(m_actionString);
	}
	else{
		RecentGroupCommandPtr& groupCommandPtr = m_parent.m_recentFilesMap[m_actionString.toLocal8Bit()];
		if (groupCommandPtr.IsValid()){
			groupCommandPtr->ResetChilds();
		}
	}

	return true;
}


// public methods of embedded class ActiveUndoManager

CMainWindowGuiComp::ActiveUndoManager::ActiveUndoManager(CMainWindowGuiComp& parent)
:	m_parent(parent)
{
}


// reimplemented (imod::IObserver)

bool CMainWindowGuiComp::ActiveUndoManager::OnAttached(imod::IModel* modelPtr)
{
	EnsureModelDetached();

	return BaseClass::OnAttached(modelPtr);
}


// protected methods of embedded class ActiveUndoManager
		
// reimplemented (imod::CSingleModelObserverBase)

void CMainWindowGuiComp::ActiveUndoManager::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.UpdateUndoMenu();
}


} // namespace iqtdoc


