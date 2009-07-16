#ifndef imath_TFastVector_included
#define imath_TFastVector_included


// STL includes
#include <cmath>
#include <memory.h>

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imath/TVector.h"


namespace imath
{


/**
	Simple implementation of fixed-size vector.
*/
template <int MaxSize, class Element = double>
class TFastVector
{
public:
	typedef Element ElementType;

	enum{
		MAX_ELEMENTS_COUNT = MaxSize
	};

	/**
		Create an uninitialized point.
	 */
	TFastVector();

	/**
		Create vector and initialize number of components.
	*/
	explicit TFastVector(int componentsCount, const Element& value = Element());

	/**
		Copy constructor.
	 */
	TFastVector(const TFastVector<MaxSize, Element>& vector);

	template <int Size>
	TFastVector(const imath::TVector<Size, Element>& vector)
	:	m_elementsCount(Size)
	{
		I_ASSERT(Size <= MaxSize);

		for (int i = 0; i < Size; ++i){
			m_elements[i] = vector[i];
		}
	}

	/**
		Get number of elements.
	*/
	int GetElementsCount() const;

	/**
		Set number of elements.
		\param	count	number of elements.
		\return			true, if the number of set is not greater than template parameter MaxSize.
	*/
	bool SetElementsCount(int count, const Element& value = Element());

	/**
		Ensure, that number of elements vector cannot be smaller that some value.
		If number of elements was bigger or equal to specified value, it does nothing.
		In other case, number of elements will be set.
		\param	count	number of elements.
		\return			true, if the number of set is not greater than template parameter MaxSize.
	*/
	bool EnsureElementsCount(int count, const Element& value = Element());

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
	*/
	void Clear();

	/**
		Set elemenents from other vector without resizing.
		\param	vector			source of element values will be copied.
		\param	expansionValue	if actual vector has more elements than \c vector, rest will be replaced with this value.
	*/
	void SetElementsFrom(const TFastVector& vector, const Element& expansionValue = Element());

	/**
		Translate the point.
	*/
	void Translate(const TFastVector<MaxSize, Element>& vector);

	/**
		Get translated point.
	*/
	TFastVector<MaxSize, Element> GetTranslated(const TFastVector<MaxSize, Element>& vector);

	/**
		/overloaded
	*/
	void GetTranslated(const TFastVector<MaxSize, Element>& vector, TFastVector<MaxSize, Element>& result);

	/**
		Add second vector scaled by specified factor.
		It is equal of Translate(vector * scale) but can be faster implemented.
	*/
	void ScaledCumulate(const TFastVector<MaxSize, Element>& vector, Element scale);

	/**
		Check if this vector is null.
	*/
	bool IsNull(Element tolerance = I_BIG_EPSILON) const;

	/**
		Return dot product of two vectors.
	*/
	Element GetDotProduct(const TFastVector<MaxSize, Element>& vector) const;

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
	Element GetDistance2(const TFastVector<MaxSize, Element>& vector) const;

	/**
		Return distance between two vectors.
	*/
	Element GetDistance(const TFastVector<MaxSize, Element>& vector) const;

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
	bool GetNormalized(TFastVector<MaxSize, Element>& result, Element length = 1.0) const;

