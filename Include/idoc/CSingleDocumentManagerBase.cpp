#include <idoc/CSingleDocumentManagerBase.h>


// ACF includes
#include <istd/CChangeNotifier.h>

#include <imod/IModel.h>
#include <imod/IModelEditor.h>

#include <icomp/CComponentBase.h>

#include <idoc/IDocumentTemplate.h>


namespace idoc
{


const istd::IChangeable::ChangeSet s_newDocumentChangeSet(IDocumentManager::CF_DOCUMENT_COUNT_CHANGED, IDocumentManager::CF_DOCUMENT_CREATED);
const istd::IChangeable::ChangeSet s_openDocumentChangeSet(IDocumentManager::CF_DOCUMENT_COUNT_CHANGED, IDocumentManager::CF_DOCUMENT_CREATED);
const istd::IChangeable::ChangeSet s_closeViewChangeSet(IDocumentManager::CF_DOCUMENT_REMOVED, IDocumentManager::CF_DOCUMENT_COUNT_CHANGED, IDocumentManager::CF_VIEW_ACTIVATION_CHANGED);


CSingleDocumentManagerBase::CSingleDocumentManagerBase()
: m_isDirty(false)
{
}


// reimplemented (idoc::IDocumentManager)

idoc::IUndoManager* CSingleDocumentManagerBase::GetUndoManagerForDocument(const istd::IChangeable* documentPtr) const
{
	if (documentPtr == m_documentPtr.GetPtr()){
		return m_undoManagerPtr.GetPtr();
	}

	return NULL;
}


int CSingleDocumentManagerBase::GetDocumentsCount() const
{
	return m_documentPtr.IsValid()? 1: 0;
}


istd::IChangeable& CSingleDocumentManagerBase::GetDocumentFromIndex(int I_IF_DEBUG(index), DocumentInfo* documentInfoPtr) const
{
	I_IF_DEBUG(Q_ASSERT(index >= 0));
	I_IF_DEBUG(Q_ASSERT(index < GetDocumentsCount()));
	Q_ASSERT(m_documentPtr.IsValid());

	if (documentInfoPtr != NULL){
		documentInfoPtr->filePath = m_filePath;
		documentInfoPtr->documentTypeId = m_documentTypeId;
		documentInfoPtr->isDirty = m_isDirty;
	}

	return *m_documentPtr;
}


int CSingleDocumentManagerBase::GetViewsCount(int I_IF_DEBUG(documentIndex)) const
{
	I_IF_DEBUG(Q_ASSERT(documentIndex >= 0));
	I_IF_DEBUG(Q_ASSERT(documentIndex < GetDocumentsCount()));

	return m_viewPtr.IsValid()? 1: 0;
}


istd::IPolymorphic* CSingleDocumentManagerBase::GetViewFromIndex(int I_IF_DEBUG(documentIndex), int I_IF_DEBUG(viewIndex)) const
{
	I_IF_DEBUG(Q_ASSERT(documentIndex >= 0));
	I_IF_DEBUG(Q_ASSERT(documentIndex < GetDocumentsCount()));
	I_IF_DEBUG(Q_ASSERT(viewIndex < GetViewsCount(documentIndex)));

	return m_viewPtr.GetPtr();
}


istd::IPolymorphic* CSingleDocumentManagerBase::GetActiveView() const
{
	return m_viewPtr.GetPtr();
}


istd::IChangeable* CSingleDocumentManagerBase::GetDocumentFromView(const istd::IPolymorphic& view, DocumentInfo* documentInfoPtr) const
{
	if ((&view == m_viewPtr.GetPtr()) && m_documentPtr.IsValid()){
		if (documentInfoPtr != NULL){
			documentInfoPtr->filePath = m_filePath;
			documentInfoPtr->documentTypeId = m_documentTypeId;
			documentInfoPtr->isDirty = m_isDirty;
		}

		return m_documentPtr.GetPtr();
	}

	return NULL;
}


istd::IPolymorphic* CSingleDocumentManagerBase::AddViewToDocument(const istd::IChangeable& document, const QByteArray& viewTypeId)
{
	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();
	if (documentTemplatePtr != NULL){
		if (m_documentPtr.IsValid() && m_documentPtr.GetPtr() == &document){
			EnsureViewRemoved();

			istd::IPolymorphic* viewPtr = documentTemplatePtr->CreateView(
						m_documentTypeId,
						m_documentPtr.GetPtr(),
						viewTypeId);
			if (viewPtr != NULL){
				m_viewPtr.SetPtr(viewPtr);

				m_viewTypeId = viewTypeId;

				OnViewRegistered(viewPtr);
			}

			return viewPtr;
		}
	}

	return NULL;
}


QByteArray CSingleDocumentManagerBase::GetDocumentTypeId(const istd::IChangeable& document) const
{
	if (&document == m_documentPtr.GetPtr()){
		return m_documentTypeId;
	}

	return QByteArray();
}


bool CSingleDocumentManagerBase::InsertNewDocument(
			const QByteArray& documentTypeId,
			bool createView,
			const QByteArray& viewTypeId,
			istd::IChangeable** newDocumentPtr,
			bool beQuiet,
			bool* ignoredPtr)
{
	istd::CChangeNotifier notifier(this, &s_newDocumentChangeSet);
	Q_UNUSED(notifier);

	CloseDocument(-1, beQuiet, ignoredPtr);

	if ((ignoredPtr != NULL) && *ignoredPtr){
		if (newDocumentPtr != NULL){
			*newDocumentPtr = m_documentPtr.GetPtr();
		}

		return true;
	}

	if (NewDocument(documentTypeId, createView, viewTypeId, true, beQuiet, ignoredPtr)){
		RegisterDocument();

		if (newDocumentPtr != NULL){
			*newDocumentPtr = m_documentPtr.GetPtr();
		}

		return true;
	}

	return false;
}


bool CSingleDocumentManagerBase::OpenDocument(
			const QByteArray* documentTypeIdPtr,
			const QString* fileNamePtr,
			bool createView,
			const QByteArray& viewTypeId,
			istd::IChangeable** documentPtr,
			FileToTypeMap* loadedMapPtr,
			bool beQuiet,
			bool* ignoredPtr,
			ibase::IProgressManager* progressManagerPtr)
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

