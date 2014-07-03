#include "idoc/CSerializedUndoManagerComp.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "iser/CMemoryReadArchive.h"
#include "icomp/CComponentBase.h"


namespace idoc
{


CSerializedUndoManagerComp::CSerializedUndoManagerComp()
:	m_hasStoredDocumentState(false),
	m_isBlocked(false)
{
}


// reimplemented (idoc::IUndoManager)

bool CSerializedUndoManagerComp::IsUndoAvailable() const
{
	return !m_undoList.isEmpty();
}


bool CSerializedUndoManagerComp::IsRedoAvailable() const
{
	return !m_redoList.isEmpty();
}


void CSerializedUndoManagerComp::ResetUndo()
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	m_undoList.clear();
	m_redoList.clear();
}


bool CSerializedUndoManagerComp::DoUndo()
{
	bool retVal = false;

	UndoArchivePtr redoArchivePtr(new iser::CMemoryWriteArchive());
	if (IsUndoAvailable() && redoArchivePtr.IsValid()){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		iser::ISerializable* objectPtr = GetObjectPtr();

		if (objectPtr != NULL){
			Q_ASSERT(!m_isBlocked);
			m_isBlocked = true;

			if (objectPtr->Serialize(*redoArchivePtr)){
				m_redoList.push_back(UndoArchivePtr());
				m_redoList.back().TakeOver(redoArchivePtr);
			}

			const UndoArchivePtr& writeArchivePtr = m_undoList.back();
			Q_ASSERT(writeArchivePtr.IsValid());

			iser::CMemoryReadArchive readArchive(*writeArchivePtr);

			if (objectPtr->Serialize(readArchive)){
				m_undoList.pop_back();

				retVal = true;
			}

			m_isBlocked = false;
		}
	}

	return retVal;
}


bool CSerializedUndoManagerComp::DoRedo()
{
	bool retVal = false;

	UndoArchivePtr undoArchivePtr(new iser::CMemoryWriteArchive());
	if (IsRedoAvailable() && undoArchivePtr.IsValid()){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		iser::ISerializable* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			Q_ASSERT(!m_isBlocked);
			m_isBlocked = true;

			if (objectPtr->Serialize(*undoArchivePtr)){
				m_undoList.push_back(UndoArchivePtr());
				m_undoList.back().TakeOver(undoArchivePtr);
			}

			const UndoArchivePtr& writeArchivePtr = m_redoList.back();
			Q_ASSERT(writeArchivePtr.IsValid());

			iser::CMemoryReadArchive readArchive(*writeArchivePtr);

			if (objectPtr->Serialize(readArchive)){
				m_redoList.pop_back();

				retVal = true;
			}

			m_isBlocked = false;
		}
	}

	return retVal;
}


// reimplemented (imod::IObserver)

bool CSerializedUndoManagerComp::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	if (BaseClass2::OnModelAttached(modelPtr, changeMask)){
		m_hasStoredDocumentState = false;
		m_storedStateArchive.Reset();

		m_stateChangedFlag = DCF_UNKNOWN;

		return true;
	}

	return false;
}


bool CSerializedUndoManagerComp::OnModelDetached(imod::IModel* modelPtr)
{
	if (BaseClass2::OnModelDetached(modelPtr)){
		m_hasStoredDocumentState = false;
		m_storedStateArchive.Reset();

		m_stateChangedFlag = DCF_UNKNOWN;

		return true;
	}

	return false;
}


// protected methods

// reimplemented (imod::TSingleModelObserverBase<iser::ISerializable>)

iser::ISerializable* CSerializedUndoManagerComp::CastFromModel(imod::IModel* modelPtr) const
{
	return CompCastPtr<iser::ISerializable>(modelPtr);
}


// reimplemented (imod::IObserver)

void CSerializedUndoManagerComp::BeforeUpdate(imod::IModel* modelPtr)
{
	BaseClass2::BeforeUpdate(modelPtr);

	if (!m_isBlocked && !m_beginStateArchivePtr.IsValid()){
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


void CSerializedUndoManagerComp::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(!changeSet.IsEmpty());

	m_isStateChangedFlagValid = false;

	if (		!m_isBlocked &&
				!changeSet.Contains(istd::IChangeable::CF_NO_UNDO) &&
				m_beginStateArchivePtr.IsValid()){
		iser::ISerializable* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			UndoArchivePtr archivePtr(new iser::CMemoryWriteArchive());

			if (objectPtr->Serialize(*archivePtr) && (*archivePtr != *m_beginStateArchivePtr)){
				istd::CChangeNotifier notifier(this);
				Q_UNUSED(notifier);

				m_undoList.push_back(UndoArchivePtr());
				m_undoList.back().TakeOver(m_beginStateArchivePtr);

				if (m_maxBufferSizeAttrPtr.IsValid() && (GetUsedMemorySize() > *m_maxBufferSizeAttrPtr * (1 << 20))){
					m_undoList.pop_front();
				}

				m_redoList.clear();
			}
		}

		m_beginStateArchivePtr.Reset();
	}

	BaseClass2::AfterUpdate(modelPtr, changeSet);
}


// reimplemented (idoc::IDocumentStateComparator)

bool CSerializedUndoManagerComp::HasStoredDocumentState() const
{
	return m_hasStoredDocumentState;
}


bool CSerializedUndoManagerComp::StoreDocumentState()
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

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


bool CSerializedUndoManagerComp::RestoreDocumentState()
{
	iser::CMemoryReadArchive restoreArchive(m_storedStateArchive);

	if (m_hasStoredDocumentState){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		iser::ISerializable* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			Q_ASSERT(!m_isBlocked);
			m_isBlocked = true;

			if (objectPtr->Serialize(restoreArchive)){
				m_stateChangedFlag = DCF_EQUAL;
				m_isStateChangedFlagValid = true;

				return true;
			}

			m_isBlocked = false;
		}

		m_stateChangedFlag = DCF_UNKNOWN;
		m_isStateChangedFlagValid = true;

		m_undoList.clear();
		m_redoList.clear();
	}

	return false;
}


IDocumentStateComparator::DocumentChangeFlag CSerializedUndoManagerComp::GetDocumentChangeFlag() const
{
	if (!m_isStateChangedFlagValid){
		m_stateChangedFlag = DCF_UNKNOWN;

		if (m_hasStoredDocumentState){
			iser::CMemoryWriteArchive compareArchive;

			iser::ISerializable* serializablePtr = GetObjectPtr();
			if ((serializablePtr != NULL) && const_cast<iser::ISerializable*>(serializablePtr)->Serialize(compareArchive)){
				m_stateChangedFlag = (compareArchive != m_storedStateArchive)? DCF_DIFFERENT: DCF_EQUAL;
			}
		}

		m_isStateChangedFlagValid = true;
	}

	return m_stateChangedFlag;
}


// private methods

qint64 CSerializedUndoManagerComp::GetUsedMemorySize() const
{
	qint64 memorySize = 0;

	for (UndoList::ConstIterator iter = m_undoList.constBegin(); iter != m_undoList.constEnd(); ++iter){
		memorySize += (*iter)->GetBufferSize();
	}

	return memorySize;
}


} // namespace idoc


