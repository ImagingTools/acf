#include "iqtdoc/CMultiDocumentWorkspaceGuiComp.h"


// Qt includes
#include <QtCore/QMap>
#include <QtCore/QFileInfo>
#include <QtCore/QEvent>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif

// ACF includes
#include "istd/TChangeNotifier.h"
#include "idoc/IDocumentTemplate.h"
#include "iqt/CSettingsWriteArchive.h"
#include "iqt/CSettingsReadArchive.h"


namespace iqtdoc
{


// public methods

CMultiDocumentWorkspaceGuiComp::CMultiDocumentWorkspaceGuiComp()
:	m_workspaceModeCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU),
	m_subWindowCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_ONOFF | ibase::ICommand::CF_EXCLUSIVE),
	m_tabbedCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_ONOFF | ibase::ICommand::CF_EXCLUSIVE),
	m_viewsCount(0),
	m_forceQuietClose(false)
{
	m_documentSelectionInfo.SetParent(*this);

	m_workspaceModeCommand.InsertChild(&m_subWindowCommand, false);
	m_workspaceModeCommand.InsertChild(&m_tabbedCommand, false);
	m_commands.InsertChild(&m_windowCommand, false);
	m_windowCommand.SetPriority(130);
	m_cascadeCommand.SetGroupId(GI_WINDOW);
	m_windowCommand.InsertChild(&m_workspaceModeCommand, false);
	m_windowCommand.InsertChild(&m_cascadeCommand, false);
	m_tileHorizontallyCommand.SetGroupId(GI_WINDOW);
	m_windowCommand.InsertChild(&m_tileHorizontallyCommand, false);
	m_tileVerticallyCommand.SetGroupId(GI_WINDOW);
	m_windowCommand.InsertChild(&m_tileVerticallyCommand, false);
	m_closeAllDocumentsCommand.SetGroupId(GI_DOCUMENT);
	m_windowCommand.InsertChild(&m_closeAllDocumentsCommand, false);
	m_workspaceModeCommand.SetGroupId(GI_VIEW);

	connect(&m_cascadeCommand, SIGNAL(triggered()), this, SLOT(OnCascade()));
	connect(&m_tileHorizontallyCommand, SIGNAL(triggered()), this, SLOT(OnTileHorizontally()));
	connect(&m_tileVerticallyCommand, SIGNAL(triggered()), this, SLOT(OnTile()));
	connect(&m_closeAllDocumentsCommand, SIGNAL(triggered()), this, SLOT(OnCloseAllViews()));

	m_subWindowCommand.setChecked(true);
	connect(&m_subWindowCommand, SIGNAL(triggered()), this, SLOT(OnWorkspaceModeChanged()));
	connect(&m_tabbedCommand, SIGNAL(triggered()), this, SLOT(OnWorkspaceModeChanged()));
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CMultiDocumentWorkspaceGuiComp::GetCommands() const
{
	return &m_commands;
}


// reimplemented (iqtgui::IGuiObject)

void CMultiDocumentWorkspaceGuiComp::OnTryClose(bool* ignoredPtr)
{
	//Save open document settings before exit
	if (m_rememberOpenDocumentsParamPtr.IsValid() && m_rememberOpenDocumentsParamPtr->IsEnabled()){
		if (!m_organizationName.isEmpty() && !m_applicationName.isEmpty()){
			iqt::CSettingsWriteArchive archive(
							m_organizationName,
							m_applicationName,
							"OpenDocumentList",
							QSettings::UserScope);

			SerializeOpenDocumentList(archive);
		}
	}

	if (SaveDirtyDocuments(false, ignoredPtr)){
		CloseAllDocuments();
	}

	if (ignoredPtr != NULL){
		*ignoredPtr = (GetDocumentsCount() > 0);
	}
}


// protected members

void CMultiDocumentWorkspaceGuiComp::UpdateAllTitles()
{
	typedef QMap<QString, int> NameFrequencies;
	NameFrequencies nameFrequencies;

	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		const SingleDocumentData& info = GetSingleDocumentData(i);

		QString titleName = info.filePath.isEmpty()?
					tr("<no name>"):
					QFileInfo(info.filePath).fileName();

		NameFrequencies::Iterator freqIter = nameFrequencies.find(titleName);
		int& frequency = freqIter.value();
		if (freqIter != nameFrequencies.end()){
			frequency++;

			titleName = tr("%1 <%2>").arg(titleName).arg(frequency + 1);
		}
		else{
			nameFrequencies[titleName] = 0;
		}

		if (info.isDirty){
			titleName += " *";
		}

		for (		Views::ConstIterator viewIter = info.views.begin();
					viewIter != info.views.end();
					++viewIter){
			const ViewInfo& viewInfo = *viewIter;
			Q_ASSERT(viewInfo.viewPtr.IsValid());

			const iqtgui::IGuiObject* guiObjectPtr = CompCastPtr<iqtgui::IGuiObject>(viewInfo.viewPtr.GetPtr());
			if (guiObjectPtr != NULL){
				QWidget* widgetPtr = guiObjectPtr->GetWidget();
				Q_ASSERT(widgetPtr != NULL);

				widgetPtr->setWindowTitle(titleName);
			}
		}
	}
}


