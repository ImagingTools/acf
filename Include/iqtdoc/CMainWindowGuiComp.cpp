#include "iqtdoc/CMainWindowGuiComp.h"


// Qt includes
#include <QMessageBox>
#include <QApplication>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QFileInfo>
#include <QStatusBar>
#include <QStyle>


// ACF includes
#include "imod/IObserver.h"

#include "idoc/ICommandsProvider.h"

#include "iqt/CSettingsWriteArchive.h"
#include "iqt/CSettingsReadArchive.h"

#include "iqtgui/CGuiComponentDialog.h"


namespace iqtdoc
{


// public methods

CMainWindowGuiComp::CMainWindowGuiComp()
:	m_activeUndoManager(*this),
	m_activeViewPtr(NULL),
	m_activeDocumentPtr(NULL),
	m_menuCommands("Global"),
	m_newCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_TOOLBAR),
	m_openCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_TOOLBAR),
	m_saveCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_TOOLBAR),
	m_undoCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_TOOLBAR),
	m_redoCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_TOOLBAR),
	m_fullScreenCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_ONOFF),
	m_showToolBarsCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_ONOFF),
	m_menuBarPtr(NULL),
	m_standardToolBarPtr(NULL)
{
	connect(&m_newCommand, SIGNAL(activated()), this, SLOT(OnNew()));
	connect(&m_openCommand, SIGNAL(activated()), this, SLOT(OnOpen()));
	connect(&m_saveCommand, SIGNAL(activated()), this, SLOT(OnSave()));
	connect(&m_saveAsCommand, SIGNAL(activated()), this, SLOT(OnSaveAs()));
	connect(&m_quitCommand, SIGNAL(activated()), this, SLOT(OnQuit()));
	connect(&m_undoCommand, SIGNAL(activated()), this, SLOT(OnUndo()));
	connect(&m_redoCommand, SIGNAL(activated()), this, SLOT(OnRedo()));
	connect(&m_fullScreenCommand, SIGNAL(activated()), this, SLOT(OnFullScreen()));
	connect(&m_showToolBarsCommand, SIGNAL(activated()), this, SLOT(OnShowToolbars()));
	connect(&m_aboutCommand, SIGNAL(activated()), this, SLOT(OnAbout()));
}


// reimplemented (iqtgui::IGuiObject)

void CMainWindowGuiComp::OnTryClose(bool* ignoredPtr)
{
	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->OnTryClose(ignoredPtr);

		return;
	}

	BaseClass::OnTryClose(ignoredPtr);
}


// reimplemented (icomp::IComponent)

void CMainWindowGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_documentManagerModelCompPtr.IsValid()){
		m_documentManagerModelCompPtr->AttachObserver(this);
	}
	
	SerializeRecentFiles<iqt::CSettingsReadArchive>();
}


void CMainWindowGuiComp::OnComponentDestroyed()
{
	m_fileCommand.ResetChilds();
	m_editCommand.ResetChilds();
	m_viewCommand.ResetChilds();
	m_helpCommand.ResetChilds();
	m_fixedCommands.ResetChilds();

	if (m_documentManagerModelCompPtr.IsValid()){
		m_documentManagerModelCompPtr->DetachObserver(this);
	}

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::IObserver)

