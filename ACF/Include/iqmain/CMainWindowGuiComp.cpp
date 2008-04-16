#include <QStatusBar>
#include <QWorkspace>
#include <QMessageBox>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QComboBox>
#include <QFrame>
#include <QStyleFactory>

#include "imod/IModel.h"
#include "imod/IObserver.h"

#include "iqmain/CMainWindowGuiComp.h"


namespace iqmain
{


// public methods

CMainWindowGuiComp::CMainWindowGuiComp()
	:m_activeUndoManager(*this)
{
	m_menuBar = NULL;
	m_helpMenu = NULL;
	m_editMenu = NULL;
	m_viewMenu = NULL;
	m_windowMenu = NULL;
	m_fileMenu = NULL;
	m_cascadeAction = NULL;
	m_tileHorizontallyAction = NULL;
	m_tileVerticallyAction = NULL;
	m_fullScreenAction = NULL;
	m_closeAllDocumentsAction = NULL;
	m_aboutAction = NULL;

	m_newMenu = NULL;
	m_openMenu = NULL;
	m_newAction = NULL;
	m_openAction = NULL;
	m_saveAction = NULL;
	m_saveAsAction = NULL;
	m_quitAction = NULL;
	m_languageGroup = NULL;
	m_standardToolBar = NULL;
}


CMainWindowGuiComp::~CMainWindowGuiComp()
{
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


// reimplemented (QtDockManagerInterface)

void CMainWindowGuiComp::AddDockWidget(int flags, QDockWidget* widget)
{
	QMainWindow* mainWindowPtr = dynamic_cast<QMainWindow*>(GetWidget());
	I_ASSERT(mainWindowPtr != NULL);

	widget->show();

	mainWindowPtr->addDockWidget((Qt::DockWidgetArea)flags, widget);
}


void CMainWindowGuiComp::RemoveDockWidget(QDockWidget* /*widgetPtr*/)
{
}


// reimplemented (icomp::IComponent)

void CMainWindowGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_documentManagerCompPtr.IsValid()){
		imod::IModel* documentManagerModelPtr = dynamic_cast<imod::IModel*>(m_documentManagerCompPtr.GetPtr());
		if (documentManagerModelPtr != NULL){
			documentManagerModelPtr->AttachObserver(this);
		}
	}
}


void CMainWindowGuiComp::OnComponentDestroyed()
{
	if (m_documentManagerCompPtr.IsValid()){
		imod::IModel* documentManagerModelPtr = dynamic_cast<imod::IModel*>(m_documentManagerCompPtr.GetPtr());
		if (documentManagerModelPtr != NULL){
			documentManagerModelPtr->DetachObserver(this);
		}
	}

	BaseClass::OnComponentDestroyed();
}


// protected methods

void CMainWindowGuiComp::OnDocumentCountChanged(int currentCount)
{
	I_ASSERT(IsModelAttached(NULL));

	if (!IsGuiCreated()){
		return;
	}

	m_cascadeAction->setEnabled(currentCount > 1);
	m_tileHorizontallyAction->setEnabled(currentCount > 1);
	m_tileVerticallyAction->setEnabled(currentCount > 1);
	m_closeAllDocumentsAction->setEnabled(currentCount > 0);
}