iqtgui::IGuiObject* CMultiDocumentWorkspaceGuiComp::GetViewFromWidget(const QWidget& widget) const
{
	int documentInfosCount = GetDocumentsCount();
	for (int documentIndex = 0; documentIndex < documentInfosCount; ++documentIndex){
		SingleDocumentData& info = GetSingleDocumentData(documentIndex);

		for (		Views::ConstIterator viewIter = info.views.begin();
					viewIter != info.views.end();
					++viewIter){
			const ViewInfo& viewInfo = *viewIter;

			iqtgui::IGuiObject* guiObjectPtr = CompCastPtr<iqtgui::IGuiObject>(viewInfo.viewPtr.GetPtr());
			if (guiObjectPtr != NULL){
				if (guiObjectPtr->GetWidget() == &widget){
					return guiObjectPtr;
				}
			}
		}
	}

	return NULL;
}


int CMultiDocumentWorkspaceGuiComp::GetDocumentIndexFromWidget(const QWidget& widget) const
{
	int documentInfosCount = GetDocumentsCount();
	for (int documentIndex = 0; documentIndex < documentInfosCount; ++documentIndex){
		SingleDocumentData& info = GetSingleDocumentData(documentIndex);

		for (		Views::ConstIterator viewIter = info.views.begin();
					viewIter != info.views.end();
					++viewIter){
			const ViewInfo& viewInfo = *viewIter;

			iqtgui::IGuiObject* guiObjectPtr = CompCastPtr<iqtgui::IGuiObject>(viewInfo.viewPtr.GetPtr());
			if (guiObjectPtr != NULL){
				if (guiObjectPtr->GetWidget() == &widget){
					return documentIndex;
				}
			}
		}
	}

	return -1;
}


void CMultiDocumentWorkspaceGuiComp::CreateConnections()
{
	connect(GetWidget(), SIGNAL( subWindowActivated(QMdiSubWindow*)), this, SLOT(OnWindowActivated(QMdiSubWindow*)));
}


void CMultiDocumentWorkspaceGuiComp::OnViewsCountChanged()
{
	m_cascadeCommand.SetEnabled(m_viewsCount > 1);
	m_tileHorizontallyCommand.SetEnabled(m_viewsCount > 1);
	m_tileVerticallyCommand.SetEnabled(m_viewsCount > 1);
	m_closeAllDocumentsCommand.SetEnabled(m_viewsCount > 0);
}


// reimplemented (idoc::CMultiDocumentManagerBase)

istd::IChangeable* CMultiDocumentWorkspaceGuiComp::OpenDocument(
			const QString& filePath,
			bool createView,
			const QByteArray& viewTypeId,
			QByteArray& documentTypeId,
			bool beQuiet,
			bool* ignoredPtr)
{
	bool allowViewRepeating = true;
	if (m_allowViewRepeatingAttrPtr.IsValid()){
		allowViewRepeating = *m_allowViewRepeatingAttrPtr;
	}

	SingleDocumentData* documentInfoPtr = GetDocumentInfoFromPath(filePath);
	if (documentInfoPtr != NULL && !allowViewRepeating){
		createView = false;
	}

	return BaseClass::OpenDocument(filePath, createView, viewTypeId, documentTypeId, beQuiet, ignoredPtr);
}


