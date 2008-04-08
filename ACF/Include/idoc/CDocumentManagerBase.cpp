#include "idoc/CDocumentManagerBase.h"


#include <algorithm>

#include "istd/TChangeNotifier.h"

#include "idoc/IDocumentTemplate.h"


namespace idoc
{		


CDocumentManagerBase::CDocumentManagerBase()
:	m_activeViewPtr(NULL), m_documentTemplatePtr(NULL)
{
}


CDocumentManagerBase::~CDocumentManagerBase()
{
}


imod::IModel* CDocumentManagerBase::OpenDocument(const istd::CString& filePath, bool createView, const std::string& viewTypeId)
{
	if (filePath.IsEmpty() || (m_documentTemplatePtr == NULL)){
		return NULL;
	}

	IDocumentTemplate::Ids documentIds = m_documentTemplatePtr->GetDocumentTypeIdsForFile(filePath);

	if (!documentIds.empty()){
		const std::string& documentTypeId = documentIds.front();
		istd::TDelPtr<DocumentInfo> infoPtr(CreateDocument(documentTypeId, createView, viewTypeId));
		if (infoPtr.IsValid()){
			I_ASSERT(infoPtr->documentPtr.IsValid());

			istd::CChangeNotifier documentPtr(dynamic_cast<istd::IChangeable*>(infoPtr->documentPtr.GetPtr()));

			infoPtr->filePath = filePath;
			infoPtr->documentTypeId = documentTypeId;
			if (	m_documentTemplatePtr->LoadDocumentFromFile(filePath, *infoPtr->documentPtr) &&
					RegisterDocument(infoPtr.GetPtr())){
				return infoPtr.PopPtr()->documentPtr.GetPtr();
			}
		}
	}

	return NULL;
}


void CDocumentManagerBase::SetDocumentTemplate(const IDocumentTemplate* documentTemplatePtr)
{
	m_documentTemplatePtr = documentTemplatePtr;
}


// reimplemented (idoc::IDocumentManager)

const idoc::IDocumentTemplate* CDocumentManagerBase::GetDocumentTemplate() const
{
	return m_documentTemplatePtr;
}


int CDocumentManagerBase::GetDocumentsCount() const
{
	return m_documentInfos.GetCount();
}


imod::IModel& CDocumentManagerBase::GetDocumentFromIndex(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < m_documentInfos.GetCount());
	I_ASSERT(m_documentInfos.GetAt(index) != NULL);
	I_ASSERT(m_documentInfos.GetAt(index)->documentPtr.IsValid());

	return *(m_documentInfos.GetAt(index)->documentPtr);
}


istd::IPolymorphic* CDocumentManagerBase::GetActiveView() const
{
	return m_activeViewPtr;
}


void CDocumentManagerBase::SetActiveView(istd::IPolymorphic* viewPtr)
{
	m_activeViewPtr = viewPtr;
}


imod::IModel* CDocumentManagerBase::GetDocumentFromView(const istd::IPolymorphic& view) const
{
	const DocumentInfo* infoPtr = GetViewDocumentInfo(view);
	if (infoPtr != NULL){
		I_ASSERT(infoPtr != NULL);
		I_ASSERT(infoPtr->documentPtr.IsValid());

		return infoPtr->documentPtr.GetPtr();
	}

	return NULL;
}


imod::IModel* CDocumentManagerBase::FileNew(const std::string& documentTypeId, bool createView, const std::string& viewTypeId)
{
	istd::TDelPtr<DocumentInfo> newInfoPtr(CreateDocument(documentTypeId, createView, viewTypeId));
	if (newInfoPtr.IsValid() && RegisterDocument(newInfoPtr.GetPtr())){
		return newInfoPtr.PopPtr()->documentPtr.GetPtr();
	}

	return NULL;
}


bool CDocumentManagerBase::FileOpen(const std::string* documentTypeIdPtr, bool createView, const std::string& viewTypeId)
{
	bool retVal = true;

	istd::CStringList files = GetOpenFileNames(documentTypeIdPtr);
	for (int fileIndex = 0; fileIndex < int(files.size()); fileIndex++){
		retVal = (OpenDocument(files.at(fileIndex), createView, viewTypeId) != NULL) && retVal;
	}

	return retVal;
}