	bool retVal = true;

	QString fileName;

	if (fileNamePtr != NULL){
		fileName = *fileNamePtr;
	}
	else{
		if (beQuiet){
			return false;
		}

		fileName = GetOpenFilePath(documentTypeIdPtr);
	}

	if (!fileName.isEmpty()){
		QByteArray documentTypeId;
		if (OpenSingleDocument(fileName, createView, viewTypeId, documentTypeId, beQuiet, ignoredPtr, progressManagerPtr)){
			if (loadedMapPtr != NULL){
				loadedMapPtr->operator[](fileName) = documentTypeId;
			}

			if (documentPtr != NULL){
				*documentPtr = m_documentPtr.GetPtr();
			}
		}
		else{
			retVal = false;
		}
	}

	return retVal;
}


bool CSingleDocumentManagerBase::SaveDocument(
			int /*documentIndex*/,
			bool requestFileName,
			FileToTypeMap* savedMapPtr,
			bool beQuiet,
			bool* ignoredPtr,
			ibase::IProgressManager* progressManagerPtr)
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();
	if ((documentTemplatePtr == NULL) || !m_documentPtr.IsValid()){
		return false;
	}

	QString filePath = m_filePath;

	requestFileName  = requestFileName || filePath.isEmpty();

	if (requestFileName){
		if (beQuiet){
			return false;
		}

		filePath = GetSaveFilePath(m_documentTypeId, m_documentPtr.GetPtr(), filePath);

		if (filePath.isEmpty()){
			if (ignoredPtr != NULL){
				*ignoredPtr = true;
			}

			return false;
		}
	}

	const imod::IModelEditor* editorPtr = CompCastPtr<imod::IModelEditor>(m_viewPtr.GetPtr());
	if ((editorPtr != NULL) && (!editorPtr->IsReadOnly())){
		editorPtr->UpdateModelFromEditor();
	}

	const ifile::IFilePersistence* loaderPtr = documentTemplatePtr->GetFileLoader(m_documentTypeId);
	if (loaderPtr == NULL){
		return false;
	}

	int saveState = loaderPtr->SaveToFile(*m_documentPtr, filePath, progressManagerPtr);
	if (saveState == ifile::IFilePersistence::OS_OK){
		if ((m_filePath != filePath) || m_isDirty){
			istd::CChangeNotifier notifier(this);
			Q_UNUSED(notifier);

			m_filePath = filePath;
			m_isDirty = false;

			if (m_undoManagerPtr.IsValid()){
				m_undoManagerPtr->StoreDocumentState();
			}
		}

		if (savedMapPtr != NULL){
			savedMapPtr->operator[](filePath) = m_documentTypeId;
		}

		return true;
	}
	else if ((saveState == ifile::IFilePersistence::OS_CANCELED) && (ignoredPtr != NULL)){
		*ignoredPtr = true;
	}

	return false;
}


