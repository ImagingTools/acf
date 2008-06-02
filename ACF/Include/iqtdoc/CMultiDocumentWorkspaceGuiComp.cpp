#include "iqtdoc/CMultiDocumentWorkspaceGuiComp.h"


#include <QEvent>
#include <QFileDialog>
#include <QMap>


#include "istd/TChangeNotifier.h"

#include "idoc/IDocumentTemplate.h"

#include "iqt/CSettingsWriteArchive.h"
#include "iqt/CSettingsReadArchive.h"

#include "iser/CXmlFileWriteArchive.h"
#include "iser/CXmlFileReadArchive.h"


namespace iqtdoc
{


// public methods

// reimplemented (iqtdoc::IWorkspaceController)

void CMultiDocumentWorkspaceGuiComp::TileHorizontally()
{
	QWorkspace* workspacePtr = GetQtWidget();
	if (workspacePtr == NULL){
		return;
	}

	QWidgetList widgets = workspacePtr->windowList();

	int workspaceHeight = workspacePtr->height();
	int workspaceWidth = workspacePtr->width();
	int heightForEach = workspaceHeight / widgets.count();
	
	int y = 0;
	for (int viewIndex = 0; viewIndex < widgets.count(); viewIndex++){
		QWidget* widgetPtr = widgets.at(viewIndex);
		widgetPtr->showNormal();									
		if (widgetPtr->parentWidget() != NULL){
			int preferredHeight = widgetPtr->minimumHeight() + widgetPtr->parentWidget()->baseSize().height();
			int currentHeight = istd::Max(heightForEach, preferredHeight);

			widgetPtr->parentWidget()->setGeometry(0, y, workspaceWidth, currentHeight);
			y += currentHeight;
		}
	}
}


void CMultiDocumentWorkspaceGuiComp::Tile()
{
	QWorkspace* workspacePtr = GetQtWidget();
	if (workspacePtr == NULL){
		return;
	}

	workspacePtr->tile();
}


void CMultiDocumentWorkspaceGuiComp::Cascade()
{
	QWorkspace* workspacePtr = GetQtWidget();
	if (workspacePtr == NULL){
		return;
	}

	workspacePtr->cascade();
}


void CMultiDocumentWorkspaceGuiComp::CloseAllViews()
{
	QWorkspace* workspacePtr = GetQtWidget();
	if (workspacePtr == NULL){
		return;
	}

	workspacePtr->closeAllWindows();
}


// reimplemented (iqt::IGuiObject)

void CMultiDocumentWorkspaceGuiComp::OnTryClose(bool* ignoredPtr)
{
	CloseAllDocuments();

	if (ignoredPtr != NULL){
		*ignoredPtr = (GetDocumentsCount() > 0);
	}
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

		for (		Views::const_iterator viewIter = info.views.begin();
					viewIter != info.views.end();
					++viewIter){
			const ViewPtr& viewPtr = *viewIter;
			I_ASSERT(viewPtr.IsValid());

			const iqt::IGuiObject* guiObjectPtr = dynamic_cast<const iqt::IGuiObject*>(viewPtr.GetPtr());
			if (guiObjectPtr != NULL){
				QWidget* widgetPtr = guiObjectPtr->GetWidget();
				I_ASSERT(widgetPtr != NULL);

				widgetPtr->setWindowTitle(titleName);
			}
		}
	}
}


iqt::IGuiObject* CMultiDocumentWorkspaceGuiComp::GetViewFromWidget(const QWidget& widget) const
{
	int documentInfosCount = GetDocumentsCount();
	for (int i = 0; i < documentInfosCount; ++i){
		DocumentInfo& info = GetDocumentInfo(i);

		for (		Views::const_iterator viewIter = info.views.begin();
					viewIter != info.views.end();
					++viewIter){
			iqt::IGuiObject* guiObjectPtr = dynamic_cast<iqt::IGuiObject*>(viewIter->GetPtr());
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
	connect(GetWidget(), SIGNAL(windowActivated(QWidget*)), this, SLOT(OnWindowActivated(QWidget*)));
}


QString CMultiDocumentWorkspaceGuiComp::CreateFileDialogFilter(const std::string* documentTypeIdPtr) const
{
	QString retVal;

	const idoc::IDocumentTemplate* templatePtr = GetDocumentTemplate();
	if (templatePtr != NULL){
		istd::CStringList filters = templatePtr->GetFileFilters(documentTypeIdPtr);

		if (filters.size() > 1){
			QString extText;
			istd::CStringList extensions = templatePtr->GetFileExtensions(documentTypeIdPtr);

			for (		istd::CStringList::iterator extIter = extensions.begin();
						extIter != extensions.end();
						++extIter){
				if (!extText.isEmpty()){
					extText += "; ";
				}

				extText += "*." + iqt::GetQString(*extIter);
			}

			if (!extText.isEmpty()){
				retVal = tr("All known documents (%1)").arg(extText);
			}
		}

		for (		istd::CStringList::iterator filterIter = filters.begin();
					filterIter != filters.end();
					++filterIter){
			if (!retVal.isEmpty()){
				retVal += "\n";
			}
			retVal += iqt::GetQString(*filterIter);
		}
	}

	return retVal;
}


// reimplemented (QObject)

bool CMultiDocumentWorkspaceGuiComp::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::Close){
		const QWidget* widgetPtr = dynamic_cast<const QWidget*>(obj);
		if (widgetPtr != NULL){
			iqt::IGuiObject* guiObjectPtr = GetViewFromWidget(*widgetPtr);
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

	QStringList files = QFileDialog::getOpenFileNames(NULL, tr("Open Files..."), "", filter);

	return iqt::GetCStringList(files);
}


istd::CString CMultiDocumentWorkspaceGuiComp::GetSaveFileName(const std::string& documentTypeId) const
{
	QString filter = CreateFileDialogFilter(&documentTypeId);

	QString filePath = QFileDialog::getSaveFileName(NULL, tr("Save..."), "", filter);

	return iqt::GetCString(filePath);
}


void CMultiDocumentWorkspaceGuiComp::OnViewRegistered(istd::IPolymorphic* viewPtr)
{
	iqt::IGuiObject* guiObjectPtr = dynamic_cast<iqt::IGuiObject*>(viewPtr);
	QWorkspace* workspacePtr = GetQtWidget();
	if ((guiObjectPtr != NULL) && (workspacePtr != NULL)){
		if (guiObjectPtr->CreateGui(workspacePtr)){
			QWidget* widgetPtr = guiObjectPtr->GetWidget();
			I_ASSERT(widgetPtr != NULL);

			widgetPtr->installEventFilter(this);
			workspacePtr->addWindow(widgetPtr);
			widgetPtr->show();

			SetActiveView(viewPtr);
		}
	}
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
	CreateConnections();

	if (m_scrollingEnabledAttrPtr.IsValid()){
		QWorkspace* workspacePtr = GetQtWidget();
	
		workspacePtr->setScrollBarsEnabled(m_scrollingEnabledAttrPtr->GetValue());
	}

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

void CMultiDocumentWorkspaceGuiComp::OnWindowActivated(QWidget* window)
{
	iqt::IGuiObject* guiObjectPtr = (window != NULL) ? GetViewFromWidget(*window): NULL;

	SetActiveView(guiObjectPtr);
}	


} // namespace iqtdoc

