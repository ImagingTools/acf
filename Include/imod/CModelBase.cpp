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
	Q_ASSERT(index >= 0 && index < int(m_observers.size()));

	return m_observers.at(index).observerPtr;
}


bool CModelBase::AttachObserver(IObserver* observerPtr)
{
	if (observerPtr == NULL){
		return false;
	}

	for (int observerIndex = 0; observerIndex < int(m_observers.size()); observerIndex++){
		if (m_observers.at(observerIndex).observerPtr == observerPtr){
			qFatal("Observer is already connected to this model");

			return false;
		}
	}

	ObserverInfo observerInfo;
	observerInfo.observerPtr = observerPtr;

	if (observerPtr->OnAttached(this)){
		observerInfo.connectionState = SF_CONNECTED;

		m_observers.push_back(observerInfo);

		return true;
	}

	return false;
}


void CModelBase::DetachObserver(IObserver* observerPtr)
{
	for (int detachIndex = 0; detachIndex < int(m_observers.size()); detachIndex++){
		ObserverInfo& observerInfo = m_observers[detachIndex];

		if (observerInfo.observerPtr == observerPtr){
			// Observer will be already detached:
			if (observerInfo.connectionState == SF_DETACHING_STAGE){
				return;
			}
			
			observerInfo.connectionState = SF_DETACHING_STAGE;

			observerPtr->OnDetached(this);
			
			break;
		}
	}

	for (int eraseIndex = 0; eraseIndex < int(m_observers.size()); eraseIndex++){
		ObserverInfo& observerInfo = m_observers[eraseIndex];
		if (observerInfo.observerPtr == observerPtr){
			m_observers.erase(m_observers.begin() + eraseIndex);
			
			return;
		}
	}

	qFatal("Observer doesn't exist");
}


void CModelBase::DetachAllObservers()
{
	for (int observerIndex = 0; observerIndex < int(m_observers.size()); observerIndex++){
		ObserverInfo& observerInfo = m_observers[observerIndex];

		Q_ASSERT(observerInfo.observerPtr != NULL);
		Q_ASSERT(observerInfo.connectionState != SF_DETACHING_STAGE);

		observerInfo.connectionState = SF_DETACHING_STAGE;
		
		observerInfo.observerPtr->OnDetached(this);
	}

	m_observers.clear();
}


bool CModelBase::IsAttached(const IObserver* observerPtr) const
{
	for (int observerIndex = 0; observerIndex < int(m_observers.size()); observerIndex++){
		const ObserverInfo& observerInfo = m_observers[observerIndex];
		Q_ASSERT(observerInfo.observerPtr != NULL);

		if ((observerInfo.observerPtr == observerPtr)){
			return true;
		}
	}

	return false;
}


// protected methods

void CModelBase::NotifyBeforeUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	for (int observerIndex = 0; observerIndex < int(m_observers.size()); observerIndex++){
		ObserverInfo& observerInfo = m_observers[observerIndex];
		Q_ASSERT(observerInfo.observerPtr != NULL);

		if (observerInfo.connectionState != SF_DETACHING_STAGE){
			observerInfo.observerPtr->BeforeUpdate(this, updateFlags, updateParamsPtr);
		}
	}
}


void CModelBase::NotifyAfterUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	for (int observerIndex = 0; observerIndex < int(m_observers.size()); observerIndex++){
		ObserverInfo& observerInfo = m_observers[observerIndex];
		Q_ASSERT(observerInfo.observerPtr != NULL);

		if (observerInfo.connectionState != SF_DETACHING_STAGE){
			observerInfo.observerPtr->AfterUpdate(this, updateFlags, updateParamsPtr);
		}
	}
}


// private methods

CModelBase::CModelBase(const CModelBase& /*modelBase*/)
{
}


} // namespace imod

