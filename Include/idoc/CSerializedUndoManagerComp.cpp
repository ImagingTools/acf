// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <idoc/CSerializedUndoManagerComp.h>


// ACF includes
#include <istd/CChangeGroup.h>
#include <istd/CChangeNotifier.h>
#include <icomp/CComponentBase.h>
#include <iser/CMemoryReadArchive.h>


namespace idoc
{


// static attributes
static const istd::IChangeable::ChangeSet s_undoChangeSet(istd::IChangeable::CF_NO_UNDO, "UNDO");


CSerializedUndoManagerComp::CSerializedUndoManagerComp()
:	m_isBlocked(false)
{
}


// reimplemented (idoc::IUndoManager)

int CSerializedUndoManagerComp::GetAvailableUndoSteps() const
{
	return m_undoList.size();
}


int CSerializedUndoManagerComp::GetAvailableRedoSteps() const
{
	return m_redoList.size();
}


QString CSerializedUndoManagerComp::GetUndoLevelDescription(int stepIndex) const
{
	if ((stepIndex > 0) && (stepIndex <= m_undoList.size())){
		return m_undoList[m_undoList.size() - stepIndex].description;
	}

	return "";
}


QString CSerializedUndoManagerComp::GetRedoLevelDescription(int stepIndex) const
{
	if ((stepIndex > 0) && (stepIndex <= m_redoList.size())){
		return m_redoList[m_redoList.size() - stepIndex].description;
	}

	return "";
}


void CSerializedUndoManagerComp::ResetUndo()
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	m_undoList.clear();
	m_redoList.clear();
	m_beginStateArchivePtr.Reset();
}


bool CSerializedUndoManagerComp::DoUndo(int steps)
{
	return DoListShift(steps, m_undoList, m_redoList);
}


bool CSerializedUndoManagerComp::DoRedo(int steps)
{
	return DoListShift(steps, m_redoList, m_undoList);
}


// protected methods

bool CSerializedUndoManagerComp::DoListShift(int steps, UndoList& fromList, UndoList& toList)
{
	bool retVal = false;

	UndoArchivePtr currentStateArchivePtr(new iser::CMemoryWriteArchive(GetVersionInfo()));
	if ((steps > 0) && (fromList.size() >= steps) && currentStateArchivePtr.IsValid()){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		iser::ISerializable* objectPtr = GetObservedObject();

		if (objectPtr != NULL){
			Q_ASSERT(!m_isBlocked);
			m_isBlocked = true;

			istd::CChangeNotifier objectNotifier(objectPtr, &s_undoChangeSet);
			Q_UNUSED(objectNotifier);

			// stores pointer to last processed description container, necessary due of description shift
			// in undo list the state corresponds to state before changes, in redo - after changes. It causes description shift.
			QString* lastDescriptionPtr = NULL;

			if (objectPtr->Serialize(*currentStateArchivePtr)){
				toList.push_back(UndoStepInfo());

				UndoStepInfo& currentStep = toList.back();

				currentStep.archivePtr.TakeOver(currentStateArchivePtr);
				lastDescriptionPtr = &currentStep.description;
			}

			const UndoArchivePtr& writeArchivePtr = fromList[fromList.size() - steps].archivePtr;
			Q_ASSERT(writeArchivePtr.IsValid());

			iser::CMemoryReadArchive readArchive(*writeArchivePtr);

			if (objectPtr->Serialize(readArchive)){
				for (int i = 1; i < steps; ++i){
					if (lastDescriptionPtr != NULL){
						*lastDescriptionPtr = fromList.back().description;
					}

					toList.push_back(UndoStepInfo());
					toList.back().archivePtr.TakeOver(fromList.back().archivePtr);
					lastDescriptionPtr = &toList.back().description;

					fromList.pop_back();
				}

				if (lastDescriptionPtr != NULL){
					*lastDescriptionPtr = fromList.back().description;
				}

				fromList.pop_back();

				retVal = true;
			}

			objectNotifier.Reset();

			m_isBlocked = false;
		}
	}

	return retVal;
}


// reimplemented (idoc::IDocumentStateComparator)

bool CSerializedUndoManagerComp::StoreDocumentState()
{
	return BaseClass2::StoreDocumentState();
}


bool CSerializedUndoManagerComp::RestoreDocumentState()
{
	if (!HasStoredDocumentState()){
		return false;
	}

	Q_ASSERT(!m_isBlocked);
	m_isBlocked = true;
	bool retVal = BaseClass2::RestoreDocumentState();
	m_isBlocked = false;

	if (!retVal){
		m_undoList.clear();
		m_redoList.clear();
	}

	return retVal;
}


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
		iser::ISerializable* objectPtr = GetObservedObject();
		if (objectPtr != NULL){
			UndoArchivePtr archivePtr(new iser::CMemoryWriteArchive(GetVersionInfo()));

			if (		archivePtr.IsValid() &&
						objectPtr->Serialize(*archivePtr) &&
						(m_undoList.isEmpty() || *archivePtr != *(m_undoList.back().archivePtr))){
				m_beginStateArchivePtr.TakeOver(archivePtr);
			}
		}
	}
}


void CSerializedUndoManagerComp::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(!changeSet.IsEmpty());

	istd::CChangeGroup changeGroup(this);

	bool skipUndo = changeSet.ContainsExplicit(istd::IChangeable::CF_NO_UNDO, true);

	if (		!m_isBlocked &&
				!skipUndo &&
				m_beginStateArchivePtr.IsValid()){
		iser::ISerializable* objectPtr = GetObservedObject();
		if (objectPtr != NULL){
			UndoArchivePtr archivePtr(new iser::CMemoryWriteArchive(GetVersionInfo()));

			if (objectPtr->Serialize(*archivePtr)){
				if (*archivePtr != *m_beginStateArchivePtr){
					istd::CChangeNotifier notifier(this);
					Q_UNUSED(notifier);

					m_undoList.push_back(UndoStepInfo());
					m_undoList.back().archivePtr.TakeOver(m_beginStateArchivePtr);
					m_undoList.back().description = changeSet.GetDescription();

					if (m_maxBufferSizeAttrPtr.IsValid() && (GetUsedMemorySize() > *m_maxBufferSizeAttrPtr * (1 << 20))){
						m_undoList.pop_front();
					}

					m_redoList.clear();
				}
			}
			else{
				qDebug("Undo Manager: Object serialization failed");
			}
		}

		m_beginStateArchivePtr.Reset();
	}

	BaseClass2::AfterUpdate(modelPtr, changeSet);
}


// reimplemented (icomp::CComponentBase)

void CSerializedUndoManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_versionInfoCompPtr.EnsureInitialized();

	SetVersionInfo(m_versionInfoCompPtr.GetPtr());
}


void CSerializedUndoManagerComp::OnComponentDestroyed()
{
	EnsureModelDetached();

	SetVersionInfo(NULL);

	BaseClass::OnComponentDestroyed();
}


// private methods

qint64 CSerializedUndoManagerComp::GetUsedMemorySize() const
{
	qint64 memorySize = 0;

	for (UndoList::ConstIterator iter = m_undoList.constBegin(); iter != m_undoList.constEnd(); ++iter){
		memorySize += iter->archivePtr->GetBufferSize();
	}

	return memorySize;
}


} // namespace idoc
