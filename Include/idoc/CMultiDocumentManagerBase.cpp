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
:	m_activeViewPtr(NULL)
{
}


// reimplemented (idoc::IDocumentManager)

idoc::IUndoManager* CMultiDocumentManagerBase::GetUndoManagerForDocument(const istd::IChangeable* documentPtr) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		const SingleDocumentData& info = GetSingleDocumentData(i);
		
		if(info.documentPtr == documentPtr){
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
	I_ASSERT(index >= 0);
	I_ASSERT(index < m_documentInfos.GetCount());

	SingleDocumentData* infoPtr = m_documentInfos.GetAt(index);
	I_ASSERT(infoPtr != NULL);
	I_ASSERT(infoPtr->documentPtr.IsValid());

	if (documentInfoPtr != NULL){
		*documentInfoPtr = *infoPtr;
	}

	return *(infoPtr->documentPtr);
}


int CMultiDocumentManagerBase::GetViewsCount(int documentIndex) const
{
	I_ASSERT(documentIndex >= 0);
	I_ASSERT(documentIndex < m_documentInfos.GetCount());

	SingleDocumentData* infoPtr = m_documentInfos.GetAt(documentIndex);
	I_ASSERT(infoPtr != NULL);

	return int(infoPtr->views.size());
}


istd::IPolymorphic* CMultiDocumentManagerBase::GetViewFromIndex(int documentIndex, int viewIndex) const
{
	I_ASSERT(documentIndex >= 0);
	I_ASSERT(documentIndex < m_documentInfos.GetCount());
	I_ASSERT(viewIndex < GetViewsCount(documentIndex));

	SingleDocumentData* infoPtr = m_documentInfos.GetAt(documentIndex);
	I_ASSERT(infoPtr != NULL);

	ViewPtr viewPtr = infoPtr->views.at(viewIndex);

	return viewPtr.GetPtr();
}


istd::IPolymorphic* CMultiDocumentManagerBase::GetActiveView() const
{
	return m_activeViewPtr;
}


