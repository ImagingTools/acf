#ifndef istd_TCascadedMap_included
#define istd_TCascadedMap_included


// Qt includes
#include <QtCore/QPair>
#include <QtCore/QVector>
#include <QtCore/QMap>

// ACF includes
#include "istd/TIMap.h"


namespace istd
{


/**
	Helper class used to manage list of many connected in cascade maps.
	Please note, that elements are accessed using its index.
	At this moment no element removing is supported.
	Local elements have begining indices.
*/
template <typename Key, typename Value>
class TCascadedMap: virtual public TIMap<Key, Value>
{
public:
	/**
		Default constructor.
	*/
	TCascadedMap();
	/**
		Constructor with assigning of parent map.
	*/
	explicit TCascadedMap(const TCascadedMap<Key, Value>* parentPtr);
	/**
		Copy constructor.
	*/
	TCascadedMap(const TCascadedMap& map);

	/**
		Get access to parent map instance.
	*/
	const TCascadedMap<Key, Value>* GetParent() const;

	/**
		Set instance of parent map.
	*/
	void SetParent(const TIMap<Key, Value>* parentPtr);

	/**
		Find index index of specified key using local context only.
	*/
	int FindLocalIndex(const KeyType& key) const;

	/**
		Find value element associated with specified key using local context only.
		\return	pointer to associated value, or NULL if key not exists.
	*/
	const ValueType* FindLocalElement(const KeyType& key) const;

	/**
		Find value element associated with specified key using local context only.
		\return	pointer to associated value, or NULL if key not exists.
	*/
	ValueType* FindLocalElement(const KeyType& key);

	/**
		Get key value from local context at specified index.
	*/
	const KeyType& GetLocalKeyAt(int index) const;
	/**
		Get key value from local context at specified index.
	*/
	KeyType& GetLocalKeyAt(int index);

	/**
		Get mapped value from local context at specified index.
	*/
	const ValueType& GetLocalValueAt(int index) const;

	/**
		Get mapped value from local context at specified index.
	*/
	ValueType& GetLocalValueAt(int index);

	/**
		Get number of elements in local context.
	*/
	int GetLocalElementsCount() const;

	/**
		Insert element in local context.
	*/
	bool InsertLocal(const KeyType& key, const ValueType& value);

	/**
		Removes all elements from local context.
	*/
	void ResetLocal();

	/**
		Get list of local keys stored in this map.
		\param	doAppend	if it is true, list of keys will be appended to existing key list.
	*/
	void GetLocalKeys(Keys& result, bool doAppend = false) const;

	// reimplemented (istd::TIMap)
	virtual int GetElementsCount() const;
	virtual ValueType& operator[](const KeyType& key);
	virtual const ValueType& operator[](const KeyType& key) const;
	virtual int FindIndex(const KeyType& key) const;
	virtual const ValueType* FindElement(const KeyType& key) const;
	virtual void GetKeys(Keys& result, bool doAppend = false) const;
	virtual const KeyType& GetKeyAt(int index) const;
	virtual const ValueType& GetValueAt(int index) const;

	// TODO: add element removing and correct comment in class header.

private:
	const TIMap<Key, Value>* m_parentPtr;

	typedef QMap<KeyType, int> IndicesMap;
	typedef QPair<KeyType, ValueType> Pair;
	typedef QVector<Pair> PairList;

