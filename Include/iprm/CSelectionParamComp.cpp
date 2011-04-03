#include "iprm/CSelectionParamComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


CSelectionParamComp::CSelectionParamComp()
:	m_selectedOptionIndex(0)
{
}


// reimplemented (iprm::ISelectionParam)

const ISelectionConstraints* CSelectionParamComp::GetConstraints() const
{
	return m_constraintsCompPtr.GetPtr();
}


int CSelectionParamComp::GetSelectedOptionIndex() const
{
	return m_selectedOptionIndex;
}


bool CSelectionParamComp::SetSelectedOptionIndex(int index)
{
	I_ASSERT(index > 0);

	if (m_constraintsCompPtr.IsValid() && (index >= m_constraintsCompPtr->GetOptionsCount())){
		return false;
	}

	if (m_selectedOptionIndex != index){
		istd::CChangeNotifier changeNotifier(this);

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
	static iser::CArchiveTag selectedOptionIndexTag("Index", "Selected option index");
	bool retVal = archive.BeginTag(selectedOptionIndexTag);
	retVal = retVal && archive.Process(m_selectedOptionIndex);
	retVal = retVal && archive.EndTag(selectedOptionIndexTag);

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CSelectionParamComp::OnComponentCreated()
{
	m_selectedOptionIndex = *m_defaultIndexAttrPtr;
}


} // namespace iprm


