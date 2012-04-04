#ifndef imath_TVector_included
#define imath_TVector_included


// STL includes
#include <cmath>

#include "iser/IArchive.h"

#include "imath/imath.h"


namespace imath
{


/**
	Implementation of fixed-size mathematical vector with specified type of elements.
*/
template <int Size, class Element = double>
class TVector
{
public:
	typedef Element ElementType;
    typedef Element Elements[Size];

	/**
		Create an uninitialized point.
	 */
	TVector();
	/**
		Copy constructor.
	 */
	TVector(const TVector<Size, Element>& vector);

	/**
		Get element at specified i.
	*/
	const Element& GetElement(int i) const;

	/**
		Get reference to element at specified i.
	*/
	Element& GetElementRef(int i);

	/**
		Set element at specified i.
	*/
	void SetElement(int i, const Element& value);

	/**
		Set all coordinates to zero.
		It makes the same as Clear() and is used for compatibility with other vector implementations.
	*/
	void Reset();

	/**
		Set all coordinates to zero.
		It makes the same as Clear() and is used for compatibility with other vector implementations.
	*/
	void Clear();

	/**
		Get read-only access to internal element container.
	*/
	const typename TVector<Size, Element>::Elements& GetElements() const;
	/**
		Get access to internal element container.
	*/
	typename TVector<Size, Element>::Elements& GetElementsRef();

	/**
		Translate the point.
	*/
	void Translate(const TVector<Size, Element>& vector);

	/**
		Get translated point.
	*/
	TVector<Size, Element> GetTranslated(const TVector<Size, Element>& vector);

	/**
		/overloaded
	*/
	void GetTranslated(const TVector<Size, Element>& vector, TVector<Size, Element>& result);

	/**
		Add second vector scaled by specified factor.
		It is equal of Translate(vector * scale) but can be faster implemented.
	*/
	void ScaledCumulate(const TVector<Size, Element>& vector, Element scale);

	/**
		Check if this vector is null.
	*/
	bool IsNull(Element tolerance = I_BIG_EPSILON) const;

	/**
		Return dot product of two vectors.
	*/
	Element GetDotProduct(const TVector<Size, Element>& vector) const;

	/**
		Return euclidian length square.
	*/
	Element GetLength2() const;
	/**
		Return euclidian length.
	*/
	Element GetLength() const;

	/**
		Return distance square between two vectors.
	*/
	Element GetDistance2(const TVector<Size, Element>& vector) const;

	/**
		Return distance between two vectors.
	*/
	Element GetDistance(const TVector<Size, Element>& vector) const;

	/**
		Get simple sum of all elements.
	*/
	Element GetElementsSum() const;

	/**
		Normalize vector to specified length.
		\param	length	new vector length.
		\return	true, if normalization successed.
	*/
	bool Normalize(Element length = 1.0);
	/**
		Return normalized vector with the same direction and specified length.
		\param	length	new vector length.
		\return	true, if normalization successed.
	*/
	bool GetNormalized(TVector<Size, Element>& result, Element length = 1.0) const;

	/**
		Get vector with minimal elements values.
	*/
	void GetMinimal(const TVector<Size, Element>& vector, TVector<Size, Element>& result) const;
	/**
		Get vector with maximal elements values.
	*/
	void GetMaximal(const TVector<Size, Element>& vector, TVector<Size, Element>& result) const;

	/**
		Serialize this vector to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);

	bool operator==(const TVector<Size, Element>& vector) const;
	bool operator!=(const TVector<Size, Element>& vector) const;
	bool operator<(const TVector<Size, Element>& vector) const;
	bool operator>(const TVector<Size, Element>& vector) const;
	bool operator<=(const TVector<Size, Element>& vector) const;
	bool operator>=(const TVector<Size, Element>& vector) const;

	TVector<Size, Element> operator-() const;

	TVector<Size, Element> operator+(const TVector<Size, Element>& vector) const;
	TVector<Size, Element> operator-(const TVector<Size, Element>& vector) const;
	TVector<Size, Element> operator*(Element scalar) const;
	TVector<Size, Element> operator/(Element scalar) const;

	TVector<Size, Element>& operator+=(const TVector<Size, Element>& vector);
	TVector<Size, Element>& operator-=(const TVector<Size, Element>& vector);
	TVector<Size, Element>& operator*=(Element scalar);
	TVector<Size, Element>& operator/=(Element scalar);

	const Element& operator[](int i) const;
	Element& operator[](int i);

	// static methods

	/**
		Get number of elements.
	*/
	static int GetElementsCount();