istd::IChangeable* CMultiDocumentManagerBase::GetDocumentFromView(const istd::IPolymorphic& view, DocumentInfo* documentInfoPtr) const
{
	const SingleDocumentData* infoPtr = GetDocumentInfoFromView(view);
	if (infoPtr != NULL){
		I_ASSERT(infoPtr != NULL);
		I_ASSERT(infoPtr->documentPtr.IsValid());

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
		
		if(info.documentPtr == &document){
			istd::IPolymorphic* viewPtr = documentTemplatePtr->CreateView(
				info.documentTypeId,
				info.documentPtr.GetPtr(),
				viewTypeId);
			if (viewPtr == NULL){
				return NULL;
			}

			info.views.push_back(ViewPtr());
			info.views.back().SetPtr(viewPtr);

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
		I_ASSERT(newDocumentDataPtr != NULL);

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
		if (OpenDocument(fileName, createView, viewTypeId, documentTypeId)){
			if (loadedMapPtr != NULL){
				loadedMapPtr->operator[](fileName) = documentTypeId;
			}
		}
		else{
			retVal = false;
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
		I_ASSERT(documentIndex < GetDocumentsCount());

		infoPtr = &GetSingleDocumentData(documentIndex);
	}
	else{
		infoPtr = GetActiveDocumentInfo();
	}

	if (infoPtr == NULL){
		return false;
	}

	I_ASSERT(infoPtr->documentPtr.IsValid());

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
		I_ASSERT(infoPtr != NULL);

		for (Views::iterator iter = infoPtr->views.begin(); iter != infoPtr->views.end(); ++iter){
			if (infoPtr->isDirty){
				QueryDocumentClose(*infoPtr, ignoredPtr);
				if ((ignoredPtr != NULL) && *ignoredPtr){
					break;
				}
			}

			I_ASSERT(iter->IsValid());

			istd::IPolymorphic* viewPtr = iter->GetPtr();
			I_ASSERT(viewPtr != NULL);

			OnViewRemoved(viewPtr);

			infoPtr->views.erase(iter);

			if (viewPtr == m_activeViewPtr){
				m_activeViewPtr = NULL;
			}
		}

		int changeFlags = CF_DOCUMENT_REMOVED | CF_DOCUMENT_COUNT_CHANGED;
		// if last document was closed, force view activation update:
		if (m_documentInfos.GetCount() == 1){
			changeFlags |= CF_VIEW_ACTIVATION_CHANGED;
		}

		istd::CChangeNotifier notifier(this, changeFlags);

		m_documentInfos.RemoveAt(documentIndex);
	}
	else{
		for (int i = 0; i < documentsCount; ++i){
			SingleDocumentData* infoPtr = m_documentInfos.GetAt(i);
			I_ASSERT(infoPtr != NULL);

			Views::iterator findIter = qFind(infoPtr->views.begin(), infoPtr->views.end(), m_activeViewPtr);
			if (findIter != infoPtr->views.end()){
				if (infoPtr->isDirty){
					QueryDocumentClose(*infoPtr, ignoredPtr);
					if ((ignoredPtr != NULL) && *ignoredPtr){
						break;
					}
				}

				I_ASSERT(findIter->IsValid());
				OnViewRemoved(findIter->GetPtr());

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
		istd::CChangeNotifier changePtr(this, CF_VIEW_ACTIVATION_CHANGED);

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
		I_ASSERT(existingInfoPtr->documentPtr.IsValid());

		if (createView){
			istd::IPolymorphic* viewPtr = documentTemplatePtr->CreateView(
						existingInfoPtr->documentTypeId,
						existingInfoPtr->documentPtr.GetPtr(),
						viewTypeId);
			if (viewPtr != NULL){
				existingInfoPtr->views.push_back(ViewPtr());
				existingInfoPtr->views.back().SetPtr(viewPtr);

				OnViewRegistered(viewPtr);
			}
		}
		else{
			if (!existingInfoPtr->views.isEmpty()){
				istd::IPolymorphic* viewPtr = existingInfoPtr->views.front().GetPtr();
				I_ASSERT(viewPtr != NULL);

				SetActiveView(viewPtr);
			}
		}

		documentTypeId = existingInfoPtr->documentTypeId;

		return existingInfoPtr->documentPtr.GetPtr();
	}

	IDocumentTemplate::Ids documentIds = documentTemplatePtr->GetDocumentTypeIdsForFile(filePath);

	if (!documentIds.isEmpty()){
		istd::CChangeNotifier changePtr(this, CF_DOCUMENT_COUNT_CHANGED | CF_DOCUMENT_CREATED);

		documentTypeId = documentIds.front();
		istd::TDelPtr<SingleDocumentData> infoPtr(CreateDocument(documentTypeId, createView, viewTypeId));
		if (infoPtr.IsValid()){
			I_ASSERT(infoPtr->documentPtr.IsValid());

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
	istd::CChangeNotifier notifierPtr(this, CF_DOCUMENT_COUNT_CHANGED | CF_DOCUMENT_REMOVED);

	m_documentInfos.Reset();
}


CMultiDocumentManagerBase::SingleDocumentData& CMultiDocumentManagerBase::GetSingleDocumentData(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetDocumentsCount());

	SingleDocumentData* retVal = const_cast<SingleDocumentData*>(m_documentInfos.GetAt(index));
	I_ASSERT(retVal != NULL);

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
			}

			return infoPtr.PopPtr();
		}
	}

	return NULL;
}


bool CMultiDocumentManagerBase::RegisterDocument(SingleDocumentData* infoPtr)
{
	I_ASSERT(infoPtr != NULL);

	istd::CChangeNotifier changePtr(this, CF_DOCUMENT_COUNT_CHANGED | CF_DOCUMENT_CREATED);

	m_documentInfos.PushBack(infoPtr);

	for (		Views::iterator iter = infoPtr->views.begin();
				iter != infoPtr->views.end();
				++iter){
		I_ASSERT(iter->IsValid());

		OnViewRegistered(iter->GetPtr());
	}

	return true;
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


