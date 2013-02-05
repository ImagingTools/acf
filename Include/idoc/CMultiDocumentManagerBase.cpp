#include "idoc/CMultiDocumentManagerBase.h"


// Qt includes
#include <QtCore/QStringList>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"
#include "imod/IModelEditor.h"

#include "icomp/CComponentBase.h"

#include "idoc/IDocumentTemplate.h"


namespace idoc
{


CMultiDocumentManagerBase::CMultiDocumentManagerBase()
	:m_activeViewPtr(NULL)
{
}


// reimplemented (idoc::IDocumentManager)

idoc::IUndoManager* CMultiDocumentManagerBase::GetUndoManagerForDocument(const istd::IChangeable* documentPtr) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		const SingleDocumentData& info = GetSingleDocumentData(i);
		
		if (info.documentPtr == documentPtr){
			return info.undoManagerPtr.GetPtr();
		}
	}

	return NULL;
}


int CMultiDocumentManagerBase::GetDocumentsCount() const
{
	return m_documentInfos.GetCount();
}


istd::IChangeable& CMultiDocumentManagerBase::GetDocumentFromIndex(int index, DocumentInfo* documentInfoPtr) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < m_documentInfos.GetCount());

	SingleDocumentData* infoPtr = m_documentInfos.GetAt(index);
	Q_ASSERT(infoPtr != NULL);
	Q_ASSERT(infoPtr->documentPtr.IsValid());

	if (documentInfoPtr != NULL){
		*documentInfoPtr = *infoPtr;
	}

	return *(infoPtr->documentPtr);
}


int CMultiDocumentManagerBase::GetViewsCount(int documentIndex) const
{
	Q_ASSERT(documentIndex >= 0);
	Q_ASSERT(documentIndex < m_documentInfos.GetCount());

	SingleDocumentData* infoPtr = m_documentInfos.GetAt(documentIndex);
	Q_ASSERT(infoPtr != NULL);

	return int(infoPtr->views.size());
}


istd::IPolymorphic* CMultiDocumentManagerBase::GetViewFromIndex(int documentIndex, int viewIndex) const
{
	Q_ASSERT(documentIndex >= 0);
	Q_ASSERT(documentIndex < m_documentInfos.GetCount());
	Q_ASSERT(viewIndex < GetViewsCount(documentIndex));

	SingleDocumentData* infoPtr = m_documentInfos.GetAt(documentIndex);
	Q_ASSERT(infoPtr != NULL);

	return infoPtr->views.at(viewIndex).GetPtr();
}


istd::IPolymorphic* CMultiDocumentManagerBase::GetActiveView() const
{
	return m_activeViewPtr;
}


istd::IChangeable* CMultiDocumentManagerBase::GetDocumentFromView(const istd::IPolymorphic& view, DocumentInfo* documentInfoPtr) const
{
	const SingleDocumentData* infoPtr = GetDocumentInfoFromView(view);
	if (infoPtr != NULL){
		Q_ASSERT(infoPtr != NULL);
		Q_ASSERT(infoPtr->documentPtr.IsValid());

		if (documentInfoPtr != NULL){
			*documentInfoPtr = *infoPtr;
		}

		return infoPtr->documentPtr.GetPtr();
	}

	return NULL;
}


istd::IPolymorphic* CMultiDocumentManagerBase::AddViewToDocument(const istd::IChangeable& document, const QByteArray& viewTypeId)
{
	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();
	if (documentTemplatePtr == NULL){
		return NULL;
	}

	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		SingleDocumentData& info = GetSingleDocumentData(i);
		
		if (info.documentPtr == &document){
			istd::IPolymorphic* viewPtr = documentTemplatePtr->CreateView(
				info.documentTypeId,
				info.documentPtr.GetPtr(),
				viewTypeId);
			if (viewPtr == NULL){
				return NULL;
			}

			info.views.push_back(ViewPtr());
			info.views.back().SetPtr(viewPtr);
			info.viewTypeIds.push_back(viewTypeId);

			OnViewRegistered(viewPtr);

			return viewPtr;
		}
	}

	return NULL;
}



