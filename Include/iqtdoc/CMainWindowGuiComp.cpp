#include "iqtdoc/CMainWindowGuiComp.h"


// Qt includes
#include <QMessageBox>
#include <QApplication>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QFileInfo>
#include <QStatusBar>
#include <QClipboard>


// ACF includes
#include "imod/IObserver.h"

#include "idoc/ICommandsProvider.h"

#include "iqt/CSettingsWriteArchive.h"
#include "iqt/CSettingsReadArchive.h"


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
	m_copyPathToClippboardCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU),
	m_settingsCommand("", 200, idoc::ICommand::CF_GLOBAL_MENU)
{
	connect(&m_newCommand, SIGNAL(activated()), this, SLOT(OnNew()));
	connect(&m_openCommand, SIGNAL(activated()), this, SLOT(OnOpen()));
	connect(&m_saveCommand, SIGNAL(activated()), this, SLOT(OnSave()));
	connect(&m_saveAsCommand, SIGNAL(activated()), this, SLOT(OnSaveAs()));
	connect(&m_printCommand, SIGNAL(activated()), this, SLOT(OnPrint()));
	connect(&m_quitCommand, SIGNAL(activated()), this, SLOT(OnQuit()));
	connect(&m_undoCommand, SIGNAL(activated()), this, SLOT(OnUndo()));
	connect(&m_redoCommand, SIGNAL(activated()), this, SLOT(OnRedo()));
	connect(&m_fullScreenCommand, SIGNAL(activated()), this, SLOT(OnFullScreen()));
	connect(&m_showToolBarsCommand, SIGNAL(activated()), this, SLOT(OnShowToolbars()));
	connect(&m_copyPathToClippboardCommand, SIGNAL(activated()), this, SLOT(OnCopyPathToClippboard()));
	connect(&m_settingsCommand, SIGNAL(activated()), this, SLOT(OnSettings()));
	connect(&m_aboutCommand, SIGNAL(activated()), this, SLOT(OnAbout()));
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
	m_toolsCommand.ResetChilds();
	m_helpCommand.ResetChilds();
	m_fixedCommands.ResetChilds();

	if (m_documentManagerModelCompPtr.IsValid()){
		if (m_documentManagerModelCompPtr->IsAttached(this)){
			m_documentManagerModelCompPtr->DetachObserver(this);
		}
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
			m_printCommand.SetGroupId(GI_DOCUMENT);
			m_fileCommand.InsertChild(&m_printCommand, false);

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

			m_toolsCommand.SetPriority(120);

			if (*m_isCopyPathVisibleAttrPtr){
				m_toolsCommand.InsertChild(&m_copyPathToClippboardCommand, false);
			}

			if (m_settingsGuiCompPtr.IsValid()){
				m_settingsCommand.SetGroupId(GI_SETTINGS);
				m_toolsCommand.InsertChild(&m_settingsCommand, false);
			}

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



// reimplemented (idoc::IMainWindowCommands)

bool CMainWindowGuiComp::OpenFile(const istd::CString& fileName)
{
	bool retVal = false;

	if (m_documentManagerCompPtr.IsValid()){
		idoc::IDocumentManager::FileToTypeMap fileMap;

		retVal = m_documentManagerCompPtr->FileOpen(NULL, &fileName, true, "", &fileMap);
		if (retVal){
			UpdateRecentFileList(fileMap);
		}
		else{
			QMessageBox::warning(GetWidget(), "", tr("Document could not be opened"));

			RemoveFromRecentFileList(istd::CString(fileName));
		}

		UpdateMenuActions();
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

						OpenFile(filePath);
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

			toolbarPtr->setVisible(isVisible);
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

	// fill menu bar with main commands
	if (m_documentManagerCompPtr.IsValid()){
		m_fixedCommands.InsertChild(&m_fileCommand, false);

		if (m_activeDocumentPtr != NULL){
			if (m_documentManagerCompPtr.IsValid()){
				std::string documentTypeId = m_documentManagerCompPtr->GetDocumentTypeId(*m_activeDocumentPtr);
					
				const idoc::IDocumentTemplate* templatePtr = m_documentManagerCompPtr->GetDocumentTemplate();
				if ((templatePtr != NULL) && templatePtr->IsFeatureSupported(idoc::IDocumentTemplate::Edit, documentTypeId)){
					m_fixedCommands.InsertChild(&m_editCommand, false);
				}
			}
		}
	}

	m_fixedCommands.InsertChild(&m_viewCommand, false);
	if (*m_isCopyPathVisibleAttrPtr){
		m_fixedCommands.InsertChild(&m_toolsCommand, false);
	}
	m_fixedCommands.InsertChild(&m_helpCommand, false);
}


void CMainWindowGuiComp::UpdateUndoMenu()
{
	imod::IUndoManager* undoManagerPtr = m_activeUndoManager.GetObjectPtr();
	if (undoManagerPtr != NULL){
		m_undoCommand.SetEnabled(undoManagerPtr->IsUndoAvailable());
		m_redoCommand.SetEnabled(undoManagerPtr->IsRedoAvailable());
	}
}


void CMainWindowGuiComp::UpdateMenuActions()
{
	if (!IsGuiCreated()){
		return;
	}

	if (!m_menuBarPtr.IsValid() && !m_standardToolBarPtr.IsValid()){
		return;
	}

	m_menuCommands.ResetChilds();

	UpdateFixedCommands();

	m_menuCommands.JoinLinkFrom(&m_fixedCommands);

	int allowedOperationFlags = 0;

	if (m_documentManagerCompPtr.IsValid()){
		allowedOperationFlags = m_documentManagerCompPtr->GetAllowedOperationFlags();

		const idoc::IDocumentTemplate* templatePtr = m_documentManagerCompPtr->GetDocumentTemplate();
		const idoc::ICommandsProvider* templateProviderPtr = CompCastPtr<idoc::ICommandsProvider>(templatePtr);
		if (templateProviderPtr != NULL){
			const idoc::IHierarchicalCommand* commandsPtr = templateProviderPtr->GetCommands();
			if (commandsPtr != NULL){
				m_menuCommands.JoinLinkFrom(commandsPtr);
			}
		}
	}

	m_newCommand.SetEnabled((allowedOperationFlags & idoc::IDocumentManager::OF_FILE_NEW) != 0);
	m_openCommand.SetEnabled((allowedOperationFlags & idoc::IDocumentManager::OF_FILE_OPEN) != 0);
	m_saveCommand.SetEnabled((allowedOperationFlags & idoc::IDocumentManager::OF_FILE_SAVE) != 0);
	m_saveAsCommand.SetEnabled((allowedOperationFlags & idoc::IDocumentManager::OF_FILE_SAVE_AS) != 0);
	m_copyPathToClippboardCommand.SetEnabled((allowedOperationFlags & idoc::IDocumentManager::OF_KNOWN_PATH) != 0);
	m_printCommand.SetEnabled((allowedOperationFlags & idoc::IDocumentManager::OF_FILE_PRINT) != 0);

	if (m_documentManagerCommandsCompPtr.IsValid()){
		const idoc::IHierarchicalCommand* commandsPtr = m_documentManagerCommandsCompPtr->GetCommands();
		if (commandsPtr != NULL){
			m_menuCommands.JoinLinkFrom(commandsPtr);
		}
	}

	const idoc::ICommandsProvider* viewProviderPtr = CompCastPtr<idoc::ICommandsProvider>(m_activeViewPtr);
	if (viewProviderPtr != NULL){
		const idoc::IHierarchicalCommand* commandsPtr = viewProviderPtr->GetCommands();
		if (commandsPtr != NULL){
			m_menuCommands.JoinLinkFrom(commandsPtr);
		}
	}

	const idoc::ICommandsProvider* documentProviderPtr = CompCastPtr<idoc::ICommandsProvider>(m_activeDocumentPtr);
	if (documentProviderPtr != NULL){
		const idoc::IHierarchicalCommand* commandsPtr = documentProviderPtr->GetCommands();
		if (commandsPtr != NULL){
			m_menuCommands.JoinLinkFrom(commandsPtr);
		}
	}

	if (m_menuBarPtr.IsValid()){
		m_menuBarPtr->clear();
		CreateMenu(m_menuCommands, *m_menuBarPtr);
	}

	if (m_standardToolBarPtr.IsValid()){
		m_standardToolBarPtr->clear();
		CreateToolbar(m_menuCommands, *m_standardToolBarPtr);
		m_standardToolBarPtr->setVisible(true);
	}
}


// reimplemented (iqtgui::CSimpleMainWindowGuiComp)

void CMainWindowGuiComp::AddMainComponent(iqtgui::IMainWindowComponent* componentPtr)
{
	BaseClass::AddMainComponent(componentPtr);

	QMainWindow* mainWindowPtr = GetQtWidget();
	if (mainWindowPtr != NULL){
		iqtgui::IGuiObject* guiPtr =  dynamic_cast<iqtgui::IGuiObject*>(componentPtr);
		if (guiPtr != NULL){
			QToolBar* toolBarComponentPtr = dynamic_cast<QToolBar*>(guiPtr->GetWidget());
			if (toolBarComponentPtr != NULL){
				m_toolBarsList.PushBack(toolBarComponentPtr, false);
			}
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

	I_ASSERT(m_isMenuVisibleAttrPtr.IsValid());	// is obligatory attribute
	if (*m_isMenuVisibleAttrPtr){
		m_menuBarPtr.SetPtr(new QMenuBar(mainWindowPtr));
	}

	I_ASSERT(m_isToolbarVisibleAttrPtr.IsValid());	// is obligatory attribute
	if (*m_isToolbarVisibleAttrPtr){
		m_standardToolBarPtr.SetPtr(new QToolBar(mainWindowPtr));
		m_standardToolBarPtr->setWindowTitle(tr("Standard"));
		m_standardToolBarPtr->setObjectName("Standard");
	}

	if (m_useIconTextAttrPtr.IsValid() && m_useIconTextAttrPtr->GetValue()){
		m_standardToolBarPtr->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	}

	OnRetranslate();

	UpdateMenuActions();
	UpdateUndoMenu();
	
	SetupMainWindowComponents(*mainWindowPtr);

	mainWindowPtr->setAcceptDrops(true);

	mainWindowPtr->installEventFilter(this);

	m_showToolBarsCommand.setChecked(true);

	if (m_settingsGuiCompPtr.IsValid()){
		m_settingsDialogPtr.SetPtr(new iqtgui::CGuiComponentDialog(m_settingsGuiCompPtr.GetPtr(), 0, true, mainWindowPtr)); 
	}

	if (m_aboutGuiCompPtr.IsValid()){
		m_aboutDialogPtr.SetPtr(new iqtgui::CGuiComponentDialog(m_aboutGuiCompPtr.GetPtr(), 0, true, mainWindowPtr)); 
	}
}


void CMainWindowGuiComp::OnGuiDestroyed()
{
	m_menuBarPtr.Reset();
	m_standardToolBarPtr.Reset();

	m_settingsDialogPtr.Reset(); 
	m_aboutDialogPtr.Reset(); 

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
	m_toolsCommand.SetName(iqt::GetCString(tr("&Tools")));
	m_helpCommand.SetName(iqt::GetCString(tr("&Help")));

	// File commands
	m_newCommand.SetVisuals(tr("&New"), tr("New"), tr("Creates new document"), QIcon(":/Icons/New"));
	m_newCommand.setShortcut(tr("Ctrl+N"));
	m_openCommand.SetVisuals(tr("&Open..."), tr("Open"), tr("Opens document from file"), QIcon(":/Icons/Open"));
	m_openCommand.setShortcut(tr("Ctrl+O"));
	m_saveCommand.SetVisuals(tr("&Save"), tr("Save"), tr("Saves document to actual working file"), QIcon(":/Icons/Save"));
	m_saveCommand.setShortcut(tr("Ctrl+S"));
	m_saveAsCommand.SetVisuals(tr("&Save As..."), tr("Save As"), tr("Saves document into selected file"));
	m_printCommand.setShortcut(tr("Ctrl+P"));
	m_printCommand.SetVisuals(tr("&Print..."), tr("Print"), tr("Prints current document"), QIcon(":/Icons/Print"));
	m_quitCommand.SetVisuals(tr("&Quit"), tr("Quit"), tr("Quits this application"), QIcon(":/Icons/Exit"));
	m_undoCommand.SetVisuals(tr("&Undo"), tr("Undo"), tr("Undo last document changes"), QIcon(":/Icons/Undo"));
	m_undoCommand.setShortcut(tr("Ctrl+Z"));
	m_redoCommand.SetVisuals(tr("&Redo"), tr("Redo"), tr("Redo last document changes"), QIcon(":/Icons/Redo"));
	m_redoCommand.setShortcut(tr("Ctrl+Shift+Z"));
	
	// View commands
	m_fullScreenCommand.SetVisuals(tr("&Full Screen"), tr("Full Screen"), tr("Turn full screen mode on/off"));
	m_fullScreenCommand.setShortcut(tr("F11"));
	m_showToolBarsCommand.SetVisuals(tr("&Show Toolbars"), tr("Show Toolbars"), tr("Show and hide toolbars"));

	// Tools commands
	m_copyPathToClippboardCommand.SetVisuals(tr("&Copy Document Path"), tr("Copy Path"), tr("Copy current document path to system clippboard"));
	m_settingsCommand.SetVisuals(tr("&Settings"), tr("Settings"), tr("Show global application setting"));

	// Help commands
	m_aboutCommand.SetVisuals(tr("&About..."), tr("About"), tr("Shows information about this application"), QIcon(":/Icons/About"));
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


// static methods

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
		}
		else{
			QMessageBox::critical(GetWidget(), "", tr("File could not be saved!"));
		}

		UpdateMenuActions();
	}
}


void CMainWindowGuiComp::OnSaveAs()
{
	if (m_documentManagerCompPtr.IsValid()){
		idoc::IDocumentManager::FileToTypeMap fileMap;

		if (m_documentManagerCompPtr->FileSave(true)){
			UpdateRecentFileList(fileMap);
		}
		else{
			QMessageBox::critical(GetWidget(), "", tr("File could not be saved!"));
		}

		UpdateMenuActions();
	}
}


void CMainWindowGuiComp::OnPrint()
{
	if (m_documentManagerCompPtr.IsValid()){
		m_documentManagerCompPtr->FilePrint();
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
		}
		else{
			QMessageBox::warning(GetWidget(), "", tr("Document could not be opened"));
		}

		UpdateMenuActions();
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
		m_showToolBarsCommand.setChecked(false);
		parentWidgetPtr->showFullScreen();
	}
}


void CMainWindowGuiComp::OnShowToolbars()
{
	SetToolBarsVisible(m_showToolBarsCommand.isChecked());
}


void CMainWindowGuiComp::OnCopyPathToClippboard()
{
	if ((m_activeViewPtr != NULL) && m_documentManagerCompPtr.IsValid()){
		idoc::IDocumentManager::DocumentInfo info;
		if (m_documentManagerCompPtr->GetDocumentFromView(*m_activeViewPtr, &info) != NULL){
			QClipboard* clipboardPtr = QApplication::clipboard();
			if (clipboardPtr != NULL){
				clipboardPtr->setText(iqt::GetQString(info.filePath));
			}
		}
	}
}


void CMainWindowGuiComp::OnAbout()
{
	if (m_aboutDialogPtr.IsValid()){
		m_aboutDialogPtr->exec();
	}
}


void CMainWindowGuiComp::OnSettings()
{
	if (m_settingsDialogPtr.IsValid()){
		m_settingsDialogPtr->exec();
	}
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


