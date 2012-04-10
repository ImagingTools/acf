#include "iprm/CSelectionParamComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


CSelectionParamComp::CSelectionParamComp()
:	m_selectedOptionIndex(-1)
{
}


// reimplemented (iprm::ISelectionParam)

const ISelectionConstraints* CSelectionParamComp::GetSelectionConstraints() const
{
	return m_constraintsCompPtr.GetPtr();
}


int CSelectionParamComp::GetSelectedOptionIndex() const
{
	return m_selectedOptionIndex;
}


bool CSelectionParamComp::SetSelectedOptionIndex(int index)
{
	if (m_constraintsCompPtr.IsValid() && (index >= m_constraintsCompPtr->GetOptionsCount())){
		return false;
	}

	if (m_selectedOptionIndex != index){
		istd::CChangeNotifier changeNotifier(this, CF_SELECTION_CHANGED);

		m_selectedOptionIndex = index;
	}

	return true;
}


ISelectionParam* CSelectionParamComp::GetActiveSubselection() const
{
	return m_subselectionCompPtr.GetPtr();
}


// reimplemented (iser::ISerializable)

bool CSelectionParamComp::Serialize(iser::IArchive& archive)
{
	int selectionOptionIndex = m_selectedOptionIndex;
	QByteArray selectedOptionId;

	static iser::CArchiveTag selectedOptionIndexTag("Index", "Selected option index");
	bool retVal = archive.BeginTag(selectedOptionIndexTag);
	retVal = retVal && archive.Process(selectionOptionIndex);
	retVal = retVal && archive.EndTag(selectedOptionIndexTag);

	if (m_constraintsCompPtr.IsValid()){
		int optionsCount = m_constraintsCompPtr->GetOptionsCount();

		if ((selectionOptionIndex >= 0) && (selectionOptionIndex < optionsCount)){
			selectedOptionId = m_constraintsCompPtr->GetOptionId(selectionOptionIndex);
		}
	}

	static iser::CArchiveTag selectedOptionIdTag("OptionId", "Selected option identifier");
	retVal = retVal && archive.BeginTag(selectedOptionIdTag);
	retVal = retVal && archive.Process(selectedOptionId);
	retVal = retVal && archive.EndTag(selectedOptionIdTag);

	if (retVal && !archive.IsStoring()){
		if (		m_constraintsCompPtr.IsValid() &&
					((m_constraintsCompPtr->GetConstraintsFlags() & iprm::ISelectionConstraints::SCF_SUPPORT_UNIQUE_ID) != 0) &&
					!selectedOptionId.isEmpty()){
			if (!SetSelectedOptionIndexById(selectedOptionId) && !SetSelectedOptionIndex(selectionOptionIndex)){
				SetSelectedOptionIndex(-1);
			}
		}
		else{
			SetSelectedOptionIndex(selectionOptionIndex);
		}
	}

	return retVal;
}


// protected methods

bool CSelectionParamComp::SetSelectedOptionIndexById(const QByteArray& selectedOptionId)
{
	I_ASSERT(!selectedOptionId.isEmpty());
	I_ASSERT(m_constraintsCompPtr.IsValid());

	if (m_constraintsCompPtr.IsValid()){
		int optionsCount = m_constraintsCompPtr->GetOptionsCount();

		for (int optionIndex = 0; optionIndex < optionsCount; optionIndex++){
			QByteArray optionId = m_constraintsCompPtr->GetOptionId(optionIndex);

			if (optionId == selectedOptionId){
				return SetSelectedOptionIndex(optionIndex);
			}
		}
	}

	return false;
}


// reimplemented (icomp::CComponentBase)

void CSelectionParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_selectedOptionIndex = *m_defaultIndexAttrPtr;
}


} // namespace iprm


