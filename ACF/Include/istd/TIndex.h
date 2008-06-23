#ifndef istd_TIndex_included
#define istd_TIndex_included


#include "istd/istd.h"


namespace istd
{


/**
	Multidimensional index used to addressing fixed-size array.
*/
template <int Dimensions>
class TIndex
{
public:
	typedef int IndexType;
	typedef int* Iterator;

	enum
	{
		DIMENSIONS = Dimensions
	};

	/**
		Default constructor without member initialization.
	*/
	TIndex();

	/**
		Constructor initializing all member to specified value.
	*/
	explicit TIndex(int value);

	/**
		Copy constructor.
	*/
	TIndex(const TIndex& index);

	/**
		Check if this index is valid.
		Index is valid, if all its components are bigger or equal 0.
	*/
	bool IsValid() const;

	/**
		Check if this index point at zero element.
		In other words, it checks if all components are 0.
	*/
	bool IsZero() const;

	/**
		Check if this index interpreted as size is empty.
		It means, it returns true, is some of components is less or equal 0.
	*/
	bool IsSizeEmpty() const;

	/**
		Reset this object.
		For this (fixed-size) implementation, it does the same as clear.
		\sa Clear()
	*/
	void Reset();

	/**
		Set all components to 0.
	*/
	void Clear();

	/**
		Check, if number dimensions is fixed.
		It is provided for template implementations. It returns always true.
	*/
	bool IsDimensionsCountFixed() const;

	/**
		Get number of dimensions of this index.
	*/
	int GetDimensionsCount() const;

	/**
		Set number of dimensions of this index.
		It is provided to allows template implementations to use fixed-size or variable arrays.
		\param	count	number of dimensions will be set.
		\return			true, if number of set dimensions equals template parameter or false if isn't.
	*/
	bool SetDimensionsCount(int count) const;

	/**
		Get element stored at specified index.
	*/
	int GetAt(int index) const;

	/**
		Set element at specified index.
	*/
	void SetAt(int index, int value);

	/**
		Set all components to specified value.
	*/
	void SetAllTo(int value);

	/**
		Increase single component at specified position.
		\param	index	index of component should be increased. It must be valid.
		\return			true if success. It is provided for template implementations. In this case it returns always true.
	*/
	bool IncreaseAt(int index);

	/**
		Decrease single component at specified position.
		\param	index	index of component should be increased. It must be valid.
		\return			true if success. It is provided for template implementations. In this case it returns always true.
	*/
	bool DecreaseAt(int index);

	/**
		Check if index is inside boundaries.
		Index is inside boundaries, if all its components are smaller than according boundary components.
	*/
	bool IsInside(const TIndex& boundaries) const;

	/**
		Increase this index inside the boundaries.
		\return	false, if increase wasn't possible (e.g. overflow).
	*/
	bool Increase(const TIndex& boundaries);

	/**
		Decrese this index inside the boundaries.
		\return	false, if decrease wasn't possible (e.g. overflow).
	*/
	bool Decrease(const TIndex& boundaries);

	/**
		Get total number of elements if this index is treated as size.
		\return	multiplication of all elements.
	*/
	int GetProductVolume() const;

	/**
		Get begin value of element access iterator.
		Please refer to general description of ACF iterators, STL iterators or Qt iterators concept.
	*/
	Iterator Begin() const;
	/**
		Get end value of element access iterator.
		Please refer to general description of ACF iterators, STL iterators or Qt iterators concept.
	*/
	Iterator End() const;

	int operator[](int index) const;
	int& operator[](int index);

