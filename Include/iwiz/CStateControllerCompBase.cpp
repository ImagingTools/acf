#include "iwiz/CStateControllerCompBase.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iwiz
{


CStateControllerCompBase::CStateControllerCompBase()
	:m_isStateActive(false),
	m_isStateEnabled(true),
	m_isEnterAllowed(true),
	m_isLeaveAllowed(true)
{
}


// reimplemented (iproc::IStateController)

bool CStateControllerCompBase::IsStateEnabled() const
{
	return m_isStateEnabled;
}


bool CStateControllerCompBase::IsStateActive() const
{
	return m_isStateActive;
}


bool CStateControllerCompBase::IsEnterAllowed(bool /*isActionAllowed*/, const IStateController* /*prevStatePtr*/) const
{
	return m_isEnterAllowed;
}


bool CStateControllerCompBase::IsLeaveAllowed(bool /*isActionAllowed*/, const IStateController* /*nextStatePtr*/) const
{
	return m_isLeaveAllowed;
}


bool CStateControllerCompBase::TryEnterState(bool /*isActionAllowed*/, const IStateController* /*prevStatePtr*/)
{
	I_ASSERT(m_isStateEnabled);

	if (IsEnterAllowed()){
		if (!m_isStateActive){
			istd::CChangeNotifier notifier(this);
			m_isStateActive = true;
		}

		return true;
	}

	return false;
}


bool CStateControllerCompBase::TryLeaveState(bool /*isActionAllowed*/, const IStateController* /*nextStatePtr*/)
{
	I_ASSERT(m_isStateEnabled);

	if (IsLeaveAllowed()){
		if (m_isStateActive){
			istd::CChangeNotifier notifier(this);
			m_isStateActive = false;
		}

		return true;
	}

	return false;
}


} // namespace iwiz

