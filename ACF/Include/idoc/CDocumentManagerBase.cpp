#include "idoc/CDocumentManagerBase.h"


#include "imod/IModel.h"

#include "istd/IChangeable.h"
#include "istd/IDuplicatable.h"
#include "istd/TChangeNotifier.h"

#include "idoc/IDocumentTemplate.h"

namespace idoc
{		


CDocumentManagerBase::CDocumentManagerBase()
{
	m_activeIndex = -1;
	m_untitledIndex = 0;
}


CDocumentManagerBase::~CDocumentManagerBase()
{
}


idoc::IDocument* CDocumentManagerBase::OpenDocument(const istd::CString& fileName)
{
	if (fileName.IsEmpty()){
		return NULL;
	}

	std::string templateId = GetTemplateIdFromFile(fileName);
	if (templateId.empty()){
		return NULL;
	}

	idoc::IDocument* newDocument = OnFileNew(templateId);
	if (newDocument == NULL){
		return NULL;
	}
	
	bool retVal = newDocument->Load(fileName);
	if (retVal){
		newDocument->SetDocumentTitle(fileName);

		istd::CChangeNotifier changePtr(this, DocumentCreated, newDocument);

		changePtr.Reset();
	}
	else{
		RemoveDocument(newDocument);
	
		return NULL;
	}
		
	return newDocument;
}


void CDocumentManagerBase::RegisterDocumentTemplate(const std::string& documentId, idoc::IDocumentTemplate* templatePtr)
{
	m_documentTemplateMap[documentId] = templatePtr;
}


idoc::IDocument* CDocumentManagerBase::GetActiveDocument() const
{
	return GetDocument(m_activeIndex);
}


imod::IObserver* CDocumentManagerBase::GetActiveView() const
{
	return NULL;
}


idoc::IDocument* CDocumentManagerBase::GetDocument(int documentIndex) const
{
	if (documentIndex < 0 || documentIndex > m_documents.GetCount()){
		return NULL;
	}

	return const_cast<idoc::IDocument*>(m_documents.GetAt(documentIndex));
}


int CDocumentManagerBase::GetDocumentCount() const
{
	return m_documents.GetCount();
}


idoc::IDocument* CDocumentManagerBase::OnFileNew(const std::string& documentId)
{
	DocumentTemplateMap::iterator templateIter = m_documentTemplateMap.find(documentId);
	if (templateIter == m_documentTemplateMap.end()){
		return NULL;
	}

	idoc::IDocumentTemplate* documentTemplatePtr = templateIter->second;
	I_ASSERT(documentTemplatePtr != NULL);

	idoc::IDocument* newDocumentPtr = documentTemplatePtr->CreateDocument(documentId);
	if (newDocumentPtr != NULL){
		newDocumentPtr->SetDocumentTitle(documentTemplatePtr->GetDefaultTitle() + istd::CString(" - ") + istd::CString::FromNumber(m_untitledIndex++));
		
		m_documents.PushBack(newDocumentPtr);
	}

	m_activeIndex = m_documents.GetCount() - 1;

	istd::CChangeNotifier changePtr(this, DocumentCountChanged | DocumentCreated);
	changePtr.Reset();

	return newDocumentPtr;
}


bool CDocumentManagerBase::OnFileSave()
{
	idoc::IDocument* activeDocument = GetActiveDocument();

	if (activeDocument != NULL){
		istd::CString fileName = activeDocument->GetFileName();
		if (fileName.IsEmpty()){
			return OnFileSaveAs();
		}

		return activeDocument->Save(fileName);
	}

	return false;
}


bool CDocumentManagerBase::OnFileSaveAs()
{
	idoc::IDocument* activeDocumentPtr = GetActiveDocument();
	if (activeDocumentPtr != NULL){
		istd::CString fileName = GetSaveFileName(activeDocumentPtr->GetDocumentId());
		if (fileName.IsEmpty()){
			return true;
		}

		bool retVal = activeDocumentPtr->Save(fileName);
		if (retVal){
			activeDocumentPtr->SetDocumentTitle(fileName);
		}

		return retVal;
	}

	return false;
}


bool CDocumentManagerBase::OnFileOpen(const std::string& documentId)
{
	bool retVal = true;

	istd::CStringList files = GetOpenFileNames(documentId);
	for (int fileIndex = 0; fileIndex < int(files.size()); fileIndex++){
		retVal = (OpenDocument(files.at(fileIndex)) != NULL) && retVal;
	}

	return retVal;
}


bool CDocumentManagerBase::OnFileClose()
{
	bool retVal = true;

	idoc::IDocument* activeDocumentPtr = GetActiveDocument();
	if (activeDocumentPtr != NULL){
		imod::IObserver* viewPtr = GetActiveView();
		activeDocumentPtr->RemoveView(viewPtr);

		if (activeDocumentPtr->GetViewCount() == 0){
			RemoveDocument(activeDocumentPtr);
		}
	}

	return retVal;
}


istd::CStringList CDocumentManagerBase::GetDocumentIds() const
{
	return istd::CStringList();
}


// protected methods

std::string CDocumentManagerBase::GetTemplateIdFromFile(const istd::CString& fileName) const
{
	static std::string emptyId;

	int pointPos = fileName.rfind('.');

	istd::CString fileExtension = fileName.substr(pointPos + 1);
	istd::CString fileExtensionUpper = fileExtension.ToUpper();

	DocumentTemplateMap::const_iterator iter = m_documentTemplateMap.begin();
	for (;iter != m_documentTemplateMap.end(); iter++){
		idoc::IDocumentTemplate* documentTemplatePtr = iter->second;
		istd::CStringList extensions = documentTemplatePtr->GetFileExtensions();
		
		istd::CStringList::iterator found =  std::find(extensions.begin(), extensions.end(), fileExtension);
		istd::CStringList::iterator found1 =  std::find(extensions.begin(), extensions.end(), fileExtensionUpper);
		if (found != extensions.end() || found1 != extensions.end()){
			return iter->first;
		}
	}

	return emptyId;
}


bool CDocumentManagerBase::CloseDocument(int index)
{
	if (index < 0 || index >= m_documents.GetCount()){
		return false;
	}

	m_documents.RemoveAt(index);
	
	istd::TChangeNotifier<CDocumentManagerBase> notifier(this, DocumentCountChanged | DocumentRemoved);

	notifier.Reset();

	return true;
}


void CDocumentManagerBase::CloseAllDocuments()
{
	bool isDone = false;

	// close all documents
	while (GetDocumentCount() && !isDone){
		isDone = false;
		bool retVal = CloseDocument(0);
		if (!retVal){
			isDone = true;
		}
	}
}


void CDocumentManagerBase::RemoveDocument(idoc::IDocument* document)
{
	for(int documentIndex = 0; documentIndex < m_documents.GetCount(); documentIndex++){
		if (m_documents.GetAt(documentIndex) == document){
			CloseDocument(documentIndex);
			break;
		}
	}
}


istd::CString CDocumentManagerBase::CalculateCopyName(const istd::CString& documentTitle) const
{
	bool foundOccurence = false;

	istd::CString docTitle = documentTitle;

	for (int documentIndex = 0; documentIndex < GetDocumentCount(); documentIndex++){
		istd::CString currentDocTitle = GetDocument(documentIndex)->GetDocumentTitle();

		if (docTitle == currentDocTitle){
			docTitle += istd::CString("*");
			foundOccurence = true;
			
			CalculateCopyName(docTitle);
		}
	}

	return docTitle;
}


} // namespace idoc