QByteArray CMultiDocumentManagerBase::GetDocumentTypeId(const istd::IChangeable& document) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		SingleDocumentData& info = GetSingleDocumentData(i);

		if (info.documentPtr == &document){
			return info.documentTypeId;
		}
	}

	return QByteArray();
}


bool CMultiDocumentManagerBase::FileNew(
			const QByteArray& documentTypeId, 
			bool createView, 
			const QByteArray& viewTypeId,
			istd::IChangeable** newDocumentPtr)
{
	istd::TDelPtr<SingleDocumentData> newInfoPtr(CreateDocument(documentTypeId, createView, viewTypeId));
	if (newInfoPtr.IsValid() && RegisterDocument(newInfoPtr.PopPtr())){
		SingleDocumentData* newDocumentDataPtr = m_documentInfos.GetAt(m_documentInfos.GetCount() - 1);
		Q_ASSERT(newDocumentDataPtr != NULL);

		newDocumentDataPtr->isDirty = false;
		if (newDocumentPtr != NULL){
			*newDocumentPtr = newDocumentDataPtr->documentPtr.GetPtr();
		}

		return true;
	}

	return false;
}


bool CMultiDocumentManagerBase::FileOpen(
			const QByteArray* documentTypeIdPtr,
			const QString* fileNamePtr,
			bool createView,
			const QByteArray& viewTypeId,
			istd::IChangeable** documentPtr,
			FileToTypeMap* loadedMapPtr)
{
	bool retVal = true;

	QStringList files;

	if (fileNamePtr != NULL){
		files.push_back(*fileNamePtr);
	}
	else{
		files = GetOpenFilePaths(documentTypeIdPtr);
	}

	for (		QStringList::const_iterator iter = files.begin();
				iter != files.end();
				++iter){
		const QString& fileName = *iter;

		QByteArray documentTypeId;
		istd::IChangeable* openDocumentPtr = OpenDocument(fileName, createView, viewTypeId, documentTypeId);
		if (openDocumentPtr != NULL){
			if (loadedMapPtr != NULL){
				loadedMapPtr->operator[](fileName) = documentTypeId;
			}
		}
		else{
			retVal = false;
		}

		if (documentPtr != NULL){
			*documentPtr = openDocumentPtr;
		}
	}

	return retVal;
}


bool CMultiDocumentManagerBase::FileSave(
			int documentIndex,
			bool requestFileName,
			FileToTypeMap* savedMapPtr)
{
	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();
	if (documentTemplatePtr == NULL){
		return false;
	}

	SingleDocumentData* infoPtr = NULL;
	
	if (documentIndex >= 0){
		Q_ASSERT(documentIndex < GetDocumentsCount());

		infoPtr = &GetSingleDocumentData(documentIndex);
	}
	else{
		infoPtr = GetActiveDocumentInfo();
	}

	if (infoPtr == NULL){
		return false;
	}

	Q_ASSERT(infoPtr->documentPtr.IsValid());

	QString filePath = infoPtr->filePath;

	requestFileName  = requestFileName || filePath.isEmpty();

	if (requestFileName){
		filePath = GetSaveFilePath(infoPtr->documentTypeId);
		if (filePath.isEmpty()){
			return true;
		}
	}

	for (		Views::const_iterator iter = infoPtr->views.begin();
				iter != infoPtr->views.end();
				++iter){
		const imod::IModelEditor* editorPtr = CompCastPtr<imod::IModelEditor>(iter->GetPtr());
		if ((editorPtr != NULL) && (!editorPtr->IsReadOnly())){
			editorPtr->UpdateModel();
		}
	}

	const iser::IFileLoader* loaderPtr = documentTemplatePtr->GetFileLoader(infoPtr->documentTypeId);
	if ((loaderPtr != NULL) && loaderPtr->SaveToFile(*infoPtr->documentPtr, filePath) == iser::IFileLoader::StateOk){
		if ((infoPtr->filePath != filePath) || infoPtr->isDirty){
			istd::CChangeNotifier notifierPtr(this);

			infoPtr->filePath = filePath;
			infoPtr->isDirty = false;

			if (infoPtr->undoManagerPtr.IsValid()){
				infoPtr->undoManagerPtr->StoreDocumentState();
			}
		}

		if (savedMapPtr != NULL){
			savedMapPtr->operator[](filePath) = infoPtr->documentTypeId;
		}

		return true;
	}

	return false;
}