	bool operator==(const TIndex& index) const;
	bool operator!=(const TIndex& index) const;

private:
	int m_elements[Dimensions];
};


// inline methods

template <int Dimensions>
inline bool TIndex<Dimensions>::IsDimensionsCountFixed() const
{
	return true;
}


template <int Dimensions>
inline int TIndex<Dimensions>::GetDimensionsCount() const
{
	return Dimensions;
}


template <int Dimensions>
inline bool TIndex<Dimensions>::SetDimensionsCount(int count) const
{
	return (count == GetDimensionsCount());
}


template <int Dimensions>
inline bool TIndex<Dimensions>::IsValid() const
{
	for (int i = 0; i < Dimensions; ++i){
		if (m_elements[i] < 0){
			return false;
		}
	}

	return true;
}


template <int Dimensions>
inline bool TIndex<Dimensions>::IsZero() const
{
	for (int i = 0; i < Dimensions; ++i){
		if (m_elements[i] > 0){
			return false;
		}
	}

	return true;
}


template <int Dimensions>
inline bool TIndex<Dimensions>::IsSizeEmpty() const
{
	for (int i = 0; i < Dimensions; ++i){
		if (m_elements[i] <= 0){
			return true;
		}
	}

	return false;
}


template <int Dimensions>
inline void TIndex<Dimensions>::Reset()
{
	Clear();
}


template <int Dimensions>
inline void TIndex<Dimensions>::Clear()
{
	SetAllTo(0);
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
inline bool TIndex<Dimensions>::IncreaseAt(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < Dimensions);

	++m_elements[index];

	return true;
}


template <int Dimensions>
inline bool TIndex<Dimensions>::DecreaseAt(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < Dimensions);

	--m_elements[index];

	return true;
}


template <int Dimensions>
inline typename TIndex<Dimensions>::Iterator TIndex<Dimensions>::Begin() const
{
	return (Iterator)&m_elements[0];
}


template <int Dimensions>
inline typename TIndex<Dimensions>::Iterator TIndex<Dimensions>::End() const
{
	return (Iterator)&m_elements[Dimensions];
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


template <int Dimensions>
bool TIndex<Dimensions>::operator==(const TIndex& index) const
{
	for (int i = 0; i < Dimensions; ++i){
		if (m_elements[i] != index.m_elements[i]){
			return false;
		}
	}

	return true;
}


template <int Dimensions>
bool TIndex<Dimensions>::operator!=(const TIndex& index) const
{
	return !operator==(index);
}


// public methods

template <int Dimensions>
TIndex<Dimensions>::TIndex(int value)
{
	SetAllTo(value);
}


template <int Dimensions>
TIndex<Dimensions>::TIndex()
{
	for (int i = 0; i < Dimensions; ++i){
		m_elements[i] = 0;
	}
}


template <int Dimensions>
TIndex<Dimensions>::TIndex(const TIndex& index)
{
	for (int i = 0; i < Dimensions; ++i){
		m_elements[i] = index.m_elements[i];
	}
}


template <int Dimensions>
void TIndex<Dimensions>::SetAllTo(int value)
{
	for (int i = 0; i < Dimensions; ++i){
		m_elements[i] = value;
	}
}


template <int Dimensions>
bool TIndex<Dimensions>::IsInside(const TIndex& boundaries) const
{
	for (int i = 0; i < Dimensions; ++i){
		I_ASSERT(m_elements[i] >= 0);

		if (m_elements[i] >= boundaries.m_elements[i]){
			return false;
		}
	}

	return true;
}


template <int Dimensions>
bool TIndex<Dimensions>::Increase(const TIndex& boundaries)
{
	I_ASSERT(IsInside(boundaries));

	for (int i = 0; i < Dimensions; ++i){
		if (m_elements[i] < boundaries.m_elements[i] - 1){
			m_elements[i]++;
			for (int j = 0; j < i; ++j){
				m_elements[j] = 0;
			}

			return true;
		}
	}

	SetAllTo(0);

	return false;
}


template <int Dimensions>
bool TIndex<Dimensions>::Decrease(const TIndex& boundaries)
{
	I_ASSERT(IsInside(boundaries));

	for (int i = 0; i < Dimensions; ++i){
		if (m_elements[i] > 0){
			m_elements[i]--;

			for (int j = 0; j < i; ++j){
				m_elements[j] = boundaries.m_elements[j] - 1;
			}

			return true;
		}
	}

	for (int j = 0; j < Dimensions; ++j){
		m_elements[j] = boundaries.m_elements[j] - 1;
	}

	return false;
}


template <int Dimensions>
int TIndex<Dimensions>::GetProductVolume() const
{
	int retVal = 1;

	for (int i = 0; i < Dimensions; ++i){
		retVal *= m_elements[i];
	}

	return retVal;
}


} // namespace istd


#endif // !istd_TIndex_included


