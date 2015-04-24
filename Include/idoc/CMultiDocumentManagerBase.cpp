#include "idoc/CMultiDocumentManagerBase.h"


// Qt includes
#include <QtCore/QStringList>


// ACF includes
#include "istd/CChangeNotifier.h"

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
	for (int documentIndex = 0; documentIndex < documentsCount; ++documentIndex){
		const SingleDocumentData& info = GetSingleDocumentData(documentIndex);

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

	const ViewInfo& viewInfo = infoPtr->views.at(viewIndex);

	return viewInfo.viewPtr.GetPtr();
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
	for (int documentIndex = 0; documentIndex < documentsCount; ++documentIndex){
		SingleDocumentData& info = GetSingleDocumentData(documentIndex);

		if (info.documentPtr == &document){
			istd::IPolymorphic* viewPtr = documentTemplatePtr->CreateView(
				info.documentTypeId,
				info.documentPtr.GetPtr(),
				viewTypeId);
			if (viewPtr == NULL){
				return NULL;
			}


			info.views.push_back(ViewInfo());

			ViewInfo& newViewInfo = info.views.back();
			newViewInfo.viewPtr.SetPtr(viewPtr);
			newViewInfo.viewTypeId = viewTypeId;

			OnViewRegistered(viewPtr, info);

			return viewPtr;
		}
	}

	return NULL;
}



QByteArray CMultiDocumentManagerBase::GetDocumentTypeId(const istd::IChangeable& document) const
{
	int documentsCount = GetDocumentsCount();
	for (int documentIndex = 0; documentIndex < documentsCount; ++documentIndex){
		SingleDocumentData& info = GetSingleDocumentData(documentIndex);

		if (info.documentPtr == &document){
			return info.documentTypeId;
		}
	}

	return QByteArray();
}


bool CMultiDocumentManagerBase::InsertNewDocument(
			const QByteArray& documentTypeId,
			bool createView,
			const QByteArray& viewTypeId,
			istd::IChangeable** newDocumentPtr,
			bool beQuiet,
			bool* ignoredPtr)
{
	istd::TDelPtr<SingleDocumentData> newInfoPtr(CreateUnregisteredDocument(documentTypeId, createView, viewTypeId, true, beQuiet, ignoredPtr));
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


bool CMultiDocumentManagerBase::OpenDocument(
			const QByteArray* documentTypeIdPtr,
			const QString* fileNamePtr,
			bool createView,
			const QByteArray& viewTypeId,
			istd::IChangeable** documentPtr,
			FileToTypeMap* loadedMapPtr,
			bool beQuiet,
			bool* ignoredPtr)
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

	bool retVal = true;

	QStringList files;

	if (fileNamePtr != NULL){
		files.push_back(*fileNamePtr);
	}
	else{
		if (beQuiet){
			return false;
		}

		files = GetOpenFilePaths(documentTypeIdPtr);
	}

	for (		QStringList::const_iterator iter = files.begin();
				iter != files.end();
				++iter){
		const QString& fileName = *iter;

		QByteArray documentTypeId;
		istd::IChangeable* openDocumentPtr = OpenSingleDocument(fileName, createView, viewTypeId, documentTypeId, beQuiet, ignoredPtr);
		if (openDocumentPtr != NULL){
			if (loadedMapPtr != NULL){
				loadedMapPtr->operator[](fileName) = documentTypeId;
			}
		}
		else{
			retVal = false;

			if ((ignoredPtr != NULL) && *ignoredPtr){
				break;
			}
		}

		if (documentPtr != NULL){
			*documentPtr = openDocumentPtr;
		}
	}

	return retVal;
}


bool CMultiDocumentManagerBase::SaveDocument(
			int documentIndex,
			bool requestFileName,
			FileToTypeMap* savedMapPtr,
			bool beQuiet,
			bool* ignoredPtr)
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

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
		if (beQuiet){
			return false;
		}

		filePath = GetSaveFilePath(infoPtr->documentTypeId, infoPtr->documentPtr.GetPtr(), filePath);
		if (filePath.isEmpty()){
			return true;
		}
	}

	for (		Views::const_iterator viewIter = infoPtr->views.begin();
				viewIter != infoPtr->views.end();
				++viewIter){
		const ViewInfo& viewInfo = *viewIter;

		const imod::IModelEditor* editorPtr = CompCastPtr<imod::IModelEditor>(viewInfo.viewPtr.GetPtr());
		if ((editorPtr != NULL) && (!editorPtr->IsReadOnly())){
			editorPtr->UpdateModel();
		}
	}

	const ifile::IFilePersistence* loaderPtr = documentTemplatePtr->GetFileLoader(infoPtr->documentTypeId);
	if (loaderPtr == NULL){
		return false;
	}

	int saveState = loaderPtr->SaveToFile(*infoPtr->documentPtr, filePath);
	if (saveState == ifile::IFilePersistence::OS_OK){
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
	else if ((saveState == ifile::IFilePersistence::OS_CANCELED) && (ignoredPtr != NULL)){
		*ignoredPtr = true;
	}

	return false;
}