void CMultiDocumentManagerBase::FileClose(int documentIndex, bool* ignoredPtr)
{
	int documentsCount = m_documentInfos.GetCount();

	if (documentIndex >= 0){
		SingleDocumentData* infoPtr = m_documentInfos.GetAt(documentIndex);
		Q_ASSERT(infoPtr != NULL);

		while (!infoPtr->views.empty()){
			for (Views::iterator iter = infoPtr->views.begin(); iter != infoPtr->views.end(); ++iter){
				if (infoPtr->isDirty){
					QueryDocumentClose(*infoPtr, ignoredPtr);
					if ((ignoredPtr != NULL) && *ignoredPtr){
						break;
					}
				}

				Q_ASSERT(iter->IsValid());

				istd::IPolymorphic* viewPtr = iter->GetPtr();
				Q_ASSERT(viewPtr != NULL);

				OnViewRemoved(viewPtr);

				infoPtr->viewTypeIds.removeAt(iter - infoPtr->views.begin());
				infoPtr->views.erase(iter);

				if (viewPtr == m_activeViewPtr){
					m_activeViewPtr = NULL;
				}
				break;
			}
		}

		int changeFlags = CF_MODEL | CF_DOCUMENT_REMOVED | CF_DOCUMENT_COUNT_CHANGED;
		
		// If last document was closed, force view activation update:
		if (m_documentInfos.GetCount() == 1){
			changeFlags |= CF_VIEW_ACTIVATION_CHANGED;
		}

		istd::CChangeNotifier notifier(this, changeFlags);

		m_documentInfos.RemoveAt(documentIndex);
	}
	else{
		for (int i = 0; i < documentsCount; ++i){
			SingleDocumentData* infoPtr = m_documentInfos.GetAt(i);
			Q_ASSERT(infoPtr != NULL);

			Views::iterator findIter = qFind(infoPtr->views.begin(), infoPtr->views.end(), m_activeViewPtr);
			if (findIter != infoPtr->views.end()){
				if (infoPtr->isDirty){
					QueryDocumentClose(*infoPtr, ignoredPtr);
					if ((ignoredPtr != NULL) && *ignoredPtr){
						break;
					}
				}

				Q_ASSERT(findIter->IsValid());
				OnViewRemoved(findIter->GetPtr());
				
				infoPtr->viewTypeIds.removeAt(findIter - infoPtr->views.begin());
				infoPtr->views.erase(findIter);	// remove active view

				m_activeViewPtr = NULL;

				if (infoPtr->views.isEmpty()){	// last view was removed
					int changeFlags = CF_DOCUMENT_REMOVED | CF_DOCUMENT_COUNT_CHANGED;
					// if last document was closed, force view activation update:
					if (m_documentInfos.GetCount() == 1){
						changeFlags |= CF_VIEW_ACTIVATION_CHANGED;
					}

					istd::CChangeNotifier notifier(this, changeFlags);

					m_documentInfos.RemoveAt(i);

					break;
				}
			}
		}
	}
}


// protected methods

void CMultiDocumentManagerBase::SetActiveView(istd::IPolymorphic* viewPtr)
{
	if (m_activeViewPtr != viewPtr){
		istd::CChangeNotifier changePtr(this, CF_VIEW_ACTIVATION_CHANGED | CF_MODEL);

		m_activeViewPtr = viewPtr;
	}
}


