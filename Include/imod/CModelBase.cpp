#include "imod/CModelBase.h"


// Qt includes
#include <QtCore/QSet>


namespace imod
{		


CModelBase::CModelBase()
:	m_notifyState(NS_NONE),
	m_notifyFlags(0)
{
}


CModelBase::~CModelBase()
{
	DetachAllObservers();
}


int CModelBase::GetObserverCount() const
{
	return m_observers.size();
}


CModelBase::Observers CModelBase::GetObservers() const
{
	return m_observers.keys().toSet();
}


// reimplemented (imod::IModel)

bool CModelBase::AttachObserver(IObserver* observerPtr)
{
	if (observerPtr == NULL){
		return false;
	}

	Q_ASSERT_X(!m_observers.contains(observerPtr) || (m_observers[observerPtr] >= AS_DETACHING), "Attaching observer", "Observer is already connected to this model");

	AttachingState& state = m_observers[observerPtr];
	state = AS_ATTACHING;

	if (observerPtr->OnAttached(this)){
		state = AS_ATTACHED;

		// If the model already sent a notification about the begin of the transaction, do it also for the newly connected observer:
		if (m_notifyState > NS_NONE){
			observerPtr->BeforeUpdate(this, m_notifyFlags, NULL);

			state = AS_ATTACHED_UPDATING;
		}

		// If the model already sent a notification about the end of the transaction, do it also for the newly connected observer:
		if (m_notifyState > NS_UPDATE){
			observerPtr->AfterUpdate(this, m_notifyFlags, NULL);

			state = AS_ATTACHED;
		}

		return true;
	}
	else{
		m_observers.remove(observerPtr);

		return false;
	}
}


void CModelBase::DetachObserver(IObserver* observerPtr)
{
	Q_ASSERT(observerPtr != NULL);

	// try to remove from current observer list
	ObserversMap::Iterator findIter = m_observers.find(observerPtr);
	if (findIter != m_observers.end()){
		AttachingState& state = findIter.value();

		if (state >= AS_DETACHING){
			Q_ASSERT_X(false, "Detaching observer", "Observer was already detached");
			return;
		}

		if (state == AS_ATTACHED_UPDATING){
			IObserver* observerPtr = findIter.key();

			observerPtr->AfterUpdate(this, m_notifyFlags, NULL);
		}

		state = AS_DETACHING;

		observerPtr->OnDetached(this);
		
		state = AS_DETACHED;

		if (m_notifyState == NS_NONE){
			m_observers.erase(findIter);
		}

		return;
	}
}


void CModelBase::DetachAllObservers()
{
	for (ObserversMap::Iterator iter = m_observers.begin(); iter != m_observers.end(); ++iter){
		IObserver* observerPtr = iter.key();
		Q_ASSERT(observerPtr != NULL);

		AttachingState& state = iter.value();

		if (state == AS_ATTACHED_UPDATING){
			observerPtr->AfterUpdate(this, m_notifyFlags, NULL);
		}

		if (state < AS_DETACHING){
			state = AS_DETACHING;

			observerPtr->OnDetached(this);

			state = AS_DETACHED;
		}
	}

	if (m_notifyState == NS_NONE){
		m_observers.clear();
	}
}


bool CModelBase::IsAttached(const IObserver* observerPtr) const
{
	ObserversMap::ConstIterator findIter = m_observers.constFind(const_cast<IObserver*>(observerPtr));
	if (findIter != m_observers.end()){
		return findIter.value() < AS_DETACHING;
	}

	return false;
}


// protected methods

void CModelBase::NotifyBeforeUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	m_notifyState = NS_SENDING_BEFORE;
	m_notifyFlags = updateFlags;

	for (ObserversMap::Iterator iter = m_observers.begin(); iter != m_observers.end(); ++iter){
		AttachingState& state = iter.value();

		if (state == AS_ATTACHED){
			state = AS_ATTACHED_UPDATING;

			IObserver* observerPtr = iter.key();

			observerPtr->BeforeUpdate(this, updateFlags, updateParamsPtr);
		}
	}

	m_notifyState = NS_UPDATE;
}


void CModelBase::NotifyAfterUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	Q_ASSERT(m_notifyState == NS_UPDATE);

	m_notifyState = NS_SENDING_AFTER;
	m_notifyFlags = updateFlags;

	for (ObserversMap::Iterator iter = m_observers.begin(); iter != m_observers.end(); ++iter){
		AttachingState& state = iter.value();

		if (state == AS_ATTACHED_UPDATING){
			IObserver* observerPtr = iter.key();

			state = AS_ATTACHED;

			observerPtr->AfterUpdate(this, updateFlags, updateParamsPtr);
		}
	}	
	
	CleanupObserverState();

	m_notifyState = NS_NONE;
	m_notifyFlags = 0;
}


// private methods

CModelBase::CModelBase(const CModelBase& /*modelBase*/)
{
}


void CModelBase::CleanupObserverState()
{
	ObserversMap::Iterator iter = m_observers.begin();
	while (iter != m_observers.end()){
		AttachingState& state = iter.value();

		if (state == AS_DETACHED){
			iter = m_observers.erase(iter);
		}
		else{
			++iter;
		}
	}
}


} // namespace imod