	mutable IndicesMap m_positionsMap;
	mutable PairList m_pairList;
};


// public methods

template <typename Key, typename Value>
TCascadedMap<Key, Value>::TCascadedMap()
:	m_parentPtr(NULL)
{
}


template <typename Key, typename Value>
TCascadedMap<Key, Value>::TCascadedMap(const TCascadedMap<Key, Value>* parentPtr)
:	m_parentPtr(parentPtr)
{
}


template <typename Key, typename Value>
TCascadedMap<Key, Value>::TCascadedMap(const TCascadedMap& map)
:	m_parentPtr(map.m_parentPtr),
	m_positionsMap(map.m_positionsMap),
	m_pairList(map.m_pairList)
{
}


template <typename Key, typename Value>
const TCascadedMap<Key, Value>* TCascadedMap<Key, Value>::GetParent() const
{
	return m_parentPtr;
}


template <typename Key, typename Value>
void TCascadedMap<Key, Value>::SetParent(const TIMap<Key, Value>* parentPtr)
{
	m_parentPtr = parentPtr;
}


template <typename Key, typename Value>
int TCascadedMap<Key, Value>::FindLocalIndex(const KeyType& key) const
{
	typename IndicesMap::ConstIterator iter = m_positionsMap.constFind(key);
	if (iter != m_positionsMap.constEnd()){
		return iter.value();
	}

	return -1;
}


template <typename Key, typename Value>
const typename TCascadedMap<Key, Value>::ValueType* TCascadedMap<Key, Value>::FindLocalElement(const KeyType& key) const
{
	int index = FindLocalIndex(key);
	if (index >= 0){
		return &GetLocalValueAt(index);
	}

	return NULL;
}


template <typename Key, typename Value>
typename TCascadedMap<Key, Value>::ValueType* TCascadedMap<Key, Value>::FindLocalElement(const KeyType& key)
{
	int index = FindLocalIndex(key);
	if (index >= 0){
		return &GetLocalValueAt(index);
	}

	return NULL;
}


template <typename Key, typename Value>
const typename TCascadedMap<Key, Value>::KeyType& TCascadedMap<Key, Value>::GetLocalKeyAt(int index) const
{
	Q_ASSERT(index < int(m_pairList.size()));

	return m_pairList[index].first;
}


template <typename Key, typename Value>
typename TCascadedMap<Key, Value>::KeyType& TCascadedMap<Key, Value>::GetLocalKeyAt(int index)
{
	Q_ASSERT(index < int(m_pairList.size()));

	return m_pairList[index].first;
}


template <typename Key, typename Value>
typename TCascadedMap<Key, Value>::ValueType& TCascadedMap<Key, Value>::GetLocalValueAt(int index)
{
	Q_ASSERT(index < int(m_pairList.size()));

	return m_pairList[index].second;
}


template <typename Key, typename Value>
const typename TCascadedMap<Key, Value>::ValueType& TCascadedMap<Key, Value>::GetLocalValueAt(int index) const
{
	Q_ASSERT(index < int(m_pairList.size()));

	return m_pairList[index].second;
}


template <typename Key, typename Value>
int TCascadedMap<Key, Value>::GetLocalElementsCount() const
{
	return int(m_pairList.size());
}


template <typename Key, typename Value>
bool TCascadedMap<Key, Value>::InsertLocal(const KeyType& key, const ValueType& value)
{
	typename IndicesMap::ConstIterator iter = m_positionsMap.constFind(key);
	if (iter != m_positionsMap.constEnd()){
		return false;
	}

	int newIndex = int(m_pairList.size());
	m_positionsMap[key] = newIndex;

	m_pairList.push_back(Pair(key, value));

	return true;
}


template <typename Key, typename Value>
void TCascadedMap<Key, Value>::ResetLocal()
{
	m_positionsMap.clear();
	m_pairList.clear();
}


template <typename Key, typename Value>
void TCascadedMap<Key, Value>::GetLocalKeys(Keys& result, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	for (		typename IndicesMap::const_iterator iter = m_positionsMap.begin();
				iter != m_positionsMap.end();
				++iter){
		result.insert(iter.key());
	}
}


// reimplemented (istd::TIMap)

template <typename Key, typename Value>
int TCascadedMap<Key, Value>::GetElementsCount() const
{
	if (m_parentPtr != NULL){
		return m_parentPtr->GetElementsCount() + GetLocalElementsCount();
	}
	else{
		return GetLocalElementsCount();
	}
}


template <typename Key, typename Value>
typename TCascadedMap<Key, Value>::ValueType& TCascadedMap<Key, Value>::operator[](const KeyType& key)
{
	typename IndicesMap::ConstIterator iter = m_positionsMap.constFind(key);
	if (iter != m_positionsMap.constEnd()){
		return m_pairList[iter.value()].second;
	}

	int newIndex = int(m_pairList.size());
	m_positionsMap[key] = newIndex;

	m_pairList.push_back(Pair(key, ValueType()));

	return m_pairList.back().second;
}


template <typename Key, typename Value>
const typename TCascadedMap<Key, Value>::ValueType& TCascadedMap<Key, Value>::operator[](const KeyType& key) const
{
	return const_cast<TCascadedMap*>(this)->operator[](key);
}


template <typename Key, typename Value>
int TCascadedMap<Key, Value>::FindIndex(const KeyType& key) const
{
	typename IndicesMap::ConstIterator iter = m_positionsMap.constFind(key);
	if (iter != m_positionsMap.constEnd()){
		return iter.value();
	}

	if (m_parentPtr != NULL){
		int parentIndex = m_parentPtr->FindIndex(key);
		if (parentIndex >= 0){
			return parentIndex + GetLocalElementsCount();
		}
	}

	return -1;
}


template <typename Key, typename Value>
const typename TCascadedMap<Key, Value>::ValueType* TCascadedMap<Key, Value>::FindElement(const KeyType& key) const
{
	int index = FindIndex(key);
	if (index >= 0){
		return &GetValueAt(index);
	}

	return NULL;
}


template <typename Key, typename Value>
void TCascadedMap<Key, Value>::GetKeys(Keys& result, bool doAppend) const
{
	GetLocalKeys(result, doAppend);

	if (m_parentPtr != NULL){
		m_parentPtr->GetKeys(result, true);
	}
}


template <typename Key, typename Value>
const typename TCascadedMap<Key, Value>::KeyType& TCascadedMap<Key, Value>::GetKeyAt(int index) const
{
	int elementsCount = GetLocalElementsCount();
	if (index < elementsCount){
		return GetLocalKeyAt(index);
	}

	Q_ASSERT(m_parentPtr != NULL);	// Index from outside this map!

	return m_parentPtr->GetKeyAt(index - elementsCount);
}


template <typename Key, typename Value>
const typename TCascadedMap<Key, Value>::ValueType& TCascadedMap<Key, Value>::GetValueAt(int index) const
{
	int elementsCount = GetLocalElementsCount();
	if (index < elementsCount){
		return GetLocalValueAt(index);
	}

	Q_ASSERT(m_parentPtr != NULL);	// Index from outside this map!

	return m_parentPtr->GetValueAt(index - elementsCount);
}


}//namespace istd


#endif // !istd_TCascadedMap_included


