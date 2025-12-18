#include <iprm/CParameterStateProviderComp.h>


// ACF includes
#include <iprm/IParamsSet.h>
#include <iprm/IEnableableParam.h>
#include <iprm/TParamsPtr.h>


namespace iprm
{


// public methods

// reimplemented (iprm::IParameterStateProvider)

iprm::IParameterStateProvider::ParameterState CParameterStateProviderComp::GetState(
			const iprm::IParamsSet& paramSet,
			const QByteArray& parameterId,
			StateType stateType) const
{
	if (stateType == ST_EDIT){
		int index = m_controlledIdsAttrPtr.FindValue(parameterId);
		if (index >= 0){
			const iprm::IEnableableParam* stateParamPtr = dynamic_cast<const iprm::IEnableableParam*>(paramSet.GetParameter(*m_editActivationIdAttrPtr));
			if (stateParamPtr != NULL){
				bool isEnabled = stateParamPtr->IsEnabled();
				if (isEnabled){
					if (m_slaveStateProviderCompPtr.IsValid()){
						ParameterState slaveState = m_slaveStateProviderCompPtr->GetState(paramSet, parameterId, stateType);
						if (slaveState == PS_OFF){
							isEnabled = false;
						}
					}
				}

				return isEnabled ? PS_ON : PS_OFF;
			}
		}
	}

	if (m_slaveStateProviderCompPtr.IsValid()){
		return m_slaveStateProviderCompPtr->GetState(paramSet, parameterId, stateType);
	}

	return PS_UNKNOWN;
}


} // namespace iprm


