#include <iqtdoc/CMainWindowGuiComp.h>


// Qt includes
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QMimeData>
#include <QtGui/QClipboard>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QDesktopServices>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QApplication>
#else
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#endif

// ACF includes
#include <imod/IObserver.h>
#include <ibase/ICommandsProvider.h>
#include <iqt/CSettingsWriteArchive.h>
#include <iqt/CSettingsReadArchive.h>
#include <iqt/CSignalBlocker.h>


namespace iqtdoc
{


// public methods

CMainWindowGuiComp::CMainWindowGuiComp()
:	m_activeUndoManager(*this),
	m_activeViewPtr(NULL),
	m_activeDocumentPtr(NULL),
	m_newCommand("", 31, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	m_openCommand("", 32, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	m_saveCommand("", 33, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	m_undoCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	m_redoCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	m_copyPathToClipboardCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU),
	m_openDocumentFolderCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU)
{
	m_quitCommand.setMenuRole(QAction::QuitRole);

	m_newCommand.setShortcut(Qt::CTRL + Qt::Key_N);
	m_openCommand.setShortcut(Qt::CTRL + Qt::Key_O);
	m_saveCommand.setShortcut(Qt::CTRL + Qt::Key_S);
	m_undoCommand.setShortcut(Qt::CTRL + Qt::Key_Z);
	m_redoCommand.setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Z);

	m_undoCommand.setVisible(false);
	m_redoCommand.setVisible(false);

	connect(&m_newCommand, SIGNAL(triggered()), this, SLOT(OnNew()));
	connect(&m_openCommand, SIGNAL(triggered()), this, SLOT(OnOpen()));
	connect(&m_saveCommand, SIGNAL(triggered()), this, SLOT(OnSave()));
	connect(&m_saveAsCommand, SIGNAL(triggered()), this, SLOT(OnSaveAs()));
	connect(&m_quitCommand, SIGNAL(triggered()), this, SLOT(OnQuit()));
	connect(&m_undoCommand, SIGNAL(triggered()), this, SLOT(OnUndo()));
	connect(&m_redoCommand, SIGNAL(triggered()), this, SLOT(OnRedo()));
	connect(&m_copyPathToClipboardCommand, SIGNAL(triggered()), this, SLOT(OnCopyPathToClipboard()));
	connect(&m_openDocumentFolderCommand, SIGNAL(triggered()), this, SLOT(OnOpenDocumentFolder()));
}


// reimplemented (imod::IObserver)

bool CMainWindowGuiComp::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	bool retVal = BaseClass2::OnModelAttached(modelPtr, changeMask);

