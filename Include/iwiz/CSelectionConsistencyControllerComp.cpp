#include "iwiz/CSelectionConsistencyControllerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iprm/ISelectionConstraints.h"


namespace iwiz
{


// reimplemented (iproc::IStateController)

bool CSelectionConsistencyControllerComp::IsStateEnabled() const
{
	EnsureCacheValid();

	return m_isStateEnabled;
}


bool CSelectionConsistencyControllerComp::IsStateActive() const
{
	return m_isStateActive;
}


bool CSelectionConsistencyControllerComp::IsEnterAllowed(bool /*isActionAllowed*/, const IStateController* /*prevStatePtr*/) const
{
	EnsureCacheValid();

	return m_isEnterAllowed;
}


bool CSelectionConsistencyControllerComp::IsLeaveAllowed(bool /*isActionAllowed*/, const IStateController* /*nextStatePtr*/) const
{
	EnsureCacheValid();

	return m_isLeaveAllowed;
}


bool CSelectionConsistencyControllerComp::TryEnterState(bool /*isActionAllowed*/, const IStateController* /*prevStatePtr*/)
{
	if (IsEnterAllowed()){
		if (!m_isStateActive){
			istd::CChangeNotifier notifier(this);
			m_isStateActive = true;
		}

		return true;
	}

	return false;
}


bool CSelectionConsistencyControllerComp::TryLeaveState(bool /*isActionAllowed*/, const IStateController* /*nextStatePtr*/)
{
	if (IsLeaveAllowed()){
		if (m_isStateActive){
			istd::CChangeNotifier notifier(this);
			m_isStateActive = false;
		}

		return true;
	}

	return false;
}


// reimplemented (icomp::CComponentBase)

void CSelectionConsistencyControllerComp::OnComponentCreated()
{
	m_isStateActive = false;

	m_isCacheValid = false;

	m_isStateEnabled = false;
	m_isEnterAllowed = false;
	m_isLeaveAllowed = false;

	BaseClass::OnComponentCreated();

	int enablingParamsCount = m_enablingParamModelsCompPtr.GetCount();
	for (int i = 0; i < enablingParamsCount; ++i){
		imod::IModel* modelPtr = m_enablingParamModelsCompPtr[i];
		if (modelPtr != NULL){
			modelPtr->AttachObserver(this);
		}
	}

	int enterCondParamsCount = m_enterCondParamModelsCompPtr.GetCount();
	for (int i = 0; i < enterCondParamsCount; ++i){
		imod::IModel* modelPtr = m_enterCondParamModelsCompPtr[i];
		if (modelPtr != NULL){
			modelPtr->AttachObserver(this);
		}
	}

	int leaveCondParamsCount = m_leaveCondParamModelsCompPtr.GetCount();
	for (int i = 0; i < leaveCondParamsCount; ++i){
		imod::IModel* modelPtr = m_leaveCondParamModelsCompPtr[i];
		if (modelPtr != NULL){
			modelPtr->AttachObserver(this);
		}
	}
}


void CSelectionConsistencyControllerComp::OnComponentDestroyed()
{
	int leaveCondParamsCount = m_leaveCondParamModelsCompPtr.GetCount();
	for (int i = 0; i < leaveCondParamsCount; ++i){
		imod::IModel* modelPtr = m_leaveCondParamModelsCompPtr[i];
		if ((modelPtr != NULL) && modelPtr->IsAttached(this)){
			modelPtr->DetachObserver(this);
		}
	}

	int enterCondParamsCount = m_enterCondParamModelsCompPtr.GetCount();
	for (int i = 0; i < enterCondParamsCount; ++i){
		imod::IModel* modelPtr = m_enterCondParamModelsCompPtr[i];
		if ((modelPtr != NULL) && modelPtr->IsAttached(this)){
			modelPtr->DetachObserver(this);
		}
	}

	int enablingParamsCount = m_enablingParamModelsCompPtr.GetCount();
	for (int i = 0; i < enablingParamsCount; ++i){
		imod::IModel* modelPtr = m_enablingParamModelsCompPtr[i];
		if ((modelPtr != NULL) && modelPtr->IsAttached(this)){
			modelPtr->DetachObserver(this);
		}
	}

	BaseClass::OnComponentDestroyed();
}


// protected methods

void CSelectionConsistencyControllerComp::EnsureCacheValid() const
{
	if (!m_isCacheValid){
		m_isStateEnabled = true;
		m_isEnterAllowed = true;
		m_isLeaveAllowed = true;

		int enablingParamsCount = m_enablingParamsCompPtr.GetCount();
		for (int i = 0; i < enablingParamsCount; ++i){
			const iprm::ISelectionParam* paramPtr = m_enablingParamsCompPtr[i];
			if ((paramPtr != NULL) && !CheckParamConsistency(*paramPtr)){
				m_isStateEnabled = false;
				break;
			}
		}

		int enterCondParamsCount = m_enterCondParamsCompPtr.GetCount();
		for (int i = 0; i < enterCondParamsCount; ++i){
			const iprm::ISelectionParam* paramPtr = m_enterCondParamsCompPtr[i];
			if ((paramPtr != NULL) && !CheckParamConsistency(*paramPtr)){
				m_isEnterAllowed = false;
				break;
			}
		}

		int leaveCondParamsCount = m_leaveCondParamsCompPtr.GetCount();
		for (int i = 0; i < leaveCondParamsCount; ++i){
			const iprm::ISelectionParam* paramPtr = m_leaveCondParamsCompPtr[i];
			if ((paramPtr != NULL) && !CheckParamConsistency(*paramPtr)){
				m_isLeaveAllowed = false;
				break;
			}
		}

		m_isCacheValid = true;
	}
}


bool CSelectionConsistencyControllerComp::CheckParamConsistency(const iprm::ISelectionParam& param) const
{
	int index = param.GetSelectedOptionIndex();

	if (index < 0){
		return false;
	}

	const iprm::ISelectionConstraints* constraintsPtr = param.GetSelectionConstraints();
	if (constraintsPtr != NULL){
		if (index >= constraintsPtr->GetOptionsCount()){
			return false;
		}
	}

	return true;
}


// reimplemented (imod::CMultiModelObserverBase)

void CSelectionConsistencyControllerComp::OnUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_isCacheValid = false;
}


} // namespace iwiz


