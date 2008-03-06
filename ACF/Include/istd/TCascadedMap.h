#ifndef istd_TCascadedMap_included
#define istd_TCascadedMap_included


#include <vector>
#include <map>


namespace istd
{


/**
	Helper class used to manage list of many connected in cascade maps.
	Please note, that elements are accessed using its index.
	At this moment no element removing is supported.
	Local elements have begining indices.
*/
template <typename Key, typename Value>
class TCascadedMap
{
public:
	typedef Key KeyType;
	typedef Value ValueType;

	/**
		Constructor with optional assigning of parent map.
	*/
	TCascadedMap(const TCascadedMap<Key, Value>* parentPtr = NULL);

	/**
		Get access to parent map instance.
	*/
	const TCascadedMap<Key, Value>* GetParent() const;

	/**
		Set instance of parent map.
	*/
	void SetParent(const TCascadedMap<Key, Value>* parentPtr);

	/**
		Element access operator.
		If key object not exists, it will be automatically created.
	*/
	ValueType& operator[](const KeyType& key);

	/**
		Element access operator.
		If key object not exists, it will be automatically created.
	*/
	const ValueType& operator[](const KeyType& key) const;

	/**
		Find index index of specified key.
	*/
	int FindIndex(const KeyType& key) const;

	/**
		Find index index of specified key using local context only.
	*/
	int FindLocalIndex(const KeyType& key) const;

	/**
		Find value element associated with specified key.
		\return	pointer to associated value, or NULL if key not exists.
	*/
	const ValueType* FindElement(const KeyType& key) const;

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
		Get key value at specified index.
	*/
	const KeyType& GetKeyAt(int index) const;
	/**
		Get key value from local context at specified index.
	*/
	const KeyType& GetLocalKeyAt(int index) const;
	/**
		Get key value from local context at specified index.
	*/
	KeyType& GetLocalKeyAt(int index);

	/**
		Get mapped value at specified index.
	*/
	const ValueType& GetValueAt(int index) const;

	/**
		Get mapped value from local context at specified index.
	*/
	const ValueType& GetLocalValueAt(int index) const;

	/**
		Get mapped value from local context at specified index.
	*/
	ValueType& GetLocalValueAt(int index);

	/**
		Get number of elements.
	*/
	int GetElementsCount() const;

	/**
		Get number of elements in local context.
	*/
	int GetLocalElementsCount() const;

	/**
		Insert element in local context.
	*/
	bool InsertLocal(const KeyType& key, const ValueType& value);

	/**
		Get owner context of element at specified index.
	*/
	const TCascadedMap<Key, Value>* GetOwnerMap(int index) const;

	/**
		Get owner context of element at specified index.
	*/
	TCascadedMap<Key, Value>* GetOwnerMap(int index);

	/**
		Removes all elements from local context.
	*/
	void ResetLocal();

	// TODO: add element removing and correct comment in class header.

private:
	const TCascadedMap<Key, Value>* m_parentPtr;

	typedef ::std::map<KeyType, int> IndicesMap;
	typedef ::std::pair<KeyType, ValueType> Pair;
	typedef ::std::vector<Pair> PairList;

