#ifndef istd_TVarArray_included
#define istd_TVarArray_included


// STL includes
#include <vector>

// ACF includes
#include "istd/CVarIndex.h"


namespace istd
{


/**
	Multidimensional array with fixed number of dimensions.
*/
template <class Element>
class TVarArray
{
public:
	typedef CVarIndex IndexType;
	typedef CVarIndex SizesType;
	typedef Element ElementType;

	class Iterator: public CVarIndex
	{
	public:
		typedef CVarIndex BaseClass;

		Iterator(const Iterator& iterator);

		Element& operator*();
		const Element& operator*() const;

		Element* operator->();
		const Element* operator->() const;

		Iterator& operator++();
		Iterator operator++(int);

		Iterator& operator--();
		Iterator operator--(int);

		Iterator& operator=(const Iterator& iterator);

		bool operator==(const Iterator& iterator) const;
		bool operator!=(const Iterator& iterator) const;

		friend class TVarArray<Element>;

	protected:
		Iterator(TVarArray* arrayPtr);

	private:
		TVarArray* m_arrayPtr;
	};

	TVarArray();
	TVarArray(const TVarArray& array);

	/**
		Removes all elements and set all sizes to 1.
	*/
	void Reset();

	/**
		Check, if number dimensions is fixed.
		It is provided for template implementations. It returns always false.
	*/
	bool IsDimensionsCountFixed() const;

	/**
		Get number of dimensions of this array.
	*/
	int GetDimensionsCount() const;

	/**
		Set number of dimensions of this array.
		\param	count	number of dimensions will be set.
		\return			always true.
	*/
	bool SetDimensionsCount(int count);

	/**
		Get list of all sizes.
	*/
	const SizesType& GetSizes() const;

	/**
		Set list of all sizes.
		\return			always true, it is provided for generic implementations.
	*/
	bool SetSizes(const SizesType& sizes);

	/**
		Get size of array for specified dimension.
	*/
	int GetSize(int dimension) const;

	/**
		Set size of array for specified dimension.
		Please note, that it can make all elements invalid.
		\return			always true, it is provided for generic implementations.
	*/
	bool SetSize(int dimension, int size);

	/**
		Get element stored at specified index.
	*/
	const Element& GetAt(const IndexType& index) const;

	/**
		Set element at specified index.
	*/
	void SetAt(const IndexType& index, const Element& value);

	// iterator support
	/**
		Get begin value of element access iterator.
		Please refer to general description of ACF iterators, STL iterators or Qt iterators concept.
	*/
	Iterator Begin() const;
	/**
		Get end value of element access iterator.
		Please refer to general description of ACF iterators, STL iterators or Qt iterators concept.
	*/
	const Iterator& End() const;

	const Element& operator[](const IndexType& index) const;
	Element& operator[](const IndexType& index);

	/**
		Get index of element in one dimensional array.
	*/
	int GetElementIndex(const IndexType& index) const;

protected:
	/**
		Update size of elements to size changes.
	*/
	void UpdateElementsSize();

private:
	typedef std::vector<Element> Elements;
	Elements m_elements;

	SizesType m_sizes;

	static Iterator s_endIterator;
};


// inline methods

template <class Element>
inline bool TVarArray<Element>::IsDimensionsCountFixed() const
{
	return false;
}


template <class Element>
inline int TVarArray<Element>::GetDimensionsCount() const
{
	return m_sizes.GetDimensionsCount();
}


template <class Element>
inline bool TVarArray<Element>::SetDimensionsCount(int count)
{
	bool retVal = m_sizes.SetDimensionsCount(count);

	UpdateElementsSize();

	return retVal;
}


template <class Element>
inline const typename TVarArray<Element>::SizesType& TVarArray<Element>::GetSizes() const
{
	return m_sizes;
}


template <class Element>
inline int TVarArray<Element>::GetSize(int dimension) const
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < m_sizes.GetDimensionsCount());

	return m_sizes[dimension];
}


template <class Element>
inline const Element& TVarArray<Element>::GetAt(const IndexType& index) const
{
	I_ASSERT(index.IsInside(m_sizes));

	int elementIndex = GetElementIndex(index);
	I_ASSERT(elementIndex < int(m_elements.size()));

	return m_elements[elementIndex];
}


template <class Element>
inline void TVarArray<Element>::SetAt(const IndexType& index, const Element& value)
{
	I_ASSERT(index.IsInside(m_sizes));

	int elementIndex = GetElementIndex(index);
	I_ASSERT(elementIndex < int(m_elements.size()));

	m_elements[elementIndex] = value;
}


// iterator support

template <class Element>
typename TVarArray<Element>::Iterator TVarArray<Element>::Begin() const
{
	return Iterator(const_cast<TVarArray<Element>*>(this));
}


template <class Element>
const typename TVarArray<Element>::Iterator& TVarArray<Element>::End() const
{
	return s_endIterator;
}


template <class Element>
inline const Element& TVarArray<Element>::operator[](const IndexType& index) const
{
	return GetAt(index);
}


template <class Element>
inline Element& TVarArray<Element>::operator[](const IndexType& index)
{
	int elementIndex = GetElementIndex(index);
	I_ASSERT(elementIndex < int(m_elements.size()));

	return m_elements[elementIndex];
}


