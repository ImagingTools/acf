#ifndef istd_TPointerVector_included
#define istd_TPointerVector_included


#include "istd/istd.h"


#include <vector>


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
		I_ASSERT(element != NULL);

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

	~TPointerVector();

	/**
		Get number of stored elements.
	*/
	int GetCount() const;

	/**
		Remove all elements.
	*/
	void Reset();

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
	void SetElementAt(int index, typename const ElementType& element);

	/**
		Remove element at specified index.
	*/
	void RemoveAt(int index);

	/**
		Pop element at specified index.
		It does't remove pointed object.
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
	typedef std::vector<ElementType> Elements;

	Elements m_elements;
};


// public methods

template <typename Pointer, class AccessAdapter>
TPointerVector<Pointer, AccessAdapter>::~TPointerVector()
{
	Reset();
}


template <typename Pointer, class AccessAdapter>
int TPointerVector<Pointer, AccessAdapter>::GetCount() const
{
	return int(m_elements.size());
}


template <typename Pointer, class AccessAdapter>
void TPointerVector<Pointer, AccessAdapter>::Reset()
{
	for (		Elements::iterator iter = m_elements.begin();
				iter != m_elements.end();
				++iter){
		AccessAdapter::Delete(*iter);
	}

	m_elements.clear();
}


template <typename Pointer, class AccessAdapter>
typename Pointer* TPointerVector<Pointer, AccessAdapter>::GetAt(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

	return AccessAdapter::GetPtr(m_elements[index]);
}


template <typename Pointer, class AccessAdapter>
typename const TPointerVector<Pointer, AccessAdapter>::ElementType& TPointerVector<Pointer, AccessAdapter>::GetElementAt(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

	return m_elements[index];
}


template <typename Pointer, class AccessAdapter>
void TPointerVector<Pointer, AccessAdapter>::SetElementAt(int index, const ElementType& element)
{
	m_elements[index] = element;
}


template <typename Pointer, class AccessAdapter>
void TPointerVector<Pointer, AccessAdapter>::RemoveAt(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

	Elements::iterator delIter = (m_elements.begin() + index);

	AccessAdapter::Delete(*delIter);

	m_elements.erase(delIter);
}


template <typename Pointer, class AccessAdapter>
typename Pointer* TPointerVector<Pointer, AccessAdapter>::PopAt(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

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
	I_ASSERT(index >= 0);
	I_ASSERT(index <= GetCount());

	m_elements.insert(m_elements.begin() + index, element);
}


} // namespace istd


#endif // !istd_TPointerVector_included