void CMainWindowGuiComp::OnActiveDocumentChanged(imod::IModel* activeDocumentPtr)
{
	idoc::IDocumentManager* documentManagerPtr = GetObjectPtr();
	if (documentManagerPtr != NULL){
		imod::IModel* activeUndoManagerModelPtr = dynamic_cast<imod::IModel*>(documentManagerPtr->GetUndoManagerForDocument(activeDocumentPtr));
		if (activeUndoManagerModelPtr != NULL){
			activeUndoManagerModelPtr->AttachObserver(&m_activeUndoManager);
		}
	}

	UpdateUndoMenu();

	if (!IsGuiCreated()){
		return;
	}

	bool isDocumentActive = (activeDocumentPtr != NULL);

	m_saveAction->setEnabled(isDocumentActive);
	m_saveAsAction->setEnabled(isDocumentActive);
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


void CMainWindowGuiComp::OnRetranslate()
{
	I_ASSERT(GetWidget() != NULL);

    GetWidget()->setWindowTitle(tr("MainWindow"));
    m_helpMenu->setTitle(tr("Help"));
    m_viewMenu->setTitle(tr("&View"));
    m_windowMenu->setTitle(tr("&Window"));
    m_fileMenu->setTitle(tr("&File"));
	m_editMenu->setTitle(tr("&Edit"));

	QWidget* parentWidgetPtr = GetWidget()->parentWidget();
	if (parentWidgetPtr == NULL){
		parentWidgetPtr = GetWidget();
	}

	if (parentWidgetPtr->isFullScreen()){
		m_fullScreenAction->setText(tr("Cancel Full Screen"));
	}
	else{
		m_fullScreenAction->setText(tr("Show Full Screen"));
	}

	if (m_saveAction != NULL){
		m_saveAction->setText(tr("&Save"));
	}

	if (m_saveAsAction != NULL){
		m_saveAsAction->setText(tr("&Save As..."));
	}

	if (m_cascadeAction != NULL){
		m_cascadeAction->setText(tr("Casca&de"));
	}

	if (m_tileHorizontallyAction != NULL){
		m_tileHorizontallyAction->setText(tr("Tile &Horizontaly"));
	}

	if (m_tileVerticallyAction != NULL){
		m_tileVerticallyAction->setText(tr("Tile &Verticaly"));
	}

	if (m_closeAllDocumentsAction != NULL){
		m_closeAllDocumentsAction->setText(tr("&Close All Documents"));
	}

	if (m_aboutAction != NULL){
		m_aboutAction->setText(tr("&About..."));
	}

	if (m_newAction != NULL){
		m_newAction->setText(tr("&New"));
	}

	if (m_openAction != NULL){
		m_openAction ->setText(tr("&Open..."));
	}

	if (m_newMenu != NULL){
		m_newMenu->setTitle(tr("&New"));
	}

	if (m_openMenu != NULL){
		m_openMenu ->setTitle(tr("&Open"));
	}

	if (m_quitAction != NULL){
		m_quitAction ->setText(tr("&Quit"));
	}

	if (m_undoAction != NULL){
		m_undoAction->setText(tr("&Undo"));
	}

	if (m_redoAction != NULL){
		m_redoAction->setText(tr("&Redo"));
	}
}


// reimplemented (imod::TSingleModelObserverBase)

void CMainWindowGuiComp::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if ((updateFlags & idoc::IDocumentManager::DocumentCountChanged) != 0){
		idoc::IDocumentManager* documentManagerPtr = GetObjectPtr();
		if (documentManagerPtr != NULL){
			OnDocumentCountChanged(documentManagerPtr->GetDocumentsCount());
		}
	}

	if ((updateFlags & idoc::IDocumentManager::ViewActivationChanged) != 0){
		idoc::IDocumentManager* documentManagerPtr = GetObjectPtr();
		if (documentManagerPtr != NULL){
			imod::IModel* documentPtr = NULL;

			istd::IPolymorphic* activeViewPtr = documentManagerPtr->GetActiveView();
			if (activeViewPtr != NULL){
				documentPtr = documentManagerPtr->GetDocumentFromView(*activeViewPtr);
			}

			OnActiveDocumentChanged(documentPtr);
		}
	}
}


// protected slots

void CMainWindowGuiComp::OnFileNewAction(QAction* activeAction)
{
	if (activeAction != NULL){
		OnNewDocument(activeAction->text());
	}
}


void CMainWindowGuiComp::OnFileOpenAction(QAction* activeAction)
{
	if (activeAction != NULL){
		std::string documentTypeId = activeAction->text().toStdString();

		OnOpenDocument(&documentTypeId);
	}
}


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

		OnNewDocument(iqt::GetQString(documentTypeId));
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


