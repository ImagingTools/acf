#include "iprm/CSelectionParam.h"


// ACF includes
#include "istd/CChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "iprm/IOptionsList.h"


namespace iprm
{


CSelectionParam::CSelectionParam()
:	m_selectedOptionIndex(-1),
	m_constraintsPtr(NULL)
{
}


void CSelectionParam::SetSelectionConstraints(const IOptionsList* constraintsPtr)
{
	m_constraintsPtr = constraintsPtr;
}


void CSelectionParam::SetSubselection(int selectionIndex, ISelectionParam* selectionPtr)
{
	m_subselectionMap[selectionIndex] = selectionPtr;
}


bool CSelectionParam::SetSelectedOptionById(const QByteArray& selectedOptionId)
{
	Q_ASSERT(!selectedOptionId.isEmpty());
	Q_ASSERT(m_constraintsPtr != NULL);

	if (m_constraintsPtr != NULL){
		int optionsCount = m_constraintsPtr->GetOptionsCount();

		for (int optionIndex = 0; optionIndex < optionsCount; optionIndex++){
			QByteArray optionId = m_constraintsPtr->GetOptionId(optionIndex);

			if (optionId == selectedOptionId && m_constraintsPtr->IsOptionEnabled(optionIndex)){
				return SetSelectedOptionIndex(optionIndex);
			}
		}
	}

	return false;
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

	if (m_selectedOptionIndex != index){
		if (index != iprm::ISelectionParam::NO_SELECTION){
			if ((m_constraintsPtr != NULL) && !m_constraintsPtr->IsOptionEnabled(index)){
				return false;
			}
		}

		ChangeSet changeSet(CF_SELECTION_CHANGED);
		istd::CChangeNotifier changeNotifier(this, changeSet);

		m_selectedOptionIndex = index;
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
	static iser::CArchiveTag selectedOptionIndexTag("Index", "Selected option index", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag selectedOptionIdTag("OptionId", "Selected option identifier", iser::CArchiveTag::TT_LEAF);

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	int selectionOptionIndex = m_selectedOptionIndex;
	QByteArray selectedOptionId;

	bool retVal = archive.BeginTag(selectedOptionIndexTag);
	retVal = retVal && archive.Process(selectionOptionIndex);
	retVal = retVal && archive.EndTag(selectedOptionIndexTag);

	if (m_constraintsPtr != NULL){
		int optionsCount = m_constraintsPtr->GetOptionsCount();

		if ((selectionOptionIndex >= 0) && (selectionOptionIndex < optionsCount)){
			selectedOptionId = m_constraintsPtr->GetOptionId(selectionOptionIndex);
		}
	}

	retVal = retVal && archive.BeginTag(selectedOptionIdTag);
	retVal = retVal && archive.Process(selectedOptionId);
	retVal = retVal && archive.EndTag(selectedOptionIdTag);

	if (retVal && !isStoring){
		if (		(m_constraintsPtr != NULL) &&
					((m_constraintsPtr->GetOptionsFlags() & iprm::IOptionsList::SCF_SUPPORT_UNIQUE_ID) != 0) &&
					!selectedOptionId.isEmpty()){
			if (!SetSelectedOptionById(selectedOptionId) && !SetSelectedOptionIndex(selectionOptionIndex)){
				SetSelectedOptionIndex(-1);
			}
		}
		else{
			SetSelectedOptionIndex(selectionOptionIndex);
		}
	}

	return retVal;
}


} // namespace iprm


