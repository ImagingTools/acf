#include "imod/CModelBase.h"


// Qt includes
#include <QtCore/QList>
#include <QtCore/QSet>


namespace imod
{		


CModelBase::CModelBase()
:	m_notifyState(NS_NONE)
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
	Q_ASSERT_X(!m_pendingObservers.contains(observerPtr), "Attaching observer", "Pending observer is already connected to this model");

	ObserversMap& observersMap = (m_notifyState == NS_NONE)? m_observers: m_pendingObservers;

	AttachingState& state = observersMap[observerPtr];
	state = AS_ATTACHING;

	if (observerPtr->OnAttached(this)){
		if (m_notifyState > NS_NONE){
			observerPtr->BeforeUpdate(this, 0, NULL);
		}

		state = AS_ATTACHED;

		return true;
	}
	else{
		observersMap.remove(observerPtr);

		return false;
	}
}


void CModelBase::DetachObserver(IObserver* observerPtr)
{
	Q_ASSERT(observerPtr != NULL);
	Q_ASSERT_X(m_observers.contains(observerPtr) || m_pendingObservers.contains(observerPtr), "Detaching observer", "Unknown observer");

	// try to remove from current observer list
	ObserversMap::Iterator findIter = m_observers.find(observerPtr);
	if (findIter != m_observers.end()){
		AttachingState& state = findIter.value();
		if (state > AS_ATTACHED){
			qFatal("Observer is not attached");

			// Observer was already detached or is not correctly attached
			return;
		}

		if (state == AS_ATTACHED_UPDATING){
			IObserver* observerPtr = findIter.key();

			observerPtr->AfterUpdate(this, 0, NULL);
		}

		state = AS_DETACHING;

		observerPtr->OnDetached(this);
		
		state = AS_DETACHED;

		if (m_notifyState == NS_NONE){
			m_observers.erase(findIter);
		}

		return;
	}

	// try to remove from pending observer list
	findIter = m_pendingObservers.find(observerPtr);
	if (findIter != m_pendingObservers.end()){
		observerPtr->OnDetached(this);

		m_pendingObservers.erase(findIter);

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
			observerPtr->AfterUpdate(this, 0, NULL);
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

	for (ObserversMap::Iterator iter = m_pendingObservers.begin(); iter != m_pendingObservers.end(); ++iter){
		IObserver* observerPtr = iter.key();
		Q_ASSERT(observerPtr != NULL);

		observerPtr->OnDetached(this);
	}

	m_pendingObservers.clear();
}


bool CModelBase::IsAttached(const IObserver* observerPtr) const
{
	ObserversMap::ConstIterator findIter = m_observers.constFind(const_cast<IObserver*>(observerPtr));
	if (findIter != m_observers.end()){
		return findIter.value() < AS_DETACHING;
	}

	return m_pendingObservers.contains(const_cast<IObserver*>(observerPtr));
}


// protected methods

void CModelBase::NotifyBeforeUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	Q_ASSERT(m_notifyState == NS_NONE);

	m_notifyState = NS_SENDING_BEFORE;

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
}


// private methods

CModelBase::CModelBase(const CModelBase& /*modelBase*/)
{
}


void CModelBase::CleanupObserverState()
{
	if (!m_pendingObservers.isEmpty()){
		m_observers.unite(m_pendingObservers);
		m_pendingObservers.clear();
	}

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

