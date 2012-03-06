#ifndef istd_TArray_included
#define istd_TArray_included


// STL includes
#include <vector>

// ACF includes
#include "istd/TIndex.h"


namespace istd
{


/**
	Multidimensional array with fixed number of dimensions.

	\ingroup Main
*/
template <class Element, int Dimensions>
class TArray
{
public:
	typedef TIndex<Dimensions> IndexType;
	typedef TIndex<Dimensions> SizesType;
	typedef Element ElementType;

	class Iterator: public TIndex<Dimensions>
	{
	public:
		typedef TIndex<Dimensions> BaseClass;

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

		friend class TArray<Element, Dimensions>;

	protected:
		Iterator(TArray* arrayPtr);

	private:
		TArray* m_arrayPtr;
	};

	TArray();
	TArray(const TArray& array);

	/**
		Removes all elements and set all sizes to 0.
	*/
	void Reset();

	/**
		Check if this array has no elements.
	*/
	bool IsEmpty() const;

	/**
		Check, if number dimensions is fixed.
		It is provided for template implementations. It returns always true.
	*/
	bool IsDimensionsCountFixed() const;

	/**
		Get number of dimensions of this array.
	*/
	int GetDimensionsCount() const;

	/**
		Set number of dimensions of this array.
		This is only dummy method, to provide methods compatibility with template implementations.
		\param	count	number of dimensions will be set.
		\return			true, if number of set dimensions equals template parameter, or false if isn't.
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
		Get reference to element stored at specified index.
	*/
	Element& GetAtRef(const IndexType& index);

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

protected:
	typedef std::vector<Element> Elements;

	/**
		Get index of element in one dimensional array.
	*/
	int GetElementIndex(const IndexType& index) const;
	/**
		Update size of elements to size changes.
	*/
	void UpdateElementsSize();

	void DeepCopy(const Elements& elements, const SizesType& sizes);

private:
	SizesType m_sizes;

	Elements m_elements;

	static Iterator s_endIterator;
};


// inline methods

template <class Element, int Dimensions>
bool TArray<Element, Dimensions>::IsEmpty() const
{
	return m_sizes.IsSizeEmpty();
}


template <class Element, int Dimensions>
inline bool TArray<Element, Dimensions>::IsDimensionsCountFixed() const
{
	return true;
}


template <class Element, int Dimensions>
inline int TArray<Element, Dimensions>::GetDimensionsCount() const
{
	return Dimensions;
}


template <class Element, int Dimensions>
inline bool TArray<Element, Dimensions>::SetDimensionsCount(int count)
{
	return (count == GetDimensionsCount());
}


template <class Element, int Dimensions>
inline const typename TArray<Element, Dimensions>::SizesType& TArray<Element, Dimensions>::GetSizes() const
{
	return m_sizes;
}


template <class Element, int Dimensions>
inline int TArray<Element, Dimensions>::GetSize(int dimension) const
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < Dimensions);

	return m_sizes[dimension];
}


template <class Element, int Dimensions>
inline const Element& TArray<Element, Dimensions>::GetAt(const IndexType& index) const
{
	I_ASSERT(index.IsInside(m_sizes));

	int elementIndex = GetElementIndex(index);
	I_ASSERT(elementIndex < int(m_elements.size()));

	return m_elements[elementIndex];
}


template <class Element, int Dimensions>
inline Element& TArray<Element, Dimensions>::GetAtRef(const IndexType& index)
{
	I_ASSERT(index.IsInside(m_sizes));

	int elementIndex = GetElementIndex(index);
	I_ASSERT(elementIndex < int(m_elements.size()));

	return m_elements[elementIndex];
}


template <class Element, int Dimensions>
inline void TArray<Element, Dimensions>::SetAt(const IndexType& index, const Element& value)
{
	I_ASSERT(index.IsInside(m_sizes));

	int elementIndex = GetElementIndex(index);
	I_ASSERT(elementIndex < int(m_elements.size()));

	m_elements[elementIndex] = value;
}


// iterator support

template <class Element, int Dimensions>
typename TArray<Element, Dimensions>::Iterator TArray<Element, Dimensions>::Begin() const
{
	return Iterator(const_cast<TArray<Element, Dimensions>*>(this));
}


template <class Element, int Dimensions>
const typename TArray<Element, Dimensions>::Iterator& TArray<Element, Dimensions>::End() const
{
	return s_endIterator;
}


template <class Element, int Dimensions>
inline const Element& TArray<Element, Dimensions>::operator[](const IndexType& index) const
{
	return GetAt(index);
}


template <class Element, int Dimensions>
inline Element& TArray<Element, Dimensions>::operator[](const IndexType& index)
{
	int elementIndex = GetElementIndex(index);
	I_ASSERT(elementIndex < int(m_elements.size()));

	return m_elements[elementIndex];
}


// inline protected methods

template <class Element, int Dimensions>
inline int TArray<Element, Dimensions>::GetElementIndex(const IndexType& index) const
{
	int elementIndex = 0;
	int cumulatedSizes = 1;
	for (int i = 0; i < Dimensions; ++i){
		I_ASSERT(index[i] >= 0);
		I_ASSERT(index[i] < m_sizes[i]);

		elementIndex += index[i] * cumulatedSizes;

		cumulatedSizes *= m_sizes[i];
	}

	return elementIndex;
}


// public methods

template <class Element, int Dimensions>
TArray<Element, Dimensions>::TArray()
{
	for (int i = 0; i < Dimensions; ++i){
		m_sizes[i] = 0;
	}
}


