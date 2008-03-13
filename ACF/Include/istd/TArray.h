#ifndef istd_TArray_included
#define istd_TArray_included


namespace istd
{


/**
	Multidimensional array.
*/
template <class Element, int Dimensions>
class TArray
{
public:
	TArray();
	TArray(const TArray& array);

	/**
		Get number of dimensions of this array.
	*/
	int GetDimensionsCount() const;

	/**
		Get list of all sizes.
	*/
	const TIndex<Dimensions>& GetSizes() const;

	/**
		Set list of all sizes.
	*/
	void SetSizes(const TIndex<Dimensions>& sizes);

	/**
		Get size of array for specified dimension.
	*/
	int GetSize(int dimension) const;

	/**
		Set size of array for specified dimension.
		Please note, that it can make all elements invalid.
	*/
	void SetSize(int dimension, int size);

	/**
		Get element stored at specified index.
	*/
	const Element& GetAt(const TIndex<Dimensions>& index) const;

	/**
		Set element at specified index.
	*/
	void SetAt(const TIndex<Dimensions>& index, const Element& value);

	const Element& operator[](const TIndex<Dimensions>& index) const;
	Element& operator[](const TIndex<Dimensions>& index);

protected:
	/**
		Get index of element in one dimensional array.
	*/
	int GetElementIndex(const TIndex<Dimensions>& index) const;
	/**
		Update size of elements to size changes.
	*/
	void UpdateElementsSize();

private:
	typedef ::std::vector<Element> Elements;
	Elements m_elements;

	TIndex<Dimensions> m_sizes;
};


// inline methods

template <class Element, int Dimensions>
inline int TArray<Element, Dimensions>::GetDimensionsCount() const
{
	return Dimensions;
}


template <class Element, int Dimensions>
inline const TIndex<Dimensions>& TArray<Element, Dimensions>::GetSizes() const
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
inline const TArray<Element, Dimensions>::Element& GetAt(const TIndex<Dimensions>& index) const
{
	int elementIndex = GetElementIndex(index);
	I_ASSERT(elementIndex < int(m_elements.size()));

	return m_elements[elementIndex];
}


template <class Element, int Dimensions>
inline void TArray<Element, Dimensions>::SetAt(const TIndex<Dimensions>& index, const Element& value)
{
	int elementIndex = GetElementIndex(index);
	I_ASSERT(elementIndex < int(m_elements.size()));

	m_elements[elementIndex] = value;
}


template <class Element, int Dimensions>
inline const Element& TArray<Element, Dimensions>::operator[](const TIndex<Dimensions>& index) const
{
	return GetAt(index);
}


template <class Element, int Dimensions>
inline Element& TArray<Element, Dimensions>::operator[](const TIndex<Dimensions>& index)
{
	int elementIndex = GetElementIndex(index);
	I_ASSERT(elementIndex < int(m_elements.size()));

	return m_elements[elementIndex];
}


// inline protected methods

template <class Element, int Dimensions>
inline int TArray<Element, Dimensions>::GetElementIndex(const TIndex<Dimensions>& index) const
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
void TArray<Element, Dimensions>::SetSizes(const TIndex<Dimensions>& sizes)
{
	m_sizes = sizes;

	UpdateElementsSize();
}


template <class Element, int Dimensions>
void TArray<Element, Dimensions>::SetSize(int dimension, int size)
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < Dimensions);

	m_sizes[dimension] = size;

	UpdateElementsSize();
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


} // namespace istd


#endif // !istd_TArray_included


