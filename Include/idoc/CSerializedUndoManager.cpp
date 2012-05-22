#include "idoc/CSerializedUndoManager.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/CMemoryReadArchive.h"


namespace idoc
{


CSerializedUndoManager::CSerializedUndoManager()
:	m_hasStoredDocumentState(false)
{
}


// reimplemented (idoc::IUndoManager)

bool CSerializedUndoManager::IsUndoAvailable() const
{
	return !m_undoList.isEmpty();
}


bool CSerializedUndoManager::IsRedoAvailable() const
{
	return !m_redoList.isEmpty();
}


void CSerializedUndoManager::ResetUndo()
{
	istd::CChangeNotifier selfNotifierPtr(this);

	m_undoList.clear();
	m_redoList.clear();
}


bool CSerializedUndoManager::DoUndo()
{
	UndoArchivePtr redoArchivePtr(new iser::CMemoryWriteArchive());
	if (IsUndoAvailable() && redoArchivePtr.IsValid()){
		istd::CChangeNotifier notifier(this);

		istd::TChangeNotifier<iser::ISerializable> objectPtr(GetObjectPtr(), istd::IChangeable::CF_NO_UNDO);
		if (objectPtr.IsValid()){
			if (objectPtr->Serialize(*redoArchivePtr)){
				m_redoList.push_back(UndoArchivePtr());
				m_redoList.back().TakeOver(redoArchivePtr);
			}

			const UndoArchivePtr& writeArchivePtr = m_undoList.back();
			I_ASSERT(writeArchivePtr.IsValid());

			iser::CMemoryReadArchive readArchive(*writeArchivePtr);

			if (objectPtr->Serialize(readArchive)){
				m_undoList.pop_back();

				return true;
			}
		}
	}

	return false;
}


bool CSerializedUndoManager::DoRedo()
{
	UndoArchivePtr undoArchivePtr(new iser::CMemoryWriteArchive());
	if (IsRedoAvailable() && undoArchivePtr.IsValid()){
		istd::CChangeNotifier notifier(this);

		istd::TChangeNotifier<iser::ISerializable> objectPtr(GetObjectPtr(), istd::IChangeable::CF_NO_UNDO);
		if (objectPtr.IsValid()){
			if (objectPtr->Serialize(*undoArchivePtr)){
				m_undoList.push_back(UndoArchivePtr());
				m_undoList.back().TakeOver(undoArchivePtr);
			}

			const UndoArchivePtr& writeArchivePtr = m_redoList.back();
			I_ASSERT(writeArchivePtr.IsValid());

			iser::CMemoryReadArchive readArchive(*writeArchivePtr);

			if (objectPtr->Serialize(readArchive)){
				m_redoList.pop_back();

				return true;
			}
		}
	}

	return false;
}


// reimplemented (imod::IObserver)

bool CSerializedUndoManager::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass::OnAttached(modelPtr)){
		m_hasStoredDocumentState = false;
		m_storedStateArchive.Reset();

		m_stateChangedFlag = DCF_UNKNOWN;
		m_hasStoredDocumentState = false;

		return true;
	}

	return false;
}


bool CSerializedUndoManager::OnDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnDetached(modelPtr)){
		m_hasStoredDocumentState = false;
		m_storedStateArchive.Reset();

		m_stateChangedFlag = DCF_UNKNOWN;
		m_hasStoredDocumentState = false;

		return true;
	}

	return false;
}


// protected methods

// reimplemented (imod::IObserver)

void CSerializedUndoManager::BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	BaseClass::BeforeUpdate(modelPtr, updateFlags, updateParamsPtr);

	if (((updateFlags & istd::IChangeable::CF_NO_UNDO) == 0) && !m_beginStateArchivePtr.IsValid()){
		iser::ISerializable* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			UndoArchivePtr archivePtr(new iser::CMemoryWriteArchive());

			if (		archivePtr.IsValid() &&
						objectPtr->Serialize(*archivePtr) &&
						(m_undoList.isEmpty() || *archivePtr != *m_undoList.back())){
				m_beginStateArchivePtr.TakeOver(archivePtr);
			}
		}
	}
}


void CSerializedUndoManager::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if (((updateFlags & istd::IChangeable::CF_NO_UNDO) == 0) && m_beginStateArchivePtr.IsValid()){
		iser::ISerializable* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			UndoArchivePtr archivePtr(new iser::CMemoryWriteArchive());

			if (objectPtr->Serialize(*archivePtr) && (*archivePtr != *m_beginStateArchivePtr)){
				istd::CChangeNotifier selfNotifierPtr(this);

				m_undoList.push_back(UndoArchivePtr());
				m_undoList.back().TakeOver(m_beginStateArchivePtr);
				m_redoList.clear();
			}
		}

		m_beginStateArchivePtr.Reset();
	}

	m_isStateChangedFlagValid = false;

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


// reimplemented (idoc::IDocumentStateComparator)

bool CSerializedUndoManager::HasStoredDocumentState() const
{
	return m_hasStoredDocumentState;
}


bool CSerializedUndoManager::StoreDocumentState()
{
	istd::CChangeNotifier selfNotifierPtr(this);

	m_storedStateArchive.Reset();

	iser::ISerializable* serializablePtr = GetObjectPtr();
	if ((serializablePtr != NULL) && serializablePtr->Serialize(m_storedStateArchive)){
		m_stateChangedFlag = DCF_EQUAL;
		m_hasStoredDocumentState = true;
	}
	else{
		m_stateChangedFlag = DCF_UNKNOWN;
		m_hasStoredDocumentState = false;
	}
	m_isStateChangedFlagValid = true;

	return false;
}


bool CSerializedUndoManager::RestoreDocumentState()
{
	iser::CMemoryReadArchive restoreArchive(m_storedStateArchive);

	if (m_hasStoredDocumentState){
		istd::CChangeNotifier selfNotifierPtr(this);

		istd::TChangeNotifier<iser::ISerializable> serializablePtr(GetObjectPtr(), istd::IChangeable::CF_NO_UNDO);
		if (serializablePtr.IsValid() && serializablePtr->Serialize(restoreArchive)){
			m_stateChangedFlag = DCF_EQUAL;
			m_isStateChangedFlagValid = true;

			return true;
		}

		m_stateChangedFlag = DCF_UNKNOWN;
		m_isStateChangedFlagValid = true;

		m_undoList.clear();
		m_redoList.clear();
	}

	return false;
}


IDocumentStateComparator::DocumentChangeFlag CSerializedUndoManager::GetDocumentChangeFlag() const
{
	if (!m_isStateChangedFlagValid){
		iser::CMemoryWriteArchive compareArchive;

		iser::ISerializable* serializablePtr = GetObjectPtr();
		if (serializablePtr != NULL){
			const_cast<iser::ISerializable*>(serializablePtr)->Serialize(compareArchive);
			m_stateChangedFlag = (compareArchive != m_storedStateArchive)? DCF_DIFFERENT: DCF_EQUAL;
		}
		else{
			m_stateChangedFlag = DCF_UNKNOWN;
		}

		m_isStateChangedFlagValid = true;
	}

	return m_stateChangedFlag;
}


} // namespace idoc


