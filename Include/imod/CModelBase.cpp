#include "imod/CModelBase.h"


namespace imod
{		


CModelBase::CModelBase()
{
}


CModelBase::~CModelBase()
{
	DetachAllObservers();
}


int CModelBase::GetObserverCount() const
{
	return int(m_observers.size());
}


IObserver* CModelBase::GetObserverPtr(int index) const
{
	I_ASSERT(index >= 0 && index < int(m_observers.size()));

	return m_observers.at(index);
}


bool CModelBase::AttachObserver(IObserver* observerPtr)
{
	if (observerPtr == NULL){
		return false;
	}

	for (int observerIndex = 0; observerIndex < int(m_observers.size()); observerIndex++){
		if (m_observers.at(observerIndex) == observerPtr){
			I_CRITICAL();

			return false;
		}
	}

	if (observerPtr->OnAttached(this)){
		m_observers.push_back(observerPtr);
		return true;
	}

	return false;
}


void CModelBase::DetachObserver(IObserver* observerPtr)
{
	for (int detachIndex = 0; detachIndex < int(m_observers.size()); detachIndex++){
		if (m_observers.at(detachIndex) == observerPtr){
			observerPtr->OnDetached(this);
			break;
		}
	}

	for (int eraseIndex = 0; eraseIndex < int(m_observers.size()); eraseIndex++){
		if (m_observers.at(eraseIndex) == observerPtr){
			m_observers.erase(m_observers.begin() + eraseIndex);
			return;
		}
	}

	I_ASSERT(false);
}


void CModelBase::DetachAllObservers()
{
	for (int observerIndex = 0; observerIndex < int(m_observers.size()); observerIndex++){
		imod::IObserver* observerPtr = m_observers.at(observerIndex);
		I_ASSERT(observerPtr != NULL);
		
		observerPtr->OnDetached(this);
	}

	m_observers.clear();
}


bool CModelBase::IsAttached(const IObserver* observerPtr) const
{
	for (int observerIndex = 0; observerIndex < int(m_observers.size()); observerIndex++){
		imod::IObserver* connectedObserverPtr = m_observers.at(observerIndex);
		I_ASSERT(connectedObserverPtr != NULL);

		if (connectedObserverPtr == observerPtr){
			return true;
		}
	}

	return false;
}


// protected methods

void CModelBase::NotifyBeforeUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	for (int observerIndex = 0; observerIndex < int(m_observers.size()); observerIndex++){
		imod::IObserver* observerPtr = m_observers.at(observerIndex);
		I_ASSERT(observerPtr != NULL);

		observerPtr->BeforeUpdate(this, updateFlags, updateParamsPtr);
	}
}


void CModelBase::NotifyAfterUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	for (int observerIndex = 0; observerIndex < int(m_observers.size()); observerIndex++){
		imod::IObserver* observerPtr = m_observers.at(observerIndex);
		I_ASSERT(observerPtr != NULL);

		observerPtr->AfterUpdate(this, updateFlags, updateParamsPtr);
	}
}


// private methods

CModelBase::CModelBase(const CModelBase& /*modelBase*/)
{
}


} // namespace imod

