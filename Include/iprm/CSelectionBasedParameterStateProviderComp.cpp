#include <iprm/CSelectionBasedParameterStateProviderComp.h>


// ACF includes
#include <iprm/IParamsSet.h>
#include <iprm/ISelectionParam.h>
#include <iprm/TParamsPtr.h>


namespace iprm
{


// public methods

// reimplemented (iprm::IParameterStateProvider)

iprm::IParameterStateProvider::ParameterState CSelectionBasedParameterStateProviderComp::GetState(
			const iprm::IParamsSet& paramSet,
			const QByteArray& parameterId,
			StateType stateType) const
{
	if (stateType == ST_EDIT){
		int index = m_controlledIdsAttrPtr.FindValue(parameterId);
		if (index >= 0){
			const iprm::ISelectionParam* selectorPtr = dynamic_cast<const iprm::ISelectionParam*>(paramSet.GetParameter(*m_editSelectorIdAttrPtr));
			if (selectorPtr != NULL){
				int selectedIndex = selectorPtr->GetSelectedOptionIndex();

				return m_editActiveIndexesAttrPtr.FindValue(selectedIndex) >= 0 ? PS_ON : PS_OFF;
			}
		}
	}

	if (m_slaveStateProviderCompPtr.IsValid()){
		return m_slaveStateProviderCompPtr->GetState(paramSet, parameterId, stateType);
	}

	return PS_UNKNOWN;
}


} // namespace iprm