bool CMultiDocumentManagerBase::SaveDirtyDocuments(bool beQuiet, bool* ignoredPtr)
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

	int documentsCount = m_documentInfos.GetCount();
	for (int documentIndex = 0; documentIndex < documentsCount; ++documentIndex){
		SingleDocumentData* infoPtr = m_documentInfos.GetAt(documentIndex);
		Q_ASSERT(infoPtr != NULL);

		if (infoPtr->isDirty){
			if (!beQuiet){
				// ensure some view of closing document is visible before query
				if (!infoPtr->views.isEmpty() && (infoPtr != GetActiveDocumentInfo())){
					const ViewInfo& firstViewInfo = infoPtr->views.front();

					SetActiveView(firstViewInfo.viewPtr.GetPtr());
				}
			}

			if (beQuiet || QueryDocumentSave(*infoPtr, ignoredPtr)){
				if (!SaveDocument(documentIndex, false, NULL, beQuiet, ignoredPtr)){
					return false;
				}
			}

			if ((ignoredPtr != NULL) && *ignoredPtr){
				return false;
			}
		}
	}

	return true;
}


bool CMultiDocumentManagerBase::CloseDocument(int documentIndex, bool beQuiet, bool* ignoredPtr)
{
	if (documentIndex < 0){
		int documentsCount = m_documentInfos.GetCount();
		for (int i = 0; i < documentsCount; ++i){
			const SingleDocumentData* infoPtr = m_documentInfos.GetAt(i);
			Q_ASSERT(infoPtr != NULL);

			for (		Views::ConstIterator viewIter = infoPtr->views.constBegin();
						viewIter != infoPtr->views.constEnd();
						++viewIter){
				const ViewInfo& viewInfo = *viewIter;
				Q_ASSERT(viewInfo.viewPtr.IsValid());

				if (viewInfo.viewPtr == m_activeViewPtr){
					documentIndex = i;
					break;
				}
			}
		}
		Q_ASSERT(documentIndex >= 0);	// if some view is active than document must also exist...
	}

	SingleDocumentData* infoPtr = m_documentInfos.GetAt(documentIndex);
	Q_ASSERT(infoPtr != NULL);

	if (infoPtr->isDirty){
		if (!beQuiet){
			// ensure some view of closing document is visible before query
			if (!infoPtr->views.isEmpty() && (infoPtr != GetActiveDocumentInfo())){
				const ViewInfo& firstViewInfo = infoPtr->views.front();

				SetActiveView(firstViewInfo.viewPtr.GetPtr());
			}
		}

		if (beQuiet || QueryDocumentSave(*infoPtr, ignoredPtr)){
			if (!SaveDocument(documentIndex, false, NULL, beQuiet, ignoredPtr)){
				return false;
			}
		}

		if ((ignoredPtr != NULL) && *ignoredPtr){
			return false;
		}
	}

	// Close all views of this document
	Views::iterator viewIter = infoPtr->views.begin();
	while (viewIter != infoPtr->views.end()){
		const ViewInfo& viewInfo = *viewIter;
		Q_ASSERT(viewInfo.viewPtr.IsValid());

		OnViewRemoved(viewInfo.viewPtr.GetPtr());

		if (viewInfo.viewPtr == m_activeViewPtr){
			m_activeViewPtr = NULL;
		}

		viewIter = infoPtr->views.erase(viewIter);
	}

	ChangeSet changeSet(CF_DOCUMENT_REMOVED, CF_DOCUMENT_COUNT_CHANGED);
	if (m_documentInfos.GetCount() == 1){
		// If last document was closed, force view activation update:
		changeSet += CF_VIEW_ACTIVATION_CHANGED;
	}
	istd::CChangeNotifier notifier(this, &changeSet);
	Q_UNUSED(notifier);

	SingleDocumentData* removedObjectPtr = m_documentInfos.PopAt(documentIndex);
	notifier.Reset();

	delete removedObjectPtr;

	return true;
}