bool CSingleDocumentManagerBase::SaveDirtyDocuments(bool beQuiet, bool* ignoredPtr)
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

	if (m_documentPtr.IsValid()){
		if (m_isDirty){
			if (!beQuiet){
				if (QueryDocumentSave(ignoredPtr)){
					if (!SaveDocument(-1, false) && (ignoredPtr != NULL)){
						*ignoredPtr = true;	// Cannot save, set cancel
					}
				}

				if ((ignoredPtr != NULL) && *ignoredPtr){
					return false;
				}
			}
		}
	}

	return true;
}


bool CSingleDocumentManagerBase::CloseDocument(int /*documentIndex*/, bool beQuiet, bool* ignoredPtr)
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

	if (m_documentPtr.IsValid()){
		if (!beQuiet){
			SaveDirtyDocuments(false, ignoredPtr);
		}

		if ((ignoredPtr != NULL) && *ignoredPtr){
			return false;
		}

		istd::CChangeNotifier notifier(this, &s_closeViewChangeSet);
		Q_UNUSED(notifier);

		EnsureViewRemoved();

		m_filePath = "";
		m_documentTypeId = "";
		m_documentPtr.Reset();
		m_undoManagerPtr.Reset();

		return true;
	}

	return false;
}


bool CSingleDocumentManagerBase::CloseView(istd::IPolymorphic* viewPtr, bool beQuiet, bool* ignoredPtr)
{
	if ((viewPtr == NULL) || (viewPtr = m_viewPtr.GetPtr())){
		return CloseDocument(-1, beQuiet, ignoredPtr);
	}

	return false;
}


// protected methods

bool CSingleDocumentManagerBase::OpenSingleDocument(
			const QString& filePath,
			bool createView,
			const QByteArray& viewTypeId,
			QByteArray& documentTypeId,
			bool beQuiet,
			bool* ignoredPtr,
			ibase::IProgressManager* progressManagerPtr)
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();
	if (filePath.isEmpty() || (documentTemplatePtr == NULL)){
		return false;
	}

	if (filePath == m_filePath){
		Q_ASSERT(m_documentPtr.IsValid());

		if (viewTypeId != m_viewTypeId){
			EnsureViewRemoved();

			if (createView){
				istd::IPolymorphic* viewPtr = documentTemplatePtr->CreateView(
							m_documentTypeId,
							m_documentPtr.GetPtr(),
							viewTypeId);
				if (viewPtr != NULL){
					m_viewPtr.SetPtr(viewPtr);

					OnViewRegistered(viewPtr);

					m_viewTypeId = viewTypeId;
				}
			}
		}

		documentTypeId = m_documentTypeId;

		return true;
	}

	istd::CChangeNotifier notifier(this, &s_openDocumentChangeSet);
	Q_UNUSED(notifier);

	CloseDocument(-1, beQuiet, ignoredPtr);

	if ((ignoredPtr != NULL) && *ignoredPtr){
		documentTypeId = m_documentTypeId;

		return true;
	}

	IDocumentTemplate::Ids documentIds = documentTemplatePtr->GetDocumentTypeIdsForFile(filePath);

	if (!documentIds.isEmpty()){
		documentTypeId = documentIds.front();

		if (NewDocument(documentTypeId, createView, viewTypeId, false, beQuiet, ignoredPtr)){
			Q_ASSERT(m_documentPtr.IsValid());

			ifile::IFilePersistence* loaderPtr = documentTemplatePtr->GetFileLoader(documentTypeId);
			if (loaderPtr != NULL){
				int loadState = loaderPtr->LoadFromFile(*m_documentPtr, filePath, progressManagerPtr);
				if (loadState == ifile::IFilePersistence::OS_OK){
					m_filePath = filePath;

					RegisterDocument();

					return true;
				}
				else if ((loadState == ifile::IFilePersistence::OS_OK) && (ignoredPtr != NULL)){
					*ignoredPtr = true;
				}
			}
		}
		else{
			CloseDocument(-1, true);
		}
	}

	return false;
}


bool CSingleDocumentManagerBase::NewDocument(
			const QByteArray& documentTypeId,
			bool createView,
			const QByteArray& viewTypeId,
			bool initialize,
			bool beQuiet,
			bool* ignoredPtr)
{
	m_undoManagerPtr.Reset();

	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();
	if (documentTemplatePtr != NULL){
		QByteArray realDocumentTypeId = documentTypeId;

		istd::TDelPtr<istd::IChangeable> documentPtr(documentTemplatePtr->CreateDocument(realDocumentTypeId, initialize, beQuiet, ignoredPtr));

		if (documentPtr.IsValid()){
			EnsureViewRemoved();

			if (createView){
				istd::IPolymorphic* viewPtr = documentTemplatePtr->CreateView(
							realDocumentTypeId,
							documentPtr.GetPtr(),
							viewTypeId);
				if (viewPtr == NULL){
					return false;
				}

				m_viewPtr.SetPtr(viewPtr);

				m_viewTypeId = viewTypeId;
			}

			m_documentPtr.TakeOver(documentPtr);

			m_documentTypeId = realDocumentTypeId;

			if (m_viewPtr.IsValid()){
				OnViewRegistered(m_viewPtr.GetPtr());
			}

			return true;
		}
	}

	return false;
}