template <class Element>
inline int TVarArray<Element>::GetElementIndex(const IndexType& index) const
{
	int indexDimensionsCount = index.GetDimensionsCount();
	int dimensionsCount = m_sizes.GetDimensionsCount();

	int minDimensionsCount = istd::Min(indexDimensionsCount, dimensionsCount);

	int elementIndex = 0;
	int cumulatedSizes = 1;
	for (int i = 0; i < minDimensionsCount; ++i){
		I_ASSERT(index[i] >= 0);
		I_ASSERT(index[i] < m_sizes[i]);

		elementIndex += index[i] * cumulatedSizes;

		cumulatedSizes *= m_sizes[i];
	}

	return elementIndex;
}


// public methods

template <class Element>
TVarArray<Element>::TVarArray()
{
}


template <class Element>
TVarArray<Element>::TVarArray(const TVarArray& array)
:	m_sizes(array.m_sizes), m_elements(array.m_elements)
{
}


template <class Element>
void TVarArray<Element>::Reset()
{
	m_sizes.Reset();

	m_elements.clear();
}


template <class Element>
bool TVarArray<Element>::SetSizes(const SizesType& sizes)
{
	m_sizes = sizes;

	UpdateElementsSize();

	return true;
}


template <class Element>
bool TVarArray<Element>::SetSize(int dimension, int size)
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < m_sizes.GetDimensionsCount());

	m_sizes[dimension] = size;

	UpdateElementsSize();

	return true;
}


// protected methods

template <class Element>
void TVarArray<Element>::UpdateElementsSize()
{
	int dimensionsCount = m_sizes.GetDimensionsCount();
	int cumulatedSizes = 1;
	for (int i = 0; i < dimensionsCount; ++i){
		cumulatedSizes *= m_sizes[i];
	}

	m_elements.resize(cumulatedSizes);
}


// static attributes

template <class Element>
typename TVarArray<Element>::Iterator TVarArray<Element>::s_endIterator(NULL);


// public methods of embedded class Iterator

template <class Element>
TVarArray<Element>::Iterator::Iterator(const Iterator& iterator)
:	BaseClass(iterator), m_arrayPtr(iterator.m_arrayPtr)
{
}


template <class Element>
const Element& TVarArray<Element>::Iterator::operator*() const
{
	I_ASSERT(m_arrayPtr != NULL);
	I_ASSERT(IsInside(m_arrayPtr->GetSizes()));

	return m_arrayPtr->GetAt(*this);
}


template <class Element>
Element& TVarArray<Element>::Iterator::operator*()
{
	I_ASSERT(m_arrayPtr != NULL);
	I_ASSERT(IsInside(m_arrayPtr->GetSizes()));

	return m_arrayPtr->operator[](*this);
}


template <class Element>
const Element* TVarArray<Element>::Iterator::operator->() const
{
	I_ASSERT(m_arrayPtr != NULL);
	I_ASSERT(IsInside(m_arrayPtr->GetSizes()));

	return &m_arrayPtr->GetAt(*this);
}


template <class Element>
Element* TVarArray<Element>::Iterator::operator->()
{
	I_ASSERT(m_arrayPtr != NULL);
	I_ASSERT(IsInside(m_arrayPtr->GetSizes()));

	return &m_arrayPtr->operator[](*this);
}


template <class Element>
typename TVarArray<Element>::Iterator& TVarArray<Element>::Iterator::operator++()
{
	if ((m_arrayPtr != NULL) && !Increase(m_arrayPtr->GetSizes())){
		m_arrayPtr = NULL;
	}

	return *this;
}


template <class Element>
typename TVarArray<Element>::Iterator TVarArray<Element>::Iterator::operator++(int)
{
	Iterator retVal = *this;

	if ((m_arrayPtr != NULL) && !Increase(m_arrayPtr->GetSizes())){
		m_arrayPtr = NULL;
	}

	return retVal;
}


template <class Element>
typename TVarArray<Element>::Iterator& TVarArray<Element>::Iterator::operator--()
{
	if ((m_arrayPtr != NULL) && !Decrease(m_arrayPtr->GetSizes())){
		m_arrayPtr = NULL;
	}

	return *this;
}


template <class Element>
typename TVarArray<Element>::Iterator TVarArray<Element>::Iterator::operator--(int)
{
	Iterator retVal = *this;

	if ((m_arrayPtr != NULL) && !Decrease(m_arrayPtr->GetSizes())){
		m_arrayPtr = NULL;
	}

	return retVal;
}


template <class Element>
typename TVarArray<Element>::Iterator& TVarArray<Element>::Iterator::operator=(const Iterator& iterator)
{
	BaseClass::operator=(iterator);

	m_arrayPtr = iterator.m_arrayPtr;
}


template <class Element>
bool TVarArray<Element>::Iterator::operator==(const Iterator& iterator) const
{
	if ((m_arrayPtr != NULL) && (iterator.m_arrayPtr != NULL)){
		return (m_arrayPtr == iterator.m_arrayPtr) && (BaseClass::operator==(iterator));
	}

	return (m_arrayPtr == iterator.m_arrayPtr);
}


template <class Element>
bool TVarArray<Element>::Iterator::operator!=(const Iterator& iterator) const
{
	return !operator==(iterator);
}


// protected methods of emedded class Iterator

template <class Element>
TVarArray<Element>::Iterator::Iterator(TVarArray* arrayPtr)
:	BaseClass((arrayPtr != NULL)? arrayPtr->GetDimensionsCount(): 0, 0), m_arrayPtr(arrayPtr)
{
	if ((m_arrayPtr != NULL) && !IsInside(m_arrayPtr->GetSizes())){
		m_arrayPtr = NULL;	// if it is not inside of array set it directly to the end iterator state
	}
}

typedef TVarArray<double> CVarArray;


} // namespace istd


#endif // !istd_TVarArray_included


