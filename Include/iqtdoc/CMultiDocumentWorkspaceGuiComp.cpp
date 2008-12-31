#include "iqtdoc/CMultiDocumentWorkspaceGuiComp.h"


// Qt includes
#include <QEvent>
#include <QFileDialog>
#include <QMap>
#include <QMessageBox>
#include <QFileInfo>

#include "istd/TChangeNotifier.h"

#include "idoc/IDocumentTemplate.h"

#include "iqt/CSettingsWriteArchive.h"
#include "iqt/CSettingsReadArchive.h"

#include "iqtgui/CFileDialogLoaderComp.h"

#include "iser/CXmlFileWriteArchive.h"
#include "iser/CXmlFileReadArchive.h"


namespace iqtdoc
{


// public methods

CMultiDocumentWorkspaceGuiComp::CMultiDocumentWorkspaceGuiComp()
:	m_workspaceModeCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU),
	m_tabbedCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_ONOFF | idoc::ICommand::CF_EXCLUSIVE),
	m_subWindowCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_ONOFF | idoc::ICommand::CF_EXCLUSIVE),
	m_viewsCount(0)
{
	m_workspaceModeCommand.InsertChild(&m_subWindowCommand, false);
	m_workspaceModeCommand.InsertChild(&m_tabbedCommand, false);
	m_commands.InsertChild(&m_windowCommand, false);
	m_windowCommand.SetPriority(120);
	m_cascadeCommand.SetGroupId(GI_WINDOW);
	m_windowCommand.InsertChild(&m_cascadeCommand, false);
	m_tileHorizontallyCommand.SetGroupId(GI_WINDOW);
	m_windowCommand.InsertChild(&m_tileHorizontallyCommand, false);
	m_tileVerticallyCommand.SetGroupId(GI_WINDOW);
	m_windowCommand.InsertChild(&m_tileVerticallyCommand, false);
	m_closeAllDocumentsCommand.SetGroupId(GI_DOCUMENT);
	m_windowCommand.InsertChild(&m_closeAllDocumentsCommand, false);
	m_workspaceModeCommand.SetGroupId(GI_VIEW);
	m_windowCommand.InsertChild(&m_workspaceModeCommand, false);

	connect(&m_cascadeCommand, SIGNAL(activated()), this, SLOT(OnCascade()));
	connect(&m_tileHorizontallyCommand, SIGNAL(activated()), this, SLOT(OnTileHorizontally()));
	connect(&m_tileVerticallyCommand, SIGNAL(activated()), this, SLOT(OnTile()));
	connect(&m_closeAllDocumentsCommand, SIGNAL(activated()), this, SLOT(OnCloseAllViews()));

	m_subWindowCommand.setChecked(true);
	connect(&m_subWindowCommand, SIGNAL(activated()), this, SLOT(OnWorkspaceModeChanged()));
	connect(&m_tabbedCommand, SIGNAL(activated()), this, SLOT(OnWorkspaceModeChanged()));
}


// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CMultiDocumentWorkspaceGuiComp::GetCommands() const
{
	return &m_commands;
}


// reimplemented (iqtgui::IGuiObject)