istd::IChangeable* CMultiDocumentManagerBase::OpenDocument(
			const QString& filePath,
			bool createView,
			const QByteArray& viewTypeId,
			QByteArray& documentTypeId)
{
	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();
	if (filePath.isEmpty() || (documentTemplatePtr == NULL)){
		return NULL;
	}

	SingleDocumentData* existingInfoPtr = GetDocumentInfoFromPath(filePath);
	if (existingInfoPtr != NULL){
		Q_ASSERT(existingInfoPtr->documentPtr.IsValid());

		if (createView){
			istd::IPolymorphic* viewPtr = documentTemplatePtr->CreateView(
						existingInfoPtr->documentTypeId,
						existingInfoPtr->documentPtr.GetPtr(),
						viewTypeId);
			if (viewPtr != NULL){
				existingInfoPtr->views.push_back(ViewPtr());
				existingInfoPtr->views.back().SetPtr(viewPtr);
				existingInfoPtr->viewTypeIds.push_back(viewTypeId);

				OnViewRegistered(viewPtr);
			}
		}
		else{
			if (!existingInfoPtr->views.isEmpty()){
				istd::IPolymorphic* viewPtr = existingInfoPtr->views.front().GetPtr();
				Q_ASSERT(viewPtr != NULL);

				SetActiveView(viewPtr);
			}
		}

		documentTypeId = existingInfoPtr->documentTypeId;

		return existingInfoPtr->documentPtr.GetPtr();
	}

	IDocumentTemplate::Ids documentIds = documentTemplatePtr->GetDocumentTypeIdsForFile(filePath);

	if (!documentIds.isEmpty()){
		istd::CChangeNotifier changePtr(this, CF_DOCUMENT_COUNT_CHANGED | CF_DOCUMENT_CREATED | CF_MODEL);

		documentTypeId = documentIds.front();
		istd::TDelPtr<SingleDocumentData> infoPtr(CreateDocument(documentTypeId, createView, viewTypeId));
		if (infoPtr.IsValid()){
			Q_ASSERT(infoPtr->documentPtr.IsValid());

			infoPtr->filePath = filePath;
			infoPtr->documentTypeId = documentTypeId;

			istd::CChangeNotifier documentNotifier(infoPtr->documentPtr.GetPtr(), istd::IChangeable::CF_NO_UNDO);

			iser::IFileLoader* loaderPtr = documentTemplatePtr->GetFileLoader(documentTypeId);
			if (		(loaderPtr != NULL) &&
						(loaderPtr->LoadFromFile(*infoPtr->documentPtr, filePath) == iser::IFileLoader::StateOk)){
				RegisterDocument(infoPtr.GetPtr());

				documentNotifier.Reset();

				infoPtr->isDirty = false;

				if (infoPtr->undoManagerPtr.IsValid()){
					infoPtr->undoManagerPtr->StoreDocumentState();
				}

				return infoPtr.PopPtr()->documentPtr.GetPtr();
			}
		}
	}

	return NULL;
}


void CMultiDocumentManagerBase::CloseAllDocuments()
{
	istd::CChangeNotifier notifierPtr(this, CF_DOCUMENT_COUNT_CHANGED | CF_DOCUMENT_REMOVED | CF_MODEL);

	m_documentInfos.Reset();
}


CMultiDocumentManagerBase::SingleDocumentData& CMultiDocumentManagerBase::GetSingleDocumentData(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < GetDocumentsCount());

	SingleDocumentData* retVal = const_cast<SingleDocumentData*>(m_documentInfos.GetAt(index));
	Q_ASSERT(retVal != NULL);

	return *retVal;
}


CMultiDocumentManagerBase::SingleDocumentData* CMultiDocumentManagerBase::GetActiveDocumentInfo() const
{
	const istd::IPolymorphic* viewPtr = GetActiveView();
	if (viewPtr != NULL){
		return GetDocumentInfoFromView(*viewPtr);
	}

	return NULL;
}


CMultiDocumentManagerBase::SingleDocumentData* CMultiDocumentManagerBase::GetDocumentInfoFromView(const istd::IPolymorphic& view) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		SingleDocumentData& info = GetSingleDocumentData(i);

		Views::iterator findIter = qFind(info.views.begin(), info.views.end(), &view);
		if (findIter != info.views.end()){
			return &info;
		}
	}

	return NULL;
}


