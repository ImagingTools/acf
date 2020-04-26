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
			iprm::TParamsPtr<iprm::IEnableableParam> stateParamPtr(&paramSet, *m_editActivationIdAttrPtr);
			if (stateParamPtr.IsValid()){
				return stateParamPtr->IsEnabled() ? PS_TRUE : PS_FALSE;
			}
		}
	}

	if (m_slaveStateProviderCompPtr.IsValid()){
		return m_slaveStateProviderCompPtr->GetState(paramSet, parameterId, stateType);
	}

	return PS_UNKNOWN;
}


} // namespace iprm


