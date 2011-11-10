#include "iprm/CSelectionConstraintsComp.h"


namespace iprm
{


// reimplemented (iprm::ISelectionConstraints)

int CSelectionConstraintsComp::GetOptionsCount() const
{
	I_ASSERT(m_optionNamesAttrPtr.IsValid());

	return m_optionNamesAttrPtr.GetCount();
}


istd::CString CSelectionConstraintsComp::GetOptionName(int index) const
{
	I_ASSERT(m_optionNamesAttrPtr.GetCount() > index);
	I_ASSERT(index >= 0);

	return m_optionNamesAttrPtr[index];
}


istd::CString CSelectionConstraintsComp::GetOptionDescription(int index) const
{
	I_ASSERT(index >= 0);

	if (index < m_optionDescriptionsAttrPtr.GetCount()){
		return m_optionDescriptionsAttrPtr[index];
	}

	return istd::CString();
}


} // namespace iprm


