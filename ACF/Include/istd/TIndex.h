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
inline int TIndex<Dimensions>::GetAt(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < Dimensions);

	return m_elements[index];
}


template <int Dimensions>
inline void TIndex<Dimensions>::SetAt(int index, int value)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < Dimensions);

	m_elements[index] = value;
}


template <int Dimensions>
inline int TIndex<Dimensions>::operator[](int index) const
{
	return GetAt(index);
}


template <int Dimensions>
inline int& TIndex<Dimensions>::operator[](int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < Dimensions);

	return m_elements[index];
}


// public methods

template <int Dimensions>
TIndex<Dimensions>::TIndex()
{
	for (int i = 0; i < Dimensions; ++i){
		m_elements[i] = 0;
	}
}


template <int Dimensions>
TIndex<Dimensions>::TIndex(const TIndex& array)
:	m_elements(array.m_elements)
{
}


} // namespace istd


#endif // !istd_TIndex_included


