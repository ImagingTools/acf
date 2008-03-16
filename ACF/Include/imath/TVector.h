#ifndef imath_TVector_included
#define imath_TVector_included


#include <cmath>

#include "iser/IArchive.h"

#include "imath/imath.h"


namespace imath{


/**	Simple implementation of fixed-size vector.
 */
template <int Size>
class TVector
{
public:
	/**
		Create an uninitialized point.
	 */
	TVector();
	/**
		Copy constructor.
	 */
	TVector(const TVector<Size>& vector);

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
		Set all coordinates to zero.
	*/
	void Reset();

	/**
		Translate the point.
	*/
	void Translate(const TVector<Size>& vector);

	/**
		Get translated point.
	*/
	TVector<Size> GetTranslated(const TVector<Size>& vector);

	/**
		/overloaded
	*/
	void GetTranslated(const TVector<Size>& vector, TVector<Size>& result);

	/**
		Check if this vector is null.
	*/
	bool IsNull(double tolerance = I_BIG_EPSILON) const;

	/**
		Return dot product of two vectors.
	*/
	double GetDotProduct(const TVector<Size>& vector) const;

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
	double GetDistance2(const TVector<Size>& vector) const;

	/**
		Return distance between two vectors.
	*/
	double GetDistance(const TVector<Size>& vector) const;

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
	bool GetNormalized(TVector<Size>& result, double length = 1.0) const;

	/**
		Serialize this vector to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);

	bool operator==(const TVector<Size>& vector) const;
	bool operator!=(const TVector<Size>& vector) const;

	TVector<Size> operator-() const;

	TVector<Size> operator+(const TVector<Size>& vector) const;
	TVector<Size> operator-(const TVector<Size>& vector) const;
	TVector<Size> operator*(double scalar) const;
	TVector<Size> operator/(double scalar) const;

	TVector<Size>& operator+=(const TVector<Size>& vector);
	TVector<Size>& operator-=(const TVector<Size>& vector);
	TVector<Size>& operator*=(double scalar);
	TVector<Size>& operator/=(double scalar);

	double operator[](int index) const;
	double& operator[](int index);

	// static methods

	/**
		Get number of elements.
	*/
	static int GetElementsCount();


protected:
    double m_elements[Size];
};


// inline constructors

template <int Size>
inline TVector<Size>::TVector()
{
}


template <int Size>
inline TVector<Size>::TVector(const TVector<Size>& vector)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] = vector.m_elements[index];
	}
}


// inline methods

template <int Size>
inline double TVector<Size>::GetElement(int index) const
{
	return operator[](index);
}


template <int Size>
inline double& TVector<Size>::GetElementRef(int index)
{
	return operator[](index);
}


template <int Size>
inline void TVector<Size>::SetElement(int index, double value)
{
	operator[](index) = value;
}


template <int Size>
inline void TVector<Size>::Reset()
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] = 0.0;
	}
}


template <int Size>
inline void TVector<Size>::Translate(const TVector<Size>& vector)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] += vector.m_elements[index];
	}
}


template <int Size>
TVector<Size> TVector<Size>::GetTranslated(const TVector<Size>& vector)
{
	return *this + vector;
}


template <int Size>
void TVector<Size>::GetTranslated(const TVector<Size>& vector, TVector<Size>& result)
{
	result = *this + vector;
}


template <int Size>
inline bool TVector<Size>::IsNull(double tolerance) const
{
	return GetLength2() <= tolerance * tolerance;
}


template <int Size>
inline double TVector<Size>::GetDotProduct(const TVector<Size>& vector) const
{
	double retVal = 0.0;

	for (int index = 0; index < Size; ++index){
		retVal +=  m_elements[index] * vector.m_elements[index];
	}

	return retVal;
}


template <int Size>
inline double TVector<Size>::GetLength2() const
{
	return GetDotProduct(*this);
}


template <int Size>
inline double TVector<Size>::GetLength() const
{
	return ::sqrt(GetLength2());
}


