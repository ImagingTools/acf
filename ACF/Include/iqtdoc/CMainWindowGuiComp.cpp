#include "iqtdoc/CMainWindowGuiComp.h"


#include <QStatusBar>
#include <QMessageBox>
#include <QFrame>
#include <QStyle>
#include <QStyleFactory>
#include <QApplication>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl> 

#include "imod/IObserver.h"

#include "idoc/ICommandsProvider.h"


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
	m_fullScreenCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_ONOFF)
{
	m_menuBar = NULL;
	m_standardToolBar = NULL;

	connect(&m_newCommand, SIGNAL(activated()), this, SLOT(OnNew()));
	connect(&m_openCommand, SIGNAL(activated()), this, SLOT(OnOpen()));
	connect(&m_saveCommand, SIGNAL(activated()), this, SLOT(OnSave()));
	connect(&m_saveAsCommand, SIGNAL(activated()), this, SLOT(OnSaveAs()));
	connect(&m_quitCommand, SIGNAL(activated()), this, SLOT(OnQuit()));
	connect(&m_undoCommand, SIGNAL(activated()), this, SLOT(OnUndo()));
	connect(&m_redoCommand, SIGNAL(activated()), this, SLOT(OnRedo()));
	connect(&m_fullScreenCommand, SIGNAL(activated()), this, SLOT(OnFullScreen()));
	connect(&m_cascadeCommand, SIGNAL(activated()), this, SLOT(OnCascade()));
	connect(&m_tileHorizontallyCommand, SIGNAL(activated()), this, SLOT(OnTileHorizontally()));
	connect(&m_tileVerticallyCommand, SIGNAL(activated()), this, SLOT(OnTile()));
	connect(&m_closeAllDocumentsCommand, SIGNAL(activated()), this, SLOT(OnCloseAllWindows()));
	connect(&m_aboutCommand, SIGNAL(activated()), this, SLOT(OnAbout()));
}


// reimplemented (iqt::IGuiObject)

void CMainWindowGuiComp::OnTryClose(bool* ignoredPtr)
{
	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->OnTryClose(ignoredPtr);

		return;
	}

	BaseClass::OnTryClose(ignoredPtr);
}


// reimplemented (QtToolBarManagerInterface)

void CMainWindowGuiComp::SetToolBarsVisible(bool /*isVisible*/)
{
}


int CMainWindowGuiComp::GetToolBarCount() const
{
	return 1;
}


void CMainWindowGuiComp::AddToolBar(QToolBar* /*widgetPtr*/)
{
}


void CMainWindowGuiComp::RemoveToolBar(QToolBar* /*widgetPtr*/)
{
}


// reimplemented (IDockManager)

void CMainWindowGuiComp::AddDockWidget(int flags, QDockWidget* dockWidgetPtr)
{
	QMainWindow* mainWindowPtr = GetQtWidget();
	I_ASSERT(mainWindowPtr != NULL);

	dockWidgetPtr->show();

	mainWindowPtr->addDockWidget((Qt::DockWidgetArea)flags, dockWidgetPtr);
}


void CMainWindowGuiComp::RemoveDockWidget(QDockWidget* /*widgetPtr*/)
{
}


// reimplemented (icomp::IComponent)

void CMainWindowGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_documentManagerModelCompPtr.IsValid()){
		m_documentManagerModelCompPtr->AttachObserver(this);
	}
	
	const idoc::IDocumentTemplate* templatePtr = NULL;
	
	if (m_documentManagerCompPtr.IsValid()){
		templatePtr = m_documentManagerCompPtr->GetDocumentTemplate();
		if (templatePtr != NULL){
			idoc::IDocumentTemplate::Ids ids = templatePtr->GetDocumentTypeIds();
			if (!ids.empty() && templatePtr->IsFeatureSupported(idoc::IDocumentTemplate::New)){
				m_newCommand.SetGroupId(GI_DOCUMENT);
				m_fileCommand.InsertChild(&m_newCommand, false);
				
				if (ids.size() > 1){
					for (		idoc::IDocumentTemplate::Ids::const_iterator iter = ids.begin();
								iter != ids.end();
								++iter){
						NewDocumentCommand* newCommandPtr = new NewDocumentCommand(this, *iter);
						if (newCommandPtr != NULL){
							QString commandName = iqt::GetQString(*iter);
							newCommandPtr->SetVisuals(commandName, commandName, tr("Creates new document %1").arg(commandName));
							m_newCommand.InsertChild(newCommandPtr, true);
						}
					}
				}
			}
		}

		m_fileCommand.SetPriority(30);
		m_openCommand.SetGroupId(GI_DOCUMENT);
		m_fileCommand.InsertChild(&m_openCommand, false);
		m_saveCommand.SetGroupId(GI_DOCUMENT);
		m_fileCommand.InsertChild(&m_saveCommand, false);
		m_saveAsCommand.SetGroupId(GI_DOCUMENT);
		m_fileCommand.InsertChild(&m_saveAsCommand, false);

		CreateRecentMenu();

		m_quitCommand.SetGroupId(GI_APPLICATION);
		m_fileCommand.InsertChild(&m_quitCommand, false);
	
		m_editCommand.SetPriority(60);
		m_undoCommand.SetGroupId(GI_UNDO);
		m_editCommand.InsertChild(&m_undoCommand, false);
		m_redoCommand.SetGroupId(GI_UNDO);
		m_editCommand.InsertChild(&m_redoCommand, false);

		m_viewCommand.SetPriority(90);
		m_viewCommand.InsertChild(&m_fullScreenCommand, false);

		m_windowCommand.SetPriority(120);
		m_cascadeCommand.SetGroupId(GI_WINDOW);
		m_windowCommand.InsertChild(&m_cascadeCommand, false);
		m_tileHorizontallyCommand.SetGroupId(GI_WINDOW);
		m_windowCommand.InsertChild(&m_tileHorizontallyCommand, false);
		m_tileVerticallyCommand.SetGroupId(GI_WINDOW);
		m_windowCommand.InsertChild(&m_tileVerticallyCommand, false);
		m_closeAllDocumentsCommand.SetGroupId(GI_DOCUMENT);
		m_windowCommand.InsertChild(&m_closeAllDocumentsCommand, false);

		m_helpCommand.SetPriority(150);
		m_helpCommand.InsertChild(&m_aboutCommand, false);

		// fill menu bar with main commands

		m_fixedCommands.InsertChild(&m_fileCommand, false);
		
		if (templatePtr != NULL && templatePtr->IsFeatureSupported(idoc::IDocumentTemplate::Edit)){
			m_fixedCommands.InsertChild(&m_editCommand, false);
		}

		m_fixedCommands.InsertChild(&m_viewCommand, false);
		m_fixedCommands.InsertChild(&m_windowCommand, false);
		m_fixedCommands.InsertChild(&m_helpCommand, false);
	}
}


void CMainWindowGuiComp::OnComponentDestroyed()
{
	m_fileCommand.ResetChilds();
	m_editCommand.ResetChilds();
	m_viewCommand.ResetChilds();
	m_windowCommand.ResetChilds();
	m_helpCommand.ResetChilds();
	m_fixedCommands.ResetChilds();

	if (m_documentManagerModelCompPtr.IsValid()){
		m_documentManagerModelCompPtr->DetachObserver(this);
	}

	BaseClass::OnComponentDestroyed();
}


// protected methods

void CMainWindowGuiComp::OnDocumentCountChanged()
{
	I_ASSERT(IsModelAttached(NULL));

	if (!IsGuiCreated()){
		return;
	}

	idoc::IDocumentManager* documentManagerPtr = GetObjectPtr();
	I_ASSERT(documentManagerPtr != NULL);	// Model was attached

	int documentsCount = documentManagerPtr->GetDocumentsCount();

	m_cascadeCommand.SetEnabled(documentsCount > 1);
	m_tileHorizontallyCommand.SetEnabled(documentsCount > 1);
	m_tileVerticallyCommand.SetEnabled(documentsCount > 1);
	m_closeAllDocumentsCommand.SetEnabled(documentsCount > 0);
}


void CMainWindowGuiComp::OnActiveViewChanged()
{
	UpdateMenuActions();
}


void CMainWindowGuiComp::OnActiveDocumentChanged()
{
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


void CMainWindowGuiComp::OnRecentFileListChanged()
{
	UpdateRecentFileMenu();
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
				const idoc::IDocumentTemplate* documentTemplatePtr = m_documentManagerCompPtr->GetDocumentTemplate();
				if (documentTemplatePtr != NULL){
					idoc::IDocumentTemplate::Ids availableDocumentIds = documentTemplatePtr->GetDocumentTypeIdsForFile(iqt::GetCString(filePath));
					if (!availableDocumentIds.empty()){
						OnOpenFile(filePath);
					}
				}
			}
		}
	}
}