bool CMultiDocumentManagerBase::CloseCurrentView(bool beQuiet, bool* ignoredPtr)
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

	int documentsCount = m_documentInfos.GetCount();
	for (int documentIndex = 0; documentIndex < documentsCount; ++documentIndex){
		SingleDocumentData* infoPtr = m_documentInfos.GetAt(documentIndex);
		Q_ASSERT(infoPtr != NULL);

		for (		Views::Iterator viewIter = infoPtr->views.begin();
					viewIter != infoPtr->views.end();
					++viewIter){
			ViewInfo& viewInfo = *viewIter;
			Q_ASSERT(viewInfo.viewPtr.IsValid());

			if (viewInfo.viewPtr == m_activeViewPtr){
				if (infoPtr->views.size() == 1){	// is it the last view? If yes then simple close document
					return CloseDocument(documentIndex, beQuiet, ignoredPtr);
				}
				else{
					OnViewRemoved(viewInfo.viewPtr.GetPtr());

					infoPtr->views.erase(viewIter);	// remove active view

					m_activeViewPtr = NULL;
				}

				return true;
			}
		}
	}

	return false;
}


// protected methods

void CMultiDocumentManagerBase::SetActiveView(istd::IPolymorphic* viewPtr)
{
	if (m_activeViewPtr != viewPtr){
		static const ChangeSet changeSet(CF_VIEW_ACTIVATION_CHANGED);
		istd::CChangeNotifier notifier(this, &changeSet);
		Q_UNUSED(notifier);

		m_activeViewPtr = viewPtr;
	}
}


istd::IChangeable* CMultiDocumentManagerBase::OpenSingleDocument(
			const QString& filePath,
			bool createView,
			const QByteArray& viewTypeId,
			QByteArray& documentTypeId,
			bool beQuiet,
			bool* ignoredPtr)
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

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
				existingInfoPtr->views.push_back(ViewInfo());

				ViewInfo& newViewInfo = existingInfoPtr->views.back();
				newViewInfo.viewPtr.SetPtr(viewPtr);
				newViewInfo.viewTypeId = viewTypeId;

				OnViewRegistered(viewPtr, *existingInfoPtr);
			}
		}
		else{
			// try to show some view of existing document
			if (!existingInfoPtr->views.isEmpty() && (existingInfoPtr != GetActiveDocumentInfo())){
				const ViewInfo& firstViewInfo = existingInfoPtr->views.front();
				Q_ASSERT(firstViewInfo.viewPtr.IsValid());

				SetActiveView(firstViewInfo.viewPtr.GetPtr());
			}
		}

		documentTypeId = existingInfoPtr->documentTypeId;

		return existingInfoPtr->documentPtr.GetPtr();
	}

	IDocumentTemplate::Ids documentIds = documentTemplatePtr->GetDocumentTypeIdsForFile(filePath);

	if (!documentIds.isEmpty()){
		documentTypeId = documentIds.front();
		istd::TDelPtr<SingleDocumentData> infoPtr(CreateUnregisteredDocument(documentTypeId, createView, viewTypeId, false, beQuiet, ignoredPtr));
		if (infoPtr.IsValid()){
			Q_ASSERT(infoPtr->documentPtr.IsValid());

			infoPtr->filePath = filePath;
			infoPtr->documentTypeId = documentTypeId;

			static const ChangeSet changeSet(CF_DOCUMENT_COUNT_CHANGED, CF_DOCUMENT_CREATED);
			istd::CChangeNotifier notifier(this, &changeSet);
			Q_UNUSED(notifier);

			ifile::IFilePersistence* loaderPtr = documentTemplatePtr->GetFileLoader(documentTypeId);
			if (		(loaderPtr != NULL) &&
						(loaderPtr->LoadFromFile(*infoPtr->documentPtr, filePath) == ifile::IFilePersistence::OS_OK)){
				RegisterDocument(infoPtr.GetPtr());

				infoPtr->isDirty = false;

				return infoPtr.PopPtr()->documentPtr.GetPtr();
			}
		}
	}

	return NULL;
}


void CMultiDocumentManagerBase::CloseAllDocuments()
{
	if (!m_documentInfos.IsEmpty()){
		static const ChangeSet changeSet(CF_DOCUMENT_COUNT_CHANGED, CF_DOCUMENT_REMOVED);
		istd::CChangeNotifier notifier(this, &changeSet);
		Q_UNUSED(notifier);

		m_documentInfos.Reset();
	}
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
	for (int documentIndex = 0; documentIndex < documentsCount; ++documentIndex){
		SingleDocumentData& info = GetSingleDocumentData(documentIndex);

		for (		Views::ConstIterator viewIter = info.views.begin();
					viewIter != info.views.end();
					++viewIter){
			const ViewInfo& viewInfo = *viewIter;
			if (viewInfo.viewPtr == &view){
				return &info;
			}
		}
	}

	return NULL;
}