template <int Size>
inline double TVector<Size>::GetDistance2(const TVector<Size>& vector) const
{
	return (*this - vector).GetLength2();
}


template <int Size>
inline double TVector<Size>::GetDistance(const TVector<Size>& vector) const
{
	return ::sqrt(GetDistance2(vector));
}


// operators

template <int Size>
inline bool TVector<Size>::operator==(const TVector<Size>& vector) const
{
	for (int index = 0; index < Size; ++index){
		if (m_elements[index] != vector.m_elements[index]){
			return false;
		}
	}
	return true;
}


template <int Size>
inline bool TVector<Size>::operator!=(const TVector<Size>& vector) const
{
	return !operator==(vector);
}


template <int Size>
inline TVector<Size>& TVector<Size>::operator+=(const TVector<Size>& vector)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] += vector.m_elements[index];
	}

	return *this;
}


template <int Size>
inline TVector<Size>& TVector<Size>::operator-=(const TVector<Size>& vector)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] -= vector.m_elements[index];
	}

	return *this;
}


template <int Size>
inline TVector<Size>& TVector<Size>::operator*=(double scalar)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] *= scalar;
	}

	return *this;
}


template <int Size>
inline TVector<Size>& TVector<Size>::operator/=(double scalar)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] /= scalar;
	}

	return *this;
}


template <int Size>
inline TVector<Size> TVector<Size>::operator-() const
{
	TVector<Size> retVal;

	for (int index = 0; index < Size; ++index){
		retVal.m_elements[index] = -m_elements[index];
	}

	return retVal;
}


template <int Size>
inline TVector<Size> TVector<Size>::operator+(const TVector<Size>& vector) const
{
	TVector<Size> retVal;

	for (int index = 0; index < Size; ++index){
		retVal.m_elements[index] = m_elements[index] + vector.m_elements[index];
	}

	return retVal;
}


template <int Size>
inline TVector<Size> TVector<Size>::operator-(const TVector<Size>& vector) const
{
	TVector<Size> retVal;

	for (int index = 0; index < Size; ++index){
		retVal.m_elements[index] = m_elements[index] - vector.m_elements[index];
	}

	return retVal;
}


template <int Size>
inline TVector<Size> TVector<Size>::operator*(double scalar) const
{
	TVector<Size> retVal;

	for (int index = 0; index < Size; ++index){
		retVal.m_elements[index] = m_elements[index] * scalar;
	}

	return retVal;
}


template <int Size>
inline TVector<Size> TVector<Size>::operator/(double scalar) const
{
	TVector<Size> retVal;

	for (int index = 0; index < Size; ++index){
		retVal.m_elements[index] = m_elements[index] / scalar;
	}

	return retVal;
}


template <int Size>
double TVector<Size>::operator[](int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < Size);

	return m_elements[index];
}


template <int Size>
double& TVector<Size>::operator[](int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < Size);

	return m_elements[index];
}


// static inline methods

template <int Size>
inline int TVector<Size>::GetElementsCount()
{
	return Size;
}


// public methods

template <int Size>
bool TVector<Size>::Normalize(double length)
{
    double isLength = GetLength();

    double proportion = isLength / length;

	if (::fabs(proportion) < I_BIG_EPSILON){
        for (int index = 0; index < Size; ++index){
            m_elements[index] = m_elements[index] / proportion;
        }

		return true;
    }
	else{
		return false;
	}
}


template <int Size>
bool TVector<Size>::GetNormalized(TVector<Size>& result, double length) const
{
    double isLength = GetLength();

    double proportion = isLength / length;

	if (::fabs(proportion) < I_BIG_EPSILON){
        double proportion = length / isLength;

        for (int index = 0; index < Size; ++index){
            result.m_elements[index] = m_elements[index] * proportion;
        }

		return true;
    }
	else{
		return false;
	}
}


template <int Size>
bool TVector<Size>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	for (int index = 0; index < Size; ++index){
		retVal = retVal && archive.Process(m_elements[index]);
	}

	return retVal;
}


} // namespace imath


#endif //!imath_TVector_included


