#include "iprm/CSelectionParam.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "iprm/ISelectionConstraints.h"


namespace iprm
{


CSelectionParam::CSelectionParam()
:	m_selectedOptionIndex(-1),
	m_constraintsPtr(NULL)
{
}


void CSelectionParam::SetSelectionConstraints(const ISelectionConstraints* constraintsPtr)
{
	m_constraintsPtr = constraintsPtr;
}


bool CSelectionParam::SetSelectedOptionById(const QByteArray& selectedOptionId)
{
	I_ASSERT(!selectedOptionId.isEmpty());
	I_ASSERT(m_constraintsPtr != NULL);

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


// reimplemented (iprm::ISelectionParam)

const ISelectionConstraints* CSelectionParam::GetSelectionConstraints() const
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
		if ((m_constraintsPtr != NULL) && !m_constraintsPtr->IsOptionEnabled(index)){
			return false;
		}

		istd::CChangeNotifier changeNotifier(this, CF_SELECTION_CHANGED | CF_MODEL);

		m_selectedOptionIndex = index;
	}

	return true;
}


ISelectionParam* CSelectionParam::GetActiveSubselection() const
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CSelectionParam::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag selectedOptionIndexTag("Index", "Selected option index");
	static iser::CArchiveTag selectedOptionIdTag("OptionId", "Selected option identifier");

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
					((m_constraintsPtr->GetConstraintsFlags() & iprm::ISelectionConstraints::SCF_SUPPORT_UNIQUE_ID) != 0) &&
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