int CMainWindowGuiComp::CreateToolbar(const iqt::CHierarchicalCommand& command, QToolBar& result, int prevGroupId) const
{
	int childsCount = command.GetChildsCount();

	std::map<int, istd::TPointer<QActionGroup> > groups;

	for (int i = 0; i < childsCount; ++i){
		iqt::CHierarchicalCommand* hierarchicalPtr = const_cast<iqt::CHierarchicalCommand*>(
					dynamic_cast<const iqt::CHierarchicalCommand*>(command.GetChild(i)));

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


void CMainWindowGuiComp::SetupMainWindow(QMainWindow& mainWindow)
{
	if (m_iconSizeAttrPtr.IsValid() && m_iconSizeAttrPtr->GetValue() != 0){
		mainWindow.setIconSize(QSize(m_iconSizeAttrPtr->GetValue(), m_iconSizeAttrPtr->GetValue()));
	}

	m_menuBar = new QMenuBar(&mainWindow);
	m_menuBar->setGeometry(QRect(0, 0, 625, 45));

	m_standardToolBar = new QToolBar(&mainWindow);
	m_standardToolBar->setWindowTitle(tr("Standard"));

	if (m_useIconTextAttrPtr.IsValid()){
		m_standardToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	}

	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->CreateGui(NULL);
		QWidget* workspacePtr = m_workspaceCompPtr->GetWidget();
		if (workspacePtr != NULL){
			mainWindow.setCentralWidget(workspacePtr);
		}
	}

	SetupMainWindowComponents(mainWindow);

	OnRetranslate();

	UpdateMenuActions();
	UpdateUndoMenu();

	// TODO: Get desktop resolution and calculate the right initial size of the main window.
	QSize size(800, 600);
	size = size.expandedTo(mainWindow.minimumSizeHint());
	mainWindow.resize(size);

	mainWindow.setAcceptDrops(true);

	mainWindow.installEventFilter(this);
}


void CMainWindowGuiComp::SetupMainWindowComponents(QMainWindow& mainWindow)
{
	if (m_mainWindowComponentsPtr.IsValid()){
		for (int componentIndex = 0; componentIndex < m_mainWindowComponentsPtr.GetCount(); componentIndex++){
			iqt::IMainWindowComponent* mainWindowComponentPtr =  m_mainWindowComponentsPtr[componentIndex];
			iqt::IGuiObject* guiPtr =  dynamic_cast<iqt::IGuiObject*>(mainWindowComponentPtr);
			if (mainWindowComponentPtr != NULL && guiPtr != NULL && guiPtr->CreateGui(NULL)){
				mainWindowComponentPtr->AddToMainWindow(mainWindow);
			}
		}
	}

	mainWindow.setMenuBar(m_menuBar);

	if (HasDocumentTemplate()){
		mainWindow.addToolBar(Qt::TopToolBarArea, m_standardToolBar);
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
	if (!documentTypeIds.size()){
		return false;
	}

	return true;
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
	bool isDocumentActive = (m_activeDocumentPtr != NULL);

	m_saveCommand.SetEnabled(isDocumentActive);
	m_saveAsCommand.SetEnabled(isDocumentActive);

	if (m_menuBar == NULL){
		return;
	}

	m_menuCommands.ResetChilds();

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

	m_menuBar->clear();
	CreateMenu(m_menuCommands, *m_menuBar);

	m_standardToolBar->clear();
	CreateToolbar(m_menuCommands, *m_standardToolBar);
}


void CMainWindowGuiComp::UpdateRecentFileMenu()
{
	idoc::IDocumentManager* documentManagerPtr = GetObjectPtr();
	I_ASSERT(documentManagerPtr != NULL);
	if (documentManagerPtr != NULL){
		const idoc::IDocumentTemplate* templatePtr = documentManagerPtr->GetDocumentTemplate();
		if (templatePtr == NULL){
			return;
		}

		idoc::IDocumentTemplate::Ids ids = templatePtr->GetDocumentTypeIds();

		for (		idoc::IDocumentTemplate::Ids::const_iterator index = ids.begin();
					index != ids.end();
					index++){
			std::string documentTypeId = (*index);

			iqt::CHierarchicalCommand* recentGroupCommandPtr = m_recentFilesCommands[documentTypeId].GetPtr();
			I_ASSERT(recentGroupCommandPtr != NULL);

			recentGroupCommandPtr->ResetChilds();

			istd::CStringList recentFileList = documentManagerPtr->GetRecentFileList(documentTypeId);

			for (int fileIndex = 0; fileIndex < int(recentFileList.size()); fileIndex++){
				QString filePath = iqt::GetQString(recentFileList.at(fileIndex));
				
				RecentFileCommand* recentFileCommand = new RecentFileCommand(this, filePath);

				recentGroupCommandPtr->InsertChild(recentFileCommand, true);
			}
		}
	}

	UpdateMenuActions();
}


void CMainWindowGuiComp::CreateRecentMenu()
{
	if (m_documentManagerCompPtr.IsValid()){
		const idoc::IDocumentTemplate* templatePtr = m_documentManagerCompPtr->GetDocumentTemplate();
		if (templatePtr != NULL){
			idoc::IDocumentTemplate::Ids ids = templatePtr->GetDocumentTypeIds();
			if (!ids.empty()){
				if (ids.size() == 1){
					QString recentListTitle = QString(tr("Recent Files"));
		
					iqt::CHierarchicalCommand* fileListCommandPtr = new iqt::CHierarchicalCommand(iqt::GetCString(recentListTitle));
					if (fileListCommandPtr != NULL){
						m_fileCommand.InsertChild(fileListCommandPtr, false);

						m_recentFilesCommands[ids.front()] = fileListCommandPtr;
					}
				}
				else{
					for (		idoc::IDocumentTemplate::Ids::const_iterator iter = ids.begin();
									iter != ids.end();
									++iter){
						QString recentListTitle = QString(tr("Recent ")) + (*iter).c_str() + QString(tr("(s)"));
						iqt::CHierarchicalCommand* fileListCommandPtr = new iqt::CHierarchicalCommand(iqt::GetCString(recentListTitle));
						if (fileListCommandPtr != NULL){
							m_fileCommand.InsertChild(fileListCommandPtr, false);

							m_recentFilesCommands[ids.front()] = fileListCommandPtr;
						}
					}
				}
			}
		}
	}
}


// reimplemented (iqt::TGuiComponentBase)

void CMainWindowGuiComp::OnGuiCreated()
{
	QMainWindow* mainWindowPtr = GetQtWidget();
	if (mainWindowPtr == NULL){
		return;
	}

	SetupMainWindow(*mainWindowPtr);
}


void CMainWindowGuiComp::OnGuiDestroyed()
{
	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->DestroyGui();
	}
		
	if (m_mainWindowComponentsPtr.IsValid()){
		for (int componentIndex = 0; componentIndex < m_mainWindowComponentsPtr.GetCount(); componentIndex++){
			iqt::IMainWindowComponent* mainWindowComponentPtr =  m_mainWindowComponentsPtr[componentIndex];
			iqt::IGuiObject* guiPtr =  dynamic_cast<iqt::IGuiObject*>(mainWindowComponentPtr);
			if (mainWindowComponentPtr != NULL && guiPtr != NULL && guiPtr->IsGuiCreated()){
				guiPtr->DestroyGui();
			}
		}
	}
}


void CMainWindowGuiComp::OnRetranslate()
{
	I_ASSERT(GetWidget() != NULL);

	QWidget* parentWidgetPtr = GetWidget()->parentWidget();
	if (parentWidgetPtr == NULL){
		parentWidgetPtr = GetWidget();
	}

	m_fileCommand.SetName(iqt::GetCString(tr("&File")));
	m_editCommand.SetName(iqt::GetCString(tr("&Edit")));
	m_viewCommand.SetName(iqt::GetCString(tr("&View")));
	m_windowCommand.SetName(iqt::GetCString(tr("&Window")));
	m_helpCommand.SetName(iqt::GetCString(tr("&Help")));

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
	m_fullScreenCommand.SetVisuals(tr("&Full Screen"), tr("Full Screen"), tr("Turn full screen mode on/off"));
	m_fullScreenCommand.setShortcut(tr("F11"));
	m_cascadeCommand.SetVisuals(tr("Casca&de"), tr("Cascade"), tr("Lays out all document windows in cascaded mode"));
	m_tileHorizontallyCommand.SetVisuals(tr("Tile &Horizontaly"), tr("Horizontal"), tr("Lays out all document windows horizontaly"));
	m_tileVerticallyCommand.SetVisuals(tr("Tile &Verticaly"), tr("Vertical"), tr("Lays out all document windows verticaly"));
	m_closeAllDocumentsCommand.SetVisuals(tr("&Close All Documents"), tr("Close All"), tr("&Closes all opened documents"));
	m_aboutCommand.SetVisuals(tr("&About..."), tr("About"), tr("Shows information about this application"), GetIcon("info"));
}


// reimplemented (imod::TSingleModelObserverBase)

void CMainWindowGuiComp::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if ((updateFlags & idoc::IDocumentManager::RecentFileListChanged) != 0){
		idoc::IDocumentManager* documentManagerPtr = GetObjectPtr();
		if (documentManagerPtr != NULL){
			OnRecentFileListChanged();
		}
	}

	if ((updateFlags & idoc::IDocumentManager::DocumentCountChanged) != 0){
		idoc::IDocumentManager* documentManagerPtr = GetObjectPtr();
		if (documentManagerPtr != NULL){
			OnDocumentCountChanged();
		}
	}

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
		if (!m_documentManagerCompPtr->FileSave()){
			QMessageBox::critical(GetWidget(), "", tr("File could not be saved!"));
		}
	}
}


