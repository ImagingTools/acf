#ifndef iproc_TMorphLockedProducerBase_included
#define iproc_TMorphLockedProducerBase_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include "istd/TDelPtr.h"
#include "iproc/TILockedProducer.h"


namespace iproc
{


template <class Key, class CacheObject, class SourceObject>
class TMorphLockedProducerBase: public TILockedProducer<Key, CacheObject>
{
public:
	TMorphLockedProducerBase();

	double GetMaxCumulatedWeight() const;
	void SetMaxCumulatedWeight(double value);

	// reimplemented (iproc::TILockedProducer)
	virtual const CacheObject* ProduceLockedObject(const Key& key);
	virtual void UnlockObject(const CacheObject* objectPtr);

protected:
	/**
		Remove elements from list if cumulated weight is above defined maximum.
	*/
	void CleanElementList();
	
	// abstract methods
	/**
		Calculate cache object from source object.
		\return	positive weight value if succeded of negative value (-1) if failed.
	*/
	virtual double CalcCacheObject(
				const Key& key,
				const SourceObject& source,
				CacheObject& cache) const = 0;
	virtual const SourceObject* LockSourceObject(const Key& key) = 0;
	virtual void UnlockSourceObject(const Key& key, const SourceObject* sourcePtr) = 0;

private:
	double m_maxCumulatedWeight;

	mutable double m_cumulatedWeight;

	struct ListElement
	{
		bool operator==(const Key& key)
		{
			return this->key == key;
		}

		Key key;
		double weight;
		istd::TDelPtr<CacheObject> objectPtr;
		int lockedCount;
	};

	typedef QList<ListElement> CachedList;

	CachedList m_cachedList;
};


// public methods

template <class Key, class CacheObject, class SourceObject>
TMorphLockedProducerBase<Key, CacheObject, SourceObject>::TMorphLockedProducerBase()
:	m_maxCumulatedWeight(10), m_cumulatedWeight(0)
{
}


template <class Key, class CacheObject, class SourceObject>
double TMorphLockedProducerBase<Key, CacheObject, SourceObject>::GetMaxCumulatedWeight() const
{
	return m_maxCumulatedWeight;
}


template <class Key, class CacheObject, class SourceObject>
void TMorphLockedProducerBase<Key, CacheObject, SourceObject>::SetMaxCumulatedWeight(double value)
{
	m_maxCumulatedWeight = value;

	CleanElementList();
}


// reimplemented (iproc::TILockedProducer)

template <class Key, class CacheObject, class SourceObject>
const CacheObject* TMorphLockedProducerBase<Key, CacheObject, SourceObject>::ProduceLockedObject(const Key& key)
{
	typename CachedList::iterator foundIter = qFind(m_cachedList.begin(), m_cachedList.end(), key);
	if (foundIter != m_cachedList.end()){
		foundIter->lockedCount++;

		return foundIter->objectPtr.GetPtr();
	}

	const SourceObject* sourcePtr = LockSourceObject(key);
	if (sourcePtr != NULL){
		istd::TDelPtr<CacheObject> newObjectPtr(new CacheObject);
		if (!newObjectPtr.IsValid()){
			return NULL;
		}

		double weight = CalcCacheObject(key, *sourcePtr, *newObjectPtr);

		UnlockSourceObject(key, sourcePtr);

		if (weight < 0){
			return NULL;
		}

		m_cachedList.push_back(ListElement());

		ListElement& element = m_cachedList.back();

		element.key = key;
		element.objectPtr.TakeOver(newObjectPtr);
		element.weight = weight;
		element.lockedCount = 1;

		m_cumulatedWeight += weight;

		CleanElementList();

		return element.objectPtr.GetPtr();
	}

	return NULL;
}


template <class Key, class CacheObject, class SourceObject>
void TMorphLockedProducerBase<Key, CacheObject, SourceObject>::UnlockObject(const CacheObject* objectPtr)
{
	for (		typename CachedList::iterator iter = m_cachedList.begin();
				iter != m_cachedList.end();
				++iter){
		if (iter->objectPtr.GetPtr() == objectPtr){
			if (--iter->lockedCount <= 0){
				m_cachedList.erase(iter);
			}

			break;
		}
	}
}


// protected methods

template <class Key, class CacheObject, class SourceObject>
void TMorphLockedProducerBase<Key, CacheObject, SourceObject>::CleanElementList()
{
	typename CachedList::iterator iter = m_cachedList.begin();
	while (		(m_cumulatedWeight > m_maxCumulatedWeight) &&
				(iter != m_cachedList.end())){
		if (iter->lockedCount <= 0){
			m_cumulatedWeight -= iter->weight;

			iter = m_cachedList.erase(iter);
		}
		else{
			++iter;
		}
	}
}


} // namespace iproc


#endif // !iproc_TMorphLockedProducerBase_included


