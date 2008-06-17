#ifndef istd_CBinaryIndex_included
#define istd_CBinaryIndex_included


#include "istd/CBitManip.h"


namespace istd
{


class CFastBinaryIndex
{
public:
	typedef int IndexType;

	enum{
		MAX_ELEMENTS_COUNT = 31
	};

	class Iterator
	{
	public:
		Iterator();
		Iterator(const CFastBinaryIndex* indexPtr, int position);
		Iterator(const Iterator& iter);

		int operator*();
		Iterator& operator++();
		Iterator operator++(int);

		bool operator==(const Iterator& iter) const;
		bool operator!=(const Iterator& iter) const;

		Iterator& operator=(const Iterator& iter);

	private:
		int m_position;
		const CFastBinaryIndex* m_indexPtr;
	};

	/**
		Default constructor without member initialization.
	*/
	CFastBinaryIndex();

	/**
		Constructor initializing all member to specified value.
	*/
	explicit CFastBinaryIndex(int size, int value = 0);

	/**
		Constructor initializing all internal members.
		\param	bits	bit coded value of this index.
						The index of higher set bit must be smaller than 'size' value.
		\param	size	number of components of index.
		\param	dummy	not used parameter using to distinguish between this constructor and the previous one.
	*/
	explicit CFastBinaryIndex(I_DWORD bits, int size, int dummy);

	/**
		Copy constructor.
	*/
	CFastBinaryIndex(const CFastBinaryIndex& index);

	/**
		Check if this index is valid.
		Index is valid, if all its components are bigger or equal 0.
		This method is provided for template compatibility with other imlementations. It returns always true.
	*/
	bool IsValid() const;

	/**
		Reset this object.
		For this implementation, it does the same as clear.
		\sa Clear()
	*/
	void Reset();

	/**
		Set all components to 0 (false).
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
		Get bit coded value of this index.
	*/
	I_DWORD GetBits() const;

	/**
		Set all components to specified value.
	*/
	void SetAllTo(int value);

	/**
		Increase single component at specified position.
		\param	index	index of component should be increased. It must be valid.
		\return			true if bit at index was disabled and it was possible to realize increasing.
	*/
	bool IncreaseAt(int index);

	/**
		Decrease single component at specified position.
		\param	index	index of component should be increased. It must be valid.
		\return			true if bit at index was enabled and it was possible to realize increasing.
	*/
	bool DecreaseAt(int index);

	/**
		Check if index is inside boundaries.
		Index is inside boundaries, if all its components are smaller than according boundary components.
	*/
	bool IsInside(const CFastBinaryIndex& boundaries) const;

	/**
		Increase this index inside the boundaries.
		\return	in this implementation always false.
	*/
	bool Increase(const CFastBinaryIndex& boundaries);

	/**
		Decrese this index inside the boundaries.
		\return	in this implementation always false.
	*/
	bool Decrease(const CFastBinaryIndex& boundaries);

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

	bool operator==(const CFastBinaryIndex& index) const;
	bool operator!=(const CFastBinaryIndex& index) const;

private:
	I_DWORD m_bits;
	int m_size;
};


// inline methods

inline CFastBinaryIndex::CFastBinaryIndex()
:	m_bits(0), m_size(0)
{
}


inline CFastBinaryIndex::CFastBinaryIndex(int size, int value)
{
	m_size = size;

	SetAllTo(value);
}


inline CFastBinaryIndex::CFastBinaryIndex(I_DWORD bits, int size, int /*dummy*/)
:	m_bits(bits), m_size(size)
{
	I_ASSERT(CBitManip::instance.GetFirstBitIndex(bits) < size);
}


inline CFastBinaryIndex::CFastBinaryIndex(const CFastBinaryIndex& index)
:	m_bits(index.m_bits), m_size(index.m_size)
{
	I_ASSERT(m_size <= MAX_ELEMENTS_COUNT);
	I_ASSERT((m_bits >> m_size) == 0);
}


inline bool CFastBinaryIndex::IsValid() const
{
	return true;
}


inline void CFastBinaryIndex::Reset()
{
	m_size = 0;
	m_bits = 0;
}


inline void CFastBinaryIndex::Clear()
{
	m_size = 0;
	m_bits = 0;
}


inline bool CFastBinaryIndex::IsDimensionsCountFixed() const
{
	return false;
}


inline int CFastBinaryIndex::GetDimensionsCount() const
{
	return m_size;
}


inline bool CFastBinaryIndex::SetDimensionsCount(int count)
{
	if (count <= MAX_ELEMENTS_COUNT){
		m_size = count;
		m_bits &= I_DWORD(1 << m_size) - 1;

		return true;
	}

	return false;
}


inline int CFastBinaryIndex::GetAt(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < m_size);

