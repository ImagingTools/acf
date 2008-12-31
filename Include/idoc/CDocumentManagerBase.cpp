#include "idoc/CDocumentManagerBase.h"


// STL includes
#include <algorithm>

#include "istd/TChangeNotifier.h"

#include "imod/IModelEditor.h"

#include "iser/CArchiveTag.h"

#include "idoc/IDocumentTemplate.h"


namespace idoc
{		


CDocumentManagerBase::CDocumentManagerBase()
:	m_documentTemplatePtr(NULL), m_activeViewPtr(NULL)
{
}


istd::IChangeable* CDocumentManagerBase::OpenDocument(const istd::CString& filePath, bool createView, const std::string& viewTypeId)
{
	if (filePath.IsEmpty() || (m_documentTemplatePtr == NULL)){
		return NULL;
	}

	DocumentInfo* existingInfoPtr = GetDocumentInfoFromPath(filePath);
	if (existingInfoPtr != NULL){
		I_ASSERT(existingInfoPtr->documentPtr.IsValid());

		if (createView){
			istd::IPolymorphic* viewPtr = m_documentTemplatePtr->CreateView(
						existingInfoPtr->documentTypeId,
						existingInfoPtr->documentPtr.GetPtr(),
						viewTypeId);
			if (viewPtr != NULL){
				existingInfoPtr->views.push_back(ViewPtr());
				existingInfoPtr->views.back().SetPtr(viewPtr);

				OnViewRegistered(viewPtr);
			}
		}

		return existingInfoPtr->documentPtr.GetPtr();
	}

	IDocumentTemplate::Ids documentIds = m_documentTemplatePtr->GetDocumentTypeIdsForFile(filePath);

	if (!documentIds.empty()){
		const std::string& documentTypeId = documentIds.front();
		istd::TDelPtr<DocumentInfo> infoPtr(CreateDocument(documentTypeId, createView, viewTypeId));
		if (infoPtr.IsValid()){
			I_ASSERT(infoPtr->documentPtr.IsValid());

			infoPtr->filePath = filePath;
			infoPtr->documentTypeId = documentTypeId;
			iser::IFileLoader* loaderPtr = m_documentTemplatePtr->GetFileLoader(documentTypeId);
			if (loaderPtr != NULL){
				bool isLoaded = (loaderPtr->LoadFromFile(*infoPtr->documentPtr, filePath) == iser::IFileLoader::StateOk);
				UpdateRecentFileList(filePath, documentTypeId, isLoaded);
				
				if (isLoaded){
					RegisterDocument(infoPtr.GetPtr());

					return infoPtr.PopPtr()->documentPtr.GetPtr();
				}
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


imod::IUndoManager* CDocumentManagerBase::GetUndoManagerForDocument(const istd::IChangeable* documentPtr) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		const DocumentInfo& info = GetDocumentInfo(i);
		
		if(info.documentPtr == documentPtr){
			return info.undoManagerPtr.GetPtr();
		}
	}

	return NULL;
}


int CDocumentManagerBase::GetDocumentsCount() const
{
	return m_documentInfos.GetCount();
}


istd::IChangeable& CDocumentManagerBase::GetDocumentFromIndex(int index) const
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
	if (m_activeViewPtr != viewPtr){
		istd::CChangeNotifier changePtr(this, ViewActivationChanged);

		m_activeViewPtr = viewPtr;
	}
}


istd::IChangeable* CDocumentManagerBase::GetDocumentFromView(const istd::IPolymorphic& view) const
{
	const DocumentInfo* infoPtr = GetDocumentInfoFromView(view);
	if (infoPtr != NULL){
		I_ASSERT(infoPtr != NULL);
		I_ASSERT(infoPtr->documentPtr.IsValid());

		return infoPtr->documentPtr.GetPtr();
	}

	return NULL;
}


std::string CDocumentManagerBase::GetDocumentTypeId(const istd::IChangeable& document) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		DocumentInfo& info = GetDocumentInfo(i);

		if (info.documentPtr == &document){
			return info.documentTypeId;
		}
	}

	return std::string();
}


istd::IChangeable* CDocumentManagerBase::FileNew(const std::string& documentTypeId, bool createView, const std::string& viewTypeId)
{
	istd::TDelPtr<DocumentInfo> newInfoPtr(CreateDocument(documentTypeId, createView, viewTypeId));
	if (newInfoPtr.IsValid() && RegisterDocument(newInfoPtr.GetPtr())){
		return newInfoPtr.PopPtr()->documentPtr.GetPtr();
	}

	return NULL;
}