bool CMainWindowGuiComp::OnAttached(imod::IModel* modelPtr)
{
	bool retVal = BaseClass2::OnAttached(modelPtr);

	if (retVal){
		const idoc::IDocumentManager* managerPtr = GetObjectPtr();
		if (managerPtr != NULL){
			SetupNewCommand();

			m_fileCommand.SetPriority(30);
			m_openCommand.SetGroupId(GI_DOCUMENT);
			m_fileCommand.InsertChild(&m_openCommand, false);
			m_saveCommand.SetGroupId(GI_DOCUMENT);
			m_fileCommand.InsertChild(&m_saveCommand, false);
			m_saveAsCommand.SetGroupId(GI_DOCUMENT);
			m_fileCommand.InsertChild(&m_saveAsCommand, false);

			const idoc::IDocumentTemplate* templatePtr = managerPtr->GetDocumentTemplate();
			if (templatePtr != NULL){
				idoc::IDocumentTemplate::Ids ids = templatePtr->GetDocumentTypeIds();
				for (		idoc::IDocumentTemplate::Ids::const_iterator iter = ids.begin();
							iter != ids.end();
							++iter){
					const std::string& documentTypeId = *iter;
					I_ASSERT(!documentTypeId.empty());

					RecentGroupCommandPtr& groupCommandPtr = m_recentFilesMap[documentTypeId];

					QString recentListTitle = (ids.size() > 1)?
								tr("Recent %1 Files").arg(documentTypeId.c_str()):
								tr("Recent Files");
					iqtgui::CHierarchicalCommand* fileListCommandPtr = new iqtgui::CHierarchicalCommand(iqt::GetCString(recentListTitle));

					if (fileListCommandPtr != NULL){
						fileListCommandPtr->SetPriority(130);

						groupCommandPtr.SetPtr(fileListCommandPtr);

						m_fileCommand.InsertChild(fileListCommandPtr, false);
					}
				}
			}

			m_quitCommand.SetGroupId(GI_APPLICATION);
			m_fileCommand.InsertChild(&m_quitCommand, false);
		
			m_editCommand.SetPriority(60);
			m_undoCommand.SetGroupId(GI_UNDO);
			m_editCommand.InsertChild(&m_undoCommand, false);
			m_redoCommand.SetGroupId(GI_UNDO);
			m_editCommand.InsertChild(&m_redoCommand, false);

			m_viewCommand.SetPriority(90);
			m_viewCommand.InsertChild(&m_fullScreenCommand, false);
			m_viewCommand.InsertChild(&m_showToolBarsCommand, false);

			m_helpCommand.SetPriority(150);

			if (m_aboutGuiCompPtr.IsValid()){
				m_helpCommand.InsertChild(&m_aboutCommand, false);
			}

			UpdateFixedCommands();
		}
	}

	return retVal;
}


bool CMainWindowGuiComp::OnDetached(imod::IModel* modelPtr)
{
	bool retVal = BaseClass2::OnDetached(modelPtr);

	if (retVal){
		SerializeRecentFiles<iqt::CSettingsWriteArchive>();

		m_recentFilesMap.clear();
	}

	return retVal;
}


// protected methods

void CMainWindowGuiComp::OnActiveViewChanged()
{
	UpdateMenuActions();
}


void CMainWindowGuiComp::OnActiveDocumentChanged()
{
	imod::IModel* oldModelPtr = m_activeUndoManager.GetModelPtr();
	if (oldModelPtr != NULL){
		oldModelPtr->DetachObserver(&m_activeUndoManager);
	}

	idoc::IDocumentManager* documentManagerPtr = GetObjectPtr();
	if (documentManagerPtr != NULL){
		imod::IModel* activeUndoManagerModelPtr = dynamic_cast<imod::IModel*>(documentManagerPtr->GetUndoManagerForDocument(m_activeDocumentPtr));
		if (activeUndoManagerModelPtr != NULL){
			activeUndoManagerModelPtr->AttachObserver(&m_activeUndoManager);
		}
	}

	UpdateUndoMenu();

	if (!IsGuiCreated()){
		return;
	}

	UpdateMenuActions();
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
			istd::CString filePath = iqt::GetCString(files.at(fileIndex).toLocalFile());

			if (m_documentManagerCompPtr.IsValid()){
				const idoc::IDocumentTemplate* documentTemplatePtr = m_documentManagerCompPtr->GetDocumentTemplate();
				if (documentTemplatePtr != NULL){
					idoc::IDocumentTemplate::Ids availableDocumentIds = documentTemplatePtr->GetDocumentTypeIdsForFile(filePath);
					if (!availableDocumentIds.empty()){

						OnOpenFile(filePath);
					}
				}
			}
		}
	}
}