void CMultiDocumentWorkspaceGuiComp::OnTryClose(bool* ignoredPtr)
{
	CloseAllDocuments();

	if (ignoredPtr != NULL){
		*ignoredPtr = (GetDocumentsCount() > 0);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CMultiDocumentWorkspaceGuiComp::OnRetranslate()
{
	m_windowCommand.SetName(iqt::GetCString(tr("&Window")));
	// Window commands
	m_cascadeCommand.SetVisuals(tr("Casca&de"), tr("Cascade"), tr("Lays out all document windows in cascaded mode"));
	m_tileHorizontallyCommand.SetVisuals(tr("Tile &Horizontaly"), tr("Horizontal"), tr("Lays out all document windows horizontaly"));
	m_tileVerticallyCommand.SetVisuals(tr("Tile &Verticaly"), tr("Vertical"), tr("Lays out all document windows verticaly"));
	m_closeAllDocumentsCommand.SetVisuals(tr("&Close All Documents"), tr("Close All"), tr("&Closes all opened documents"));
	m_workspaceModeCommand.SetVisuals(tr("&Workspace Mode"), tr("Workspace Mode"), tr("Switch workspace mode"));
	m_subWindowCommand.SetVisuals(tr("&MDI View"), tr("MDI View"), tr("Use sub-windows mode"));
	m_tabbedCommand.SetVisuals(tr("&Tabbed View"), tr("Tabbed View"), tr("Use tabbed mode"));
}


// reimplemented (icomp::IComponent)

void CMultiDocumentWorkspaceGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetDocumentTemplate(m_documentTemplateCompPtr.GetPtr());
}


// protected members

void CMultiDocumentWorkspaceGuiComp::UpdateAllTitles()
{
	typedef QMap<QString, int> NameFrequencies;
	NameFrequencies nameFrequencies;

	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		const DocumentInfo& info = GetDocumentInfo(i);

		QString titleName = info.filePath.IsEmpty()?
					tr("<no name>"):
					QFileInfo(iqt::GetQString(info.filePath)).fileName();

		NameFrequencies::iterator freqIter = nameFrequencies.find(titleName);
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

		for (		Views::const_iterator viewIter = info.views.begin();
					viewIter != info.views.end();
					++viewIter){
			const ViewPtr& viewPtr = *viewIter;
			I_ASSERT(viewPtr.IsValid());

			const iqtgui::IGuiObject* guiObjectPtr = dynamic_cast<const iqtgui::IGuiObject*>(viewPtr.GetPtr());
			if (guiObjectPtr != NULL){
				QWidget* widgetPtr = guiObjectPtr->GetWidget();
				I_ASSERT(widgetPtr != NULL);

				widgetPtr->setWindowTitle(titleName);
			}
		}
	}
}


iqtgui::IGuiObject* CMultiDocumentWorkspaceGuiComp::GetViewFromWidget(const QWidget& widget) const
{
	int documentInfosCount = GetDocumentsCount();
	for (int i = 0; i < documentInfosCount; ++i){
		DocumentInfo& info = GetDocumentInfo(i);

		for (		Views::const_iterator viewIter = info.views.begin();
					viewIter != info.views.end();
					++viewIter){
			iqtgui::IGuiObject* guiObjectPtr = dynamic_cast<iqtgui::IGuiObject*>(viewIter->GetPtr());
			if (guiObjectPtr != NULL){
				if (guiObjectPtr->GetWidget() == &widget){
					return guiObjectPtr;
				}
			}
		}
	}

	return NULL;
}


void CMultiDocumentWorkspaceGuiComp::CreateConnections()
{
	connect(GetWidget(), SIGNAL( subWindowActivated(QMdiSubWindow*)), this, SLOT(OnWindowActivated(QMdiSubWindow*)));
}


QString CMultiDocumentWorkspaceGuiComp::CreateFileDialogFilter(const std::string* documentTypeIdPtr) const
{
	QString retVal;

	const idoc::IDocumentTemplate* templatePtr = GetDocumentTemplate();
	if (templatePtr != NULL){
		idoc::IDocumentTemplate::Ids docTypeIds = templatePtr->GetDocumentTypeIds();

		QString allExt;
		int filtersCount = 0;

		if (documentTypeIdPtr != NULL){
			iser::IFileLoader* loaderPtr = templatePtr->GetFileLoader(*documentTypeIdPtr);
			if (loaderPtr != NULL){
				filtersCount += iqtgui::CFileDialogLoaderComp::AppendLoaderFilterList(*loaderPtr, allExt, retVal);
			}
		}
		else{
			for (		idoc::IDocumentTemplate::Ids::const_iterator docTypeIter = docTypeIds.begin();
						docTypeIter != docTypeIds.end();
						++docTypeIter){
				iser::IFileLoader* loaderPtr = templatePtr->GetFileLoader(*docTypeIter);
				if (loaderPtr != NULL){
					filtersCount += iqtgui::CFileDialogLoaderComp::AppendLoaderFilterList(*loaderPtr, allExt, retVal);
				}
			}
		}

		if (filtersCount > 1){
			retVal += "\n";
			retVal += tr("All known documents (%1)").arg(allExt);
		}
	}

	return retVal;
}