CMultiDocumentManagerBase::SingleDocumentData* CMultiDocumentManagerBase::GetDocumentInfoFromPath(const QString& filePath) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		SingleDocumentData& info = GetSingleDocumentData(i);

		if (info.filePath == filePath){
			return &info;
		}
	}

	return NULL;
}


int CMultiDocumentManagerBase::GetDocumentIndex(const SingleDocumentData& document) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		SingleDocumentData& info = GetSingleDocumentData(i);

		if (&info == &document){
			return i;
		}
	}

	return -1;
}


CMultiDocumentManagerBase::SingleDocumentData* CMultiDocumentManagerBase::CreateDocument(const QByteArray& documentTypeId, bool createView, const QByteArray& viewTypeId) const
{
	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();
	if (documentTemplatePtr != NULL){
		istd::IChangeable* documentPtr = documentTemplatePtr->CreateDocument(documentTypeId);

		istd::TDelPtr<SingleDocumentData> infoPtr(new SingleDocumentData(
					const_cast<CMultiDocumentManagerBase*>(this),
					documentTypeId,
					documentPtr,
					documentTemplatePtr->CreateUndoManager(documentTypeId, documentPtr)));

		if (infoPtr->documentPtr.IsValid()){
			imod::IModel* documentModelPtr = CompCastPtr<imod::IModel>(documentPtr);
			if (documentModelPtr != NULL){
				documentModelPtr->AttachObserver(infoPtr.GetPtr());
			}

			if (createView){
				istd::IPolymorphic* viewPtr = documentTemplatePtr->CreateView(
							documentTypeId,
							infoPtr->documentPtr.GetPtr(),
							viewTypeId);
				if (viewPtr == NULL){
					return NULL;
				}

				infoPtr->views.push_back(ViewPtr());
				infoPtr->views.back().SetPtr(viewPtr);
				infoPtr->viewTypeIds.push_back(viewTypeId);
			}

			return infoPtr.PopPtr();
		}
	}

	return NULL;
}


bool CMultiDocumentManagerBase::RegisterDocument(SingleDocumentData* infoPtr)
{
	Q_ASSERT(infoPtr != NULL);

	istd::CChangeNotifier changePtr(this, CF_DOCUMENT_COUNT_CHANGED | CF_DOCUMENT_CREATED | CF_MODEL);

	m_documentInfos.PushBack(infoPtr);

	for (		Views::iterator iter = infoPtr->views.begin();
				iter != infoPtr->views.end();
				++iter){
		Q_ASSERT(iter->IsValid());

		OnViewRegistered(iter->GetPtr());
	}

	return true;
}

