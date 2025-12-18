#include <iprm/CSelectionParamComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>

#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


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

	int selectedIndex = *m_defaultIndexAttrPtr;
	if (m_constraintsCompPtr.IsValid() && (selectedIndex >= m_constraintsCompPtr->GetOptionsCount())){
		selectedIndex = -1;
	}

	SetSelectedOptionIndex(selectedIndex);
}


} // namespace iprm


