#include "idoc/CSingleDocumentManagerBase.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"
#include "imod/IModelEditor.h"

#include "icomp/CComponentBase.h"

#include "idoc/IDocumentTemplate.h"


namespace idoc
{		


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
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetDocumentsCount());
	I_ASSERT(m_documentPtr.IsValid());

	if (documentInfoPtr != NULL){
		documentInfoPtr->filePath = m_filePath;
		documentInfoPtr->documentTypeId = m_documentTypeId;
		documentInfoPtr->isDirty = m_isDirty;
	}

	return *m_documentPtr;
}


int CSingleDocumentManagerBase::GetViewsCount(int I_IF_DEBUG(documentIndex)) const
{
	I_ASSERT(documentIndex >= 0);
	I_ASSERT(documentIndex < GetDocumentsCount());

	return m_viewPtr.IsValid()? 1: 0;
}


istd::IPolymorphic* CSingleDocumentManagerBase::GetViewFromIndex(int I_IF_DEBUG(documentIndex), int I_IF_DEBUG(viewIndex)) const
{
	I_ASSERT(documentIndex >= 0);
	I_ASSERT(documentIndex < GetDocumentsCount());
	I_ASSERT(viewIndex < GetViewsCount(documentIndex));

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


bool CSingleDocumentManagerBase::FileNew(
			const QByteArray& documentTypeId, 
			bool createView, 
			const QByteArray& viewTypeId,
			istd::IChangeable** newDocumentPtr)
{
	istd::CChangeNotifier changePtr(this, CF_DOCUMENT_COUNT_CHANGED | CF_DOCUMENT_CREATED);

	bool isCloseIgnored = false;

	FileClose(-1, &isCloseIgnored);

	if (isCloseIgnored){
		if (newDocumentPtr != NULL){
			*newDocumentPtr = m_documentPtr.GetPtr();
		}

		return true;
	}

	if (NewDocument(documentTypeId, createView, viewTypeId)){
		if (newDocumentPtr != NULL){
			*newDocumentPtr = m_documentPtr.GetPtr();
		}

		return true;
	}

	return false;
}


bool CSingleDocumentManagerBase::FileOpen(
			const QByteArray* documentTypeIdPtr,
			const QString* fileNamePtr,
			bool createView,
			const QByteArray& viewTypeId,
			FileToTypeMap* loadedMapPtr)
{
	bool retVal = true;

	QString fileName;

	if (fileNamePtr != NULL){
		fileName = *fileNamePtr;
	}
	else{
		fileName = GetOpenFilePath(documentTypeIdPtr);
	}

	if (!fileName.isEmpty()){
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


bool CSingleDocumentManagerBase::FileSave(
			int /*documentIndex*/,
			bool requestFileName,
			FileToTypeMap* savedMapPtr)
{
	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();
	if ((documentTemplatePtr == NULL) || !m_documentPtr.IsValid()){
		return false;
	}

	QString filePath = m_filePath;

	requestFileName  = requestFileName || filePath.isEmpty();

	if (requestFileName){
		filePath = GetSaveFilePath(m_documentTypeId);

		if (filePath.isEmpty()){
			return true;
		}
	}

	const imod::IModelEditor* editorPtr = CompCastPtr<imod::IModelEditor>(m_viewPtr.GetPtr());
	if ((editorPtr != NULL) && (!editorPtr->IsReadOnly())){
		editorPtr->UpdateModel();
	}

	const iser::IFileLoader* loaderPtr = documentTemplatePtr->GetFileLoader(m_documentTypeId);
	if ((loaderPtr != NULL) && loaderPtr->SaveToFile(*m_documentPtr, filePath) == iser::IFileLoader::StateOk){
		if ((m_filePath != filePath) || m_isDirty){
			istd::CChangeNotifier notifierPtr(this);

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

	return false;
}


void CSingleDocumentManagerBase::FileClose(int /*documentIndex*/, bool* ignoredPtr)
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

	if (m_documentPtr.IsValid()){
		if (m_isDirty){
			QueryDocumentClose(ignoredPtr);

			if ((ignoredPtr != NULL) && *ignoredPtr){
				return;
			}
		}

		istd::CChangeNotifier notifier(this, CF_DOCUMENT_REMOVED | CF_DOCUMENT_COUNT_CHANGED | CF_VIEW_ACTIVATION_CHANGED);

		EnsureViewRemoved();

		m_filePath = "";
		m_documentTypeId = "";
		m_documentPtr.Reset();
		m_undoManagerPtr.Reset();
	}
}


// protected methods

bool CSingleDocumentManagerBase::OpenDocument(
			const QString& filePath,
			bool createView,
			const QByteArray& viewTypeId,
			QByteArray& documentTypeId)
{
	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();
	if (filePath.isEmpty() || (documentTemplatePtr == NULL)){
		return false;
	}

	if (filePath == m_filePath){
		I_ASSERT(m_documentPtr.IsValid());

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
	
	bool isCloseIgnored = false;

	FileClose(-1, &isCloseIgnored);

	if (isCloseIgnored){
		documentTypeId = m_documentTypeId;

		return true;
	}

	IDocumentTemplate::Ids documentIds = documentTemplatePtr->GetDocumentTypeIdsForFile(filePath);

	if (!documentIds.isEmpty()){
		documentTypeId = documentIds.front();

		istd::CChangeNotifier changePtr(this, CF_DOCUMENT_COUNT_CHANGED | CF_DOCUMENT_CREATED);

		if (NewDocument(documentTypeId, createView, viewTypeId)){
			I_ASSERT(m_documentPtr.IsValid());

			istd::CChangeNotifier documentNotifier(m_documentPtr.GetPtr(), istd::IChangeable::CF_NO_UNDO);

			iser::IFileLoader* loaderPtr = documentTemplatePtr->GetFileLoader(documentTypeId);
			if (		(loaderPtr != NULL) &&
						(loaderPtr->LoadFromFile(*m_documentPtr, filePath) == iser::IFileLoader::StateOk)){
				m_filePath = filePath;
				documentNotifier.Reset();

				m_isDirty = false;

				if (m_undoManagerPtr.IsValid()){
					m_undoManagerPtr->StoreDocumentState();
				}

				return true;
			}
		}
	}

	return false;
}


bool CSingleDocumentManagerBase::NewDocument(
			const QByteArray& documentTypeId, 
			bool createView, 
			const QByteArray& viewTypeId)
{
	const IDocumentTemplate* documentTemplatePtr = GetDocumentTemplate();

	if (documentTemplatePtr != NULL){
		istd::TDelPtr<istd::IChangeable> documentPtr(documentTemplatePtr->CreateDocument(documentTypeId));

		if (documentPtr.IsValid()){
			istd::IPolymorphic* viewPtr = NULL;

			EnsureViewRemoved();

			if (createView){
				viewPtr = documentTemplatePtr->CreateView(
							documentTypeId,
							documentPtr.GetPtr(),
							viewTypeId);
				if (viewPtr == NULL){
					return false;
				}

				m_viewPtr.SetPtr(viewPtr);

				m_viewTypeId = viewTypeId;

				OnViewRegistered(viewPtr);
			}

			m_documentPtr.TakeOver(documentPtr);
			m_undoManagerPtr.SetPtr(documentTemplatePtr->CreateUndoManager(documentTypeId, m_documentPtr.GetPtr()));

			imod::IModel* documentModelPtr = CompCastPtr<imod::IModel>(m_documentPtr.GetPtr());
			if (documentModelPtr != NULL){
				documentModelPtr->AttachObserver(this);
			}

			m_documentTypeId = documentTypeId;

			m_isDirty = false;

			if (m_undoManagerPtr.IsValid()){
				m_undoManagerPtr->StoreDocumentState();
			}

			return true;
		}
	}

	return false;
}


void CSingleDocumentManagerBase::EnsureViewRemoved()
{
	if (m_viewPtr.IsValid()){
		OnViewRemoved(m_viewPtr.GetPtr());

		m_viewPtr.Reset();

		m_viewTypeId = "";
	}
}


// reimplemented (imod::CSingleModelObserverBase)

void CSingleDocumentManagerBase::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
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


