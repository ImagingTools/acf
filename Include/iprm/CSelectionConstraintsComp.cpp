#include "iprm/CSelectionConstraintsComp.h"


namespace iprm
{


// reimplemented (iprm::ISelectionConstraints)

int CSelectionConstraintsComp::GetConstraintsFlags() const
{
	return SCF_NONE;
}


int CSelectionConstraintsComp::GetOptionsCount() const
{
	I_ASSERT(m_optionNamesAttrPtr.IsValid());

	return m_optionNamesAttrPtr.GetCount();
}


QString CSelectionConstraintsComp::GetOptionName(int index) const
{
	I_ASSERT(m_optionNamesAttrPtr.GetCount() > index);
	I_ASSERT(index >= 0);

	return m_optionNamesAttrPtr[index];
}


QString CSelectionConstraintsComp::GetOptionDescription(int index) const
{
	I_ASSERT(index >= 0);

	if (index < m_optionDescriptionsAttrPtr.GetCount()){
		return m_optionDescriptionsAttrPtr[index];
	}

	return QString();
}


std::string CSelectionConstraintsComp::GetOptionId(int /*index*/) const
{
	return std::string();
}


} // namespace iprm