void CMainWindowGuiComp::OnSaveAs()
{
	if (m_documentManagerCompPtr.IsValid()){
		if (!m_documentManagerCompPtr->FileSave(true)){
			QMessageBox::critical(GetWidget(), "", tr("File could not be saved!"));
		}
	}
}


void CMainWindowGuiComp::OnNewDocument(const std::string& documentFactoryId)
{
	if (m_documentManagerCompPtr.IsValid()){
		istd::IChangeable* documentPtr = m_documentManagerCompPtr->FileNew(documentFactoryId);
		if (documentPtr == NULL){
			QMessageBox::warning(GetWidget(), "", tr("Document could not be created"));
			return;
		}
	}
}


void CMainWindowGuiComp::OnOpenFile(const QString& fileName)
{
	if (m_documentManagerCompPtr.IsValid()){
		istd::CString documentFile = iqt::GetCString(fileName);
		bool result = m_documentManagerCompPtr->FileOpen(NULL, &documentFile);
		if (!result){
			QMessageBox::warning(GetWidget(), "", tr("Document could not be opened"));
			return;
		}
	}
}


void CMainWindowGuiComp::OnOpenDocument(const std::string* documentTypeIdPtr)
{
	if (m_documentManagerCompPtr.IsValid()){
		bool result = m_documentManagerCompPtr->FileOpen(documentTypeIdPtr);
		if (!result){
			QMessageBox::warning(GetWidget(), "", tr("Document could not be opened"));
			return;
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

	if (parentWidgetPtr->isFullScreen()){
		parentWidgetPtr->showMaximized();
		m_standardToolBar->show();
		mainWidgetPtr->statusBar()->show();
	}
	else{
		mainWidgetPtr->statusBar()->hide();
		m_standardToolBar->hide();
		parentWidgetPtr->showFullScreen();
	}
}


void CMainWindowGuiComp::OnAbout()
{
	// TODO: implement about for MVC.
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

	QApplication::setPalette(QApplication::style()->standardPalette());
}


void CMainWindowGuiComp::OnCloseAllWindows()
{
	if (m_workspaceControllerCompPtr.IsValid()){
		m_workspaceControllerCompPtr->CloseAllViews();
	}
}


void CMainWindowGuiComp::OnCascade()
{
	if (m_workspaceControllerCompPtr.IsValid()){
		m_workspaceControllerCompPtr->Cascade();
	}
}


void CMainWindowGuiComp::OnTileHorizontally()
{
	if (m_workspaceControllerCompPtr.IsValid()){
		m_workspaceControllerCompPtr->TileHorizontally();
	}
}


void CMainWindowGuiComp::OnTile()
{
	if (m_workspaceControllerCompPtr.IsValid()){
		m_workspaceControllerCompPtr->Tile();
	}
}


// public methods of embedded class ActiveUndoManager

CMainWindowGuiComp::ActiveUndoManager::ActiveUndoManager(CMainWindowGuiComp& parent)
	:m_parent(parent)
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