	/**
		Serialize this vector to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);

	bool operator==(const TFastVector<MaxSize, Element>& vector) const;
	bool operator!=(const TFastVector<MaxSize, Element>& vector) const;
	bool operator<(const TFastVector<MaxSize, Element>& vector) const;
	bool operator>(const TFastVector<MaxSize, Element>& vector) const;
	bool operator<=(const TFastVector<MaxSize, Element>& vector) const;
	bool operator>=(const TFastVector<MaxSize, Element>& vector) const;

	TFastVector<MaxSize, Element> operator-() const;

	TFastVector<MaxSize, Element> operator+(const TFastVector<MaxSize, Element>& vector) const;
	TFastVector<MaxSize, Element> operator-(const TFastVector<MaxSize, Element>& vector) const;
	TFastVector<MaxSize, Element> operator*(Element scalar) const;
	TFastVector<MaxSize, Element> operator/(Element scalar) const;

	TFastVector<MaxSize, Element>& operator=(const TFastVector<MaxSize, Element>& vector);

	TFastVector<MaxSize, Element>& operator+=(const TFastVector<MaxSize, Element>& vector);
	TFastVector<MaxSize, Element>& operator-=(const TFastVector<MaxSize, Element>& vector);
	TFastVector<MaxSize, Element>& operator*=(Element scalar);
	TFastVector<MaxSize, Element>& operator/=(Element scalar);

	const Element& operator[](int i) const;
	Element& operator[](int i);

protected:
    Element m_elements[MaxSize];
	int m_elementsCount;
};


// inline methods

template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>::TFastVector()
:	m_elementsCount(0)
{
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>::TFastVector(int componentsCount, const Element& value)
:	m_elementsCount(istd::Min(MaxSize, componentsCount))
{
	for (int i = 0; i < m_elementsCount; ++i){
		m_elements[i] = value;
	}
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>::TFastVector(const TFastVector<MaxSize, Element>& vector)
:	m_elementsCount(vector.m_elementsCount)
{
	I_ASSERT(m_elementsCount <= MaxSize);

	memcpy(m_elements, vector.m_elements, sizeof(Element) * m_elementsCount);
}


template <int MaxSize, class Element>
inline int TFastVector<MaxSize, Element>::GetElementsCount() const
{
	return m_elementsCount;
}


template <int MaxSize, class Element>
inline bool TFastVector<MaxSize, Element>::SetElementsCount(int count, const Element& value)
{
	if (count <= MaxSize){
		for (int i = m_elementsCount; i < count; ++i){
			m_elements[i] = value;
		}

		m_elementsCount = count;

		return true;
	}
	else{
		return false;
	}
}


template <int MaxSize, class Element>
inline bool TFastVector<MaxSize, Element>::EnsureElementsCount(int count, const Element& value)
{
	if (count <= MaxSize){
		if (m_elementsCount < count){
			for (int i = m_elementsCount; i < count; ++i){
				m_elements[i] = value;
			}

			m_elementsCount = count;
		}

		return true;
	}
	else{
		return false;
	}
}


template <int MaxSize, class Element>
inline const Element& TFastVector<MaxSize, Element>::GetElement(int i) const
{
	I_ASSERT(i >= 0);
	I_ASSERT(i < m_elementsCount);

	return m_elements[i];
}


template <int MaxSize, class Element>
inline Element& TFastVector<MaxSize, Element>::GetElementRef(int i)
{
	I_ASSERT(i >= 0);
	I_ASSERT(i < m_elementsCount);

	return m_elements[i];
}


template <int MaxSize, class Element>
inline void TFastVector<MaxSize, Element>::SetElement(int i, const Element& value)
{
	I_ASSERT(i >= 0);
	I_ASSERT(i < m_elementsCount);

	m_elements[i] = value;
}


template <int MaxSize, class Element>
inline void TFastVector<MaxSize, Element>::Clear()
{
	for (int i = 0; i < m_elementsCount; ++i){
		m_elements[i] = 0.0;
	}
}


template <int MaxSize, class Element>
inline void TFastVector<MaxSize, Element>::Translate(const TFastVector<MaxSize, Element>& vector)
{
	int commonSize = istd::Min(m_elementsCount, vector.m_elementsCount);
	for (int i = 0; i < commonSize; ++i){
		m_elements[i] += vector.m_elements[i];
	}
}


template <int MaxSize, class Element>
inline void TFastVector<MaxSize, Element>::ScaledCumulate(const TFastVector<MaxSize, Element>& vector, Element scale)
{
	if (m_elementsCount < vector.m_elementsCount){
		int i = 0;
		for (; i < m_elementsCount; ++i){
			m_elements[i] += vector.m_elements[i] * scale;
		}

		for (; i < vector.m_elementsCount; ++i){
			m_elements[i] = vector.m_elements[i] * scale;
		}

		m_elementsCount = vector.m_elementsCount;
	}
	else{
		for (int i = 0; i < vector.m_elementsCount; ++i){
			m_elements[i] += vector.m_elements[i] * scale;
		}
	}
}


template <int MaxSize, class Element>
void TFastVector<MaxSize, Element>::SetElementsFrom(const TFastVector& vector, const Element& expansionValue)
{
	int commonSize = istd::Min(m_elementsCount, vector.m_elementsCount);

	for (int i = 0; i < commonSize; ++i){
		SetElement(i, vector[i]);
	}

	for (int i = commonSize; i < m_elementsCount; ++i){
		SetElement(i, expansionValue);
	}
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element> TFastVector<MaxSize, Element>::GetTranslated(const TFastVector<MaxSize, Element>& vector)
{
	return *this + vector;
}


template <int MaxSize, class Element>
void TFastVector<MaxSize, Element>::GetTranslated(const TFastVector<MaxSize, Element>& vector, TFastVector<MaxSize, Element>& result)
{

	result = *this;
	result.Translate(vector);
}


template <int MaxSize, class Element>
inline bool TFastVector<MaxSize, Element>::IsNull(Element tolerance) const
{
	return GetLength2() <= tolerance * tolerance;
}


template <int MaxSize, class Element>
inline Element TFastVector<MaxSize, Element>::GetDotProduct(const TFastVector<MaxSize, Element>& vector) const
{
	Element retVal = 0.0;

	int commonSize = istd::Min(m_elementsCount, vector.m_elementsCount);
	for (int i = 0; i < commonSize; ++i){
		retVal +=  m_elements[i] * vector.m_elements[i];
	}

	return retVal;
}


template <int MaxSize, class Element>
inline Element TFastVector<MaxSize, Element>::GetLength2() const
{
	return GetDotProduct(*this);
}


template <int MaxSize, class Element>
inline Element TFastVector<MaxSize, Element>::GetLength() const
{
	return std::sqrt(GetLength2());
}


template <int MaxSize, class Element>
inline Element TFastVector<MaxSize, Element>::GetDistance2(const TFastVector<MaxSize, Element>& vector) const
{
	return (*this - vector).GetLength2();
}


template <int MaxSize, class Element>
inline Element TFastVector<MaxSize, Element>::GetDistance(const TFastVector<MaxSize, Element>& vector) const
{
	return std::sqrt(GetDistance2(vector));
}


// operators

template <int MaxSize, class Element>
inline bool TFastVector<MaxSize, Element>::operator==(const TFastVector<MaxSize, Element>& vector) const
{
	if (m_elementsCount != vector.m_elementsCount){
		return false;
	}

	for (int i = 0; i < m_elementsCount; ++i){
		if (m_elements[i] != vector.m_elements[i]){
			return false;
		}
	}

	return true;
}


template <int MaxSize, class Element>
inline bool TFastVector<MaxSize, Element>::operator!=(const TFastVector<MaxSize, Element>& vector) const
{
	return !operator==(vector);
}


template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::operator<(const TFastVector<MaxSize, Element>& vector) const
{
	int commonSize = istd::Min(m_elementsCount, vector.m_elementsCount);
	for (int i = 0; i < commonSize; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return false;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return true;
		}
	}

	return m_elementsCount < vector.m_elementsCount;
}


template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::operator>(const TFastVector<MaxSize, Element>& vector) const
{
	int commonSize = istd::Min(m_elementsCount, vector.m_elementsCount);
	for (int i = 0; i < commonSize; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return true;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return false;
		}
	}

	return m_elementsCount > vector.m_elementsCount;
}


template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::operator<=(const TFastVector<MaxSize, Element>& vector) const
{
	int commonSize = istd::Min(m_elementsCount, vector.m_elementsCount);
	for (int i = 0; i < commonSize; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return false;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return true;
		}
	}

	return m_elementsCount <= vector.m_elementsCount;
}


template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::operator>=(const TFastVector<MaxSize, Element>& vector) const
{
	int commonSize = istd::Min(m_elementsCount, vector.m_elementsCount);
	for (int i = 0; i < commonSize; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return true;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return false;
		}
	}

	return m_elementsCount >= vector.m_elementsCount;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>& TFastVector<MaxSize, Element>::operator=(const TFastVector<MaxSize, Element>& vector)
{
	m_elementsCount = vector.m_elementsCount;
	I_ASSERT(m_elementsCount < MaxSize);

	for (int i = 0; i < m_elementsCount; ++i){
		m_elements[i] = vector.m_elements[i];
	}

	return *this;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>& TFastVector<MaxSize, Element>::operator+=(const TFastVector<MaxSize, Element>& vector)
{
	if (m_elementsCount < vector.m_elementsCount){
		int i = 0;
		for (; i < m_elementsCount; ++i){
			m_elements[i] += vector.m_elements[i];
		}

		for (; i < vector.m_elementsCount; ++i){
			m_elements[i] = vector.m_elements[i];
		}

		m_elementsCount = vector.m_elementsCount;
	}
	else{
		for (int i = 0; i < vector.m_elementsCount; ++i){
			m_elements[i] += vector.m_elements[i];
		}
	}

	return *this;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>& TFastVector<MaxSize, Element>::operator-=(const TFastVector<MaxSize, Element>& vector)
{
	if (m_elementsCount < vector.m_elementsCount){
		int i = 0;
		for (; i < m_elementsCount; ++i){
			m_elements[i] -= vector.m_elements[i];
		}

		for (; i < vector.m_elementsCount; ++i){
			m_elements[i] = -vector.m_elements[i];
		}

		m_elementsCount = vector.m_elementsCount;
	}
	else{
		for (int i = 0; i < vector.m_elementsCount; ++i){
			m_elements[i] -= vector.m_elements[i];
		}
	}

	return *this;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>& TFastVector<MaxSize, Element>::operator*=(Element scalar)
{
	for (int i = 0; i < m_elementsCount; ++i){
		m_elements[i] *= scalar;
	}

	return *this;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>& TFastVector<MaxSize, Element>::operator/=(Element scalar)
{
	for (int i = 0; i < m_elementsCount; ++i){
		m_elements[i] /= scalar;
	}

	return *this;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element> TFastVector<MaxSize, Element>::operator-() const
{
	TFastVector<MaxSize, Element> retVal(*this);

	for (int i = 0; i < m_elementsCount; ++i){
		retVal.m_elements[i] = -m_elements[i];
	}

	return retVal;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element> TFastVector<MaxSize, Element>::operator+(const TFastVector<MaxSize, Element>& vector) const
{
	TFastVector<MaxSize, Element> retVal(*this);

	retVal += vector;

	return retVal;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element> TFastVector<MaxSize, Element>::operator-(const TFastVector<MaxSize, Element>& vector) const
{
	TFastVector<MaxSize, Element> retVal(*this);

	retVal -= vector;

	return retVal;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element> TFastVector<MaxSize, Element>::operator*(Element scalar) const
{
	TFastVector<MaxSize, Element> retVal(*this);

	retVal *= scalar;

	return retVal;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element> TFastVector<MaxSize, Element>::operator/(Element scalar) const
{
	TFastVector<MaxSize, Element> retVal(*this);

	retVal /= scalar;

	return retVal;
}


template <int MaxSize, class Element>
const Element& TFastVector<MaxSize, Element>::operator[](int i) const
{
	I_ASSERT(i >= 0);
	I_ASSERT(i < m_elementsCount);

	return m_elements[i];
}


template <int MaxSize, class Element>
Element& TFastVector<MaxSize, Element>::operator[](int i)
{
	I_ASSERT(i >= 0);
	I_ASSERT(i < MaxSize);

	return m_elements[i];
}


// public methods

template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::Normalize(Element length)
{
    Element isLength = GetLength();

    Element proportion = isLength / length;

	if (::fabs(proportion) > I_BIG_EPSILON){
		for (int i = 0; i < m_elementsCount; ++i){
            m_elements[i] = m_elements[i] / proportion;
        }

		return true;
    }
	else{
		return false;
	}
}


template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::GetNormalized(TFastVector<MaxSize, Element>& result, Element length) const
{
	result = *this;

	return result.Normalize(length);
}


template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag elementsTag("Elements", "List of vector element");
	static iser::CArchiveTag elementTag("Element", "Single vector element");

	retVal = retVal && archive.BeginMultiTag(elementsTag, elementTag, m_elementsCount);

	if (!retVal || (m_elementsCount > MaxSize)){
		if (!archive.IsStoring()){
			m_elementsCount = 0;
		}

		return false;
	}

    for (int i = 0; i < m_elementsCount; ++i){
		retVal = retVal && archive.BeginTag(elementTag);
		retVal = retVal && archive.Process(m_elements[i]);
		retVal = retVal && archive.EndTag(elementTag);
	}

	retVal = retVal && archive.EndTag(elementsTag);

	return retVal;
}


} // namespace imath


#endif // !imath_TFastVector_included