template <class Element, int Dimensions>
TArray<Element, Dimensions>::TArray(const TArray& array)
:	m_sizes(array.m_sizes), m_elements(array.m_elements)
{
}


template <class Element, int Dimensions>
void TArray<Element, Dimensions>::Reset()
{
	m_sizes.Reset();

	m_elements.clear();
}


template <class Element, int Dimensions>
bool TArray<Element, Dimensions>::SetSizes(const SizesType& sizes)
{
	m_sizes = sizes;

	UpdateElementsSize();

	return true;
}


template <class Element, int Dimensions>
bool TArray<Element, Dimensions>::SetSize(int dimension, int size)
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < Dimensions);

	m_sizes[dimension] = size;

	UpdateElementsSize();

	return true;
}


// protected methods

template <class Element, int Dimensions>
void TArray<Element, Dimensions>::UpdateElementsSize()
{
	int cumulatedSizes = 1;
	for (int i = 0; i < Dimensions; ++i){
		cumulatedSizes *= m_sizes[i];
	}

	m_elements.resize(cumulatedSizes);
}


template <class Element, int Dimensions>
void TArray<Element, Dimensions>::DeepCopy(const Elements& elements, const SizesType& sizes)
{
	I_ASSERT(int(elements.size()) == sizes[0] * sizes[1]);

	m_elements = elements;
	m_sizes = sizes;
}


// static attributes

template <class Element, int Dimensions>
typename TArray<Element, Dimensions>::Iterator TArray<Element, Dimensions>::s_endIterator(NULL);


// public methods of embedded class Iterator

template <class Element, int Dimensions>
TArray<Element, Dimensions>::Iterator::Iterator(const Iterator& iterator)
:	BaseClass(iterator), m_arrayPtr(iterator.m_arrayPtr)
{
}


template <class Element, int Dimensions>
const Element& TArray<Element, Dimensions>::Iterator::operator*() const
{
	I_ASSERT(m_arrayPtr != NULL);
	I_ASSERT(IsInside(m_arrayPtr->GetSizes()));

	return m_arrayPtr->GetAt(*this);
}


template <class Element, int Dimensions>
Element& TArray<Element, Dimensions>::Iterator::operator*()
{
	I_ASSERT(m_arrayPtr != NULL);
	I_ASSERT(IsInside(m_arrayPtr->GetSizes()));

	return m_arrayPtr->operator[](*this);
}


template <class Element, int Dimensions>
const Element* TArray<Element, Dimensions>::Iterator::operator->() const
{
	I_ASSERT(m_arrayPtr != NULL);
	I_ASSERT(IsInside(m_arrayPtr->GetSizes()));

	return &m_arrayPtr->GetAt(*this);
}


template <class Element, int Dimensions>
Element* TArray<Element, Dimensions>::Iterator::operator->()
{
	I_ASSERT(m_arrayPtr != NULL);
	I_ASSERT(IsInside(m_arrayPtr->GetSizes()));

	return &m_arrayPtr->operator[](*this);
}


template <class Element, int Dimensions>
typename TArray<Element, Dimensions>::Iterator& TArray<Element, Dimensions>::Iterator::operator++()
{
	if ((m_arrayPtr != NULL) && !Increase(m_arrayPtr->GetSizes())){
		m_arrayPtr = NULL;
	}

	return *this;
}


template <class Element, int Dimensions>
typename TArray<Element, Dimensions>::Iterator TArray<Element, Dimensions>::Iterator::operator++(int)
{
	Iterator retVal = *this;

	if ((m_arrayPtr != NULL) && !Increase(m_arrayPtr->GetSizes())){
		m_arrayPtr = NULL;
	}

	return retVal;
}


template <class Element, int Dimensions>
typename TArray<Element, Dimensions>::Iterator& TArray<Element, Dimensions>::Iterator::operator--()
{
	if ((m_arrayPtr != NULL) && !Decrease(m_arrayPtr->GetSizes())){
		m_arrayPtr = NULL;
	}

	return *this;
}


template <class Element, int Dimensions>
typename TArray<Element, Dimensions>::Iterator TArray<Element, Dimensions>::Iterator::operator--(int)
{
	Iterator retVal = *this;

	if ((m_arrayPtr != NULL) && !Decrease(m_arrayPtr->GetSizes())){
		m_arrayPtr = NULL;
	}

	return retVal;
}


template <class Element, int Dimensions>
typename TArray<Element, Dimensions>::Iterator& TArray<Element, Dimensions>::Iterator::operator=(const Iterator& iterator)
{
	BaseClass::operator=(iterator);

	m_arrayPtr = iterator.m_arrayPtr;
}


template <class Element, int Dimensions>
bool TArray<Element, Dimensions>::Iterator::operator==(const Iterator& iterator) const
{
	if ((m_arrayPtr != NULL) && (iterator.m_arrayPtr != NULL)){
		return (m_arrayPtr == iterator.m_arrayPtr) && (BaseClass::operator==(iterator));
	}

	return (m_arrayPtr == iterator.m_arrayPtr);
}


template <class Element, int Dimensions>
bool TArray<Element, Dimensions>::Iterator::operator!=(const Iterator& iterator) const
{
	return !operator==(iterator);
}


// protected methods of emedded class Iterator

template <class Element, int Dimensions>
TArray<Element, Dimensions>::Iterator::Iterator(TArray* arrayPtr)
:	BaseClass(0), m_arrayPtr(arrayPtr)
{
	if ((m_arrayPtr != NULL) && !IsInside(m_arrayPtr->GetSizes())){
		m_arrayPtr = NULL;	// if it is not inside of array set it directly to the end iterator state
	}
}


} // namespace istd


#endif // !istd_TArray_included