bool CMultiDocumentManagerBase::SerializeOpenDocumentList(iser::IArchive& archive) 
{
	static iser::CArchiveTag openDocumentsListTag("OpenDocumentsList", "List of open documents ");
	static iser::CArchiveTag openDocumentTag("OpenDocument", "Single document properties");
	static iser::CArchiveTag filePathTag("FilePath", "File path");
	static iser::CArchiveTag documentTypeIdTag("DocumentTypeId", "Document Type ID");
	
	static iser::CArchiveTag viewListTag("ViewList", "View list");
	static iser::CArchiveTag viewTag("View", "View");
	static iser::CArchiveTag viewTypeIdTag("ViewTypeId", "View type ID");
	static iser::CArchiveTag viewIsActiveTag("ViewIsActive", "Active view");

	int documentsCount = GetDocumentsCount();

	bool retVal = archive.BeginMultiTag(openDocumentsListTag, openDocumentTag, documentsCount);
		
	if (archive.IsStoring()){
		for (int i = 0; i < documentsCount; ++i){
			SingleDocumentData& info = GetSingleDocumentData(i);

			retVal = retVal && archive.BeginTag(openDocumentTag);
			
			retVal = retVal && archive.BeginTag(filePathTag);
			retVal = retVal && archive.Process(info.filePath);
			retVal = retVal && archive.EndTag(filePathTag);

			retVal = retVal && archive.BeginTag(documentTypeIdTag);
			retVal = retVal && archive.Process(info.documentTypeId);
			retVal = retVal && archive.EndTag(documentTypeIdTag);

			int viewCount = info.viewTypeIds.count();	
			retVal = archive.BeginMultiTag(viewListTag, viewTag, viewCount);

			for (int j = 0; j < viewCount; ++j){
				retVal = retVal && archive.BeginTag(viewTag);
				
				retVal = retVal && archive.BeginTag(viewTypeIdTag);
				retVal = retVal && archive.Process(info.viewTypeIds[j]);
				retVal = retVal && archive.EndTag(viewTypeIdTag);

				bool isActive = (info.views[j] == m_activeViewPtr);
				retVal = retVal && archive.BeginTag(viewIsActiveTag);
				retVal = retVal && archive.Process(isActive);
				retVal = retVal && archive.EndTag(viewIsActiveTag);

				retVal = retVal && archive.EndTag(viewTag);
			}

			retVal = retVal && archive.EndTag(viewListTag);			

			retVal = retVal && archive.EndTag(openDocumentTag);
		}
	} 
	else {
		istd::IPolymorphic* activeView = NULL;

		for (int i = 0; i < documentsCount; ++i){
			retVal = retVal && archive.BeginTag(openDocumentTag);
			
			// Loading document info:
			QString filePath;
			retVal = retVal && archive.BeginTag(filePathTag);
			retVal = retVal && archive.Process(filePath);
			retVal = retVal && archive.EndTag(filePathTag);

			QByteArray documentTypeId;
			retVal = retVal && archive.BeginTag(documentTypeIdTag);
			retVal = retVal && archive.Process(documentTypeId);
			retVal = retVal && archive.EndTag(documentTypeIdTag);

			// Loading document's view info:
			int viewCount = 0;	
			retVal = archive.BeginMultiTag(viewListTag, viewTag, viewCount);
			
			for (int j = 0; j < viewCount; ++j)
			{
				QByteArray viewTypeId;
				retVal = retVal && archive.BeginTag(viewTag);
				
				retVal = retVal && archive.BeginTag(viewTypeIdTag);
				retVal = retVal && archive.Process(viewTypeId);
				retVal = retVal && archive.EndTag(viewTypeIdTag);

				bool isActive = false;
				retVal = retVal && archive.BeginTag(viewIsActiveTag);
				retVal = retVal && archive.Process(isActive);
				retVal = retVal && archive.EndTag(viewIsActiveTag);

				retVal = retVal && archive.EndTag(viewTag);

				// Open document:
				istd::IChangeable* openDocumentPtr = OpenDocument(filePath, true, viewTypeId, documentTypeId);
				if (openDocumentPtr == NULL){
					retVal = false;
				}	
				else {
					if (retVal && isActive){
						activeView = GetViewFromIndex(i, j);						
					}
				}
			}

			retVal = retVal && archive.EndTag(viewListTag);

			retVal = retVal && archive.EndTag(openDocumentTag);
		}

		//all documents have been loaded, set previously active view
		if (activeView != NULL){
			SetActiveView(activeView);
		}
	}	

	retVal = retVal && archive.EndTag(openDocumentsListTag);

	return retVal;
}


// protected methods of embedded class SingleDocumentData

// reimplemented (imod::CSingleModelObserverBase)

void CMultiDocumentManagerBase::SingleDocumentData::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	bool newDirty = true;
	if (documentPtr.IsValid() && undoManagerPtr.IsValid()){
		newDirty = (undoManagerPtr->GetDocumentChangeFlag() != IDocumentStateComparator::DCF_EQUAL);
	}

	if (isDirty != newDirty){
		istd::CChangeNotifier notifier(parentPtr);

		isDirty = newDirty;
	}
}


} // namespace idoc


