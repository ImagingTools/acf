#ifndef imath_CVarVector_included
#define imath_CVarVector_included


// Qt includes
#include <QtCore/qmath.h>
#include <QtCore/QVector>

// ACF includes
#include "imath/TVector.h"


namespace iser
{
	class IArchive;
}


namespace imath
{


/**
	Simple implementation of variable-size vector.
 */
class CVarVector
{
public:
	typedef QVector<double> Elements;

	/**
		Create the vector without components.
	 */
	CVarVector();

	/**
		Create vector and initialize number of components.
	*/
	explicit CVarVector(int componentsCount, double value = 0);

	/**
		Copy constructor.
	 */
	CVarVector(const CVarVector& vector);

	template <int Size>
	CVarVector(const TVector<Size, double>& vector);

	/**
		Get \c true, if the element ist is empty.
	*/
	bool IsEmpty() const;

	/**
		Get number of elements.
	*/
	int GetElementsCount() const;

	/**
		Set number of elements.
		\return	always true, this value is provided for template implementations.
	*/
	bool SetElementsCount(int count, double value = 0);

	/**
		Get element at specified index.
	*/
	double GetElement(int index) const;

	/**
		Get reference to element at specified index.
	*/
	double& GetElementRef(int index);

	/**
		Set element at specified index.
	*/
	void SetElement(int index, double value);

	/**
		Set size to zero.
	*/
	void Reset();

	/**
		Set number of elements and set all elements to specified value.
	*/
	void Reset(int elementsCount, double value);

	/**
		Set all coordinates to zero.
	*/
	void Clear();

	/**
		Set elemenents from other vector without resizing.
		\param	vector			source of element values will be copied.
		\param	expansionValue	if actual vector has more elements than \c vector, rest will be replaced with this value.
	*/
	void SetElementsFrom(const CVarVector& vector, double expansionValue = 0);

	/**
		Get read-only access to internal element container.
	*/
	const Elements& GetElements() const;
	/**
		Get access to internal element container.
	*/
	Elements& GetElementsRef();

	/**
		Ensure, that number of elements is at least the specified value.
		It resize the vector if the new size is bigger than the current one.
		\return	always true, this value is provided for template implementations.
	*/
	bool EnsureElementsCount(int count, double value = 0);

	/**
		Translate the point.
	*/
	void Translate(const CVarVector& vector);

	/**
		Get translated point.
	*/
	CVarVector GetTranslated(const CVarVector& vector);

	/**
		/overloaded
	*/
	void GetTranslated(const CVarVector& vector, CVarVector& result);

	/**
		Add second vector scaled by specified factor.
		It is equal of Translate(vector * scale) but can be faster implemented.
	*/
	void ScaledCumulate(const CVarVector& vector, double scale);

	/**
		Check if this vector is null.
	*/
	bool IsNull(double tolerance = I_BIG_EPSILON) const;

	/**
		Return dot product of two vectors.
	*/
	double GetDotProduct(const CVarVector& vector) const;

	/**
		Return euclidian length square.
	*/
	double GetLength2() const;

	/**
		Return euclidian length.
	*/
	double GetLength() const;

	/**
		Return distance square between two vectors.
	*/
	double GetDistance2(const CVarVector& vector) const;

	/**
		Return distance between two vectors.
	*/
	double GetDistance(const CVarVector& vector) const;

	/**
		Get simple sum of all elements.
	*/
	double GetElementsSum() const;

	/**
		Normalize vector to specified length.
		\param	length	new vector length.
		\return	true, if normalization successed.
	*/
	bool Normalize(double length = 1.0);
	/**
		Return normalized vector with the same direction and specified length.
		\param	length	new vector length.
		\return	true, if normalization successed.
	*/
	bool GetNormalized(CVarVector& result, double length = 1.0) const;

	/**
		Get vector with minimal elements values.
	*/
	void GetMinimal(const CVarVector& vector, CVarVector& result) const;
	/**
		Get vector with maximal elements values.
	*/
	void GetMaximal(const CVarVector& vector, CVarVector& result) const;

