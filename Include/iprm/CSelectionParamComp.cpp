#include "iprm/CSelectionParamComp.h"


// ACF includes
#include "istd/CChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


// reimplemented (iprm::ISelectionParam)

ISelectionParam* CSelectionParamComp::GetSubselection(int /*index*/) const
{
	return m_subselectionCompPtr.GetPtr();
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CSelectionParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetSelectionConstraints(m_constraintsCompPtr.GetPtr());

	bool defaultIndexValid = true;
	if (m_constraintsCompPtr.IsValid()){
		defaultIndexValid = (*m_defaultIndexAttrPtr < m_constraintsCompPtr->GetOptionsCount());
	}

	if (defaultIndexValid){
		m_selectedOptionIndex = *m_defaultIndexAttrPtr;
	}
}


} // namespace iprm


