#include "imod/CModelBase.h"


// Qt includes
#include <QtCore/QSet>


namespace imod
{


CModelBase::CModelBase()
:	m_blockCounter(0),
	m_isDuringChanges(false)
{
}


CModelBase::~CModelBase()
{
	m_cumulatedChangeIds += istd::IChangeable::CF_DESTROYING;

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

	Q_ASSERT_X(!m_observers.contains(observerPtr) || (m_observers[observerPtr].state >= AS_DETACHING), "Attaching observer", "Observer is already connected to this model");

	ObserverInfo& info = m_observers[observerPtr];
	info.state = AS_ATTACHING;
	info.mask.Reset();

	if (observerPtr->OnModelAttached(this, info.mask)){
		info.state = AS_ATTACHED;

		// If the model already sent a notification about the begin of the transaction, do it also for the newly connected observer:
		if (m_isDuringChanges && m_cumulatedChangeIds.ContainsAny(info.mask)){
			observerPtr->BeforeUpdate(this);

			info.state = AS_ATTACHED_UPDATING;
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
		ObserverInfo& info = findIter.value();

		if (info.state >= AS_DETACHING){
			Q_ASSERT_X(false, "Detaching observer", "Observer was already detached");
			return;
		}

		if (info.state == AS_ATTACHED_UPDATING){
			IObserver* observerPtr = findIter.key();

			observerPtr->AfterUpdate(this, m_cumulatedChangeIds);
		}

		info.state = AS_DETACHING;

		observerPtr->OnModelDetached(this);
		
		info.state = AS_DETACHED;

		if (m_cumulatedChangeIds.IsEmpty()){
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

		ObserverInfo& info = iter.value();

		if (info.state == AS_ATTACHED_UPDATING){
			observerPtr->AfterUpdate(this, m_cumulatedChangeIds);
		}

		if (info.state < AS_DETACHING){
			info.state = AS_DETACHING;

			observerPtr->OnModelDetached(this);

			info.state = AS_DETACHED;
		}
	}

	if (m_cumulatedChangeIds.IsEmpty()){
		m_observers.clear();
	}
}


bool CModelBase::IsAttached(const IObserver* observerPtr) const
{
	ObserversMap::ConstIterator findIter = m_observers.constFind(const_cast<IObserver*>(observerPtr));
	if (findIter != m_observers.end()){
		const ObserverInfo& info = findIter.value();
		return info.state < AS_DETACHING;
	}

	return false;
}


// protected methods

void CModelBase::NotifyBeforeChange(const istd::IChangeable::ChangeSet& changeSet, bool isGroup)
{
	Q_ASSERT(m_blockCounter >= 0);
	Q_ASSERT((m_blockCounter > 0) || m_cumulatedChangeIds.IsEmpty());
	Q_ASSERT((m_blockCounter > 0) || !m_isDuringChanges);

	m_blockCounter++;

	if (changeSet.IsEmpty()){
		return;
	}

	m_cumulatedChangeIds += changeSet;

	if (isGroup){
		return;
	}

	bool isFirstChange = !m_isDuringChanges;
	m_isDuringChanges = true;

	for (ObserversMap::Iterator iter = m_observers.begin(); iter != m_observers.end(); ++iter){
		ObserverInfo& info = iter.value();

		if ((info.state == AS_ATTACHED) && m_cumulatedChangeIds.ContainsAny(info.mask)){
			info.state = AS_ATTACHED_UPDATING;

			IObserver* observerPtr = iter.key();

			observerPtr->BeforeUpdate(this);
		}
	}

	if (isFirstChange){
		OnBeginGlobalChanges();
	}
}


void CModelBase::NotifyAfterChange()
{
	Q_ASSERT(m_blockCounter > 0);

	// check if we are at end of outer change block
	if (--m_blockCounter > 0){
		return;	// no, it is not outer change block, nothing to do
	}

	if (m_isDuringChanges){	// if there were some global changes...
		m_isDuringChanges = false;

		OnEndGlobalChanges(m_cumulatedChangeIds);

		for (ObserversMap::Iterator iter = m_observers.begin(); iter != m_observers.end(); ++iter){
			ObserverInfo& info = iter.value();

			if (info.state == AS_ATTACHED_UPDATING){
				IObserver* observerPtr = iter.key();

				info.state = AS_ATTACHED;

				observerPtr->AfterUpdate(this, m_cumulatedChangeIds);
			}
		}
	}

	m_cumulatedChangeIds.Reset();	// we leave the outer block with clean state

	CleanupObserverState();
}


// private methods

CModelBase::CModelBase(const CModelBase& /*modelBase*/)
{
}


void CModelBase::CleanupObserverState()
{
	ObserversMap::Iterator iter = m_observers.begin();
	while (iter != m_observers.end()){
		ObserverInfo& info = iter.value();

		if (info.state == AS_DETACHED){
			iter = m_observers.erase(iter);
		}
		else{
			++iter;
		}
	}
}


} // namespace imod


