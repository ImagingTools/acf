#ifndef imath_TVarVector_included
#define imath_TVarVector_included


#include <vector>

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imath/TVector.h"


namespace imath{


/**	Simple implementation of variable-size vector.
 */
template <class Element = double>
class TVarVector
{
public:
	typedef Element ElementType;

	/**
		Create the vector without components.
	 */
	TVarVector();

	/**
		Create vector and initialize number of components.
	*/
	explicit TVarVector(int componentsCount, const Element& value = Element());

	/**
		Copy constructor.
	 */
	TVarVector(const TVarVector<Element>& vector);

	template <int Size>
	TVarVector(const TVector<Size, Element>& vector)
	{
		m_elements.resize(Size);


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
		\return	always true, this value is provided for template implementations.
	*/
	bool SetElementsCount(int size, const Element& value = Element());

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
		Set size to zero.
	*/
	void Reset();

	/**
		Set all coordinates to zero.
	*/
	void Clear();

	/**
		Set elemenents from other vector without resizing.
		\param	vector			source of element values will be copied.
		\param	expansionValue	if actual vector has more elements than \c vector, rest will be replaced with this value.
	*/
	void SetElementsFrom(const TVarVector& vector, const Element& expansionValue = Element());

	/**
		Translate the point.
	*/
	void Translate(const TVarVector<Element>& vector);

	/**
		Get translated point.
	*/
	TVarVector<Element> GetTranslated(const TVarVector<Element>& vector);

	/**
		/overloaded
	*/
	void GetTranslated(const TVarVector<Element>& vector, TVarVector<Element>& result);

	/**
		Add second vector scaled by specified factor.
		It is equal of Translate(vector * scale) but can be faster implemented.
	*/
	void ScaledCumulate(const TVarVector<Element>& vector, Element scale);

	/**
		Check if this vector is null.
	*/
	bool IsNull(Element tolerance = I_BIG_EPSILON) const;

	/**
		Return dot product of two vectors.
	*/
	Element GetDotProduct(const TVarVector<Element>& vector) const;

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
	Element GetDistance2(const TVarVector<Element>& vector) const;

	/**
		Return distance between two vectors.
	*/
	Element GetDistance(const TVarVector<Element>& vector) const;

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
	bool GetNormalized(TVarVector<Element>& result, Element length = 1.0) const;

	/**
		Serialize this vector to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);

	bool operator==(const TVarVector<Element>& vector) const;
	bool operator!=(const TVarVector<Element>& vector) const;
	bool operator<(const TVarVector<Element>& vector) const;
	bool operator>(const TVarVector<Element>& vector) const;
	bool operator<=(const TVarVector<Element>& vector) const;
	bool operator>=(const TVarVector<Element>& vector) const;

	TVarVector<Element> operator-() const;

	TVarVector<Element> operator+(const TVarVector<Element>& vector) const;
	TVarVector<Element> operator-(const TVarVector<Element>& vector) const;
	TVarVector<Element> operator*(Element scalar) const;
	TVarVector<Element> operator/(Element scalar) const;

	TVarVector<Element>& operator+=(const TVarVector<Element>& vector);
	TVarVector<Element>& operator-=(const TVarVector<Element>& vector);
	TVarVector<Element>& operator*=(Element scalar);
	TVarVector<Element>& operator/=(Element scalar);

	const Element& operator[](int i) const;
	Element& operator[](int i);

protected:
	typedef std::vector<Element> Elements;

	Elements m_elements;
};


// inline constructors

template <class Element>
inline TVarVector<Element>::TVarVector()
{
}


template <class Element>
inline TVarVector<Element>::TVarVector(int componentsCount, const Element& value)
:	m_elements(componentsCount, value)
{
}


template <class Element>
inline TVarVector<Element>::TVarVector(const TVarVector<Element>& vector)
:	m_elements(vector.m_elements)
{
}


// inline methods

template <class Element>
inline int TVarVector<Element>::GetElementsCount() const
{
	return int(m_elements.size());
}


template <class Element>
inline bool TVarVector<Element>::SetElementsCount(int size, const Element& value = Element())
{
	I_ASSERT(size >= 0);

	m_elements.resize(size, value);

	return true;
}


template <class Element>
inline typename const Element& TVarVector<Element>::GetElement(int i) const
{
	return operator[](i);
}


template <class Element>
inline typename Element& TVarVector<Element>::GetElementRef(int i)
{
	return operator[](i);
}


template <class Element>
inline void TVarVector<Element>::SetElement(int i, const Element& value)
{
	operator[](i) = value;
}


template <class Element>
inline void TVarVector<Element>::Reset()
{
	m_elements.clear();
}


template <class Element>
inline void TVarVector<Element>::Clear()
{
	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] = 0;
	}
}


template <class Element>
inline void TVarVector<Element>::Translate(const TVarVector<Element>& vector)
{
	I_ASSERT(GetElementsCount() == vector.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] += vector.m_elements[i];
	}
}


template <class Element>
inline TVarVector<Element> TVarVector<Element>::GetTranslated(const TVarVector<Element>& vector)
{
	return *this + vector;
}


template <class Element>
inline void TVarVector<Element>::GetTranslated(const TVarVector<Element>& vector, TVarVector<Element>& result)
{
	result = *this + vector;
}


template <class Element>
inline void TVarVector<Element>::ScaledCumulate(const TVarVector<Element>& vector, Element scale)
{
	I_ASSERT(GetElementsCount() == vector.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] += vector.m_elements[i] * scale;
	}
}


template <class Element>
inline bool TVarVector<Element>::IsNull(Element tolerance) const
{
	return GetLength2() <= tolerance * tolerance;
}


template <class Element>
inline Element TVarVector<Element>::GetDotProduct(const TVarVector<Element>& vector) const
{
	I_ASSERT(GetElementsCount() == vector.GetElementsCount());

	Element retVal = 0.0;

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		retVal +=  m_elements[i] * vector.m_elements[i];
	}

	return retVal;
}


template <class Element>
inline Element TVarVector<Element>::GetLength2() const
{
	return GetDotProduct(*this);
}


template <class Element>
inline Element TVarVector<Element>::GetLength() const
{
	return ::sqrt(GetLength2());
}


template <class Element>
inline Element TVarVector<Element>::GetDistance2(const TVarVector<Element>& vector) const
{
	return (*this - vector).GetLength2();
}


template <class Element>
inline Element TVarVector<Element>::GetDistance(const TVarVector<Element>& vector) const
{
	return ::sqrt(GetDistance2(vector));
}


// operators

template <class Element>
inline bool TVarVector<Element>::operator==(const TVarVector<Element>& vector) const
{
	return m_elements == vector.m_elements;
}


template <class Element>
inline bool TVarVector<Element>::operator!=(const TVarVector<Element>& vector) const
{
	return m_elements != vector.m_elements;
}


template <class Element>
bool TVarVector<Element>::operator<(const TVarVector<Element>& vector) const
{
	return m_elements < vector.m_elements;
}


template <class Element>
bool TVarVector<Element>::operator>(const TVarVector<Element>& vector) const
{
	return m_elements > vector.m_elements;
}


template <class Element>
bool TVarVector<Element>::operator<=(const TVarVector<Element>& vector) const
{
	return m_elements <= vector.m_elements;
}


template <class Element>
bool TVarVector<Element>::operator>=(const TVarVector<Element>& vector) const
{
	return m_elements >= vector.m_elements;
}


template <class Element>
inline TVarVector<Element>& TVarVector<Element>::operator+=(const TVarVector<Element>& vector)
{
	I_ASSERT(GetElementsCount() == vector.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] += vector.m_elements[i];
	}

	return *this;
}


template <class Element>
inline TVarVector<Element>& TVarVector<Element>::operator-=(const TVarVector<Element>& vector)
{
	I_ASSERT(GetElementsCount() == vector.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] -= vector.m_elements[i];
	}

	return *this;
}


template <class Element>
inline TVarVector<Element>& TVarVector<Element>::operator*=(Element scalar)
{
	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] *= scalar;
	}

	return *this;
}


template <class Element>
inline TVarVector<Element>& TVarVector<Element>::operator/=(Element scalar)
{
	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] /= scalar;
	}

	return *this;
}


template <class Element>
inline TVarVector<Element> TVarVector<Element>::operator-() const
{
	TVarVector<Element> retVal;

	int elementsCount = GetElementsCount();

	retVal.resize(elementsCount);
	I_ASSERT(GetElementsCount() == retVal.GetElementsCount());

	for (int i = 0; i < elementsCount; ++i){
		retVal.m_elements[i] = -m_elements[i];
	}

	return retVal;
}


template <class Element>
inline TVarVector<Element> TVarVector<Element>::operator+(const TVarVector<Element>& vector) const
{
	I_ASSERT(GetElementsCount() == vector.GetElementsCount());

	TVarVector<Element> retVal;

	int elementsCount = GetElementsCount();

	retVal.resize(elementsCount);
	I_ASSERT(elementsCount == retVal.GetElementsCount());

	for (int i = 0; i < elementsCount; ++i){
		retVal.m_elements[i] = m_elements[i] + vector.m_elements[i];
	}

	return retVal;
}


template <class Element>
inline TVarVector<Element> TVarVector<Element>::operator-(const TVarVector<Element>& vector) const
{
	I_ASSERT(GetElementsCount() == vector.GetElementsCount());

	TVarVector<Element> retVal;

	int elementsCount = GetElementsCount();

	retVal.SetElementsCount(elementsCount);
	I_ASSERT(elementsCount == retVal.GetElementsCount());

	for (int i = 0; i < elementsCount; ++i){
		retVal.m_elements[i] = m_elements[i] - vector.m_elements[i];
	}

	return retVal;
}


template <class Element>
inline TVarVector<Element> TVarVector<Element>::operator*(Element scalar) const
{
	TVarVector<Element> retVal;

	int elementsCount = GetElementsCount();

	retVal.resize(elementsCount);
	I_ASSERT(GetElementsCount() == retVal.GetElementsCount());

	for (int i = 0; i < elementsCount; ++i){
		retVal.m_elements[i] = m_elements[i] * scalar;
	}

	return retVal;
}


template <class Element>
inline TVarVector<Element> TVarVector<Element>::operator/(Element scalar) const
{
	TVarVector<Element> retVal;

	int elementsCount = GetElementsCount();

	retVal.resize(elementsCount);
	I_ASSERT(GetElementsCount() == retVal.GetElementsCount());

	for (int i = 0; i < elementsCount; ++i){
		retVal.m_elements[i] = m_elements[i] / scalar;
	}

	return retVal;
}


template <class Element>
const Element& TVarVector<Element>::operator[](int i) const
{
	I_ASSERT(i >= 0);
	I_ASSERT(i < GetElementsCount());

	return m_elements[i];
}


template <class Element>
Element& TVarVector<Element>::operator[](int i)
{
	I_ASSERT(i >= 0);
	I_ASSERT(i < GetElementsCount());

	return m_elements[i];
}


// public methods

template <class Element>
void TVarVector<Element>::SetElementsFrom(const TVarVector& vector, const Element& expansionValue)
{
	int elementsCount = GetElementsCount();
	int minElements = istd::Min(elementsCount, vector.GetElementsCount());

	for (int i = 0; i < minElements; ++i){
		SetElement(i, vector[i]);
	}

	for (int i = minElements; i < elementsCount; ++i){
		SetElement(i, expansionValue);
	}
}


template <class Element>
bool TVarVector<Element>::Normalize(Element length)
{
    Element isLength = GetLength();

    Element proportion = isLength / length;

	if (::fabs(proportion) > I_BIG_EPSILON){
		int elementsCount = GetElementsCount();

        for (int i = 0; i < elementsCount; ++i){
            m_elements[i] = m_elements[i] / proportion;
        }

		return true;
    }
	else{
		return false;
	}
}


template <class Element>
bool TVarVector<Element>::GetNormalized(TVarVector<Element>& result, Element length) const
{
    Element isLength = GetLength();

    Element proportion = isLength / length;

	if (::fabs(proportion) > I_BIG_EPSILON){
		int elementsCount = GetElementsCount();

		result.m_elements.resize(elementsCount);

        for (int i = 0; i < elementsCount; ++i){
            result.m_elements[i] = m_elements[i] / proportion;
        }

		return true;
    }
	else{
		return false;
	}
}


template <class Element>
bool TVarVector<Element>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	int elementsCount = GetElementsCount();

	static iser::CArchiveTag elementsTag("Elements", "List of vector element");
	static iser::CArchiveTag elementTag("Element", "Single vector element");

	retVal = retVal && archive.BeginMultiTag(elementsTag, elementTag, elementsCount);

	if (!retVal){
		return false;
	}

	if (!archive.IsStoring()){
		m_elements.resize(elementsCount);
	}

    for (int i = 0; i < elementsCount; ++i){
		retVal = retVal && archive.BeginTag(elementTag);
		retVal = retVal && archive.Process(m_elements[i]);
		retVal = retVal && archive.EndTag(elementTag);
	}

	retVal = retVal && archive.EndTag(elementsTag);

	return retVal;
}


typedef TVarVector<double> CVarVector;


} // namespace imath


#endif // !imath_TVarVector_included