void CMultiDocumentWorkspaceGuiComp::OnViewsCountChanged()
{
	m_cascadeCommand.SetEnabled(m_viewsCount > 1);
	m_tileHorizontallyCommand.SetEnabled(m_viewsCount > 1);
	m_tileVerticallyCommand.SetEnabled(m_viewsCount > 1);
	m_closeAllDocumentsCommand.SetEnabled(m_viewsCount > 0);
}


// reimplemented (QObject)

bool CMultiDocumentWorkspaceGuiComp::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::Close){
		const QWidget* widgetPtr = dynamic_cast<const QWidget*>(obj);
		if (widgetPtr != NULL){
			iqtgui::IGuiObject* guiObjectPtr = GetViewFromWidget(*widgetPtr);
			if (guiObjectPtr != NULL){
				SetActiveView(guiObjectPtr);

				if (FileClose()){
					return true;
				}
			}
		}

		event->ignore();

		return true;
	}

	return false;
}


// reimplemented (idoc::CDocumentManagerBase)

istd::CStringList CMultiDocumentWorkspaceGuiComp::GetOpenFileNames(const std::string* documentTypeIdPtr) const
{
	QString filter = CreateFileDialogFilter(documentTypeIdPtr);

	QStringList files = QFileDialog::getOpenFileNames(NULL, tr("Open Files..."), m_lastDirectory, filter);

	if (!files.isEmpty()){
		UpdateLastDirectory(files.at(0));
	}

	return iqt::GetCStringList(files);
}


istd::CString CMultiDocumentWorkspaceGuiComp::GetSaveFileName(const std::string& documentTypeId) const
{
	QString filter = CreateFileDialogFilter(&documentTypeId);

	QString filePath = QFileDialog::getSaveFileName(NULL, tr("Save..."), m_lastDirectory, filter);

	UpdateLastDirectory(filePath);

	return iqt::GetCString(filePath);
}


void CMultiDocumentWorkspaceGuiComp::OnViewRegistered(istd::IPolymorphic* viewPtr)
{
	iqtgui::IGuiObject* guiObjectPtr = dynamic_cast<iqtgui::IGuiObject*>(viewPtr);
	QMdiArea* workspacePtr = GetQtWidget();
	if ((guiObjectPtr != NULL) && (workspacePtr != NULL)){
		if (guiObjectPtr->CreateGui(workspacePtr)){
			QWidget* widgetPtr = guiObjectPtr->GetWidget();
			I_ASSERT(widgetPtr != NULL);

			widgetPtr->installEventFilter(this);
			QMdiSubWindow* subWindow = workspacePtr->addSubWindow(widgetPtr);
			I_ASSERT(subWindow != NULL);

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


void CMultiDocumentWorkspaceGuiComp::OnViewRemoved(istd::IPolymorphic* /*viewPtr*/)
{
	--m_viewsCount;

	OnViewsCountChanged();
}


bool CMultiDocumentWorkspaceGuiComp::QueryDocumentClose(const DocumentInfo& info)
{
	QFileInfo fileInfo(iqt::GetQString(info.filePath));
	int response = QMessageBox::information(
				NULL,
				QObject::tr("Close document"),
				QObject::tr("Do you want to save your changes made in document\n%1").arg(fileInfo.fileName()),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
				QMessageBox::Yes);

	if (response == QMessageBox::Yes){
		return FileSave();
	}

	return response == QMessageBox::No;
}


int CMultiDocumentWorkspaceGuiComp::GetMaxRecentFilesCount() const
{
	if (m_maxRecentFilesCountAttrPtr.IsValid()){
		return *m_maxRecentFilesCountAttrPtr;
	}

	return BaseClass2::GetMaxRecentFilesCount();
}


// reimplemented (imod::CMultiModelObserverBase)

void CMultiDocumentWorkspaceGuiComp::OnUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	BaseClass2::OnUpdate(modelPtr, updateFlags, updateParamsPtr);

	UpdateAllTitles();

	// TODO: implement support for modify flag.
}


// reimplemented (iqt:CGuiObjectBase)

void CMultiDocumentWorkspaceGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QMdiArea* workspacePtr = GetQtWidget();
	I_ASSERT(workspacePtr != NULL);

	CreateConnections();

	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		DocumentInfo& info = GetDocumentInfo(i);

		for (		Views::const_iterator viewIter = info.views.begin();
					viewIter != info.views.end();
					++viewIter){
				const ViewPtr& viewPtr = *viewIter;
				I_ASSERT(viewPtr.IsValid());

				OnViewRegistered(viewPtr.GetPtr());
		}
	}

	SerializeRecentFiles<iqt::CSettingsReadArchive>();

	OnRetranslate();
}


