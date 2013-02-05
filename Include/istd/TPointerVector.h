#ifndef istd_TPointerVector_included
#define istd_TPointerVector_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/istd.h"


namespace istd
{


/**
	Default delete adapter.
*/
template<typename Pointer>
class TDeleteAdapter
{
public:
	typedef Pointer* ElementType;

	static Pointer* GetPtr(const ElementType& element)
	{
		return element;
	}

	static Pointer* PopPtr(const ElementType& element)
	{
		return GetPtr(element);
	}

	static void Delete(const ElementType& element)
	{
		delete element;
	}
};


/**
	Implementation of a pointer container, 
	which controls the live cycle of the pointer object.
*/
template <typename Pointer, class AccessAdapter = TDeleteAdapter<Pointer> >
class TPointerVector
{
public:
	typedef typename AccessAdapter::ElementType ElementType;

	enum
	{
		/**
			Invalid index in the vector
		*/
		InvalidIndex = -1
	};

	TPointerVector();
	TPointerVector(const TPointerVector&);

	~TPointerVector();

	/**
		Check if there is element stored in this set.
	*/
	bool IsEmpty() const;

	/**
		Get number of stored elements.
	*/
	int GetCount() const;

	/**
		Set number of elements.
	*/
	void SetCount(int count);

	/**
		Remove all elements.
	*/
	void Reset();

	/**
		Check if some element is stored in this vector and return the index of the element,
		if anyone exists, otherwise return \c InvalidIndex.
	*/
	int HasElement(const Pointer* elementPtr) const;

	/**
		Get pointer at specified index.
	*/
	Pointer* GetAt(int index) const;

	/**
		Get element at specified index.
		Dependent on implementation of \c AccessAdapter it can differ from GetAt().
	*/
	const ElementType& GetElementAt(int index) const;

	/**
		Set element at specified index.
	*/
	void SetElementAt(int index, const ElementType& element);

	/**
		Remove element at specified index.
		The element will be deleted and removed from this vector.
		It means after call of this method number of elements will be decreased.
	*/
	void RemoveAt(int index);

	/**
		Remove element \c elementPtr.
		The element will be deleted and removed from this vector.
		It means after call of this method number of elements will be decreased if pointer is found.
		\return	true, if element was removed, false elsewhere.
	*/
	bool Remove(Pointer* elementPtr);

	/**
		Pop element at specified index.
		It does't delete pointed object, it removes it only from the vector.
	*/
	Pointer* PopAt(int index);

	/**
		Add new element at the end of collection.
	*/
	void PushBack(const ElementType& element);

	/**
		Insert element at specified position.
	*/
	void InsertElementAt(int index, const ElementType& element);

private:
	typedef QVector<ElementType> Elements;

	Elements m_elements;
};


// inline methods

template <typename Pointer, class AccessAdapter>
inline TPointerVector<Pointer, AccessAdapter>::TPointerVector()
{
}


template <typename Pointer, class AccessAdapter>
inline TPointerVector<Pointer, AccessAdapter>::TPointerVector(const TPointerVector& I_IF_DEBUG(otherVector))
{
	Q_ASSERT(otherVector.IsEmpty());
}


template <typename Pointer, class AccessAdapter>
inline bool TPointerVector<Pointer, AccessAdapter>::IsEmpty() const
{
	return m_elements.isEmpty();
}


template <typename Pointer, class AccessAdapter>
inline int TPointerVector<Pointer, AccessAdapter>::GetCount() const
{
	return int(m_elements.size());
}


// public methods

template <typename Pointer, class AccessAdapter>
TPointerVector<Pointer, AccessAdapter>::~TPointerVector()
{
	Reset();
}


template <typename Pointer, class AccessAdapter>
void TPointerVector<Pointer, AccessAdapter>::SetCount(int count)
{
	m_elements.resize(count);
}


template <typename Pointer, class AccessAdapter>
void TPointerVector<Pointer, AccessAdapter>::Reset()
{
	for (		typename Elements::iterator iter = m_elements.begin();
				iter != m_elements.end();
				++iter){
		AccessAdapter::Delete(*iter);
	}

	m_elements.clear();
}


template <typename Pointer, class AccessAdapter>
int TPointerVector<Pointer, AccessAdapter>::HasElement(const Pointer* elementPtr) const
{
	int elementsCount = GetCount();

	for (int elementIndex = 0; elementIndex < elementsCount; elementIndex++){
		typename Elements::const_iterator delIter = (m_elements.begin() + elementIndex);
		if (AccessAdapter::GetPtr(*delIter) == elementPtr){
			return elementIndex;
		}
	}

	return InvalidIndex;
}


template <typename Pointer, class AccessAdapter>
Pointer* TPointerVector<Pointer, AccessAdapter>::GetAt(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_elements.size()));

	return AccessAdapter::GetPtr(m_elements[index]);
}


template <typename Pointer, class AccessAdapter>
const typename TPointerVector<Pointer, AccessAdapter>::ElementType& TPointerVector<Pointer, AccessAdapter>::GetElementAt(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_elements.size()));

	return m_elements[index];
}


template <typename Pointer, class AccessAdapter>
void TPointerVector<Pointer, AccessAdapter>::SetElementAt(int index, const ElementType& element)
{
	typename Elements::iterator delIter = (m_elements.begin() + index);

	AccessAdapter::Delete(*delIter);

	m_elements[index] = element;
}


template <typename Pointer, class AccessAdapter>
void TPointerVector<Pointer, AccessAdapter>::RemoveAt(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_elements.size()));

	typename Elements::iterator delIter = (m_elements.begin() + index);

	AccessAdapter::Delete(*delIter);

	m_elements.erase(delIter);
}

	
template <typename Pointer, class AccessAdapter>
bool TPointerVector<Pointer, AccessAdapter>::Remove(Pointer* elementPtr)
{
	int elementsCount = GetCount();

	for (int elementIndex = 0; elementIndex < elementsCount; elementIndex++){
		typename Elements::iterator delIter = (m_elements.begin() + elementIndex);
		if (AccessAdapter::GetPtr(*delIter) == elementPtr){
			RemoveAt(elementIndex);

			return true;
		}
	}

	return false;
}


template <typename Pointer, class AccessAdapter>
Pointer* TPointerVector<Pointer, AccessAdapter>::PopAt(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_elements.size()));

	Pointer* popPtr = AccessAdapter::PopPtr(m_elements[index]);

	m_elements.erase(m_elements.begin() + index);

	return popPtr;
}


template <typename Pointer, class AccessAdapter>
void TPointerVector<Pointer, AccessAdapter>::PushBack(const ElementType& element)
{
	m_elements.push_back(element);
}


template <typename Pointer, class AccessAdapter>
void TPointerVector<Pointer, AccessAdapter>::InsertElementAt(int index, const ElementType& element)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index <= GetCount());
	Q_ASSERT(HasElement(AccessAdapter::GetPtr(element)) == InvalidIndex);

	m_elements.insert(m_elements.begin() + index, element);
}


} // namespace istd


#endif // !istd_TPointerVector_included