void CMainWindowGuiComp::OnNewDocument(const QString& documentFactoryId)
{
	if (m_documentManagerCompPtr.IsValid()){
		imod::IModel* document = m_documentManagerCompPtr->FileNew(documentFactoryId.toStdString());
		if (document == NULL){
			QMessageBox::warning(GetWidget(), "", tr("Document could not be created"));
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

	if(parentWidgetPtr->isFullScreen()){
		m_fullScreenAction->setText(tr("Show Full Screen"));
		parentWidgetPtr->showMaximized();
		mainWidgetPtr->statusBar()->show();
	}
	else{
		m_fullScreenAction->setText(tr("Cancel Full Screen"));
		mainWidgetPtr->statusBar()->hide();
		parentWidgetPtr->showFullScreen();
	}
}


void CMainWindowGuiComp::OnAbout()
{

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


// private methods

void CMainWindowGuiComp::SetupMainWindow(QMainWindow& mainWindow)
{
	if (m_iconSizeAttrPtr.IsValid() && m_iconSizeAttrPtr->GetValue() != 0){
		mainWindow.setIconSize(QSize(m_iconSizeAttrPtr->GetValue(), m_iconSizeAttrPtr->GetValue()));
	}

	SetupStatusBar(mainWindow);

	CreateMenuComponents(mainWindow);

	SetupMenuComponents(mainWindow);

	SetupTranslationMenu();

	SetupStyleMenu();

	SetupWorkspace(mainWindow);

	SetupMainWindowComponents(mainWindow);

	OnRetranslate();

	// TODO: Get desktop resolution and calculate the right initial size of the main window.
	QSize size(800, 600);
	size = size.expandedTo(mainWindow.minimumSizeHint());
	mainWindow.resize(size);
}


void CMainWindowGuiComp::CreateMenuComponents(QMainWindow& mainWindow)
{
	m_menuBar = new QMenuBar();
	m_menuBar->setGeometry(QRect(0, 0, 625, 45));
	m_helpMenu = new QMenu(m_menuBar);
	m_viewMenu = new QMenu(m_menuBar);
	m_windowMenu = new QMenu(m_menuBar);
	m_fileMenu = new QMenu(m_menuBar);
	m_editMenu = new QMenu(m_menuBar);

	m_menuBar->addAction(m_fileMenu->menuAction());
	m_menuBar->addAction(m_editMenu->menuAction());
	m_menuBar->addAction(m_viewMenu->menuAction());
	m_menuBar->addAction(m_windowMenu->menuAction());
	m_menuBar->addAction(m_helpMenu->menuAction());
	
	m_standardToolBar = new QToolBar(&mainWindow);
	m_standardToolBar->setWindowTitle(tr("Standard"));
}


void CMainWindowGuiComp::SetupMenuComponents(QMainWindow& /*mainWindow*/)
{
	SetupFileMenu();

	SetupEditMenu();

	m_aboutAction = new QAction(tr("&About..."), m_helpMenu);
	m_helpMenu->addAction(m_aboutAction);
	connect(m_aboutAction, SIGNAL(activated()), this, SLOT(OnAbout()));

	m_fullScreenAction = new QAction(tr("&Show Full Screen"), m_viewMenu);
	m_viewMenu->addAction(m_fullScreenAction);
	connect(m_fullScreenAction, SIGNAL(activated()), this, SLOT(OnFullScreen()));

	m_quitAction = new QAction(tr("&Quit"), m_fileMenu);
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_quitAction);
	m_quitAction->setIcon(QIcon(QString::fromUtf8(":/Resources/Icons/exit.png")));
	connect(m_quitAction, SIGNAL(activated()), this, SLOT(OnQuit()));
}


void CMainWindowGuiComp::SetupFileMenu()
{
	I_ASSERT(m_documentManagerCompPtr.IsValid());

	if (!m_documentManagerCompPtr.IsValid()){
		return;
	}

	const idoc::IDocumentTemplate* templatePtr = m_documentManagerCompPtr->GetDocumentTemplate();
	if (templatePtr == NULL){
		return;
	}

	idoc::IDocumentTemplate::Ids documentTypeIds = templatePtr->GetDocumentTypeIds();

	bool isSingleFileAction = (documentTypeIds.size() == 1);
	if (isSingleFileAction){
		m_newAction = new QAction(tr("&New"), m_fileMenu);
		m_fileMenu->addAction(m_newAction);
		m_fileMenu->addSeparator();
		connect(m_newAction, SIGNAL(activated()), this, SLOT(OnNew()));
		m_newAction->setIcon(QIcon(QString::fromUtf8(":/Resources/Icons/document_128.png")));
		m_newAction->setShortcut(tr("Ctrl+N"));
		m_standardToolBar->addAction(m_newAction);

		m_openAction = new QAction(tr("&Open..."), m_fileMenu);
		m_fileMenu->addAction(m_openAction);
		m_fileMenu->addSeparator();
		connect(m_openAction, SIGNAL(activated()), this, SLOT(OnOpen()));
		m_openAction->setIcon(QIcon(QString::fromUtf8(":/Resources/Icons/folder_128.png")));
		m_openAction->setShortcut(tr("Ctrl+O"));
		m_standardToolBar->addAction(m_openAction);
	}
	else{
		m_newMenu = new QMenu(tr("&New"), m_fileMenu);
		m_fileMenu->addAction(m_newMenu->menuAction());
		m_fileMenu->addSeparator();
		connect(m_newMenu, SIGNAL(triggered(QAction*)), this, SLOT(OnFileNewAction(QAction*)));
		m_newMenu->setIcon(QIcon(QString::fromUtf8(":/Resources/Icons/document_128.png")));
		m_standardToolBar->addAction(m_newMenu->menuAction());

		m_openMenu = new QMenu(tr("&Open"), m_fileMenu);
		m_fileMenu->addAction(m_openMenu->menuAction());
		m_fileMenu->addSeparator();
		connect(m_openMenu, SIGNAL(triggered(QAction*)), this, SLOT(OnFileOpenAction(QAction*)));
		m_openMenu->setIcon(QIcon(QString::fromUtf8(":/Resources/Icons/folder_128.png")));
		m_standardToolBar->addAction(m_openMenu->menuAction());

		m_newMenu->clear();
		m_openMenu->clear();

		int idsCount = int(documentTypeIds.size());
		for (int menuIndex = 0; menuIndex < idsCount; menuIndex++){
			QString actionName = QString(documentTypeIds[menuIndex].c_str());
			QAction* newAction = new QAction(tr(actionName.toAscii()), this);
			m_fileMenu->addAction(newAction);

			QAction* openAction = new QAction(tr(actionName.toAscii()), this);
			m_openMenu->addAction(openAction);
		}
	}

	m_saveAction = new QAction(tr("&Save"), m_fileMenu);
	m_fileMenu->addAction(m_saveAction);
	connect(m_saveAction, SIGNAL(activated()), this, SLOT(OnSave()));
	m_saveAction->setEnabled(false);
	m_saveAction->setIcon(QIcon(QString::fromUtf8(":/Resources/Icons/diskette_128.png")));
	m_saveAction->setShortcut(tr("Ctrl+S"));
	m_standardToolBar->addAction(m_saveAction);

	m_saveAsAction = new QAction(tr("&Save As..."), m_fileMenu);
	m_fileMenu->addAction(m_saveAsAction);
	connect(m_saveAsAction, SIGNAL(activated()), this, SLOT(OnSaveAs()));
	m_saveAsAction->setEnabled(false);
}


void CMainWindowGuiComp::SetupEditMenu()
{
	m_undoAction = new QAction(tr("&Undo"), m_editMenu);
	m_undoAction->setShortcut(tr("Ctrl+Z"));
	connect(m_undoAction, SIGNAL(activated()), this, SLOT(OnUndo()));
	m_undoAction->setIcon(QIcon(QString::fromUtf8(":/Resources/Icons/Undo.png")));

	m_redoAction = new QAction(tr("&Redo"), m_editMenu);
	m_redoAction->setShortcut(tr("Ctrl+Y"));
	connect(m_redoAction, SIGNAL(activated()), this, SLOT(OnRedo()));
	m_redoAction->setIcon(QIcon(QString::fromUtf8(":/Resources/Icons/Redo.png")));
	
	m_editMenu->addAction(m_undoAction);
	m_editMenu->addAction(m_redoAction);
	m_editMenu->addSeparator();

	UpdateUndoMenu();
}


void CMainWindowGuiComp::SetupTranslationMenu()
{
	if (m_translationManagerCompPtr.IsValid()){
		m_languageGroup = new QActionGroup(this);
		m_languageGroup->setExclusive(true);

		for (int langIndex = 0; langIndex < m_translationManagerCompPtr->GetLanguageCount(); langIndex++){
			QString languageId = m_translationManagerCompPtr->GetLanguageId(langIndex);
			QAction* langAction = new QAction(languageId, m_languageGroup);
			langAction->setCheckable(true);
			langAction->setIcon(m_translationManagerCompPtr->GetLanguageIcon(langIndex));
			m_languageGroup->addAction(langAction);
			if (languageId == m_translationManagerCompPtr->GetSelectedLanguageId()){
				langAction->setChecked(true);
			}
		}

		m_viewMenu->addSeparator();
		m_viewMenu->addActions(m_languageGroup->actions());
		connect( m_languageGroup, SIGNAL(selected(QAction*)), this, SLOT(OnLanguageSelected(QAction *)));
		m_viewMenu->addSeparator();
	}
}


void CMainWindowGuiComp::SetupStyleMenu()
{
	QMenu* styleMenu = new QMenu(tr("&Styles"), m_viewMenu);
	m_viewMenu->addAction(styleMenu->menuAction());

	QActionGroup* styleActionGroup = new QActionGroup(this);
	styleActionGroup->setExclusive(true);

	QStringList styleList = QStyleFactory::keys();
	QString currentStyle = qApp->style()->objectName();
	for (int styleIndex = 0; styleIndex < styleList.count(); styleIndex++){
		QAction* styleAction = new QAction(styleList.at(styleIndex), styleActionGroup);
		styleAction->setCheckable(true);
		styleActionGroup->addAction(styleAction);
		if (currentStyle.toLower() == styleList.at(styleIndex).toLower()){
			styleAction->setChecked(true);
		}
	}

	styleMenu->addActions(styleActionGroup->actions());
	connect(styleActionGroup, SIGNAL(selected(QAction*)), this, SLOT(OnStyleSelected(QAction *)));
}


void CMainWindowGuiComp::SetupStatusBar(QMainWindow& mainWindow)
{
	QStatusBar* statusBar = mainWindow.statusBar();
	if (statusBar != NULL){
		QFrame* framePtr = new QFrame(statusBar);
		framePtr->setFrameShadow(QFrame::Plain);
		framePtr->setLineWidth(1);
		statusBar->addPermanentWidget(framePtr, 100);	
	}
}


void CMainWindowGuiComp::SetupWorkspace(QMainWindow& mainWindow)
{
	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->CreateGui(NULL);
		QWidget* workspacePtr = m_workspaceCompPtr->GetWidget();
		if (workspacePtr != NULL){
			mainWindow.setCentralWidget(workspacePtr);
		}
	}


	if (m_workspaceControllerCompPtr.IsValid()){
		m_cascadeAction = new QAction(tr("Casca&de"), m_windowMenu);
		m_windowMenu->addAction(m_cascadeAction);
		connect(m_cascadeAction , SIGNAL(activated()), this, SLOT(OnCascade()) ); 
		m_cascadeAction->setEnabled(false);

		m_tileHorizontallyAction = new QAction(tr("Tile &Horizontaly"), m_windowMenu);
		m_windowMenu->addAction(m_tileHorizontallyAction);
		connect(m_tileHorizontallyAction, SIGNAL(activated()), this, SLOT(OnTileHorizontally()) ); 
		m_tileHorizontallyAction->setEnabled(false);

		m_tileVerticallyAction = new QAction(tr("Tile &Verticaly"), m_windowMenu);
		m_windowMenu->addAction(m_tileVerticallyAction);
		connect(m_tileVerticallyAction, SIGNAL(activated()), this, SLOT(OnTile()) ); 
		m_tileVerticallyAction->setEnabled(false);

		m_windowMenu->addSeparator();

		m_closeAllDocumentsAction = new QAction(tr("&Close All Documents"), m_windowMenu);
		m_windowMenu->addAction(m_closeAllDocumentsAction);
		connect(m_closeAllDocumentsAction, SIGNAL(activated()), this, SLOT(OnCloseAllWindows())); 
		m_closeAllDocumentsAction->setEnabled(false);

		m_windowMenu->setEnabled(true);
	}
	else{
		m_windowMenu->setEnabled(false);
	}
}


void CMainWindowGuiComp::SetupMainWindowComponents(QMainWindow& mainWindow)
{
	if (m_mainWindowComponentsPtr.IsValid()){
		for (int componentIndex = 0; componentIndex < m_mainWindowComponentsPtr.GetCount(); componentIndex++){
			if (m_mainWindowComponentsPtr.IsValid()){
				iqt::IMainWindowComponent* mainWindowComponentPtr =  m_mainWindowComponentsPtr[componentIndex];
				if (mainWindowComponentPtr != NULL){
					mainWindowComponentPtr->AddToMainWindow(mainWindow);
				}
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
	if (m_activeUndoManager.GetObjectPtr() == NULL){
		m_undoAction->setEnabled(false);
		m_redoAction->setEnabled(false);

		return;
	}

	m_undoAction->setEnabled(m_activeUndoManager.GetObjectPtr()->IsUndoAvailable());
	m_redoAction->setEnabled(m_activeUndoManager.GetObjectPtr()->IsRedoAvailable());
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


} // namespace iqmain