	/**
		Serialize this vector to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);

	bool operator==(const CVarVector& vector) const;
	bool operator!=(const CVarVector& vector) const;
	bool operator<(const CVarVector& vector) const;
	bool operator>(const CVarVector& vector) const;
	bool operator<=(const CVarVector& vector) const;
	bool operator>=(const CVarVector& vector) const;

	CVarVector operator-() const;

	CVarVector operator+(const CVarVector& vector) const;
	CVarVector operator-(const CVarVector& vector) const;
	CVarVector operator*(double scalar) const;
	CVarVector operator/(double scalar) const;

	CVarVector& operator+=(const CVarVector& vector);
	CVarVector& operator-=(const CVarVector& vector);
	CVarVector& operator*=(double scalar);
	CVarVector& operator/=(double scalar);

	CVarVector& operator=(const CVarVector& vector);

	double operator[](int i) const;
	double& operator[](int i);

private:
	Elements m_elements;
};


// inline constructors

inline CVarVector::CVarVector()
{
}


inline CVarVector::CVarVector(int componentsCount, double value)
:	m_elements(componentsCount, value)
{
}


inline CVarVector::CVarVector(const CVarVector& vector)
:	m_elements(vector.m_elements)
{
}


// inline methods

inline bool CVarVector::IsEmpty() const
{
	return m_elements.isEmpty();
}


inline int CVarVector::GetElementsCount() const
{
	return int(m_elements.size());
}


inline bool CVarVector::SetElementsCount(int count, double value)
{
	Q_ASSERT(count >= 0);

	int oldCount = m_elements.size();

	m_elements.resize(count);

	for (int i = oldCount; i < count; ++i){
		m_elements[i] = value;
	}

	return true;
}


inline double CVarVector::GetElement(int i) const
{
	return operator[](i);
}


inline double& CVarVector::GetElementRef(int i)
{
	return operator[](i);
}


inline void CVarVector::SetElement(int i, double value)
{
	operator[](i) = value;
}


inline void CVarVector::Reset()
{
	m_elements.clear();
}


inline void CVarVector::Clear()
{
	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] = 0;
	}
}


inline void CVarVector::Reset(int elementsCount, double value)
{
	m_elements.resize(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] = value;
	}
}


inline const CVarVector::Elements& CVarVector::GetElements() const
{
	return m_elements;
}


inline CVarVector::Elements& CVarVector::GetElementsRef()
{
	return m_elements;
}


inline void CVarVector::Translate(const CVarVector& vector)
{
	Q_ASSERT(GetElementsCount() == vector.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] += vector.m_elements[i];
	}
}


inline CVarVector CVarVector::GetTranslated(const CVarVector& vector)
{
	return *this + vector;
}


inline void CVarVector::GetTranslated(const CVarVector& vector, CVarVector& result)
{
	result = *this + vector;
}


inline void CVarVector::ScaledCumulate(const CVarVector& vector, double scale)
{
	Q_ASSERT(GetElementsCount() == vector.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] += vector.m_elements[i] * scale;
	}
}


inline bool CVarVector::IsNull(double tolerance) const
{
	return GetLength2() <= tolerance * tolerance;
}


inline double CVarVector::GetDotProduct(const CVarVector& vector) const
{
	Q_ASSERT(GetElementsCount() == vector.GetElementsCount());

	double retVal = 0.0;

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		retVal +=  m_elements[i] * vector.m_elements[i];
	}

	return retVal;
}


inline double CVarVector::GetLength2() const
{
	return GetDotProduct(*this);
}


inline double CVarVector::GetLength() const
{
	return qSqrt(GetLength2());
}


inline double CVarVector::GetDistance2(const CVarVector& vector) const
{
	return (*this - vector).GetLength2();
}


inline double CVarVector::GetDistance(const CVarVector& vector) const
{
	return qSqrt(GetDistance2(vector));
}


// operators

inline bool CVarVector::operator==(const CVarVector& vector) const
{
	return m_elements == vector.m_elements;
}


inline bool CVarVector::operator!=(const CVarVector& vector) const
{
	return m_elements != vector.m_elements;
}


inline bool CVarVector::operator<(const CVarVector& vector) const
{
	int count = m_elements.size();
	int vectorCount = vector.m_elements.size();
	int commonSize = qMin(count, vectorCount);
	for (int i = 0; i < commonSize; ++i){
		int element = m_elements[i];
		int vectorElement = vector.m_elements[i];
		if (element != vectorElement){
			return element < vectorElement;
		}
	}

	return count < vectorCount;
}


inline bool CVarVector::operator>(const CVarVector& vector) const
{
	int count = m_elements.size();
	int vectorCount = vector.m_elements.size();
	int commonSize = qMin(count, vectorCount);
	for (int i = 0; i < commonSize; ++i){
		int element = m_elements[i];
		int vectorElement = vector.m_elements[i];
		if (element != vectorElement){
			return element > vectorElement;
		}
	}

	return count > vectorCount;
}


inline bool CVarVector::operator<=(const CVarVector& vector) const
{
	int count = m_elements.size();
	int vectorCount = vector.m_elements.size();
	int commonSize = qMin(count, vectorCount);
	for (int i = 0; i < commonSize; ++i){
		int element = m_elements[i];
		int vectorElement = vector.m_elements[i];
		if (element != vectorElement){
			return element < vectorElement;
		}
	}

	return count <= vectorCount;
}


inline bool CVarVector::operator>=(const CVarVector& vector) const
{
	int count = m_elements.size();
	int vectorCount = vector.m_elements.size();
	int commonSize = qMin(count, vectorCount);
	for (int i = 0; i < commonSize; ++i){
		int element = m_elements[i];
		int vectorElement = vector.m_elements[i];
		if (element != vectorElement){
			return element > vectorElement;
		}
	}

	return count >= vectorCount;
}


inline CVarVector& CVarVector::operator+=(const CVarVector& vector)
{
	Q_ASSERT(GetElementsCount() == vector.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] += vector.m_elements[i];
	}

	return *this;
}


inline CVarVector& CVarVector::operator-=(const CVarVector& vector)
{
	Q_ASSERT(GetElementsCount() == vector.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] -= vector.m_elements[i];
	}

	return *this;
}


inline CVarVector& CVarVector::operator*=(double scalar)
{
	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] *= scalar;
	}

	return *this;
}


inline CVarVector& CVarVector::operator/=(double scalar)
{
	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] /= scalar;
	}

	return *this;
}


inline CVarVector& CVarVector::operator=(const CVarVector& vector)
{
	SetElementsCount(vector.GetElementsCount(), 0);
	SetElementsFrom(vector);

	return *this;
}


inline CVarVector CVarVector::operator-() const
{
	int elementsCount = GetElementsCount();

	CVarVector retVal(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		retVal.m_elements[i] = -m_elements[i];
	}

	return retVal;
}


inline CVarVector CVarVector::operator+(const CVarVector& vector) const
{
	CVarVector retVal(*this);

	retVal += vector;

	return retVal;
}


inline CVarVector CVarVector::operator-(const CVarVector& vector) const
{
	CVarVector retVal(*this);

	retVal -= vector;

	return retVal;
}


inline CVarVector CVarVector::operator*(double scalar) const
{
	CVarVector retVal(*this);

	retVal *= scalar;

	return retVal;
}


inline CVarVector CVarVector::operator/(double scalar) const
{
	CVarVector retVal(*this);

	retVal /= scalar;

	return retVal;
}


inline double CVarVector::operator[](int i) const
{
	Q_ASSERT(i >= 0);
	Q_ASSERT(i < GetElementsCount());

	return m_elements[i];
}


inline double& CVarVector::operator[](int i)
{
	Q_ASSERT(i >= 0);
	Q_ASSERT(i < GetElementsCount());

	return m_elements[i];
}


// template methods

template <int Size>
CVarVector::CVarVector(const TVector<Size, double>& vector)
{
	m_elements.resize(Size);


	for (int i = 0; i < Size; ++i){
		m_elements[i] = vector[i];
	}
}


} // namespace imath


#endif // !imath_CVarVector_included