void CMultiDocumentWorkspaceGuiComp::SetActiveView(istd::IPolymorphic* viewPtr)
{
	if (viewPtr != GetActiveView()){
		QMdiArea* workspacePtr = GetQtWidget();
		Q_ASSERT(workspacePtr != NULL);

		QList<QMdiSubWindow *> windows = workspacePtr->subWindowList();
		for (int viewIndex = 0; viewIndex < windows.count(); viewIndex++){
			QMdiSubWindow* windowPtr = windows.at(viewIndex);
			if (windowPtr != NULL){
				QWidget* viewWidgetPtr = windowPtr->widget();
				if (viewWidgetPtr != NULL){
					iqtgui::IGuiObject* guiObjectPtr = NULL;
					guiObjectPtr = GetViewFromWidget(*viewWidgetPtr);
					if (viewPtr == guiObjectPtr){
						workspacePtr->setActiveSubWindow(windowPtr);
					}
				}
			}
		}

		idoc::CMultiDocumentManagerBase::SingleDocumentData* activeDocumentInfoPtr = GetDocumentInfoFromView(*viewPtr);
		if (activeDocumentInfoPtr == NULL){
			m_documentSelectionInfo.SetSelectedOptionIndex(iprm::ISelectionParam::NO_SELECTION);
		}
		else{
			int documentIndex = GetDocumentIndex(*activeDocumentInfoPtr);

			m_documentSelectionInfo.SetSelectedOptionIndex(documentIndex);
		}
	}

	BaseClass::SetActiveView(viewPtr);
}


// reimplemented (QObject)

