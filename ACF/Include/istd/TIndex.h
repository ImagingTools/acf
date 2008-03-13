#ifndef istd_TIndex_included
#define istd_TIndex_included


namespace istd
{


/**
	Multidimensional index used to addressing array.
*/
template <int Dimensions>
class TIndex
{
public:
	TIndex();
	TIndex(const TIndex& array);

	/**
		Get number of dimensions of this array.
	*/
	int GetDimensionsCount() const;

	/**
		Get element stored at specified index.
	*/
	int GetAt(int index) const;

	/**
		Set element at specified index.
	*/
	void SetAt(int index, int value);

	int operator[](int index) const;
	int operator[](int index);

private:
	int m_elements[Dimensions];
};


// inline methods

template <int Dimensions>
inline int TIndex<Dimensions>::GetDimensionsCount() const
{
	return Dimensions;
}


template <int Dimensions>
inline int GetAt(int index) const
{
	int elementIndex = GetElementIndex(index);
	I_ASSERT(elementIndex < int(m_elements.size()));

	return m_elements[elementIndex];
}


template <int Dimensions>
inline void TIndex<Dimensions>::SetAt(const TIndex<Dimensions>& index, const Element& value)
{
	int elementIndex = GetElementIndex(index);
	I_ASSERT(elementIndex < int(m_elements.size()));

	m_elements[elementIndex] = value;
}


template <int Dimensions>
inline const Element& TIndex<Dimensions>::operator[](const TIndex<Dimensions>& index) const
{
	return GetAt(index);
}


template <int Dimensions>
inline Element& TIndex<Dimensions>::operator[](const TIndex<Dimensions>& index)
{
	int elementIndex = GetElementIndex(index);
	I_ASSERT(elementIndex < int(m_elements.size()));

	return m_elements[elementIndex];
}


// public methods

template <int Dimensions>
TIndex<Dimensions>::TIndex()
{
	for (int i = 0; i < Dimensions; ++i){
		m_sizes[i] = 0;
	}
}


template <int Dimensions>
TIndex<Dimensions>::TIndex(const TIndex& array)
:	m_sizes(array.m_sizes), m_elements(array.m_elements)
{
}


} // namespace istd


#endif // !istd_TIndex_included