int CMainWindowGuiComp::CreateToolbar(const iqtgui::CHierarchicalCommand& command, QToolBar& result, int prevGroupId) const
{
	int childsCount = command.GetChildsCount();

	std::map<int, istd::TPointer<QActionGroup> > groups;

	for (int i = 0; i < childsCount; ++i){
		iqtgui::CHierarchicalCommand* hierarchicalPtr = const_cast<iqtgui::CHierarchicalCommand*>(
					dynamic_cast<const iqtgui::CHierarchicalCommand*>(command.GetChild(i)));

		if (hierarchicalPtr != NULL){
			int groupId = hierarchicalPtr->GetGroupId();
			int flags = hierarchicalPtr->GetStaticFlags();

			if ((flags & idoc::ICommand::CF_TOOLBAR) != 0){
				if (hierarchicalPtr->GetChildsCount() > 0){
					QMenu* newMenuPtr = new QMenu(&result);
					if (newMenuPtr != NULL){
						newMenuPtr->setTitle(iqt::GetQString(hierarchicalPtr->GetName()));

						CreateMenu<QMenu>(*hierarchicalPtr, *newMenuPtr);

						newMenuPtr->setIcon(hierarchicalPtr->icon());
						result.addAction(newMenuPtr->menuAction());
					}
				}
				else{
					if ((groupId != prevGroupId) && (prevGroupId != idoc::ICommand::GI_NONE)){
						result.addSeparator();
					}

					result.addAction(hierarchicalPtr);
				}

				if (groupId != idoc::ICommand::GI_NONE){
					prevGroupId = groupId;
				}
			}
			else if (hierarchicalPtr->GetChildsCount() > 0){
				prevGroupId = CreateToolbar(*hierarchicalPtr, result, prevGroupId);
			}
		}
	}

	return prevGroupId;
}


void CMainWindowGuiComp::SetToolBarsVisible(bool isVisible)
{
	QMainWindow* mainWindowPtr = GetQtWidget();
	I_ASSERT(mainWindowPtr != NULL);

	if (mainWindowPtr != NULL){
		for (int toolbarIndex = 0; toolbarIndex < m_toolBarsList.GetCount(); toolbarIndex++){
			QToolBar* toolbarPtr = m_toolBarsList.GetAt(toolbarIndex);

			if (!isVisible){
				mainWindowPtr->removeToolBar(toolbarPtr);
			}
			else{
				toolbarPtr->show();

				mainWindowPtr->addToolBar(toolbarPtr);
			}
		}
	}
}


