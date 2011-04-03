#include "imod/CSerializedUndoManager.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/CMemoryReadArchive.h"


namespace imod
{


CSerializedUndoManager::CSerializedUndoManager()
:	m_updateNestingCount(0)
{
}


// reimplemented (imod::IUndoManager)

bool CSerializedUndoManager::IsUndoAvailable() const
{
	return !m_undoList.empty();
}


bool CSerializedUndoManager::IsRedoAvailable() const
{
	return !m_redoList.empty();
}


void CSerializedUndoManager::ResetUndo()
{
	m_undoList.clear();
	m_redoList.clear();
}


bool CSerializedUndoManager::DoUndo()
{
	UndoArchivePtr redoArchivePtr(new iser::CMemoryWriteArchive());
	if (IsUndoAvailable() && redoArchivePtr.IsValid()){
		istd::TChangeNotifier<iser::ISerializable> objectPtr(GetObjectPtr(), CF_NO_UNDO);
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
		istd::TChangeNotifier<iser::ISerializable> objectPtr(GetObjectPtr(), CF_NO_UNDO);
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


// protected methods

// reimplemented (imod::IObserver)

void CSerializedUndoManager::BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	BaseClass::BeforeUpdate(modelPtr, updateFlags, updateParamsPtr);

	if (m_updateNestingCount++ <= 0){
		I_ASSERT(!m_beginStateArchivePtr.IsValid());

		iser::ISerializable* objectPtr = GetObjectPtr();
		if (		(objectPtr != NULL) &&
					((updateFlags & CF_NO_UNDO) == 0)){
			UndoArchivePtr archivePtr(new iser::CMemoryWriteArchive());

			if (		archivePtr.IsValid() &&
						objectPtr->Serialize(*archivePtr) &&
						(m_undoList.empty() || *archivePtr != *m_undoList.back())){
				m_beginStateArchivePtr.TakeOver(archivePtr);
			}
		}
	}
}


void CSerializedUndoManager::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if (--m_updateNestingCount <= 0){
		iser::ISerializable* objectPtr = GetObjectPtr();
		if (		(objectPtr != NULL) &&
					(m_beginStateArchivePtr.IsValid() &&
					((updateFlags & CF_NO_UNDO) == 0)) &&
					m_beginStateArchivePtr.IsValid()){
			UndoArchivePtr archivePtr(new iser::CMemoryWriteArchive());

			if (objectPtr->Serialize(*archivePtr) && (*archivePtr != *m_beginStateArchivePtr)){
				istd::TChangeNotifier<CSerializedUndoManager> selfNotifierPtr(this);

				m_undoList.push_back(UndoArchivePtr());
				m_undoList.back().TakeOver(m_beginStateArchivePtr);
				m_redoList.clear();
			}
		}

		m_beginStateArchivePtr.Reset();
	}

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


} // namespace imod


