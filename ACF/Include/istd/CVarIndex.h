#ifndef istd_CVarIndex_included
#define istd_CVarIndex_included


#include <vector>

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
	typedef std::vector<int>::iterator Iterator;

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
		Check if index is inside boundaries.
		Index is inside boundaries, if all its components are smaller than according boundary components.
	*/
	bool IsInside(const CVarIndex& boundaries) const;

	/**
		Increase this index inside the boundaries.
		\return	false, if increase wasn't possible (e.g. overflow).
	*/
	bool Increase(const CVarIndex& boundaries);

	/**
		Decrese this index inside the boundaries.
		\return	false, if decrease wasn't possible (e.g. overflow).
	*/
	bool Decrease(const CVarIndex& boundaries);

	Iterator Begin() const;
	Iterator End() const;

	int operator[](int index) const;
	int& operator[](int index);

	bool operator==(const CVarIndex& index) const;
	bool operator!=(const CVarIndex& index) const;

private:
	std::vector<int> m_elements;
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


inline bool CVarIndex::SetDimensionsCount(int count) const
{
	return (count == GetDimensionsCount());
}


inline int CVarIndex::GetAt(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

	return m_elements[index];
}


inline void CVarIndex::SetAt(int index, int value)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

	m_elements[index] = value;
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
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

	return m_elements[index];
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


} // namespace istd


#endif // !istd_CVarIndex_included