void CMainWindowGuiComp::SetupNewCommand()
{
	if (!m_documentManagerCompPtr.IsValid()){
		return;
	}
	
	const idoc::IDocumentTemplate* templatePtr = m_documentManagerCompPtr->GetDocumentTemplate();
	if (templatePtr == NULL){
		return;
	}
	
	idoc::IDocumentTemplate::Ids ids = templatePtr->GetDocumentTypeIds();
	if (ids.empty()){
		return;
	}

	m_newCommand.SetGroupId(GI_DOCUMENT);

	bool isNewSupported = templatePtr->IsFeatureSupported(idoc::IDocumentTemplate::New, ids.front());

	if (ids.size() > 1){
		for (		idoc::IDocumentTemplate::Ids::const_iterator iter = ids.begin();
					iter != ids.end();
					++iter){
			if (templatePtr->IsFeatureSupported(idoc::IDocumentTemplate::New, *iter)){
				NewDocumentCommand* newCommandPtr = new NewDocumentCommand(this, *iter);
				if (newCommandPtr != NULL){
					QString commandName = iqt::GetQString(*iter);
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


void CMainWindowGuiComp::SetupMainWindowComponents(QMainWindow& mainWindow)
{
	if (HasDocumentTemplate()){
		mainWindow.addToolBar(Qt::TopToolBarArea, m_standardToolBarPtr.GetPtr());

		m_toolBarsList.PushBack(m_standardToolBarPtr.GetPtr(), false);
	}

	mainWindow.setMenuBar(m_menuBarPtr.GetPtr());

	if (m_mainWindowComponentsPtr.IsValid()){
		for (int componentIndex = 0; componentIndex < m_mainWindowComponentsPtr.GetCount(); componentIndex++){
			iqtgui::IMainWindowComponent* mainWindowComponentPtr =  m_mainWindowComponentsPtr[componentIndex];
			iqtgui::IGuiObject* guiPtr =  dynamic_cast<iqtgui::IGuiObject*>(mainWindowComponentPtr);
			if (mainWindowComponentPtr != NULL && guiPtr != NULL && guiPtr->CreateGui(NULL)){
				mainWindowComponentPtr->AddToMainWindow(mainWindow);
			}
		}
	}
}


bool CMainWindowGuiComp::HasDocumentTemplate() const
{
	if (!m_documentManagerCompPtr.IsValid()){
		return false;
	}

	const idoc::IDocumentTemplate* templatePtr = m_documentManagerCompPtr->GetDocumentTemplate();
	if (templatePtr == NULL){
		return false;
	}

	idoc::IDocumentTemplate::Ids documentTypeIds = templatePtr->GetDocumentTypeIds();
	if (documentTypeIds.empty()){
		return false;
	}

	return true;
}


void CMainWindowGuiComp::UpdateFixedCommands()
{
	m_fixedCommands.ResetChilds();

	bool editMenuActive = false;
	if (m_activeDocumentPtr != NULL){
		if (m_documentManagerCompPtr.IsValid()){
			std::string documentTypeId = m_documentManagerCompPtr->GetDocumentTypeId(*m_activeDocumentPtr);
				
			const idoc::IDocumentTemplate* templatePtr = m_documentManagerCompPtr->GetDocumentTemplate();
			if (templatePtr != NULL){
				editMenuActive = templatePtr->IsFeatureSupported(idoc::IDocumentTemplate::Edit, documentTypeId);
			}
		}
	}

	// fill menu bar with main commands
	m_fixedCommands.InsertChild(&m_fileCommand, false);
	if (editMenuActive){
		m_fixedCommands.InsertChild(&m_editCommand, false);
	}
	m_fixedCommands.InsertChild(&m_viewCommand, false);
	m_fixedCommands.InsertChild(&m_helpCommand, false);
}


void CMainWindowGuiComp::UpdateUndoMenu()
{
	bool isUndoAvailable = false;
	bool isRedoAvailable = false;

	imod::IUndoManager* undoManagerPtr = m_activeUndoManager.GetObjectPtr();
	if (undoManagerPtr != NULL){
		isUndoAvailable = undoManagerPtr->IsUndoAvailable();
		isRedoAvailable = undoManagerPtr->IsRedoAvailable();
	}

	m_undoCommand.SetEnabled(isUndoAvailable);
	m_redoCommand.SetEnabled(isRedoAvailable);
}


void CMainWindowGuiComp::UpdateMenuActions()
{
	if (!IsGuiCreated()){
		return;
	}

	bool isDocumentActive = (m_activeDocumentPtr != NULL);

	m_saveCommand.SetEnabled(isDocumentActive);
	m_saveAsCommand.SetEnabled(isDocumentActive);

	if (m_menuBarPtr == NULL){
		return;
	}
	m_menuCommands.ResetChilds();

	UpdateFixedCommands();

	m_menuCommands.JoinLinkFrom(&m_fixedCommands);

	if (m_documentManagerCompPtr.IsValid()){
		const idoc::IDocumentTemplate* templatePtr = m_documentManagerCompPtr->GetDocumentTemplate();
		const idoc::ICommandsProvider* templateProviderPtr = dynamic_cast<const idoc::ICommandsProvider*>(templatePtr);
		if (templateProviderPtr != NULL){
			const idoc::IHierarchicalCommand* commandsPtr = templateProviderPtr->GetCommands();
			if (commandsPtr != NULL){
				m_menuCommands.JoinLinkFrom(commandsPtr);
			}
		}
	}

	if (m_documentManagerCommandsCompPtr.IsValid()){
		const idoc::IHierarchicalCommand* commandsPtr = m_documentManagerCommandsCompPtr->GetCommands();
		if (commandsPtr != NULL){
			m_menuCommands.JoinLinkFrom(commandsPtr);
		}
	}

	const idoc::ICommandsProvider* viewProviderPtr = dynamic_cast<const idoc::ICommandsProvider*>(m_activeViewPtr);
	if (viewProviderPtr != NULL){
		const idoc::IHierarchicalCommand* commandsPtr = viewProviderPtr->GetCommands();
		if (commandsPtr != NULL){
			m_menuCommands.JoinLinkFrom(commandsPtr);
		}
	}

	const idoc::ICommandsProvider* documentProviderPtr = dynamic_cast<const idoc::ICommandsProvider*>(m_activeDocumentPtr);
	if (documentProviderPtr != NULL){
		const idoc::IHierarchicalCommand* commandsPtr = documentProviderPtr->GetCommands();
		if (commandsPtr != NULL){
			m_menuCommands.JoinLinkFrom(commandsPtr);
		}
	}

	m_menuBarPtr->clear();
	CreateMenu(m_menuCommands, *m_menuBarPtr);

	m_standardToolBarPtr->clear();
	CreateToolbar(m_menuCommands, *m_standardToolBarPtr);
}


// reimplemented (iqtgui::TGuiComponentBase)

void CMainWindowGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QMainWindow* mainWindowPtr = GetQtWidget();
	if (mainWindowPtr == NULL){
		return;
	}

	if (m_iconSizeAttrPtr.IsValid() && m_iconSizeAttrPtr->GetValue() != 0){
		mainWindowPtr->setIconSize(QSize(m_iconSizeAttrPtr->GetValue(), m_iconSizeAttrPtr->GetValue()));
	}

	m_menuBarPtr.SetPtr(new QMenuBar(mainWindowPtr));
	m_menuBarPtr->setGeometry(QRect(0, 0, 625, 45));

	m_standardToolBarPtr.SetPtr(new QToolBar(mainWindowPtr));
	m_standardToolBarPtr->setWindowTitle(tr("Standard"));

	if (m_useIconTextAttrPtr.IsValid() && m_useIconTextAttrPtr->GetValue()){
		m_standardToolBarPtr->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	}

	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->CreateGui(NULL);
		QWidget* workspacePtr = m_workspaceCompPtr->GetWidget();
		if (workspacePtr != NULL){
			mainWindowPtr->setCentralWidget(workspacePtr);
		}
	}


	OnRetranslate();

	UpdateMenuActions();
	UpdateUndoMenu();
	
	SetupMainWindowComponents(*mainWindowPtr);

	// TODO: Get desktop resolution and calculate the right initial size of the main window.
	QSize size(800, 600);
	size = size.expandedTo(mainWindowPtr->minimumSizeHint());
	mainWindowPtr->resize(size);

	mainWindowPtr->setAcceptDrops(true);

	mainWindowPtr->installEventFilter(this);

	m_showToolBarsCommand.setChecked(true);
}


void CMainWindowGuiComp::OnGuiDestroyed()
{
	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->DestroyGui();
	}
		
	if (m_mainWindowComponentsPtr.IsValid()){
		for (int componentIndex = 0; componentIndex < m_mainWindowComponentsPtr.GetCount(); componentIndex++){
			iqtgui::IMainWindowComponent* mainWindowComponentPtr =  m_mainWindowComponentsPtr[componentIndex];
			iqtgui::IGuiObject* guiPtr =  dynamic_cast<iqtgui::IGuiObject*>(mainWindowComponentPtr);
			if (mainWindowComponentPtr != NULL && guiPtr != NULL && guiPtr->IsGuiCreated()){
				guiPtr->DestroyGui();
			}
		}
	}

	m_menuBarPtr.Reset();
	m_standardToolBarPtr.Reset();

	BaseClass::OnGuiDestroyed();
}


void CMainWindowGuiComp::OnRetranslate()
{
	I_ASSERT(GetWidget() != NULL);

	QWidget* parentWidgetPtr = GetWidget()->parentWidget();
	if (parentWidgetPtr == NULL){
		parentWidgetPtr = GetWidget();
	}

	// Main commands
	m_fileCommand.SetName(iqt::GetCString(tr("&File")));
	m_editCommand.SetName(iqt::GetCString(tr("&Edit")));
	m_viewCommand.SetName(iqt::GetCString(tr("&View")));
	m_helpCommand.SetName(iqt::GetCString(tr("&Help")));

	// File commands
	m_newCommand.SetVisuals(tr("&New"), tr("New"), tr("Creates new document"), GetIcon("new"));
	m_newCommand.setShortcut(tr("Ctrl+N"));
	m_openCommand.SetVisuals(tr("&Open..."), tr("Open"), tr("Opens document from file"), GetIcon("open"));
	m_openCommand.setShortcut(tr("Ctrl+O"));
	m_saveCommand.SetVisuals(tr("&Save"), tr("Save"), tr("Saves document to actual working file"), GetIcon("save"));
	m_saveCommand.setShortcut(tr("Ctrl+S"));
	m_saveAsCommand.SetVisuals(tr("&Save As..."), tr("Save As"), tr("Saves document into selected file"));
	m_quitCommand.SetVisuals(tr("&Quit"), tr("Quit"), tr("Quits this application"), GetIcon("exit"));
	m_undoCommand.SetVisuals(tr("&Undo"), tr("Undo"), tr("Undo last document changes"), GetIcon("undo"));
	m_undoCommand.setShortcut(tr("Ctrl+Z"));
	m_redoCommand.SetVisuals(tr("&Redo"), tr("Redo"), tr("Redo last document changes"), GetIcon("redo"));
	m_redoCommand.setShortcut(tr("Ctrl+Shift+Z"));
	
	// View commands
	m_fullScreenCommand.SetVisuals(tr("&Full Screen"), tr("Full Screen"), tr("Turn full screen mode on/off"));
	m_fullScreenCommand.setShortcut(tr("F11"));
	m_showToolBarsCommand.SetVisuals(tr("&Show Toolbars"), tr("Show Toolbars"), tr("Show/Hide toolbars"));

	// Help commands
	m_aboutCommand.SetVisuals(tr("&About..."), tr("About"), tr("Shows information about this application"), GetIcon("info"));
}


// reimplemented (imod::TSingleModelObserverBase)

void CMainWindowGuiComp::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if ((updateFlags & idoc::IDocumentManager::ViewActivationChanged) != 0){
		idoc::IDocumentManager* documentManagerPtr = GetObjectPtr();
		if (documentManagerPtr != NULL){
			istd::IChangeable* documentPtr = NULL;

			istd::IPolymorphic* activeViewPtr = documentManagerPtr->GetActiveView();

			if (activeViewPtr != NULL){
				documentPtr = documentManagerPtr->GetDocumentFromView(*activeViewPtr);
			}

			bool isViewChanged = (activeViewPtr != m_activeViewPtr);
			bool isDocumentChanged = (documentPtr != m_activeDocumentPtr);

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

bool CMainWindowGuiComp::eventFilter(QObject* /*sourcePtr*/, QEvent* eventPtr)
{
	if (eventPtr->type() == QEvent::DragEnter){
		QDragEnterEvent* dragEnterEventPtr = dynamic_cast<QDragEnterEvent*>(eventPtr);
		I_ASSERT(dragEnterEventPtr != NULL);

		OnDragEnterEvent(dragEnterEventPtr);
	}
	else if(eventPtr->type() == QEvent::Drop){
		QDropEvent* dropEventPtr = dynamic_cast<QDropEvent*>(eventPtr);
		I_ASSERT(dropEventPtr != NULL);

		OnDropEvent(dropEventPtr);
	}

	return false;
}


// static methods

QIcon CMainWindowGuiComp::GetIcon(const std::string& name)
{
	return QIcon((":/Icons/" + name).c_str());
}


// protected slots

void CMainWindowGuiComp::OnNew()
{
	if (!m_documentManagerCompPtr.IsValid()){
		return;
	}

	const idoc::IDocumentTemplate* templatePtr = m_documentManagerCompPtr->GetDocumentTemplate();
	if (templatePtr == NULL){
		return;
	}

	idoc::IDocumentTemplate::Ids ids = templatePtr->GetDocumentTypeIds();
	if (!ids.empty()){
		const std::string& documentTypeId = ids.front();
		I_ASSERT(!documentTypeId.empty());

		OnNewDocument(documentTypeId);
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

		if (m_documentManagerCompPtr->FileSave(false, &fileMap)){
			UpdateRecentFileList(fileMap);

			UpdateMenuActions();
		}
		else{
			QMessageBox::critical(GetWidget(), "", tr("File could not be saved!"));
		}
	}
}


void CMainWindowGuiComp::OnSaveAs()
{
	if (m_documentManagerCompPtr.IsValid()){
		idoc::IDocumentManager::FileToTypeMap fileMap;

		if (m_documentManagerCompPtr->FileSave(true)){
			UpdateRecentFileList(fileMap);

			UpdateMenuActions();
		}
		else{
			QMessageBox::critical(GetWidget(), "", tr("File could not be saved!"));
		}
	}
}


void CMainWindowGuiComp::OnNewDocument(const std::string& documentFactoryId)
{
	if (m_documentManagerCompPtr.IsValid()){
		if (!m_documentManagerCompPtr->FileNew(documentFactoryId)){
			QMessageBox::warning(GetWidget(), "", tr("Document could not be created"));
			return;
		}
	}
}


void CMainWindowGuiComp::OnOpenFile(const istd::CString& fileName)
{
	if (m_documentManagerCompPtr.IsValid()){
		idoc::IDocumentManager::FileToTypeMap fileMap;

		if (m_documentManagerCompPtr->FileOpen(NULL, &fileName, true, "", &fileMap)){
			UpdateRecentFileList(fileMap);
		}
		else{
			QMessageBox::warning(GetWidget(), "", tr("Document could not be opened"));

			RemoveFromRecentFileList(istd::CString(fileName));
		}

		UpdateMenuActions();
	}
}


bool CMainWindowGuiComp::SerializeRecentFileList(iser::IArchive& archive)
{
	int documentTypeIdsCount = m_recentFilesMap.size();

	static iser::CArchiveTag recentFilesTag("RecentFileList", "List of application's recent files");
	static iser::CArchiveTag documentTypeIdsTag("DocumentIds", "List of document ID's");
	static iser::CArchiveTag documentTypeIdTag("DocumentTypeId", "Document Type ID");
	static iser::CArchiveTag fileListTag("FileList", "List of recent files");
	static iser::CArchiveTag filePathTag("FilePath", "File path");

	bool retVal = archive.BeginTag(recentFilesTag);
	retVal = retVal && archive.BeginMultiTag(documentTypeIdsTag, documentTypeIdTag, documentTypeIdsCount);

	if (archive.IsStoring()){
		for (		RecentFilesMap::const_iterator index = m_recentFilesMap.begin();
					index != m_recentFilesMap.end();
					index++){
			std::string documentTypeId = index->first;
			I_ASSERT(!documentTypeId.empty());

			retVal = retVal && archive.BeginTag(documentTypeIdTag);
			retVal = retVal && archive.Process(documentTypeId);
			retVal = retVal && archive.EndTag(documentTypeIdTag);

			const RecentGroupCommandPtr& groupCommandPtr = index->second;

			int filesCount = groupCommandPtr.IsValid()? groupCommandPtr->GetChildsCount(): 0;
			retVal = retVal && archive.BeginMultiTag(fileListTag, filePathTag, filesCount);

			for (int i = filesCount - 1; i >= 0; --i){
				I_ASSERT(groupCommandPtr.IsValid());

				idoc::ICommand* commandPtr = groupCommandPtr->GetChild(i);
				istd::CString filePath = (commandPtr != NULL)? commandPtr->GetName(): "";

				retVal = retVal && archive.BeginTag(filePathTag);
				retVal = retVal && archive.Process(filePath);					
				retVal = retVal && archive.EndTag(filePathTag);
			}

			retVal = retVal && archive.EndTag(fileListTag);
		}
	}
	else{
		for (int typeIndex = 0; typeIndex < documentTypeIdsCount; typeIndex++){
			std::string documentTypeId;

			retVal = retVal && archive.BeginTag(documentTypeIdTag);
			retVal = retVal && archive.Process(documentTypeId);
			retVal = retVal && archive.EndTag(documentTypeIdTag);

			if (documentTypeId.empty()){
				return false;
			}

			RecentGroupCommandPtr& groupCommandPtr = m_recentFilesMap[documentTypeId];

			if (groupCommandPtr.IsValid()){
				groupCommandPtr->ResetChilds();
			}

			int filesCount = 0;
			retVal = retVal && archive.BeginMultiTag(fileListTag, filePathTag, filesCount);

			for (int fileIndex = 0; fileIndex < filesCount; fileIndex++){
				istd::CString filePath;
				retVal = retVal && archive.BeginTag(filePathTag);
				retVal = retVal && archive.Process(filePath);					
				retVal = retVal && archive.EndTag(filePathTag);

				if (retVal && !filePath.IsEmpty()){
					idoc::IDocumentManager::FileToTypeMap fileMap;

					fileMap[filePath] = documentTypeId;

					UpdateRecentFileList(fileMap);
				}
			}

			retVal = retVal && archive.EndTag(fileListTag);
		}

		UpdateMenuActions();
	}

	retVal = retVal && archive.EndTag(documentTypeIdsTag);

	retVal = retVal && archive.EndTag(recentFilesTag);

	return retVal;
}

	
void CMainWindowGuiComp::UpdateRecentFileList(const idoc::IDocumentManager::FileToTypeMap& fileToTypeMap)
{
	for (		idoc::IDocumentManager::FileToTypeMap::const_iterator iter = fileToTypeMap.begin();
				iter != fileToTypeMap.end();
				++iter){
		QFileInfo fileInfo(iqt::GetQString(iter->first));

		istd::CString filePath = iqt::GetCString(fileInfo.absoluteFilePath());
		const std::string documentTypeId = iter->second;
		I_ASSERT(!documentTypeId.empty());

		RemoveFromRecentFileList(filePath);

		RecentGroupCommandPtr& groupCommandPtr = m_recentFilesMap[documentTypeId];

		if (groupCommandPtr.IsValid()){
			RecentFileCommand* commandPtr = new RecentFileCommand(this, filePath);
			groupCommandPtr->InsertChild(commandPtr, true, 0);

			int childsCount = groupCommandPtr->GetChildsCount();
			if ((childsCount > 0) && (childsCount > *m_maxRecentFilesCountAttrPtr)){
				groupCommandPtr->RemoveChild(childsCount - 1);
			}
		}
	}
}


void CMainWindowGuiComp::RemoveFromRecentFileList(const istd::CString& filePath)
{
	for (		RecentFilesMap::iterator iter = m_recentFilesMap.begin();
				iter != m_recentFilesMap.end();
				++iter){
		RecentGroupCommandPtr& groupCommandPtr = iter->second;

		if (!groupCommandPtr.IsValid()){
			continue;
		}

		int childsCount = groupCommandPtr->GetChildsCount();

		for (int i = 0; i < childsCount; ++i){
			const idoc::ICommand* commandPtr = groupCommandPtr->GetChild(i);
			if ((commandPtr != NULL) && (commandPtr->GetName() == filePath)){
				groupCommandPtr->RemoveChild(i);
				--childsCount;
				--i;
			}
		}
	}
}


void CMainWindowGuiComp::OnOpenDocument(const std::string* documentTypeIdPtr)
{
	idoc::IDocumentManager::FileToTypeMap fileMap;

	if (m_documentManagerCompPtr.IsValid()){
		if (m_documentManagerCompPtr->FileOpen(documentTypeIdPtr, NULL, true, "", &fileMap)){
			UpdateRecentFileList(fileMap);

			UpdateMenuActions();
		}
		else{
			QMessageBox::warning(GetWidget(), "", tr("Document could not be opened"));
		}
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

	QStatusBar* barPtr = mainWidgetPtr->statusBar();
	if (parentWidgetPtr->isFullScreen()){
		parentWidgetPtr->showMaximized();
		m_standardToolBarPtr->show();
		barPtr->show();
	}
	else{
		barPtr->hide();
		m_standardToolBarPtr->hide();
		parentWidgetPtr->showFullScreen();
	}
}


void CMainWindowGuiComp::OnShowToolbars()
{
	SetToolBarsVisible(m_showToolBarsCommand.isChecked());
}


void CMainWindowGuiComp::OnAbout()
{
	if (m_aboutGuiCompPtr.IsValid()){
		iqtgui::CGuiComponentDialog aboutDialog(m_aboutGuiCompPtr.GetPtr()); 

		aboutDialog.exec();
	}
}


void CMainWindowGuiComp::OnLanguageSelected(QAction* actionPtr) 
{
	if (m_translationManagerCompPtr.IsValid()){
		m_translationManagerCompPtr->SetSelectedLanguage(actionPtr->text());
	}
}


void CMainWindowGuiComp::OnStyleSelected(QAction* actionPtr) 
{
	qApp->setStyle(actionPtr->text());

	QStyle* appStylePtr = QApplication::style();
	QApplication::setPalette(appStylePtr->standardPalette());
}


// public methods of embedded class ActiveUndoManager

CMainWindowGuiComp::ActiveUndoManager::ActiveUndoManager(CMainWindowGuiComp& parent)
:	m_parent(parent)
{
}


// reimplemented (imod::IObserver)

bool CMainWindowGuiComp::ActiveUndoManager::OnAttached(imod::IModel* modelPtr)
{
	EnsureDetached();

	return BaseClass::OnAttached(modelPtr);
}


// protected methods of embedded class ActiveUndoManager
		
// reimplemented (imod::CSingleModelObserverBase)

void CMainWindowGuiComp::ActiveUndoManager::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.UpdateUndoMenu();
}


} // namespace iqtdoc