	/**
		Set number of elements.
		This method is provided for template implementations.
		\param	count	number of elements.
		\return			true, if the number of set elements equals template parameter.
	*/
	static bool SetElementsCount(int count);

protected:
    Elements m_elements;
};


// inline constructors

template <int Size, class Element>
inline TVector<Size, Element>::TVector()
{
}


template <int Size, class Element>
inline TVector<Size, Element>::TVector(const TVector<Size, Element>& vector)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] = vector.m_elements[i];
	}
}


// inline methods

template <int Size, class Element>
inline const Element& TVector<Size, Element>::GetElement(int i) const
{
	return operator[](i);
}


template <int Size, class Element>
inline Element& TVector<Size, Element>::GetElementRef(int i)
{
	return operator[](i);
}


template <int Size, class Element>
inline void TVector<Size, Element>::SetElement(int i, const Element& value)
{
	operator[](i) = value;
}


template <int Size, class Element>
inline void TVector<Size, Element>::Reset()
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] = 0;
	}
}


template <int Size, class Element>
inline void TVector<Size, Element>::Clear()
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] = 0;
	}
}


template <int Size, class Element>
inline const typename TVector<Size, Element>::Elements& TVector<Size, Element>::GetElements() const
{
	return m_elements;
}


template <int Size, class Element>
inline typename TVector<Size, Element>::Elements& TVector<Size, Element>::GetElementsRef()
{
	return m_elements;
}


template <int Size, class Element>
inline void TVector<Size, Element>::Translate(const TVector<Size, Element>& vector)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] += vector.m_elements[i];
	}
}


template <int Size, class Element>
TVector<Size, Element> TVector<Size, Element>::GetTranslated(const TVector<Size, Element>& vector)
{
	return *this + vector;
}


template <int Size, class Element>
void TVector<Size, Element>::GetTranslated(const TVector<Size, Element>& vector, TVector<Size, Element>& result)
{
	result = *this + vector;
}


template <int Size, class Element>
inline void TVector<Size, Element>::ScaledCumulate(const TVector<Size, Element>& vector, Element scale)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] += vector.m_elements[i] * scale;
	}
}


template <int Size, class Element>
inline bool TVector<Size, Element>::IsNull(Element tolerance) const
{
	return GetLength2() <= tolerance * tolerance;
}


template <int Size, class Element>
inline Element TVector<Size, Element>::GetDotProduct(const TVector<Size, Element>& vector) const
{
	Element retVal = 0.0;

	for (int i = 0; i < Size; ++i){
		retVal +=  m_elements[i] * vector.m_elements[i];
	}

	return retVal;
}


template <int Size, class Element>
inline Element TVector<Size, Element>::GetLength2() const
{
	return GetDotProduct(*this);
}


template <int Size, class Element>
inline Element TVector<Size, Element>::GetLength() const
{
	return std::sqrt(GetLength2());
}


template <int Size, class Element>
inline Element TVector<Size, Element>::GetDistance2(const TVector<Size, Element>& vector) const
{
	return (*this - vector).GetLength2();
}


template <int Size, class Element>
inline Element TVector<Size, Element>::GetDistance(const TVector<Size, Element>& vector) const
{
	return std::sqrt(GetDistance2(vector));
}


// operators

template <int Size, class Element>
inline bool TVector<Size, Element>::operator==(const TVector<Size, Element>& vector) const
{
	for (int i = 0; i < Size; ++i){
		if (m_elements[i] != vector.m_elements[i]){
			return false;
		}
	}
	return true;
}


template <int Size, class Element>
inline bool TVector<Size, Element>::operator!=(const TVector<Size, Element>& vector) const
{
	return !operator==(vector);
}


template <int Size, class Element>
bool TVector<Size, Element>::operator<(const TVector<Size, Element>& vector) const
{
	for (int i = 0; i < Size; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return false;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return true;
		}
	}

	return false;
}


template <int Size, class Element>
bool TVector<Size, Element>::operator>(const TVector<Size, Element>& vector) const
{
	for (int i = 0; i < Size; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return true;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return false;
		}
	}

	return false;
}


template <int Size, class Element>
bool TVector<Size, Element>::operator<=(const TVector<Size, Element>& vector) const
{
	for (int i = 0; i < Size; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return false;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return true;
		}
	}

	return true;
}


