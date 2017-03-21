#include <iprm/CSelectionParam.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iprm/IOptionsList.h>


namespace iprm
{


// static constants
static const istd::IChangeable::ChangeSet s_selectionChangeSet(ISelectionParam::CF_SELECTION_CHANGED, "Change selection");
static const istd::IChangeable::ChangeSet s_selectionCorrectionChangeSet(ISelectionParam::CF_SELECTION_CHANGED, istd::IChangeable::CF_NO_UNDO);
static const iser::CArchiveTag s_selectedOptionIndexTag("Index", "Selected option index", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_selectedOptionIdTag("OptionId", "Selected option identifier", iser::CArchiveTag::TT_LEAF);


CSelectionParam::CSelectionParam()
:	m_selectedOptionIndex(NO_SELECTION),
	m_constraintsPtr(NULL),
	m_constraintsObserver(*this)
{
}


void CSelectionParam::SetSelectionConstraints(const IOptionsList* constraintsPtr)
{
	if (m_constraintsPtr != constraintsPtr){
		m_constraintsObserver.EnsureModelDetached();

		m_constraintsPtr = constraintsPtr;

		if (m_constraintsPtr != NULL){
			if (m_selectedOptionIndex >= 0){
				if (m_selectedOptionIndex < m_constraintsPtr->GetOptionsCount()){
					m_selectedOptionId = m_constraintsPtr->GetOptionId(m_selectedOptionIndex);
				}
				else{
					istd::CChangeNotifier notifier(this, &s_selectionCorrectionChangeSet);
					Q_UNUSED(notifier);

					m_selectedOptionId.clear();
					m_selectedOptionIndex = NO_SELECTION;
				}
			}

			imod::IModel* constraintsModelPtr = const_cast<imod::IModel*>(dynamic_cast<const imod::IModel*>(m_constraintsPtr));
			if (constraintsModelPtr != NULL){
				constraintsModelPtr->AttachObserver(&m_constraintsObserver);
			}
		}
	}
}


void CSelectionParam::SetSubselection(int selectionIndex, ISelectionParam* selectionPtr)
{
	m_subselectionMap[selectionIndex] = selectionPtr;
}


bool CSelectionParam::SetSelectedOptionById(const QByteArray& selectedOptionId)
{
	if (selectedOptionId == m_selectedOptionId){
		return true;
	}

	int index = NO_SELECTION;
	if (!selectedOptionId.isEmpty()){
		index = CalcIndexFromId(selectedOptionId, NO_SELECTION);
		if (index == NO_SELECTION){
			return false;
		}
	}

	istd::CChangeNotifier notifier(this, &s_selectionChangeSet);
	Q_UNUSED(notifier);

	m_selectedOptionId = selectedOptionId;
	m_selectedOptionIndex = index;

	return true;
}


int CSelectionParam::GetOptionIndexById(const QByteArray& optionId) const
{
	if (optionId.isEmpty()){
		return NO_SELECTION;
	}

	if (m_constraintsPtr != NULL){
		int optionsCount = m_constraintsPtr->GetOptionsCount();

		for (int optionIndex = 0; optionIndex < optionsCount; optionIndex++){
			QByteArray currentOptionId = m_constraintsPtr->GetOptionId(optionIndex);

			if (currentOptionId == optionId){
				return optionIndex;
			}
		}
	}

	return NO_SELECTION;
}


ISelectionParam* CSelectionParam::GetActiveSubselection() const
{
	return GetSubselection(m_selectedOptionIndex);
}


// reimplemented (iprm::ISelectionParam)

const IOptionsList* CSelectionParam::GetSelectionConstraints() const
{
	return m_constraintsPtr;
}


int CSelectionParam::GetSelectedOptionIndex() const
{
	return m_selectedOptionIndex;
}


bool CSelectionParam::SetSelectedOptionIndex(int index)
{
	if ((m_constraintsPtr != NULL) && (index >= m_constraintsPtr->GetOptionsCount())){
		return false;
	}

	if (index != m_selectedOptionIndex){
		if (index != iprm::ISelectionParam::NO_SELECTION){
			if ((m_constraintsPtr != NULL) && !m_constraintsPtr->IsOptionEnabled(index)){
				return false;
			}
		}

		istd::CChangeNotifier notifier(this, &s_selectionChangeSet);
		Q_UNUSED(notifier);

		m_selectedOptionIndex = index;

		if (m_constraintsPtr != NULL){
			if (index >= 0){
				m_selectedOptionId = m_constraintsPtr->GetOptionId(index);
			}
			else{
				m_selectedOptionId.clear();
			}
		}
	}

	return true;
}


ISelectionParam* CSelectionParam::GetSubselection(int index) const
{
	if (m_subselectionMap.contains(index)){
		return m_subselectionMap[index];
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CSelectionParam::Serialize(iser::IArchive& archive)
{
	bool isStoring = archive.IsStoring();

	QByteArray optionId = m_selectedOptionId;
	int index = NO_SELECTION;
	if (isStoring){
		index = CalcIndexFromId(m_selectedOptionId, m_selectedOptionIndex);
	}

	bool retVal = archive.BeginTag(s_selectedOptionIndexTag);
	retVal = retVal && archive.Process(index);
	retVal = retVal && archive.EndTag(s_selectedOptionIndexTag);

	retVal = retVal && archive.BeginTag(s_selectedOptionIdTag);
	retVal = retVal && archive.Process(optionId);
	retVal = retVal && archive.EndTag(s_selectedOptionIdTag);

	if (!isStoring){
		if (retVal){
			index = CalcIndexFromId(optionId, index);
		}
		else{
			index = NO_SELECTION;
			optionId.clear();
		}

		if ((index != m_selectedOptionIndex) || (optionId != m_selectedOptionId)){
			istd::CChangeNotifier notifier(this, &GetAllChanges());
			Q_UNUSED(notifier);

			m_selectedOptionId = optionId;
			m_selectedOptionIndex = index;
		}
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CSelectionParam::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	const ISelectionParam* sourcePtr = dynamic_cast<const ISelectionParam*>(&object);
	if (sourcePtr != NULL){
		int optionIndex = sourcePtr->GetSelectedOptionIndex();
		switch (mode)
		{
		case CM_WITHOUT_REFS:
			if (m_selectedOptionIndex != optionIndex){
				return SetSelectedOptionIndex(optionIndex);
			}
			break;

		case CM_WITH_REFS:
			{
				const iprm::IOptionsList* constraintsPtr = NULL;
				int selectedIndex = NO_SELECTION;
				QByteArray selectedId;

				const CSelectionParam* sourceImplPtr = dynamic_cast<const CSelectionParam*>(sourcePtr);
				if (sourceImplPtr != NULL){
					constraintsPtr = sourceImplPtr->m_constraintsPtr;
					selectedIndex = sourceImplPtr->m_selectedOptionIndex;
					selectedId = sourceImplPtr->m_selectedOptionId;
				}
				else{
					constraintsPtr = sourcePtr->GetSelectionConstraints();
					selectedIndex = sourcePtr->GetSelectedOptionIndex();

					if ((constraintsPtr != NULL) && (selectedIndex >= 0) && (selectedIndex < constraintsPtr->GetOptionsCount())){
						selectedId = constraintsPtr->GetOptionId(selectedIndex);
					}
				}

				if (constraintsPtr != m_constraintsPtr){
					istd::TDelPtr<IOptionsList> copiedConstraintsPtr;
					if (constraintsPtr != NULL){
						copiedConstraintsPtr.SetCastedOrRemove(constraintsPtr->CloneMe());
					}

					constraintsPtr = copiedConstraintsPtr.PopPtr();
				}

				istd::CChangeNotifier notifier(this, &GetAllChanges());

				m_selectedOptionIndex = selectedIndex;
				m_selectedOptionId = selectedId;

				SetSelectionConstraints(constraintsPtr);

				return true;
			}

		case CM_CONVERT:
			{
				int selectedIndex = NO_SELECTION;
				QByteArray selectedId;

				const CSelectionParam* sourceImplPtr = dynamic_cast<const CSelectionParam*>(sourcePtr);
				if (sourceImplPtr != NULL){
					selectedIndex = sourceImplPtr->m_selectedOptionIndex;
					selectedId = sourceImplPtr->m_selectedOptionId;
				}
				else{
					selectedIndex = sourcePtr->GetSelectedOptionIndex();

					const iprm::IOptionsList* constraintsPtr = sourcePtr->GetSelectionConstraints();
					if ((constraintsPtr != NULL) && (selectedIndex >= 0) && (selectedIndex < constraintsPtr->GetOptionsCount())){
						selectedId = constraintsPtr->GetOptionId(selectedIndex);
					}
				}

				istd::CChangeNotifier notifier(this, &GetAllChanges());

				m_selectedOptionIndex = selectedIndex;
				m_selectedOptionId = selectedId;

				SyncIndexWithId();

				return true;
			}

		default:
			break;
		}
	}

	return false;
}


istd::IChangeable* CSelectionParam::CloneMe(CompatibilityMode mode) const
{
	istd::TDelPtr<CSelectionParam> clonePtr(new CSelectionParam);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// protected methods

int CSelectionParam::CalcIndexFromId(const QByteArray& optionId, int suggestedIndex) const
{
	if (m_constraintsPtr != NULL){
		if (		(suggestedIndex >= 0) &&
					(suggestedIndex < m_constraintsPtr->GetOptionsCount()) &&
					(optionId == m_constraintsPtr->GetOptionId(suggestedIndex)) &&
					m_constraintsPtr->IsOptionEnabled(suggestedIndex)){
			return suggestedIndex;
		}

		int optionsCount = m_constraintsPtr->GetOptionsCount();
		for (int optionIndex = 0; optionIndex < optionsCount; optionIndex++){
			QByteArray id = m_constraintsPtr->GetOptionId(optionIndex);

			if ((optionId == id) && m_constraintsPtr->IsOptionEnabled(optionIndex)){
				return optionIndex;
			}
		}

		return NO_SELECTION;
	}

	return suggestedIndex;
}


bool CSelectionParam::SyncIndexWithId()
{
	int index = CalcIndexFromId(m_selectedOptionId, m_selectedOptionIndex);

	if (index != m_selectedOptionIndex){
		istd::CChangeNotifier notifier(this, &s_selectionCorrectionChangeSet);
		Q_UNUSED(notifier);

		m_selectedOptionIndex = index;
	}

	return m_selectedOptionId.isEmpty() || (m_selectedOptionIndex >= 0);
}


// public methods of the embedded class ConstraintsObserver

CSelectionParam::ConstraintsObserver::ConstraintsObserver(CSelectionParam& parent)
:	m_parent(parent)
{
}


// reimplemented (imod::TSingleModelObserverBase<iprm::IOptionsList>)

void CSelectionParam::ConstraintsObserver::OnUpdate(const istd::IChangeable::ChangeSet& changeSet)
{
	if (!changeSet.Contains(iprm::IOptionsList::CF_OPTIONS_CHANGED)){
		return;
	}

	Q_ASSERT(m_parent.m_constraintsPtr != NULL);

	m_parent.SyncIndexWithId();
}


} // namespace iprm