bool CSingleDocumentManagerBase::RegisterDocument()
{
	m_isDirty = false;

	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();
	if (documentTemplatePtr != NULL){
		m_undoManagerPtr.SetPtr(documentTemplatePtr->CreateUndoManager(m_documentTypeId, m_documentPtr.GetPtr()));

		if (m_undoManagerPtr.IsValid()){
			m_undoManagerPtr->StoreDocumentState();

			imod::IModel* documentModelPtr = CompCastPtr<imod::IModel>(m_documentPtr.GetPtr());
			if (documentModelPtr != NULL){
				documentModelPtr->AttachObserver(this);
			}
		}
	}

	return true;
}


void CSingleDocumentManagerBase::EnsureViewRemoved()
{
	if (m_viewPtr.IsValid()){
		OnViewRemoved(m_viewPtr.GetPtr());

		m_viewPtr.Reset();

		m_viewTypeId = "";
	}
}


QString CSingleDocumentManagerBase::GetCurrentDocumentFilePath() const
{
	return m_filePath;
}


bool CSingleDocumentManagerBase::HasDocumentPendingChanges() const
{
	return m_isDirty;
}


bool CSingleDocumentManagerBase::SerializeOpenDocument(iser::IArchive& archive)
{
	static iser::CArchiveTag openDocumentTag("OpenDocument", "Single document properties", iser::CArchiveTag::TT_GROUP);
	static iser::CArchiveTag filePathTag("FilePath", "File path", iser::CArchiveTag::TT_LEAF, &openDocumentTag);
	static iser::CArchiveTag documentTypeIdTag("DocumentTypeId", "Document Type ID", iser::CArchiveTag::TT_LEAF, &openDocumentTag);
	static iser::CArchiveTag viewTypeIdTag("ViewTypeId", "View type ID", iser::CArchiveTag::TT_LEAF, &openDocumentTag);

	bool retVal = archive.BeginTag(openDocumentTag);

	if (archive.IsStoring()){
		retVal = retVal && archive.BeginTag(filePathTag);
		retVal = retVal && archive.Process(m_filePath);
		retVal = retVal && archive.EndTag(filePathTag);

		retVal = retVal && archive.BeginTag(documentTypeIdTag);
		retVal = retVal && archive.Process(m_documentTypeId);
		retVal = retVal && archive.EndTag(documentTypeIdTag);

		retVal = retVal && archive.BeginTag(viewTypeIdTag);
		retVal = retVal && archive.Process(m_viewTypeId);
		retVal = retVal && archive.EndTag(viewTypeIdTag);
	}
	else{
		QString filePath;
		QByteArray documentTypeId;
		QByteArray viewTypeId;

		retVal = retVal && archive.BeginTag(filePathTag);
		retVal = retVal && archive.Process(filePath);
		retVal = retVal && archive.EndTag(filePathTag);

		retVal = retVal && archive.BeginTag(documentTypeIdTag);
		retVal = retVal && archive.Process(documentTypeId);
		retVal = retVal && archive.EndTag(documentTypeIdTag);

		retVal = retVal && archive.BeginTag(viewTypeIdTag);
		retVal = retVal && archive.Process(viewTypeId);
		retVal = retVal && archive.EndTag(viewTypeIdTag);

		if (retVal && !filePath.isEmpty()){
			OpenSingleDocument(filePath, true, viewTypeId, documentTypeId, true, NULL, NULL);
		}
	}

	retVal = retVal && archive.EndTag(openDocumentTag);

	return retVal;
}


// reimplemented (imod::CSingleModelObserverBase)

void CSingleDocumentManagerBase::OnUpdate(const ChangeSet& /*changeSet*/)
{
	bool newDirty = true;
	if (m_documentPtr.IsValid() && m_undoManagerPtr.IsValid()){
		newDirty = (m_undoManagerPtr->GetDocumentChangeFlag() != IDocumentStateComparator::DCF_EQUAL);
	}

	if (m_isDirty != newDirty){
		istd::CChangeNotifier notifier(this);

		m_isDirty = newDirty;
	}
}


} // namespace idoc