CMultiDocumentManagerBase::SingleDocumentData* CMultiDocumentManagerBase::GetDocumentInfoFromPath(const QString& filePath) const
{
	int documentsCount = GetDocumentsCount();
	for (int documentIndex = 0; documentIndex < documentsCount; ++documentIndex){
		SingleDocumentData& info = GetSingleDocumentData(documentIndex);

		if (info.filePath == filePath){
			return &info;
		}
	}

	return NULL;
}


int CMultiDocumentManagerBase::GetDocumentIndex(const SingleDocumentData& document) const
{
	int documentsCount = GetDocumentsCount();
	for (int documentIndex = 0; documentIndex < documentsCount; ++documentIndex){
		SingleDocumentData& info = GetSingleDocumentData(documentIndex);

		if (&info == &document){
			return documentIndex;
		}
	}

	return -1;
}


CMultiDocumentManagerBase::SingleDocumentData* CMultiDocumentManagerBase::CreateUnregisteredDocument(
			const QByteArray& documentTypeId,
			bool createView,
			const QByteArray& viewTypeId,
			bool initialize,
			bool beQuiet,
			bool* ignoredPtr) const
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();
	if (documentTemplatePtr != NULL){
		QByteArray realDocumentTypeId = documentTypeId;

		istd::IChangeable* documentPtr = documentTemplatePtr->CreateDocument(realDocumentTypeId, initialize, beQuiet, ignoredPtr);

		istd::TDelPtr<SingleDocumentData> infoPtr(new SingleDocumentData(
					const_cast<CMultiDocumentManagerBase*>(this),
					realDocumentTypeId,
					documentPtr));

		if (infoPtr->documentPtr.IsValid()){
			imod::IModel* documentModelPtr = CompCastPtr<imod::IModel>(documentPtr);
			if (documentModelPtr != NULL){
				documentModelPtr->AttachObserver(infoPtr.GetPtr());
			}

			if (createView){
				istd::IPolymorphic* viewPtr = documentTemplatePtr->CreateView(
							realDocumentTypeId,
							infoPtr->documentPtr.GetPtr(),
							viewTypeId);
				if (viewPtr == NULL){
					return NULL;
				}

				infoPtr->views.push_back(ViewInfo());

				ViewInfo& newViewInfo = infoPtr->views.back();
				newViewInfo.viewPtr.SetPtr(viewPtr);
				newViewInfo.viewTypeId = viewTypeId;
			}

			return infoPtr.PopPtr();
		}
	}

	return NULL;
}


bool CMultiDocumentManagerBase::RegisterDocument(SingleDocumentData* infoPtr)
{
	Q_ASSERT(infoPtr != NULL);

	static const ChangeSet changeSet(CF_DOCUMENT_COUNT_CHANGED, CF_DOCUMENT_CREATED);
	istd::CChangeNotifier notifier(this, &changeSet);
	Q_UNUSED(notifier);

	m_documentInfos.PushBack(infoPtr);

	// create and connect undo manager
	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();
	if (documentTemplatePtr != NULL){
		infoPtr->undoManagerPtr.SetPtr(documentTemplatePtr->CreateUndoManager(infoPtr->documentTypeId, infoPtr->documentPtr.GetPtr()));

		if (infoPtr->undoManagerPtr.IsValid()){
			// connect undo manager to single document descriptor to provide general dirty flag
			imod::IModel* undoManagerModelPtr = CompCastPtr<imod::IModel>(infoPtr->undoManagerPtr.GetPtr());
			if (undoManagerModelPtr != NULL){
				undoManagerModelPtr->AttachObserver(infoPtr);
			}

			infoPtr->undoManagerPtr->StoreDocumentState();
		}
	}

	// registration feedback for all views
	for (		Views::iterator viewIter = infoPtr->views.begin();
				viewIter != infoPtr->views.end();
				++viewIter){
		ViewInfo& viewInfo = *viewIter;
		Q_ASSERT(viewInfo.viewPtr.IsValid());

		OnViewRegistered(viewInfo.viewPtr.GetPtr(), *infoPtr);
	}

	return true;
}