void CMultiDocumentWorkspaceGuiComp::OnGuiDestroyed()
{
	CloseAllDocuments();

	SerializeRecentFiles<iqt::CSettingsWriteArchive>();
}


// reimplemented (istd:IChangeable)

void CMultiDocumentWorkspaceGuiComp::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass2::OnEndChanges(changeFlags, changeParamsPtr);

	if (IsGuiCreated()){
		UpdateAllTitles();
	}
}


// protected slots

void CMultiDocumentWorkspaceGuiComp::OnWindowActivated(QMdiSubWindow* window)
{
	iqtgui::IGuiObject* guiObjectPtr = NULL;
	if (window != NULL){
		QWidget* widgetPtr = window->widget();
		if (widgetPtr != NULL){
			guiObjectPtr = GetViewFromWidget(*widgetPtr);
		}
	}

	SetActiveView(guiObjectPtr);
}	


void CMultiDocumentWorkspaceGuiComp::OnTileHorizontally()
{
	QMdiArea* workspacePtr = GetQtWidget();
	I_ASSERT(workspacePtr != NULL);
	if (workspacePtr == NULL){
		return;
	}

	QList<QMdiSubWindow *> widgets = workspacePtr->subWindowList();

	int workspaceHeight = workspacePtr->height();
	int workspaceWidth = workspacePtr->width();
	int heightForEach = workspaceHeight / widgets.count();
	
	int y = 0;
	for (int viewIndex = 0; viewIndex < widgets.count(); viewIndex++){
		QMdiSubWindow* widgetPtr = widgets.at(viewIndex);
		widgetPtr->showNormal();									
		if (widgetPtr->parentWidget() != NULL){
			int preferredHeight = widgetPtr->minimumHeight() + widgetPtr->parentWidget()->baseSize().height();
			int currentHeight = istd::Max(heightForEach, preferredHeight);

			widgetPtr->parentWidget()->setGeometry(0, y, workspaceWidth, currentHeight);
			y += currentHeight;
		}
	}
}


void CMultiDocumentWorkspaceGuiComp::OnTile()
{
	QMdiArea* workspacePtr = GetQtWidget();
	I_ASSERT(workspacePtr != NULL);
	if (workspacePtr == NULL){
		return;
	}

	workspacePtr->tileSubWindows();
}


void CMultiDocumentWorkspaceGuiComp::OnCascade()
{
	QMdiArea* workspacePtr = GetQtWidget();
	I_ASSERT(workspacePtr != NULL);
	if (workspacePtr == NULL){
		return;
	}

	workspacePtr->cascadeSubWindows();
}


void CMultiDocumentWorkspaceGuiComp::OnCloseAllViews()
{
	QMdiArea* workspacePtr = GetQtWidget();
	I_ASSERT(workspacePtr != NULL);
	if (workspacePtr == NULL){
		return;
	}

	workspacePtr->closeAllSubWindows();
}


void CMultiDocumentWorkspaceGuiComp::OnWorkspaceModeChanged()
{
	QMdiArea* mdiAreaPtr = GetQtWidget();

	if (mdiAreaPtr != NULL){
		if (m_subWindowCommand.isChecked()){
			mdiAreaPtr->setViewMode(QMdiArea::SubWindowView);
		}
		else{
			mdiAreaPtr->setViewMode(QMdiArea::TabbedView);
		}
	}
}


// private methods

void CMultiDocumentWorkspaceGuiComp::UpdateLastDirectory(const QString& filePath) const
{
	QFileInfo fileInfo(filePath);

	m_lastDirectory = fileInfo.absolutePath();
}


} // namespace iqtdoc


