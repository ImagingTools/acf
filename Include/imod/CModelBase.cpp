#include "imod/CModelBase.h"


// Qt includes
#include <QtCore/QList>
#include <QtCore/QSet>


namespace imod
{		


CModelBase::CModelBase()
	:m_areObserversLocked(false)
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

	if (m_observers.contains(observerPtr) && (m_observers[observerPtr] != AS_DETACHED)){
		qFatal("Observer is already connected to this model");

		return false;
	}

	AttachingState& state = m_observers[observerPtr];
	state = AS_ATTACHING;

	if (observerPtr->OnAttached(this)){
		state = AS_ATTACHED;

		return true;
	}
	else{
		m_observers.remove(observerPtr);

		return false;
	}
}


void CModelBase::DetachObserver(IObserver* observerPtr)
{
	ObserversMap::Iterator findIter = m_observers.find(observerPtr);
	if (findIter == m_observers.end()){
		qFatal("Observer doesn't exist");

		return;
	}

	AttachingState& state = findIter.value();
	if (state != AS_ATTACHED){
		qFatal("Observer is not attached");

		// Observer was already detached or is not correctly attached
		return;
	}
	
	state = AS_DETACHING;

	observerPtr->OnDetached(this);
	
	state = AS_DETACHED;

	if (!IsInternalDataLocked()){
		m_observers.erase(findIter);
	}
}


void CModelBase::DetachAllObservers()
{
	for (ObserversMap::Iterator iter = m_observers.begin(); iter != m_observers.end(); ++iter){
		AttachingState& state = iter.value();
		if (state == AS_ATTACHED){
			state = AS_DETACHING;
		}
	}

	for (ObserversMap::Iterator iter = m_observers.begin(); iter != m_observers.end(); ++iter){
		AttachingState& state = iter.value();
		if (state == AS_DETACHING){
			IObserver* observerPtr = iter.key();
			Q_ASSERT(observerPtr != NULL);

			observerPtr->OnDetached(this);
		}

		state = AS_DETACHED;
	}

	m_observers.clear();
}


bool CModelBase::IsAttached(const IObserver* observerPtr) const
{
	ObserversMap::ConstIterator findIter = m_observers.constFind(const_cast<IObserver*>(observerPtr));
	if (findIter != m_observers.end()){
		return findIter.value() == AS_ATTACHED;
	}

	return false;
}


// protected methods

void CModelBase::NotifyBeforeUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	LockInternalData();

	for (ObserversMap::Iterator iter = m_observers.constBegin(); iter != m_observers.constEnd(); ++iter){
		AttachingState& state = iter.value();

		if (state == AS_ATTACHED){
			IObserver* observerPtr = iter.key();

			observerPtr->BeforeUpdate(this, updateFlags, updateParamsPtr);
		}
	}

	UnlockInternalData();
}


void CModelBase::NotifyAfterUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	LockInternalData();

	for (ObserversMap::Iterator iter = m_observers.constBegin(); iter != m_observers.constEnd(); ++iter){
		AttachingState& state = iter.value();

		if (state == AS_ATTACHED){
			IObserver* observerPtr = iter.key();

			observerPtr->AfterUpdate(this, updateFlags, updateParamsPtr);
		}
	}

	UnlockInternalData();
}


// private methods

CModelBase::CModelBase(const CModelBase& /*modelBase*/)
{
}


bool CModelBase::IsInternalDataLocked() const
{
	return m_areObserversLocked;
}


void CModelBase::LockInternalData()
{
	m_areObserversLocked = true;
}


void CModelBase::UnlockInternalData()
{
	m_areObserversLocked = false;

	CleanupDetachedObservers();
}


void CModelBase::CleanupDetachedObservers()
{
	for (ObserversMap::Iterator iter = m_observers.begin(); iter != m_observers.end(); ++iter){
		AttachingState& state = iter.value();

		if (state == AS_DETACHED){
			m_observers.erase(iter);

			iter = m_observers.begin();
		}
	}
}


} // namespace imod

