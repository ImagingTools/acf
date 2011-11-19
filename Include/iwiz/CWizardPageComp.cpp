#include "iwiz/CWizardPageComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iwiz
{


// public methods

CWizardPageComp::CWizardPageComp()
:	m_isStateEnabled(true),
	m_isStateActive(false)
{
}


// reimplemented (iproc::IStateController)

bool CWizardPageComp::IsStateEnabled() const
{
	return m_isStateEnabled;
}


bool CWizardPageComp::IsStateActive() const
{
	return m_isStateActive;
}


bool CWizardPageComp::IsEnterAllowed(bool isActionAllowed, const IStateController* prevStatePtr) const
{
	if (m_isStateActive){
		return false;
	}

	if (isActionAllowed && m_processOnEnterCompPtr.IsValid()){
		if (		(m_processOnEnterCompPtr->GetProcessorState(this) != iproc::IProcessor::PS_READY) ||
					!m_processOnEnterCompPtr->AreParamsAccepted(this, NULL, NULL)){
			return false;
		}
	}

	int slavesCount = m_slaveControllersCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		const iproc::IStateController* slaveConstrollerPtr = m_slaveControllersCompPtr[i];
		if ((slaveConstrollerPtr != NULL) && !slaveConstrollerPtr->IsEnterAllowed(isActionAllowed)){
			return false;	// if any subcontroller block entering this state, it will be blocked
		}
	}

	return true;
}


bool CWizardPageComp::IsLeaveAllowed(bool isActionAllowed, const IStateController* nextStatePtr) const
{
	if (!m_isStateActive){
		return false;
	}

	if (isActionAllowed && m_processOnLeaveCompPtr.IsValid()){
		if (		(m_processOnLeaveCompPtr->GetProcessorState(this) != iproc::IProcessor::PS_READY) ||
					!m_processOnLeaveCompPtr->AreParamsAccepted(this, NULL, NULL)){
			return false;
		}
	}

	int slavesCount = m_slaveControllersCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		const iproc::IStateController* slaveConstrollerPtr = m_slaveControllersCompPtr[i];
		if ((slaveConstrollerPtr != NULL) && !slaveConstrollerPtr->IsEnterAllowed(isActionAllowed)){
			return false;	// if any subcontroller block leaving this state, it will be blocked
		}
	}

	return true;
}


bool CWizardPageComp::TryEnterState(bool isActionAllowed, const IStateController* /*prevStatePtr*/)
{
	if (m_isStateActive){
		return false;
	}

	if (isActionAllowed && m_processOnEnterCompPtr.IsValid()){
		if (		(m_processOnEnterCompPtr->GetProcessorState(this) != iproc::IProcessor::PS_READY) ||
					!m_processOnEnterCompPtr->AreParamsAccepted(this, NULL, NULL) ||
					(m_processOnEnterCompPtr->DoProcessing(this, NULL, NULL)  != iproc::IProcessor::TS_OK)){
			return false;
		}
	}

	istd::CChangeNotifier notifier(this, CF_STATE_ENTERED);

	m_isStateActive = true;

	return true;
}


bool CWizardPageComp::TryLeaveState(bool isActionAllowed, const IStateController* /*prevStatePtr*/)
{
	if (!m_isStateActive){
		return false;
	}

	if (isActionAllowed && m_processOnLeaveCompPtr.IsValid()){
		if (		(m_processOnLeaveCompPtr->GetProcessorState(this) != iproc::IProcessor::PS_READY) ||
					!m_processOnLeaveCompPtr->AreParamsAccepted(this, NULL, NULL) ||
					(m_processOnLeaveCompPtr->DoProcessing(this, NULL, NULL)  != iproc::IProcessor::TS_OK)){
			return false;
		}
	}

	istd::CChangeNotifier notifier(this, CF_STATE_LEAVED);

	m_isStateActive = false;

	return true;
}


// protected methods

void CWizardPageComp::UpdateAllMembers()
{
	bool isEnabled = false;

	int slavesCount = m_slaveControllersCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		const iproc::IStateController* slaveConstrollerPtr = m_slaveControllersCompPtr[i];
		if ((slaveConstrollerPtr != NULL) && slaveConstrollerPtr->IsStateEnabled()){
			isEnabled = true;
		}
	}

	m_isStateEnabled = isEnabled;
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CWizardPageComp::OnModelChanged(int /*modelId*/, int changeFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if ((changeFlags & (CF_STATE_ENTERED | CF_STATE_LEAVED | CF_GRAPH_CHANGED)) != 0){
		istd::CChangeNotifier notifier(this, changeFlags);

		UpdateAllMembers();
	}
}


// reimplemented (icomp::CComponentBase)

void CWizardPageComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int modelsCount = m_slaveControllerModelsCompPtr.GetCount();
	for (int i = 0; i < modelsCount; ++i){
		imod::IModel* parameterModelPtr = m_slaveControllerModelsCompPtr[i];
		if (parameterModelPtr != NULL){
			RegisterModel(parameterModelPtr, i);
		}
	}

	UpdateAllMembers();
}


void CWizardPageComp::OnComponentDestroyed()
{
	BaseClass2::UnregisterAllModels();

	BaseClass::OnComponentDestroyed();
}


} // namespace iprm