template <int Size, class Element>
bool TVector<Size, Element>::operator>=(const TVector<Size, Element>& vector) const
{
	for (int i = 0; i < Size; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return true;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return false;
		}
	}

	return true;
}


template <int Size, class Element>
inline TVector<Size, Element>& TVector<Size, Element>::operator+=(const TVector<Size, Element>& vector)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] += vector.m_elements[i];
	}

	return *this;
}


template <int Size, class Element>
inline TVector<Size, Element>& TVector<Size, Element>::operator-=(const TVector<Size, Element>& vector)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] -= vector.m_elements[i];
	}

	return *this;
}


template <int Size, class Element>
inline TVector<Size, Element>& TVector<Size, Element>::operator*=(Element scalar)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] *= scalar;
	}

	return *this;
}


template <int Size, class Element>
inline TVector<Size, Element>& TVector<Size, Element>::operator/=(Element scalar)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] /= scalar;
	}

	return *this;
}


template <int Size, class Element>
inline TVector<Size, Element> TVector<Size, Element>::operator-() const
{
	TVector<Size, Element> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = -m_elements[i];
	}

	return retVal;
}


template <int Size, class Element>
inline TVector<Size, Element> TVector<Size, Element>::operator+(const TVector<Size, Element>& vector) const
{
	TVector<Size, Element> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] + vector.m_elements[i];
	}

	return retVal;
}


template <int Size, class Element>
inline TVector<Size, Element> TVector<Size, Element>::operator-(const TVector<Size, Element>& vector) const
{
	TVector<Size, Element> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] - vector.m_elements[i];
	}

	return retVal;
}


template <int Size, class Element>
inline TVector<Size, Element> TVector<Size, Element>::operator*(Element scalar) const
{
	TVector<Size, Element> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] * scalar;
	}

	return retVal;
}


template <int Size, class Element>
inline TVector<Size, Element> TVector<Size, Element>::operator/(Element scalar) const
{
	TVector<Size, Element> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] / scalar;
	}

	return retVal;
}


template <int Size, class Element>
const Element& TVector<Size, Element>::operator[](int i) const
{
	I_ASSERT(i >= 0);
	I_ASSERT(i < Size);

	return m_elements[i];
}


template <int Size, class Element>
Element& TVector<Size, Element>::operator[](int i)
{
	I_ASSERT(i >= 0);
	I_ASSERT(i < Size);

	return m_elements[i];
}


// static inline methods

template <int Size, class Element>
inline int TVector<Size, Element>::GetElementsCount()
{
	return Size;
}


template <int Size, class Element>
inline bool TVector<Size, Element>::SetElementsCount(int count)
{
	return count == Size;
}


// public methods

template <int Size, class Element>
Element TVector<Size, Element>::GetElementsSum() const
{
	Element retVal = 0;

	for (int i = 0; i < Size; ++i){
        retVal += m_elements[i];
    }

	return retVal;
}


template <int Size, class Element>
bool TVector<Size, Element>::Normalize(Element length)
{
    Element isLength = GetLength();

    Element proportion = isLength / length;

	if (::fabs(proportion) > I_BIG_EPSILON){
        for (int i = 0; i < Size; ++i){
            m_elements[i] = m_elements[i] / proportion;
        }

		return true;
    }
	else{
		return false;
	}
}


template <int Size, class Element>
bool TVector<Size, Element>::GetNormalized(TVector<Size, Element>& result, Element length) const
{
    Element isLength = GetLength();

    Element proportion = isLength / length;

	if (::fabs(proportion) > I_BIG_EPSILON){
        for (int i = 0; i < Size; ++i){
            result.m_elements[i] = m_elements[i] / proportion;
        }

		return true;
    }
	else{
		return false;
	}
}


template <int Size, class Element>
void TVector<Size, Element>::GetMinimal(const TVector<Size, Element>& vector, TVector<Size, Element>& result) const
{
	for (int i = 0; i < Size; ++i){
		result.SetElement(i, qMin(GetElement(i), vector.GetElement(i)));
	}
}


template <int Size, class Element>
void TVector<Size, Element>::GetMaximal(const TVector<Size, Element>& vector, TVector<Size, Element>& result) const
{
	for (int i = 0; i < Size; ++i){
		result.SetElement(i, qMax(GetElement(i), vector.GetElement(i)));
	}
}


template <int Size, class Element>
bool TVector<Size, Element>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	for (int i = 0; i < Size; ++i){
		retVal = retVal && archive.Process(m_elements[i]);
	}

	return retVal;
}


} // namespace imath


#endif // !imath_TVector_included