bool CMultiDocumentWorkspaceGuiComp::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	if (!m_forceQuietClose && (eventPtr->type() == QEvent::Close)){
		const QWidget* widgetPtr = dynamic_cast<const QWidget*>(sourcePtr);
		if (widgetPtr != NULL){
			int documentIndex = GetDocumentIndexFromWidget(*widgetPtr);
			if (documentIndex >= 0){
				bool isCloseIgnored = false;
				CloseDocument(documentIndex, m_forceQuietClose, &isCloseIgnored);

				if (isCloseIgnored){
					eventPtr->ignore();

					return true;
				}
			}
		}
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


// reimplemented (TRestorableGuiWrap)

void CMultiDocumentWorkspaceGuiComp::OnRestoreSettings(const QSettings& settings)
{
	BaseClass::OnRestoreSettings(settings);

	Q_ASSERT(IsGuiCreated());

	QVariant valueNotSet = QVariant(-1);

	m_organizationName = settings.organizationName();
	m_applicationName = settings.applicationName();

	QVariant viewModeEntry = settings.value("MDIWorkspace/ViewMode", valueNotSet);
	if (viewModeEntry != valueNotSet){
		QMdiArea* workspacePtr = GetQtWidget();

		#if QT_VERSION >= 0x040400
			QMdiArea::ViewMode viewMode = QMdiArea::ViewMode(viewModeEntry.toInt());
			workspacePtr->setViewMode(viewMode);
			if (viewMode == QMdiArea::SubWindowView){
				m_subWindowCommand.setChecked(true);
				m_tabbedCommand.setChecked(false);
			}
			else{
				m_tabbedCommand.setChecked(true);
				m_subWindowCommand.setChecked(false);
			}
		#endif
	}

	if (m_rememberOpenDocumentsParamPtr.IsValid() && m_rememberOpenDocumentsParamPtr->IsEnabled()){
		iqt::CSettingsReadArchive archive(
					m_organizationName,
					m_applicationName,
					"OpenDocumentList");

		SerializeOpenDocumentList(archive);
	}
}


void CMultiDocumentWorkspaceGuiComp::OnSaveSettings(QSettings& settings) const
{
	BaseClass::OnSaveSettings(settings);

	Q_ASSERT(IsGuiCreated());

	QMdiArea* workspacePtr = GetQtWidget();

#if QT_VERSION >= 0x040400
	settings.setValue("MDIWorkspace/ViewMode", workspacePtr->viewMode());
#endif
}


// reimplemented (idoc::CMultiDocumentManagerBase)

void CMultiDocumentWorkspaceGuiComp::CloseAllDocuments()
{
	QMdiArea* workspacePtr = GetQtWidget();
	Q_ASSERT(workspacePtr != NULL);
	if (workspacePtr == NULL){
		return;
	}

	m_forceQuietClose = true;

	m_documentSelectionInfo.SetSelectedOptionIndex(iprm::ISelectionParam::NO_SELECTION);

	workspacePtr->closeAllSubWindows();

	BaseClass::CloseAllDocuments();

	m_forceQuietClose = false;
}


QStringList CMultiDocumentWorkspaceGuiComp::GetOpenFilePaths(const QByteArray* documentTypeIdPtr) const
{
	QStringList files = GetOpenFilePathesFromDialog(documentTypeIdPtr);

	if (!files.isEmpty()){
		UpdateLastDirectory(files.at(0));
	}

	return files;
}


void CMultiDocumentWorkspaceGuiComp::OnViewRegistered(istd::IPolymorphic* viewPtr)
{
	iqtgui::IGuiObject* guiObjectPtr = CompCastPtr<iqtgui::IGuiObject>(viewPtr);
	QMdiArea* workspacePtr = GetQtWidget();
	if ((guiObjectPtr != NULL) && (workspacePtr != NULL)){
		if (guiObjectPtr->CreateGui(workspacePtr)){
			QWidget* widgetPtr = guiObjectPtr->GetWidget();
			Q_ASSERT(widgetPtr != NULL);

			widgetPtr->installEventFilter(this);
			QMdiSubWindow* subWindow = workspacePtr->addSubWindow(widgetPtr);
			Q_ASSERT(subWindow != NULL);

			QIcon windowIcon = widgetPtr->windowIcon();
			if (!windowIcon.isNull()){
				subWindow->setWindowIcon(windowIcon);
			}

			if (m_showMaximizedAttrPtr.IsValid() && *m_showMaximizedAttrPtr){
				subWindow->showMaximized();
			}
			else{
				subWindow->show();
			}

			SetActiveView(viewPtr);
		}
	}

	++m_viewsCount;

	OnViewsCountChanged();
}


void CMultiDocumentWorkspaceGuiComp::OnViewRemoved(istd::IPolymorphic* viewPtr)
{
	--m_viewsCount;

	OnViewsCountChanged();

	iqtgui::IGuiObject* guiObjectPtr = CompCastPtr<iqtgui::IGuiObject>(viewPtr);
	if (guiObjectPtr != NULL){
		guiObjectPtr->DestroyGui();
	}
}


bool CMultiDocumentWorkspaceGuiComp::QueryDocumentSave(const SingleDocumentData& info, bool* ignoredPtr)
{
	QFileInfo fileInfo(info.filePath);
	QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No;

	if (ignoredPtr != NULL){
		*ignoredPtr = false;
		buttons |= QMessageBox::Cancel;
	}

	int response = QMessageBox::information(
				GetQtWidget(),
				tr("Close document"),
				tr("Do you want to save your changes made in document\n%1").arg(fileInfo.fileName()),
				buttons,
				QMessageBox::Yes);

	if (response == QMessageBox::Yes){
		return true;
	}
	else if ((ignoredPtr != NULL) && (response == QMessageBox::Cancel)){
		*ignoredPtr = true;
	}

	return false;
}


// reimplemented (iqt:CGuiObjectBase)

void CMultiDocumentWorkspaceGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CreateConnections();

	int documentsCount = GetDocumentsCount();
	for (int docIndex = 0; docIndex < documentsCount; ++docIndex){
		int viewsCount = GetViewsCount(docIndex);

		for (int viewIndex = 0; viewIndex < viewsCount; ++viewIndex){
			istd::IPolymorphic* viewPtr = GetViewFromIndex(docIndex, viewIndex);
			Q_ASSERT(viewPtr != NULL);

			OnViewRegistered(viewPtr);
		}
	}

	QMdiArea* mdiAreaPtr = GetQtWidget();
	Q_ASSERT(mdiAreaPtr != NULL);

	mdiAreaPtr->setActivationOrder(QMdiArea::ActivationHistoryOrder);

	OnViewsCountChanged();
}


void CMultiDocumentWorkspaceGuiComp::OnGuiDestroyed()
{
	CloseAllDocuments();

	BaseClass::OnGuiDestroyed();
}


