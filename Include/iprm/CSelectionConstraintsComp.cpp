#include "iprm/CSelectionConstraintsComp.h"


namespace iprm
{


// reimplemented (iprm::IOptionsList)

int CSelectionConstraintsComp::GetOptionsFlags() const
{
	return SCF_NONE;
}


int CSelectionConstraintsComp::GetOptionsCount() const
{
	Q_ASSERT(m_optionNamesAttrPtr.IsValid());

	return m_optionNamesAttrPtr.GetCount();
}


QString CSelectionConstraintsComp::GetOptionName(int index) const
{
	Q_ASSERT(m_optionNamesAttrPtr.GetCount() > index);
	Q_ASSERT(index >= 0);

	return m_optionNamesAttrPtr[index];
}


QString CSelectionConstraintsComp::GetOptionDescription(int index) const
{
	Q_ASSERT(index >= 0);

	if (index < m_optionDescriptionsAttrPtr.GetCount()){
		return m_optionDescriptionsAttrPtr[index];
	}

	return QString();
}


QByteArray CSelectionConstraintsComp::GetOptionId(int index) const
{
	Q_ASSERT(index >= 0);

	if (index < m_optionIdsAttrPtr.GetCount()){
		return m_optionIdsAttrPtr[index];
	}

	return QByteArray();
}


bool CSelectionConstraintsComp::IsOptionEnabled(int /*index*/) const
{
	return true;
}


} // namespace iprm