bool CDocumentManagerBase::FileOpen(const std::string* documentTypeIdPtr, const istd::CString* fileNamePtr, bool createView, const std::string& viewTypeId)
{
	bool retVal = true;

	if (fileNamePtr == NULL){
		istd::CStringList files = GetOpenFileNames(documentTypeIdPtr);
		for (int fileIndex = 0; fileIndex < int(files.size()); fileIndex++){
			retVal = (OpenDocument(files.at(fileIndex), createView, viewTypeId) != NULL) && retVal;
		}
	}
	else{
		retVal = (OpenDocument(*fileNamePtr, createView, viewTypeId) != NULL);
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

	requestFileName  = requestFileName || filePath.IsEmpty();

	if (requestFileName){
		filePath = GetSaveFileName(infoPtr->documentTypeId);
		if (filePath.IsEmpty()){
			return true;
		}
	}

	for (		Views::const_iterator iter = infoPtr->views.begin();
				iter != infoPtr->views.end();
				++iter){
		const imod::IModelEditor* editorPtr = dynamic_cast<const imod::IModelEditor*>(iter->GetPtr());
		if ((editorPtr != NULL) && (!editorPtr->IsReadOnly())){
			editorPtr->UpdateModel();
		}
	}

	const iser::IFileLoader* loaderPtr = m_documentTemplatePtr->GetFileLoader(infoPtr->documentTypeId);
	if ((loaderPtr != NULL) && loaderPtr->SaveToFile(*infoPtr->documentPtr, filePath) == iser::IFileLoader::StateOk){
		if ((infoPtr->filePath != filePath) || infoPtr->isDirty){
			istd::CChangeNotifier notifierPtr(this);

			infoPtr->filePath = filePath;
			infoPtr->isDirty = false;
		}

		if (requestFileName){
			UpdateRecentFileList(infoPtr->filePath, infoPtr->documentTypeId, true);
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
			if (info.isDirty){
				if (!QueryDocumentClose(info)){
					return false;
				}
			}

			I_ASSERT(findIter->IsValid());
			OnViewRemoved(findIter->GetPtr());

			info.views.erase(findIter);	// remove active view

			m_activeViewPtr = NULL;

			if (info.views.empty()){	// last view was removed
				int changeFlags = DocumentRemoved | DocumentCountChanged;
				// if last document was closed, force view activation update:
				if (m_documentInfos.GetCount() == 1){
					changeFlags |= ViewActivationChanged;
				}

				istd::CChangeNotifier notifier(this, changeFlags);

				m_documentInfos.RemoveAt(i);
			}

			return true;
		}
	}

	return false;
}


istd::CStringList CDocumentManagerBase::GetRecentFileList(const std::string& documentTypeId) const
{
	static istd::CStringList emptyRecentFileList;

	RecentFilesMap::const_iterator recentFileIter = m_recentFilesMap.find(documentTypeId);
	if (recentFileIter != m_recentFilesMap.end()){
		istd::CStringList recentFileList;

		for (		FileList::const_iterator index = recentFileIter->second.begin();
					index != recentFileIter->second.end();
					index++){
			recentFileList.push_back(*index);
		}
		return recentFileList;
	}

	return emptyRecentFileList;
}


// protected methods

void CDocumentManagerBase::CloseAllDocuments()
{
	istd::CChangeNotifier notifierPtr(this, DocumentCountChanged | DocumentRemoved);

	m_documentInfos.Reset();
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
		return GetDocumentInfoFromView(*viewPtr);
	}

	return NULL;
}


CDocumentManagerBase::DocumentInfo* CDocumentManagerBase::GetDocumentInfoFromView(const istd::IPolymorphic& view) const
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


CDocumentManagerBase::DocumentInfo* CDocumentManagerBase::GetDocumentInfoFromPath(const istd::CString& filePath) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		DocumentInfo& info = GetDocumentInfo(i);

		if (info.filePath == filePath){
			return &info;
		}
	}

	return NULL;
}