	mutable IndicesMap m_positionsMap;
	mutable PairList m_pairList;
};


// public methods

template <typename Key, typename Value>
TCascadedMap<Key, Value>::TCascadedMap(const TCascadedMap<Key, Value>* parentPtr)
:	m_parentPtr(parentPtr)
{
}


template <typename Key, typename Value>
typename const TCascadedMap<Key, Value>* TCascadedMap<Key, Value>::GetParent() const
{
	return m_parentPtr;
}


template <typename Key, typename Value>
void TCascadedMap<Key, Value>::SetParent(const TCascadedMap<Key, Value>* parentPtr)
{
	m_parentPtr = parentPtr;
}


template <typename Key, typename Value>
typename TCascadedMap<Key, Value>::ValueType& TCascadedMap<Key, Value>::operator[](const KeyType& key)
{
	IndicesMap::const_iterator iter = m_positionsMap.find(key);
	if (iter != m_positionsMap.end()){
		return m_pairList[iter->second];
	}

	int newIndex = int(m_pairList.size());
	m_positionsMap[key] = newIndex;

	m_pairList.push_back(Pair(key, ValueType()));

	return m_positionsMap.back();
}


template <typename Key, typename Value>
typename const TCascadedMap<Key, Value>::ValueType& TCascadedMap<Key, Value>::operator[](const KeyType& key) const
{
	return const_cast<TCascadedMap*>(this)->operator[](key);
}


template <typename Key, typename Value>
int TCascadedMap<Key, Value>::FindIndex(const KeyType& key) const
{
	IndicesMap::const_iterator iter = m_positionsMap.find(key);
	if (iter != m_positionsMap.end()){
		return iter->second;
	}

	if (m_parentPtr != NULL){
		return m_parentPtr->FindIndex(key);
	}

	return -1;
}


template <typename Key, typename Value>
int TCascadedMap<Key, Value>::FindLocalIndex(const KeyType& key) const
{
	IndicesMap::const_iterator iter = m_positionsMap.find(key);
	if (iter != m_positionsMap.end()){
		return iter->second;
	}

	return -1;
}


template <typename Key, typename Value>
typename const TCascadedMap<Key, Value>::ValueType* TCascadedMap<Key, Value>::FindElement(const KeyType& key) const
{
	int index = FindIndex(key);
	if (index >= 0){
		return &GetValueAt(index);
	}

	return NULL;
}


template <typename Key, typename Value>
typename const TCascadedMap<Key, Value>::ValueType* TCascadedMap<Key, Value>::FindLocalElement(const KeyType& key) const
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
typename const TCascadedMap<Key, Value>::KeyType& TCascadedMap<Key, Value>::GetKeyAt(int index) const
{
	int elementsCount = GetLocalElementsCount();
	if (index < elementsCount){
		return GetLocalKeyAt(index);
	}

	I_ASSERT(m_parentPtr != NULL);	// Index from outside this map!

	return m_parentPtr->GetKeyAt(index - elementsCount);
}


template <typename Key, typename Value>
typename const TCascadedMap<Key, Value>::KeyType& TCascadedMap<Key, Value>::GetLocalKeyAt(int index) const
{
	I_ASSERT(index < int(m_pairList.size()));

	return m_pairList[index].first;
}


template <typename Key, typename Value>
typename TCascadedMap<Key, Value>::KeyType& TCascadedMap<Key, Value>::GetLocalKeyAt(int index)
{
	I_ASSERT(index < int(m_pairList.size()));

	return m_pairList[index].first;
}


template <typename Key, typename Value>
typename const TCascadedMap<Key, Value>::ValueType& TCascadedMap<Key, Value>::GetValueAt(int index) const
{
	int elementsCount = GetLocalElementsCount();
	if (index < elementsCount){
		return GetLocalValueAt(index);
	}

	I_ASSERT(m_parentPtr != NULL);	// Index from outside this map!

	return m_parentPtr->GetValueAt(index - elementsCount);
}


template <typename Key, typename Value>
typename TCascadedMap<Key, Value>::ValueType& TCascadedMap<Key, Value>::GetLocalValueAt(int index)
{
	I_ASSERT(index < int(m_pairList.size()));

	return m_pairList[index].second;
}


template <typename Key, typename Value>
typename const TCascadedMap<Key, Value>::ValueType& TCascadedMap<Key, Value>::GetLocalValueAt(int index) const
{
	I_ASSERT(index < int(m_pairList.size()));

	return m_pairList[index].second;
}


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
int TCascadedMap<Key, Value>::GetLocalElementsCount() const
{
	return int(m_pairList.size());
}


template <typename Key, typename Value>
bool TCascadedMap<Key, Value>::InsertLocal(const KeyType& key, const ValueType& value)
{
	IndicesMap::const_iterator iter = m_positionsMap.find(key);
	if (iter != m_positionsMap.end()){
		return false;
	}

	int newIndex = int(m_pairList.size());
	m_positionsMap[key] = newIndex;

	m_pairList.push_back(Pair(key, value));

	return true;
}


template <typename Key, typename Value>
const TCascadedMap<Key, Value>* TCascadedMap<Key, Value>::GetOwnerMap(int index) const
{
	int elementsCount = GetLocalElementsCount();
	if (index < elementsCount){
		return this;
	}

	I_ASSERT(m_parentPtr != NULL);	// Index from outside this map!

	return m_parentPtr->GetOwnerMap(index - elementsCount);
}


template <typename Key, typename Value>
TCascadedMap<Key, Value>* TCascadedMap<Key, Value>::GetOwnerMap(int index)
{
	int elementsCount = GetLocalElementsCount();
	if (index < elementsCount){
		return this;
	}

	I_ASSERT(m_parentPtr != NULL);	// Index from outside this map!

	return m_parentPtr->GetOwnerMap(index - elementsCount);
}


template <typename Key, typename Value>
void TCascadedMap<Key, Value>::ResetLocal()
{
	m_positionsMap.clear();
	m_pairList.clear();
}


}//namespace istd


#endif // !istd_TCascadedMap_included


