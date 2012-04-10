#ifndef iproc_TMemCachedProducerComp_included
#define iproc_TMemCachedProducerComp_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include "iproc/TILockedProducer.h"


namespace iproc
{


/**
	Template implementation iproc::TILockedProducer buffering objects in memory cache.
	As a source of produced object instances some slave implementation is used.
*/
template <class Key, class CacheObject>
class TMemCachedProducerComp:
			public icomp::CComponentBase,
			virtual public TILockedProducer<Key, CacheObject>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef TILockedProducer<Key, CacheObject> LockedProducerType;

	I_BEGIN_BASE_COMPONENT(TMemCachedProducerComp);
		I_REGISTER_INTERFACE(LockedProducerType);
		I_TASSIGN(m_slaveCacheEngineCompPtr, "SlaveCacheEngine", "Slave cache engine providing access to cached object", true, "SlaveCacheEngine");
		I_ASSIGN(m_maxCachedObjectsAttrPtr, "MaxCachedObjects", "Maximal number of cached objects", true, 20);
	I_END_COMPONENT;

	// reimplemented (iproc::TILockedProducer)
	virtual const CacheObject* ProduceLockedObject(const Key& key);
	virtual void UnlockObject(const CacheObject* objectPtr);

protected:
	/**
		Remove elements from list if cumulated weight is above defined maximum.
	*/
	void CleanElementList();

private:
	I_TREF(LockedProducerType, m_slaveCacheEngineCompPtr);
	I_ATTR(int, m_maxCachedObjectsAttrPtr);

	struct ListElement
	{
		bool operator==(const Key& key)
		{
			return this->key == key;
		}

		Key key;
		const CacheObject* objectPtr;
		int lockedCount;
	};

	typedef QList<ListElement> CachedList;
	typedef QMap<const CacheObject*, typename CachedList::reverse_iterator> ObjectToListMap;

	CachedList m_cachedList;
	ObjectToListMap m_objectToListMap;
};


// public methods

// reimplemented (iproc::TILockedProducer)

template <class Key, class CacheObject>
const CacheObject* TMemCachedProducerComp<Key, CacheObject>::ProduceLockedObject(const Key& key)
{
	typename CachedList::iterator foundIter = qFind(m_cachedList.begin(), m_cachedList.end(), key);
	if (foundIter != m_cachedList.end()){
		foundIter->lockedCount++;

		return foundIter->objectPtr;
	}

	if (m_slaveCacheEngineCompPtr.IsValid()){
		const CacheObject* objectPtr = m_slaveCacheEngineCompPtr->ProduceLockedObject(key);
		if (objectPtr != NULL){
			m_cachedList.push_back(ListElement());

			ListElement& element = m_cachedList.back();
			m_objectToListMap[objectPtr] = m_cachedList.rbegin();

			element.key = key;
			element.objectPtr = objectPtr;
			element.lockedCount = 1;

			CleanElementList();
			
			return objectPtr;
		}
	}

	return NULL;
}


template <class Key, class CacheObject>
void TMemCachedProducerComp<Key, CacheObject>::UnlockObject(const CacheObject* objectPtr)
{
	typename ObjectToListMap::iterator foundIter = m_objectToListMap.find(objectPtr);
	I_ASSERT(foundIter != m_objectToListMap.end());	// if locked is done correctly, this element must exist.

	typename CachedList::reverse_iterator objectIter = foundIter.value();
	I_ASSERT(objectIter != m_cachedList.rend());

	objectIter->lockedCount--;

	CleanElementList();
}


// protected methods

template <class Key, class CacheObject>
void TMemCachedProducerComp<Key, CacheObject>::CleanElementList()
{
	int maxCachedObjects = qMax(0, *m_maxCachedObjectsAttrPtr);

	typename CachedList::iterator iter = m_cachedList.begin();
	while (		(int(m_cachedList.size()) > maxCachedObjects) &&
				(iter != m_cachedList.end())){
		I_ASSERT(m_objectToListMap.find(iter->objectPtr) != m_objectToListMap.end());	// object is present in objects map

		if (iter->lockedCount <= 0){
			I_ASSERT(m_slaveCacheEngineCompPtr.IsValid());

			m_slaveCacheEngineCompPtr->UnlockObject(iter->objectPtr);

			m_objectToListMap.erase(iter->objectPtr);
			iter = m_cachedList.erase(iter);
		}
		else{
			++iter;
		}
	}

	I_ASSERT(m_cachedList.size() == m_objectToListMap.size());	// this both structures are coupled, the number of elements must be the same
}


} // namespace iproc


#endif // !iproc_TMemCachedProducerComp_included