CDocumentManagerBase::DocumentInfo* CDocumentManagerBase::CreateDocument(const std::string& documentTypeId, bool createView, const std::string& viewTypeId) const
{
	if (m_documentTemplatePtr != NULL){
		istd::IChangeable* documentPtr = m_documentTemplatePtr->CreateDocument(documentTypeId);

		istd::TDelPtr<DocumentInfo> infoPtr(new DocumentInfo(
					const_cast<CDocumentManagerBase*>(this),
					documentTypeId,
					documentPtr,
					m_documentTemplatePtr->CreateUndoManager(documentTypeId, documentPtr)));

		if (infoPtr->documentPtr.IsValid()){
			imod::IModel* documentModelPtr = dynamic_cast<imod::IModel*>(documentPtr);
			if (documentModelPtr != NULL){
				documentModelPtr->AttachObserver(infoPtr.GetPtr());
			}

			if (createView){
				istd::IPolymorphic* viewPtr = m_documentTemplatePtr->CreateView(
							documentTypeId,
							infoPtr->documentPtr.GetPtr(),
							viewTypeId);
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

	infoPtr->isDirty = false;

	m_documentInfos.PushBack(infoPtr);

	for (		Views::iterator iter = infoPtr->views.begin();
				iter != infoPtr->views.end();
				++iter){
		I_ASSERT(iter->IsValid());

		OnViewRegistered(iter->GetPtr());
	}

	return true;
}


bool CDocumentManagerBase::SerializeRecentFileList(iser::IArchive& archive)
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
			I_ASSERT(!documentTypeId.empty())

			retVal = retVal && archive.BeginTag(documentTypeIdTag);
			retVal = retVal && archive.Process(documentTypeId);
			retVal = retVal && archive.EndTag(documentTypeIdTag);

			const FileList& recentFileList = index->second;

			int filesCount = recentFileList.size();
			retVal = retVal && archive.BeginMultiTag(fileListTag, filePathTag, filesCount);

			for (		FileList::const_iterator fileIndex = recentFileList.begin();
						fileIndex != recentFileList.end();
						fileIndex++){
				istd::CString filePath = *fileIndex;

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
			I_ASSERT(!documentTypeId.empty())

			FileList& recentFileList = m_recentFilesMap[documentTypeId];

			int filesCount = 0;
			retVal = retVal && archive.BeginMultiTag(fileListTag, filePathTag, filesCount);

			for (int fileIndex = 0; fileIndex < filesCount; fileIndex++){
				istd::CString filePath;

				retVal = retVal && archive.BeginTag(filePathTag);
				retVal = retVal && archive.Process(filePath);					
				retVal = retVal && archive.EndTag(filePathTag);

				if (retVal){
					recentFileList.push_back(filePath);
				}
			}

			retVal = retVal && archive.EndTag(fileListTag);
		}

		istd::CChangeNotifier changeNotifier(this, RecentFileListChanged);
	}

	retVal = retVal && archive.EndTag(documentTypeIdsTag);

	retVal = retVal && archive.EndTag(recentFilesTag);

	return retVal;
}

	
int CDocumentManagerBase::GetMaxRecentFilesCount() const
{
	return 10;
}


void CDocumentManagerBase::OnViewRegistered(istd::IPolymorphic* /*viewPtr*/)
{
}


void CDocumentManagerBase::OnViewRemoved(istd::IPolymorphic* /*viewPtr*/)
{
}


// private methods

void CDocumentManagerBase::UpdateRecentFileList(const istd::CString& requestedFilePath, const std::string& documentTypeId, bool wasSuccess)
{
	if (wasSuccess){
		istd::CChangeNotifier changeNotifier(this, RecentFileListChanged);

		FileList& recentFileList = m_recentFilesMap[documentTypeId];

		FileList::iterator foundFileIter = std::find(recentFileList.begin(), recentFileList.end(), requestedFilePath);

		// move current file item to the top of the list:
		if (foundFileIter != recentFileList.end()){
			recentFileList.erase(foundFileIter);
		}

		recentFileList.push_front(requestedFilePath);

		if (int(recentFileList.size()) > GetMaxRecentFilesCount()){
			recentFileList.pop_back();
		}
	}
	else{
		RecentFilesMap::iterator recentFileListIter = m_recentFilesMap.find(documentTypeId);
		if (recentFileListIter != m_recentFilesMap.end()){
			FileList& fileList = recentFileListIter->second;
			FileList::iterator recentFileIter = std::find(fileList.begin(), fileList.end(), requestedFilePath);
			if (recentFileIter != fileList.end()){
				istd::CChangeNotifier changeNotifier(this, RecentFileListChanged);
		
				fileList.erase(recentFileIter);
			}
		}
	}
}


// protected methods of embedded class DocumentInfo

// reimplemented (imod::CSingleModelObserverBase)

void CDocumentManagerBase::DocumentInfo::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (!isDirty){
		istd::CChangeNotifier notifier(parentPtr);

		isDirty = true;
	}
}


} // namespace idoc