bool CDocumentManagerBase::FileSave(bool requestFileName)
{
	if (m_documentTemplatePtr == NULL){
		return false;
	}

	DocumentInfo* infoPtr = GetActiveDocumentInfo();
	if (infoPtr == NULL){
		return false;
	}

	I_ASSERT(infoPtr->documentPtr.IsValid());

	istd::CString filePath = infoPtr->filePath;

	if (requestFileName || filePath.IsEmpty()){
		filePath = GetSaveFileName(infoPtr->documentTypeId);
		if (filePath.IsEmpty()){
			return true;
		}
	}

	if (m_documentTemplatePtr->SaveDocumentToFile(*infoPtr->documentPtr, filePath)){
		if (infoPtr->filePath != filePath){
			istd::CChangeNotifier notifierPtr(this);

			infoPtr->filePath = filePath;
		}

		return true;
	}

	return false;
}


bool CDocumentManagerBase::FileClose()
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		DocumentInfo& info = GetDocumentInfo(i);

		Views::iterator findIter = std::find(info.views.begin(), info.views.end(), m_activeViewPtr);
		if (findIter != info.views.end()){
			istd::CChangeNotifier notifier(this, DocumentRemoved);

			info.views.erase(findIter);	// remove active view

			m_activeViewPtr = NULL;

			if (info.views.empty()){	// last view was removed
				m_documentInfos.RemoveAt(i);
			}

			return true;
		}
	}

	return false;
}


// protected methods

bool CDocumentManagerBase::CloseDocument(int index)
{
	if ((index < 0) || (index >= m_documentInfos.GetCount())){
		return false;
	}

	istd::CChangeNotifier notifierPtr(this, DocumentCountChanged | DocumentRemoved);

	m_documentInfos.RemoveAt(index);
	
	return true;
}


void CDocumentManagerBase::CloseAllDocuments()
{
	istd::CChangeNotifier notifierPtr(this, DocumentCountChanged | DocumentRemoved);

	m_documentInfos.Reset();
}


void CDocumentManagerBase::RemoveDocument(imod::IModel* documentPtr)
{
	int documentsCount = GetDocumentsCount();
	for (int documentIndex = 0; documentIndex < documentsCount; documentIndex++){
		const DocumentInfo& info = GetDocumentInfo(documentIndex);
		I_ASSERT(info.documentPtr.IsValid());

		if (info.documentPtr.GetPtr() == documentPtr){
			CloseDocument(documentIndex);

			break;
		}
	}
}


CDocumentManagerBase::DocumentInfo& CDocumentManagerBase::GetDocumentInfo(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetDocumentsCount());

	DocumentInfo* retVal = const_cast<DocumentInfo*>(m_documentInfos.GetAt(index));
	I_ASSERT(retVal != NULL);

	return *retVal;
}


CDocumentManagerBase::DocumentInfo* CDocumentManagerBase::GetActiveDocumentInfo() const
{
	const istd::IPolymorphic* viewPtr = GetActiveView();
	if (viewPtr != NULL){
		return GetViewDocumentInfo(*viewPtr);
	}

	return NULL;
}


CDocumentManagerBase::DocumentInfo* CDocumentManagerBase::GetViewDocumentInfo(const istd::IPolymorphic& view) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		DocumentInfo& info = GetDocumentInfo(i);

		Views::iterator findIter = std::find(info.views.begin(), info.views.end(), &view);
		if (findIter != info.views.end()){
			return &info;
		}
	}

	return NULL;
}


CDocumentManagerBase::DocumentInfo* CDocumentManagerBase::CreateDocument(const std::string& documentTypeId, bool createView, const std::string& viewTypeId) const
{
	if (m_documentTemplatePtr != NULL){
		istd::TDelPtr<DocumentInfo> infoPtr(new DocumentInfo);

		infoPtr->documentPtr.SetPtr(m_documentTemplatePtr->CreateDocument(documentTypeId));

		if (infoPtr->documentPtr.IsValid()){
			if (createView){
				istd::IPolymorphic* viewPtr = m_documentTemplatePtr->CreateView(infoPtr->documentPtr.GetPtr(), viewTypeId);
				if (viewPtr == NULL){
					return NULL;
				}

				infoPtr->views.push_back(ViewPtr());
				infoPtr->views.back().SetPtr(viewPtr);
			}

			return infoPtr.PopPtr();
		}
	}

	return NULL;
}


bool CDocumentManagerBase::RegisterDocument(DocumentInfo* infoPtr)
{
	I_ASSERT(infoPtr != NULL);

	istd::CChangeNotifier changePtr(this, DocumentCountChanged | DocumentCreated);

	m_documentInfos.PushBack(infoPtr);

	for (		Views::iterator iter = infoPtr->views.begin();
				iter != infoPtr->views.end();
				++iter){
		I_ASSERT(iter->IsValid());

		OnViewRegistered(iter->GetPtr());
	}

	return true;
}


} // namespace idoc