	return int(m_bits >> index) & 1;
}


inline void CFastBinaryIndex::SetAt(int index, int value)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < m_size);

	if (value > 0){
		m_bits |= I_DWORD(1 << index);
	}
	else{
		m_bits &= ~I_DWORD(1 << index);
	}
}


inline I_DWORD CFastBinaryIndex::GetBits() const
{
	return m_bits;
}


inline void CFastBinaryIndex::SetAllTo(int value)
{
	if (value){
		m_bits = I_DWORD(1 << m_size) - 1;
	}
	else{
		m_bits = 0;
	}
}


inline bool CFastBinaryIndex::IncreaseAt(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < m_size);

	I_DWORD mask = I_DWORD(1 << index);

	bool retVal = ((m_bits & mask) == 0);

	m_bits |= mask;

	return retVal;
}


inline bool CFastBinaryIndex::DecreaseAt(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < m_size);

	I_DWORD mask = I_DWORD(1 << index);

	bool retVal = ((m_bits & mask) != 0);

	m_bits &= ~mask;

	return retVal;
}


inline bool CFastBinaryIndex::IsInside(const CFastBinaryIndex& boundaries) const
{
	return (((boundaries.m_bits + 1) & I_DWORD(1 << m_size)) != 0);
}


inline bool CFastBinaryIndex::Increase(const CFastBinaryIndex& /*boundaries*/)
{
	return false;
}


inline bool CFastBinaryIndex::Decrease(const CFastBinaryIndex& /*boundaries*/)
{
	return false;
}


inline CFastBinaryIndex::Iterator CFastBinaryIndex::Begin() const
{
	return Iterator(this, 0);
}


inline CFastBinaryIndex::Iterator CFastBinaryIndex::End() const
{
	return Iterator(this, m_size);
}


inline int CFastBinaryIndex::operator[](int index) const
{
	return GetAt(index);
}


inline bool CFastBinaryIndex::operator==(const CFastBinaryIndex& index) const
{
	return (m_size == index.m_size) && (m_bits == index.m_bits);
}


inline bool CFastBinaryIndex::operator!=(const CFastBinaryIndex& index) const
{
	return (m_size != index.m_size) || (m_bits != index.m_bits);
}


// inline methods of embedded class Iterator

inline CFastBinaryIndex::Iterator::Iterator()
:	m_indexPtr(NULL), m_position(0)
{
}


inline CFastBinaryIndex::Iterator::Iterator(const CFastBinaryIndex* indexPtr, int position)
:	m_indexPtr(indexPtr), m_position(position)
{
}


inline CFastBinaryIndex::Iterator::Iterator(const Iterator& iter)
:	m_indexPtr(iter.m_indexPtr), m_position(iter.m_position)
{
}


inline int CFastBinaryIndex::Iterator::operator*()
{
	I_ASSERT(m_indexPtr != NULL);

	return m_indexPtr->GetAt(m_position);
}


inline CFastBinaryIndex::Iterator& CFastBinaryIndex::Iterator::operator++()
{
	++m_position;

	return *this;
}


inline CFastBinaryIndex::Iterator CFastBinaryIndex::Iterator::operator++(int)
{
	Iterator retVal(*this);

	++m_position;

	return retVal;
}


inline bool CFastBinaryIndex::Iterator::operator==(const Iterator& iter) const
{
	return (m_indexPtr == iter.m_indexPtr) && (m_position == iter.m_position);
}


inline bool CFastBinaryIndex::Iterator::operator!=(const Iterator& iter) const
{
	return (m_indexPtr != iter.m_indexPtr) || (m_position != iter.m_position);
}


inline CFastBinaryIndex::Iterator& CFastBinaryIndex::Iterator::operator=(const Iterator& iter)
{
	m_indexPtr = iter.m_indexPtr;
	m_position = iter.m_position;

	return *this;
}


} // namespace istd


#endif // !istd_CBinaryIndex_included


