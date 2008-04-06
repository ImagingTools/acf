#include "iqmain/CMultiDocumentWorkspaceGuiComp.h"


#include <QEvent>
#include <QFileDialog>


#include "istd/TChangeNotifier.h"

#include "idoc/IDocumentTemplate.h"


namespace iqmain
{


// public methods

// reimplemented (iqt::IGuiObject)

void CMultiDocumentWorkspaceGuiComp::OnTryClose(bool* ignoredPtr)
{
	CloseAllDocuments();

	if (ignoredPtr != NULL){
		*ignoredPtr = (GetDocumentCount() > 0);
	}
}


// reimplemented (idoc::IDocumentManager)

idoc::IDocument* CMultiDocumentWorkspaceGuiComp::OnFileNew(const std::string& documentId)
{
	idoc::IDocument* document = BaseClass2::OnFileNew(documentId);
	if (document != NULL){
		QWidget* widgetPtr = dynamic_cast<QWidget*>(document->GetView(0));
		QWorkspace* workspacePtr = dynamic_cast<QWorkspace*>(GetWidget());
		if (widgetPtr != NULL && workspacePtr != NULL){
			widgetPtr->setParent(workspacePtr);
			widgetPtr->installEventFilter(this);
			workspacePtr->addWindow(widgetPtr);
			widgetPtr->show();
		}
	}

	return document;
}


istd::CStringList CMultiDocumentWorkspaceGuiComp::GetDocumentIds() const
{
	istd::CStringList documentIds;
	if (m_documentIdAttrPtr.IsValid()){
		for (int index = 0; index < m_documentIdAttrPtr.GetCount(); index++){
			documentIds.push_back(m_documentIdAttrPtr[index]);
		}
	}

	return documentIds;
}


// protected members

bool CMultiDocumentWorkspaceGuiComp::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::Close){
		imod::IObserver* viewPtr = dynamic_cast<imod::IObserver*>(obj);
		if (viewPtr != NULL){
			if (!OnFileClose()){
				event->ignore();
			}
				
			return true;
		}
	}

	return false;
}


// reimplemented (idoc::CDocumentManagerBase)

istd::CString CMultiDocumentWorkspaceGuiComp::GetSaveFileName(const std::string& documentId) const
{
	QString filter = CreateFileDialogFilter(documentId);

	QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save..."), "", filter);

	return iqt::GetCString(fileName);
}


istd::CStringList CMultiDocumentWorkspaceGuiComp::GetOpenFileNames(const std::string& documentId) const
{
	QString filter = CreateFileDialogFilter(documentId);

	QStringList files = QFileDialog::getOpenFileNames(NULL, tr("Open Files..."), "", filter);

	return iqt::GetCStringList(files);
}

// reimplemented (imod::CMultiModelObserverBase)

void CMultiDocumentWorkspaceGuiComp::OnUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	idoc::IDocument* documentPtr = dynamic_cast<idoc::IDocument*> (modelPtr);

	if (documentPtr != NULL && updateFlags == idoc::IDocument::TitleChanged){
		for (int index = 0; index < documentPtr->GetViewCount(); index++){
			QWidget* widgetPtr = dynamic_cast<QWidget*>(documentPtr->GetView(index));
			if (widgetPtr != NULL){
				widgetPtr->setWindowTitle(iqt::GetQString(documentPtr->GetDocumentTitle()));
			}
		}
	}
}


// reimplemented (iqt:CGuiObjectBase)

void CMultiDocumentWorkspaceGuiComp::OnGuiCreated()
{
	CreateConnections();

	istd::CStringList documentIds = GetDocumentIds();
	
	if (m_documentIdAttrPtr.IsValid()){
		int count = istd::Min<int>(m_documentTemplatesCompPtr.GetCount(), documentIds.size());
		for (int index = 0; index < count; index++){
			RegisterDocumentTemplate(documentIds[index].ToString(), m_documentTemplatesCompPtr[index]);
		}
	}

	if (m_scrollingEnabledAttrPtr.IsValid()){
		QWorkspace* workspacePtr = GetQtWidget();
	
		workspacePtr->setScrollBarsEnabled(m_scrollingEnabledAttrPtr->GetValue());
	}
}


void CMultiDocumentWorkspaceGuiComp::OnGuiDestroyed()
{
	CloseAllDocuments();
}


// private methods

void CMultiDocumentWorkspaceGuiComp::CreateConnections()
{
	connect(GetWidget(), 
		SIGNAL(windowActivated(QWidget*)), 
		this,
		SLOT(OnWindowActivated(QWidget*)));
}


int CMultiDocumentWorkspaceGuiComp::GetDocumentIndex(const imod::IObserver* activeView) const
{
	if (activeView == NULL){
		return -1;
	}

	for (int documentIndex = 0; documentIndex < GetDocumentCount(); documentIndex++){
		idoc::IDocument* documentPtr = GetDocument(documentIndex);
		if (documentPtr != NULL){
			if (documentPtr->HasView(activeView)){
				return documentIndex;
			}
		}
	}

	return -1;
}


QString CMultiDocumentWorkspaceGuiComp::CreateFileDialogFilter(const istd::CString& documentId) const
{
	QString filter;

	DocumentTemplateMap::const_iterator found = m_documentTemplateMap.find(documentId.ToString());
	if (found == m_documentTemplateMap.end()){
		return QString();
	}

	I_ASSERT(found->second != NULL);

	istd::CStringList filters = found->second->GetFileFilters();
	for (int index = 0; index < int(filters.size()); index++){
		filter += iqt::GetQString(filters.at(index));
		if (index < int(filters.size()) - 1){
			filter += " ";
		}
	}

	return filter;
}


// protected slots

void CMultiDocumentWorkspaceGuiComp::OnWindowActivated(QWidget* window)
{
	imod::IObserver* activeView = dynamic_cast<imod::IObserver*>(window);

	m_activeIndex = GetDocumentIndex(activeView);

	istd::CChangeNotifier changePtr(this, DocumentActivationChanged);
}	


} // namespace iqmain