bool CMultiDocumentManagerBase::SerializeOpenDocumentList(iser::IArchive& archive)
{
	static iser::CArchiveTag openDocumentsListTag("OpenDocumentsList", "List of open documents", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag openDocumentTag("OpenDocument", "Single document properties", iser::CArchiveTag::TT_GROUP, &openDocumentsListTag);
	static iser::CArchiveTag filePathTag("FilePath", "File path", iser::CArchiveTag::TT_LEAF, &openDocumentTag);
	static iser::CArchiveTag documentTypeIdTag("DocumentTypeId", "Document Type ID", iser::CArchiveTag::TT_LEAF, &openDocumentTag);

	static iser::CArchiveTag viewListTag("ViewList", "View list", iser::CArchiveTag::TT_MULTIPLE, &openDocumentTag);
	static iser::CArchiveTag viewTag("View", "View", iser::CArchiveTag::TT_GROUP, &viewListTag);
	static iser::CArchiveTag viewTypeIdTag("ViewTypeId", "View type ID", iser::CArchiveTag::TT_LEAF, &viewTag);
	static iser::CArchiveTag viewIsActiveTag("ViewIsActive", "Active view", iser::CArchiveTag::TT_LEAF, &viewTag);

	int documentsCount = GetDocumentsCount();

	bool retVal = archive.BeginMultiTag(openDocumentsListTag, openDocumentTag, documentsCount);

	if (archive.IsStoring()){
		for (int documentIndex = 0; documentIndex < documentsCount; ++documentIndex){
			SingleDocumentData& info = GetSingleDocumentData(documentIndex);

			retVal = retVal && archive.BeginTag(openDocumentTag);

			retVal = retVal && archive.BeginTag(filePathTag);
			retVal = retVal && archive.Process(info.filePath);
			retVal = retVal && archive.EndTag(filePathTag);

			retVal = retVal && archive.BeginTag(documentTypeIdTag);
			retVal = retVal && archive.Process(info.documentTypeId);
			retVal = retVal && archive.EndTag(documentTypeIdTag);

			int viewCount = info.views.count();
			retVal = archive.BeginMultiTag(viewListTag, viewTag, viewCount);

			for (int viewIndex = 0; viewIndex < viewCount; ++viewIndex){
				ViewInfo& viewInfo = info.views[viewIndex];

				retVal = retVal && archive.BeginTag(viewTag);

				retVal = retVal && archive.BeginTag(viewTypeIdTag);
				retVal = retVal && archive.Process(viewInfo.viewTypeId);
				retVal = retVal && archive.EndTag(viewTypeIdTag);

				bool isActive = (viewInfo.viewPtr == m_activeViewPtr);
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

		for (int documentIndex = 0; documentIndex < documentsCount; ++documentIndex){
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

			istd::IChangeable* openDocumentPtr = OpenSingleDocument(filePath, false, "", documentTypeId, true, NULL);
			if (openDocumentPtr == NULL){
				return false;
			}

			// Loading document's view info:
			int viewCount = 0;
			retVal = archive.BeginMultiTag(viewListTag, viewTag, viewCount);

			for (int viewIndex = 0; viewIndex < viewCount; ++viewIndex){
				retVal = retVal && archive.BeginTag(viewTag);

				QByteArray viewTypeId;
				retVal = retVal && archive.BeginTag(viewTypeIdTag);
				retVal = retVal && archive.Process(viewTypeId);
				retVal = retVal && archive.EndTag(viewTypeIdTag);

				bool isActive = false;
				retVal = retVal && archive.BeginTag(viewIsActiveTag);
				retVal = retVal && archive.Process(isActive);
				retVal = retVal && archive.EndTag(viewIsActiveTag);

				retVal = retVal && archive.EndTag(viewTag);

				if (!retVal){
					return false;
				}

				istd::IPolymorphic* viewPtr = AddViewToDocument(*openDocumentPtr, viewTypeId);

				// Open document:
				if ((viewPtr != NULL) && isActive){
					activeView = viewPtr;
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


// public methods of embedded class SingleDocumentData

CMultiDocumentManagerBase::SingleDocumentData::SingleDocumentData(
			CMultiDocumentManagerBase* parentPtr,
			const QByteArray& documentTypeId,
			istd::IChangeable* documentPtr)
{
	this->parentPtr = parentPtr;
	this->documentTypeId = documentTypeId;
	this->documentPtr.SetPtr(documentPtr);
	isDirty = false;
}


CMultiDocumentManagerBase::SingleDocumentData::~SingleDocumentData()
{
	// enforce the correct destruction order
	undoManagerPtr.Reset();
	documentPtr.Reset();
	views.clear();
}


// protected methods of embedded class SingleDocumentData

// reimplemented (imod::CSingleModelObserverBase)

void CMultiDocumentManagerBase::SingleDocumentData::OnUpdate(imod::IModel* /*modelPtr*/, const ChangeSet& /*changeSet*/)
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


