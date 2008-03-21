#ifndef istd_TIndex_included
#define istd_TIndex_included


#include "istd/istd.h"


namespace istd
{


/**
	Multidimensional index used to addressing array.
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
	TIndex(const TIndex& array);

	/**
		Set all components to 0.
	*/
	void Reset();

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

	/**
		Set all components to specified value.
	*/
	void SetAllTo(int value);

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

	Iterator Begin() const;
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
inline int TIndex<Dimensions>::GetDimensionsCount() const
{
	return Dimensions;
}


template <int Dimensions>
void TIndex<Dimensions>::Reset()
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
TIndex<Dimensions>::TIndex(const TIndex& array)
{
	for (int i = 0; i < Dimensions; ++i){
		m_elements[i] = array.m_elements[i];
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


} // namespace istd


#endif // !istd_TIndex_included