	if (retVal){
		const idoc::IDocumentManager* managerPtr = GetObservedObject();
		if (managerPtr != NULL){
			m_fileCommand.ResetChilds();
			m_editCommand.ResetChilds();

			SetupNewCommand();

			m_fileCommand.SetPriority(30);

			bool showOpenCommand = m_isOpenCommandVisibleAttrPtr.IsValid() ? *m_isOpenCommandVisibleAttrPtr : true;
			if (showOpenCommand){
				m_openCommand.SetGroupId(GI_DOCUMENT);
				m_fileCommand.InsertChild(&m_openCommand, false);
			}

			bool showSaveCommand = m_isSaveCommandVisibleAttrPtr.IsValid() ? *m_isSaveCommandVisibleAttrPtr: true;
			if (showSaveCommand){
				m_saveCommand.SetGroupId(GI_DOCUMENT);

				m_saveAsCommand.SetGroupId(GI_DOCUMENT);
				m_saveAsCommand.SetPriority(34);

				m_fileCommand.InsertChild(&m_saveCommand, false);
				m_fileCommand.InsertChild(&m_saveAsCommand, false);
			}

			// Create recent file list:
			if (showOpenCommand){
				const idoc::IDocumentManager* managerPtr = GetObservedObject();
				if (managerPtr != NULL){
					idoc::IDocumentTypesInfo::Ids ids = managerPtr->GetDocumentTypeIds();
					for (		idoc::IDocumentTypesInfo::Ids::const_iterator iter = ids.begin();
								iter != ids.end();
								++iter){
						const QByteArray& documentTypeId = *iter;
						Q_ASSERT(!documentTypeId.isEmpty());

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


bool CMainWindowGuiComp::OnModelDetached(imod::IModel* modelPtr)
{
	bool retVal = BaseClass2::OnModelDetached(modelPtr);

	if (retVal){
		if (m_applicationInfoCompPtr.IsValid()){
			QString applicationName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME, false);
			QString companyName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME, false);

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

		bool ignoredFlag = false;
		retVal = m_documentManagerCompPtr->OpenDocument(NULL, &fileName, true, "", NULL, &fileMap, false, &ignoredFlag);
		if (retVal){
			UpdateRecentFileList(fileMap);
		}
		else if (!ignoredFlag){
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
	imod::IModel* oldModelPtr = m_activeUndoManager.GetObservedModel();
	if (oldModelPtr != NULL){
		oldModelPtr->DetachObserver(&m_activeUndoManager);
	}

	bool hasUndoManager = false;
	idoc::IDocumentManager* documentManagerPtr = GetObservedObject();
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

	UpdateMainWindowComponentsVisibility();

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

	int newSupportedCount = 0;

	for (		idoc::IDocumentTypesInfo::Ids::ConstIterator iter = ids.constBegin();
				iter != ids.constEnd();
				++iter){
		const QByteArray& documentTypeId = *iter;

		if (m_documentManagerCompPtr->IsFeatureSupported(idoc::IDocumentTypesInfo::SF_NEW_DOCUMENT, documentTypeId)){
			++newSupportedCount;
		}
	}

	bool isNewSupported = (newSupportedCount > 0);

	if (newSupportedCount > 1){
		for (		idoc::IDocumentTypesInfo::Ids::ConstIterator iter = ids.constBegin();
					iter != ids.constEnd();
					++iter){
			const QByteArray& documentTypeId = *iter;

			if (m_documentManagerCompPtr->IsFeatureSupported(idoc::IDocumentTypesInfo::SF_NEW_DOCUMENT, documentTypeId)){
				NewDocumentCommand* newCommandPtr = new NewDocumentCommand(this, documentTypeId);
				if (newCommandPtr != NULL){
					QString commandName = m_documentManagerCompPtr->GetDocumentTypeName(documentTypeId);
					newCommandPtr->SetVisuals(commandName, commandName, tr("Creates new document %1").arg(commandName));
					m_newCommand.InsertChild(newCommandPtr, true);
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
	idoc::IUndoManager* undoManagerPtr = m_activeUndoManager.GetObservedObject();
	if (undoManagerPtr != NULL){
		int undoSteps = undoManagerPtr->GetAvailableUndoSteps();
		m_undoCommand.SetEnabled(undoSteps > 0);

		QString undoStepDescription;
		if (undoSteps > 0){
			undoStepDescription = undoManagerPtr->GetUndoLevelDescription(1);
		}

		if (!undoStepDescription.isEmpty()){
			m_undoCommand.setToolTip(tr("Undo: %1").arg(undoStepDescription));
		}
		else{
			m_undoCommand.setToolTip("");
		}

		int redoSteps = undoManagerPtr->GetAvailableRedoSteps();
		m_redoCommand.SetEnabled(redoSteps > 0);

		QString redoStepDescription;
		if (redoSteps > 0){
			redoStepDescription = undoManagerPtr->GetRedoLevelDescription(1);
		}

		if (!redoStepDescription.isEmpty()){
			m_redoCommand.setToolTip(tr("Redo: %1").arg(redoStepDescription));
		}
		else{
			m_redoCommand.setToolTip("");
		}
	}
}


void CMainWindowGuiComp::OnNewDocument(const QByteArray& documentFactoryId)
{
	if (m_documentManagerCompPtr.IsValid()){
		bool ignoredFlag = false;
		if (!m_documentManagerCompPtr->InsertNewDocument(documentFactoryId, true, "", NULL, false, &ignoredFlag) && !ignoredFlag){
			QMessageBox::warning(GetWidget(), "", tr("Document could not be created"));
		}
	}
}


bool CMainWindowGuiComp::SerializeRecentFileList(iser::IArchive& archive)
{
	int documentTypeIdsCount = m_recentFilesMap.size();

	static iser::CArchiveTag documentGroupsTag("DocumentGroups", "List of document ID's", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag groupTag("Group", "Group of documents with the same ID", iser::CArchiveTag::TT_GROUP, &documentGroupsTag);
	static iser::CArchiveTag documentTypeIdTag("DocumentTypeId", "Document Type ID", iser::CArchiveTag::TT_LEAF, &groupTag);
	static iser::CArchiveTag fileListTag("FileList", "List of recent files", iser::CArchiveTag::TT_MULTIPLE, &groupTag);
	static iser::CArchiveTag filePathTag("FilePath", "File path", iser::CArchiveTag::TT_LEAF, &fileListTag);

	bool retVal = archive.BeginMultiTag(documentGroupsTag, groupTag, documentTypeIdsCount);

	if (archive.IsStoring()){
		for (		RecentFilesMap::const_iterator index = m_recentFilesMap.begin();
					index != m_recentFilesMap.end();
					index++){
			retVal = retVal && archive.BeginTag(groupTag);

			QByteArray documentTypeId = index.key();
			Q_ASSERT(!documentTypeId.isEmpty());

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
				Q_ASSERT(groupCommandPtr.IsValid());

				RecentFileCommand* commandPtr = dynamic_cast<RecentFileCommand*>(groupCommandPtr->GetChild(i));
				if ((commandPtr != NULL) && commandPtr->IsOpenCommand()){
					QString filePath = commandPtr->GetActionString();

					retVal = retVal && archive.BeginTag(filePathTag);
					retVal = retVal && archive.Process(filePath);
					retVal = retVal && archive.EndTag(filePathTag);
				}
			}

			retVal = retVal && archive.EndTag(fileListTag);

			retVal = retVal && archive.EndTag(groupTag);
		}
	}
	else{
		for (int typeIndex = 0; typeIndex < documentTypeIdsCount; typeIndex++){
			retVal = retVal && archive.BeginTag(groupTag);

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

			retVal = retVal && archive.EndTag(groupTag);
		}

		BaseClass::UpdateMenuActions();
	}

	retVal = retVal && archive.EndTag(documentGroupsTag);

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
		Q_ASSERT(!documentTypeId.isEmpty());

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


bool CMainWindowGuiComp::IsMainWindowActive(int index) const
{
	Q_ASSERT(index >= 0);

	if (index >= m_mainWindowCompTypeIdsAttrPtr.GetCount()){
		return true;
	}

	QByteArray activeDocumentTypeId;
	if (m_activeDocumentPtr != NULL && m_documentManagerCompPtr.IsValid()){
		activeDocumentTypeId = m_documentManagerCompPtr->GetDocumentTypeId(*m_activeDocumentPtr);
	}

	QList<QByteArray> associatedTypeIds = m_mainWindowCompTypeIdsAttrPtr[index].split(',');

	return (associatedTypeIds.isEmpty() || (associatedTypeIds.contains(activeDocumentTypeId)));
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


void CMainWindowGuiComp::AppendMenuActions(iqtgui::CHierarchicalCommand& menuCommands)
{
	BaseClass::AppendMenuActions(menuCommands);

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

	// Load the document from command line:
	if (m_applicationCompPtr.IsValid()){
		QStringList applicationArguments = m_applicationCompPtr->GetApplicationArguments();
		if (applicationArguments.count() > 1 && m_documentManagerCompPtr.IsValid()){
			QString documentFileName = applicationArguments[1];
			idoc::IDocumentManager::FileToTypeMap fileMap;

			if (m_documentManagerCompPtr->OpenDocument(NULL, &documentFileName, true, "", NULL, &fileMap)){
				UpdateRecentFileList(fileMap);
			}
		}
	}

	if (m_documentManagerCommandsCompPtr.IsValid()){
		imod::IModel* modelPtr = CompCastPtr<imod::IModel>(m_documentManagerCommandsCompPtr.GetPtr());
		if (modelPtr != NULL){
			static const istd::IChangeable::ChangeSet commandsChangeSet(ibase::ICommandsProvider::CF_COMMANDS);
			m_commandsObserver.RegisterModel(modelPtr, CPI_DOCUMENT_MANAGER, commandsChangeSet);
		}
	}

	mainWindowPtr->installEventFilter(this);
}


void CMainWindowGuiComp::OnRetranslate()
{
	// Main commands
	m_fileCommand.SetName(tr("&File"));
	m_editCommand.SetName(tr("&Edit"));

	// File commands
	m_newCommand.SetVisuals(tr("&New"), tr("New"), tr("Creates new document"), QIcon(":/Icons/New"));
	m_openCommand.SetVisuals(tr("&Open..."), tr("Open"), tr("Opens document from file"), QIcon(":/Icons/Open"));
	m_saveCommand.SetVisuals(tr("&Save"), tr("Save"), tr("Saves document to actual working file"), QIcon(":/Icons/Save"));
	m_saveAsCommand.SetVisuals(tr("&Save As..."), tr("Save As"), tr("Saves document into selected file"));
	m_quitCommand.SetVisuals(tr("&Quit"), tr("Quit"), tr("Quits this application"), QIcon(":/Icons/Shutdown"));
	m_undoCommand.SetVisuals(tr("&Undo"), tr("Undo"), tr("Undo last document changes"), QIcon(":/Icons/Undo"));
	m_redoCommand.SetVisuals(tr("&Redo"), tr("Redo"), tr("Redo last document changes"), QIcon(":/Icons/Redo"));

	// Tools commands
	m_copyPathToClipboardCommand.SetVisuals(tr("&Copy Document Path"), tr("Copy Path"), tr("Copy current document path to system clipboard"));
	m_openDocumentFolderCommand.SetVisuals(tr("&Open Containing Folder"), tr("Open Containing Folder"), tr("Open folder containing the current document"));

	BaseClass::OnRetranslate();
}


// reimplemented (imod::TSingleModelObserverBase)

void CMainWindowGuiComp::OnUpdate(const istd::IChangeable::ChangeSet& changeSet)
{
	static const istd::IChangeable::ChangeSet commandsChangeSet(ibase::ICommandsProvider::CF_COMMANDS);

	if (changeSet.Contains(idoc::IDocumentManager::CF_VIEW_ACTIVATION_CHANGED)){
		idoc::IDocumentManager* documentManagerPtr = GetObservedObject();
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
					m_commandsObserver.RegisterModel(modelPtr, CPI_ACTIVE_VIEW, commandsChangeSet);
				}
			}

			if (isDocumentChanged){
				m_commandsObserver.UnregisterModel(CPI_ACTIVE_DOCUMENT);
				ibase::ICommandsProvider* commandsProviderPtr = CompCastPtr<ibase::ICommandsProvider>(documentPtr);
				imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(commandsProviderPtr);
				if (modelPtr != NULL){
					m_commandsObserver.RegisterModel(modelPtr, CPI_ACTIVE_DOCUMENT, commandsChangeSet);
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
		Q_ASSERT(dragEnterEventPtr != NULL);

		OnDragEnterEvent(dragEnterEventPtr);

		return true;
	}
	else if (eventPtr->type() == QEvent::Drop){
		QDropEvent* dropEventPtr = dynamic_cast<QDropEvent*>(eventPtr);
		Q_ASSERT(dropEventPtr != NULL);

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

		iqt::CSettingsReadArchive archive(companyName, applicationName, "RecentFileList", QSettings::UserScope);

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
		Q_ASSERT(!documentTypeId.isEmpty());

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

		istd::IPolymorphic* activeViewPtr = m_documentManagerCompPtr->GetActiveView();
		Q_ASSERT(activeViewPtr != NULL);

		idoc::IDocumentManager::DocumentInfo activeDocumentInfo;
		m_documentManagerCompPtr->GetDocumentFromView(*activeViewPtr, &activeDocumentInfo);

		int saveMode = -1;

		if (activeDocumentInfo.filePath.isEmpty()){
			saveMode = 0;
		}
		else{
			QFileInfo fileInfo(activeDocumentInfo.filePath);
			if (fileInfo.exists() && !fileInfo.permission(QFile::WriteUser)){
				saveMode = QMessageBox::warning(
							GetWidget(),
							tr("Save file"),
							QString(tr("The file %1 cannot be saved because it is write-protected")).arg(fileInfo.fileName()),
							tr("Save As"),
							tr("Overwrite"),
							tr("Cancel"),
							2);
			}
		}

		switch (saveMode){
		case 0:
			OnSaveAs();
			return;

		case 1:
			{
				QFile file(activeDocumentInfo.filePath);
				if (!file.setPermissions(QFile::WriteUser)){
					QMessageBox::critical(
								GetWidget(),
								tr("Save file"),
								QString(tr("The file %1 cannot be overwritten. Possible you have not enough permissions")).arg(activeDocumentInfo.filePath));
					return;	
				}
			}
			break;

		case 2:
			return;
		}

		bool ignoredFlag = false;
		if (m_documentManagerCompPtr->SaveDocument(-1, false, &fileMap, false, &ignoredFlag)){
			UpdateRecentFileList(fileMap);
		}
		else if (!ignoredFlag){
			QMessageBox::critical(GetWidget(), "", tr("File could not be saved!"));
		}

		BaseClass::UpdateMenuActions();
	}
}


void CMainWindowGuiComp::OnSaveAs()
{
	if (m_documentManagerCompPtr.IsValid()){
		idoc::IDocumentManager::FileToTypeMap fileMap;

		bool ignoredFlag = false;
		if (m_documentManagerCompPtr->SaveDocument(-1, true, &fileMap, false, &ignoredFlag)){
			UpdateRecentFileList(fileMap);
		}
		else if (!ignoredFlag){
			QMessageBox::critical(GetWidget(), "", tr("File could not be saved!"));
		}

		BaseClass::UpdateMenuActions();
	}
}


void CMainWindowGuiComp::OnOpenDocument(const QByteArray* documentTypeIdPtr)
{
	idoc::IDocumentManager::FileToTypeMap fileMap;

	bool ignoredFlag = false;
	if (m_documentManagerCompPtr.IsValid()){
		if (m_documentManagerCompPtr->OpenDocument(documentTypeIdPtr, NULL, true, "", NULL, &fileMap, false, &ignoredFlag)){
			UpdateRecentFileList(fileMap);
		}
		else if (!ignoredFlag){
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
	Q_ASSERT(m_activeUndoManager.GetObservedObject() != NULL);

	m_activeUndoManager.GetObservedObject()->DoUndo();

	UpdateUndoMenu();
}


void CMainWindowGuiComp::OnRedo()
{
	Q_ASSERT(m_activeUndoManager.GetObservedObject() != NULL);

	m_activeUndoManager.GetObservedObject()->DoRedo();

	UpdateUndoMenu();
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
				QDesktopServices::openUrl(QString("file:///") + fileInfo.absoluteDir().absolutePath());
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

bool CMainWindowGuiComp::ActiveUndoManager::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	EnsureModelDetached();

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// protected methods of embedded class ActiveUndoManager

// reimplemented (imod::CSingleModelObserverBase)

void CMainWindowGuiComp::ActiveUndoManager::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	m_parent.UpdateUndoMenu();
}


} // namespace iqtdoc


