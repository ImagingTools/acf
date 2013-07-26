#ifndef istd_CVarIndex_included
#define istd_CVarIndex_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/TIndex.h"


namespace istd
{


/**
	Multidimensional index used to addressing index.
*/
class CVarIndex
{
public:
	typedef int IndexType;
	typedef QVector<int>::iterator Iterator;

	/**
		Default constructor without member initialization.
	*/
	CVarIndex();

	/**
		Constructor initializing all member to specified value.
	*/
	explicit CVarIndex(int size, int value = 0);

	/**
		Copy constructor.
	*/
	CVarIndex(const CVarIndex& index);

	/**
		Convert fixed-size index to this object.
	*/
	template <int Dimensions>
	CVarIndex(const TIndex<Dimensions> index);

	/**
		Check if tihs index is valid.
		Index is valid if it has elements and all elements are not negative.
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
		For this (variable size) implementation, it set dimension size to 0.
	*/
	void Reset();

	/**
		Set all components to 0.
	*/
	void Clear();

	/**
		Check, if number dimensions is fixed.
		It is provided for template implementations. It returns always false.
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
		\return			always true.
	*/
	bool SetDimensionsCount(int count);

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
	bool IsInside(const CVarIndex& boundaries) const;

	/**
		Increase this index inside the boundaries.
		Please note, that the number of dimensions for this index will not be changed
		and this operation can be unclear when number of dimensions differs from boundaries number of dimensions.
		\return	false, if increase wasn't possible (e.g. overflow).
	*/
	bool Increase(const CVarIndex& boundaries);

	/**
		Decrese this index inside the boundaries.
		Please note, that the number of dimensions for this index will not be changed
		and this operation can be unclear when number of dimensions differs from boundaries number of dimensions.
		\return	false, if decrease wasn't possible (e.g. overflow).
	*/
	bool Decrease(const CVarIndex& boundaries);

	/**
		Get total number of elements if this index is treated as size.
		\return	multiplication of all elements.
	*/
	int GetProductVolume() const;

	/**
		Get minimal number of dimensions needed to to represent this index.
		In other words this is index of last non-zero component + 1.
	*/
	int GetMinDimensionsCount() const;

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

	/**
		Get access to single index components.
	*/
	int operator[](int index) const;
	/**
		Get access to single index components.
	*/
	int& operator[](int index);

	bool operator==(const CVarIndex& index) const;
	bool operator!=(const CVarIndex& index) const;
	bool operator<(const CVarIndex& index) const;
	bool operator>(const CVarIndex& index) const;
	bool operator<=(const CVarIndex& index) const;
	bool operator>=(const CVarIndex& index) const;

	CVarIndex& operator+=(const CVarIndex& index);
	CVarIndex& operator-=(const CVarIndex& index);

	friend uint qHash(const istd::CVarIndex& index);

private:
	QVector<int> m_elements;
};


// inline methods

inline bool CVarIndex::IsDimensionsCountFixed() const
{
	return false;
}


inline int CVarIndex::GetDimensionsCount() const
{
	return int(m_elements.size());
}


inline bool CVarIndex::SetDimensionsCount(int count)
{
	m_elements.resize(count);

	return true;
}


inline int CVarIndex::GetAt(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_elements.size()));

	return m_elements[index];
}


inline void CVarIndex::SetAt(int index, int value)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_elements.size()));

	m_elements[index] = value;
}


inline bool CVarIndex::IncreaseAt(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_elements.size()));

	++m_elements[index];

	return true;
}


inline bool CVarIndex::DecreaseAt(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_elements.size()));

	--m_elements[index];

	return true;
}


inline int CVarIndex::GetProductVolume() const
{
	int retVal = 1;

	int dimensionsCount = GetDimensionsCount();

	for (int i = 0; i < dimensionsCount; ++i){
		retVal *= GetAt(i);
	}

	return retVal;
}


inline int CVarIndex::GetMinDimensionsCount() const
{
	int dimensionsCount = GetDimensionsCount();

	for (int i = dimensionsCount - 1; i >= 0; --i){
		if (GetAt(i) > 0){
			return i + 1;
		}
	}

	return 0;
}


inline CVarIndex::Iterator CVarIndex::Begin() const
{
	return const_cast<CVarIndex*>(this)->m_elements.begin();
}


inline CVarIndex::Iterator CVarIndex::End() const
{
	return const_cast<CVarIndex*>(this)->m_elements.end();
}


inline int CVarIndex::operator[](int index) const
{
	return GetAt(index);
}


inline int& CVarIndex::operator[](int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_elements.size()));

	return m_elements[index];
}



inline bool CVarIndex::operator<(const CVarIndex& index) const
{
	int count = m_elements.size();
	int indexCount = index.m_elements.size();
	int commonSize = qMin(count, indexCount);
	for (int i = 0; i < commonSize; ++i){
		int element = m_elements[i];
		int indexElement = index.m_elements[i];
		if (element != indexElement){
			return element < indexElement;
		}
	}

	return count < indexCount;
}


inline bool CVarIndex::operator>(const CVarIndex& index) const
{
	int count = m_elements.size();
	int indexCount = index.m_elements.size();
	int commonSize = qMin(count, indexCount);
	for (int i = 0; i < commonSize; ++i){
		int element = m_elements[i];
		int indexElement = index.m_elements[i];
		if (element != indexElement){
			return element > indexElement;
		}
	}

	return count > indexCount;
}


inline bool CVarIndex::operator<=(const CVarIndex& index) const
{
	int count = m_elements.size();
	int indexCount = index.m_elements.size();
	int commonSize = qMin(count, indexCount);
	for (int i = 0; i < commonSize; ++i){
		int element = m_elements[i];
		int indexElement = index.m_elements[i];
		if (element != indexElement){
			return element < indexElement;
		}
	}

	return count <= indexCount;
}


inline bool CVarIndex::operator>=(const CVarIndex& index) const
{
	int count = m_elements.size();
	int indexCount = index.m_elements.size();
	int commonSize = qMin(count, indexCount);
	for (int i = 0; i < commonSize; ++i){
		int element = m_elements[i];
		int indexElement = index.m_elements[i];
		if (element != indexElement){
			return element > indexElement;
		}
	}

	return count >= indexCount;
}


// public template methods

template <int Dimensions>
CVarIndex::CVarIndex(const TIndex<Dimensions> index)
:	m_elements(Dimensions)
{
	for (int i = 0; i < Dimensions; ++i){
		m_elements[i] = index[i];
	}
}


// related methods

uint qHash(const istd::CVarIndex& index);


} // namespace istd


#endif // !istd_CVarIndex_included