void CMultiDocumentWorkspaceGuiComp::OnRetranslate()
{
	BaseClass::OnRetranslate();

	istd::CChangeNotifier changePtr(this, CF_COMMANDS);

	m_windowCommand.SetName(tr("&Window"));
	// Window commands
	m_cascadeCommand.SetVisuals(tr("Casca&de"), tr("Cascade"), tr("Lays out all document windows in cascaded mode"));
	m_tileHorizontallyCommand.SetVisuals(tr("Tile &Horizontaly"), tr("Horizontal"), tr("Lays out all document windows horizontaly"));
	m_tileVerticallyCommand.SetVisuals(tr("Tile &Verticaly"), tr("Vertical"), tr("Lays out all document windows verticaly"));
	m_closeAllDocumentsCommand.SetVisuals(tr("&Close All Documents"), tr("Close All"), tr("&Closes all opened documents"));
	m_workspaceModeCommand.SetVisuals(tr("&Workspace Mode"), tr("Workspace Mode"), tr("Switch workspace mode"));
	m_subWindowCommand.SetVisuals(tr("&Multiple Documents"), tr("Multiple Documents"), tr("Show each window in own frame"));
	m_tabbedCommand.SetVisuals(tr("&Tabbed Documents"), tr("Tabbed Documents"), tr("Show windows in tabbed frame"));
}


void CMultiDocumentWorkspaceGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	UpdateAllTitles();
}


// reimplemented (icomp::CComponentBase)

void CMultiDocumentWorkspaceGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetDocumentTemplate(m_documentTemplateCompPtr.GetPtr());
}


// reimplemented (istd:IChangeable)

void CMultiDocumentWorkspaceGuiComp::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::OnEndChanges(changeFlags, changeParamsPtr);

	if (IsGuiCreated()){
		UpdateAllTitles();
	}

	idoc::CMultiDocumentManagerBase::SingleDocumentData* activeDocumentInfoPtr = GetActiveDocumentInfo();
	if (activeDocumentInfoPtr == NULL){
		m_documentSelectionInfo.SetSelectedOptionIndex(iprm::ISelectionParam::NO_SELECTION);
	}
	else{
		int documentIndex = GetDocumentIndex(*activeDocumentInfoPtr);

		m_documentSelectionInfo.SetSelectedOptionIndex(documentIndex);
	}
}


// protected slots

void CMultiDocumentWorkspaceGuiComp::OnWindowActivated(QMdiSubWindow* /*window*/)
{
	QMdiArea* workspacePtr = GetQtWidget();
	Q_ASSERT(workspacePtr != NULL);

	QMdiSubWindow* currentWindow = workspacePtr->currentSubWindow();

	iqtgui::IGuiObject* guiObjectPtr = NULL;
	if (currentWindow != NULL){
		QWidget* widgetPtr = currentWindow->widget();
		if (widgetPtr != NULL){
			guiObjectPtr = GetViewFromWidget(*widgetPtr);
		}
	}

	SetActiveView(guiObjectPtr);
}


void CMultiDocumentWorkspaceGuiComp::OnTileHorizontally()
{
	QMdiArea* workspacePtr = GetQtWidget();
	Q_ASSERT(workspacePtr != NULL);
	if (workspacePtr == NULL){
		return;
	}

	QList<QMdiSubWindow *> widgets = workspacePtr->subWindowList();

	int workspaceHeight = workspacePtr->height();
	int workspaceWidth = workspacePtr->width();
	int heightForEach = workspaceHeight / widgets.count();

	for (int viewIndex = 0, y = 0; viewIndex < widgets.count(); viewIndex++){
		QMdiSubWindow* widgetPtr = widgets.at(viewIndex);
		widgetPtr->showNormal();

		widgetPtr->setGeometry(0, y, workspaceWidth, heightForEach);
		y += heightForEach;
	}
}


void CMultiDocumentWorkspaceGuiComp::OnTile()
{
	QMdiArea* workspacePtr = GetQtWidget();
	Q_ASSERT(workspacePtr != NULL);
	if (workspacePtr == NULL){
		return;
	}

	workspacePtr->tileSubWindows();
}


void CMultiDocumentWorkspaceGuiComp::OnCascade()
{
	QMdiArea* workspacePtr = GetQtWidget();
	Q_ASSERT(workspacePtr != NULL);
	if (workspacePtr == NULL){
		return;
	}

	workspacePtr->cascadeSubWindows();
}


void CMultiDocumentWorkspaceGuiComp::OnCloseAllViews()
{
	bool isCanceled = false;
	if (SaveDirtyDocuments(false, &isCanceled) && !isCanceled){
		CloseAllDocuments();
	}
}


void CMultiDocumentWorkspaceGuiComp::OnWorkspaceModeChanged()
{
#if QT_VERSION >= 0x040400
	QMdiArea* mdiAreaPtr = GetQtWidget();

	if (mdiAreaPtr != NULL){
		if (m_subWindowCommand.isChecked()){
			mdiAreaPtr->setViewMode(QMdiArea::SubWindowView);
		}
		else{
			mdiAreaPtr->setViewMode(QMdiArea::TabbedView);
		}
	}
#endif
}


// public methods of the embedded class DocumentSelectionInfo

CMultiDocumentWorkspaceGuiComp::DocumentSelectionInfo::DocumentSelectionInfo()
	:m_selectedDocumentIndex(iprm::ISelectionParam::NO_SELECTION)
{
}


void CMultiDocumentWorkspaceGuiComp::DocumentSelectionInfo::SetParent(CMultiDocumentWorkspaceGuiComp& parent)
{
	m_parent = &parent;
}


// reimplemented (iprm::ISelectionParam)

const iprm::IOptionsList* CMultiDocumentWorkspaceGuiComp::DocumentSelectionInfo::GetSelectionConstraints() const
{
	return this;
}


int CMultiDocumentWorkspaceGuiComp::DocumentSelectionInfo::GetSelectedOptionIndex() const
{
	return m_selectedDocumentIndex;
}


bool CMultiDocumentWorkspaceGuiComp::DocumentSelectionInfo::SetSelectedOptionIndex(int index)
{
	if (index >= GetOptionsCount()){
		return false;
	}

	if (m_selectedDocumentIndex != index){
		istd::CChangeNotifier changePtr(this, CF_SELECTION_CHANGED);

		m_selectedDocumentIndex = index;

		istd::IPolymorphic* viewPtr = NULL;
		if (m_selectedDocumentIndex >= 0){
			viewPtr = m_parent->GetViewFromIndex(m_selectedDocumentIndex, 0);

			m_parent->SetActiveView(viewPtr);
		}
	}

	return true;
}


iprm::ISelectionParam* CMultiDocumentWorkspaceGuiComp::DocumentSelectionInfo::GetSubselection(int /*index*/) const
{
	return NULL;
}


// reimplemented (iprm::IOptionsList)

int CMultiDocumentWorkspaceGuiComp::DocumentSelectionInfo::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int CMultiDocumentWorkspaceGuiComp::DocumentSelectionInfo::GetOptionsCount() const
{
	Q_ASSERT(m_parent != NULL);

	return m_parent->GetDocumentsCount();
}


QString CMultiDocumentWorkspaceGuiComp::DocumentSelectionInfo::GetOptionName(int index) const
{
	SingleDocumentData& documentData = m_parent->GetSingleDocumentData(index);

	if (!documentData.filePath.isEmpty()){
		QFileInfo fileInfo(documentData.filePath);

		return fileInfo.fileName();
	}

	return QString(tr("<no name>"));
}


QString CMultiDocumentWorkspaceGuiComp::DocumentSelectionInfo::GetOptionDescription(int /*index*/) const
{
	return QString();
}


QByteArray CMultiDocumentWorkspaceGuiComp::DocumentSelectionInfo::GetOptionId(int index) const
{
	return GetOptionName(index).toLatin1();
}


bool CMultiDocumentWorkspaceGuiComp::DocumentSelectionInfo::IsOptionEnabled(int /*index*/) const
{
	return true;
}


// reimplemented (iser::ISerializable)

bool CMultiDocumentWorkspaceGuiComp::DocumentSelectionInfo::Serialize(iser::IArchive& /*archive*/)
{
	I_CRITICAL(); // NOT IMPLEMENTED

	return false;
}


} // namespace iqtdoc


