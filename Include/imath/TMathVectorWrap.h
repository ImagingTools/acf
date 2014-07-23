#ifndef imath_TMathVectorWrap_included
#define imath_TMathVectorWrap_included


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imath/imath.h"


namespace imath
{


/**
	Implementation of mathematical vector with carthesian operations over elements container (vector).
*/
template <typename Base>
class TMathVectorWrap: public Base
{
public:
	typedef Base BaseClass;

	/**
		Create an uninitialized point.
	 */
	TMathVectorWrap();

	/**
		Copy constructor.
	 */
	TMathVectorWrap(const Base& vector);

	/**
		Set all coordinates to zero.
	*/
	void Clear();

	/**
		Translate the point.
	*/
	void Translate(const TMathVectorWrap<Base>& vector);

	/**
		Get translated point.
	*/
	TMathVectorWrap<Base> GetTranslated(const TMathVectorWrap<Base>& vector);

	/**
		/overloaded
	*/
	void GetTranslated(const TMathVectorWrap<Base>& vector, TMathVectorWrap<Base>& result);

	/**
		Check if this vector is null.
	*/
	bool IsNull(typename Base::ElementType tolerance = I_BIG_EPSILON) const;

	/**
		Return dot product of two vectors.
	*/
	typename Base::ElementType GetDotProduct(const TMathVectorWrap<Base>& vector) const;

	/**
		Return euclidian length square.
	*/
	typename Base::ElementType GetLength2() const;
	/**
		Return euclidian length.
	*/
	typename Base::ElementType GetLength() const;

	/**
		Return distance square between two vectors.
	*/
	typename Base::ElementType GetDistance2(const TMathVectorWrap<Base>& vector) const;

	/**
		Return distance between two vectors.
	*/
	typename Base::ElementType GetDistance(const TMathVectorWrap<Base>& vector) const;

	/**
		Normalize vector to specified length.
		\param	length	new vector length.
		\return	true, if normalization successed.
	*/
	bool Normalize(typename Base::ElementType length = 1.0);
	/**
		Return normalized vector with the same direction and specified length.
		\param	length	new vector length.
		\return	true, if normalization successed.
	*/
	bool GetNormalized(TMathVectorWrap<Base>& result, typename Base::ElementType length = 1.0) const;

	/**
		Serialize this vector to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);

	bool operator==(const TMathVectorWrap<Base>& vector) const;
	bool operator!=(const TMathVectorWrap<Base>& vector) const;
	bool operator<(const TMathVectorWrap<Base>& vector) const;
	bool operator>(const TMathVectorWrap<Base>& vector) const;
	bool operator<=(const TMathVectorWrap<Base>& vector) const;
	bool operator>=(const TMathVectorWrap<Base>& vector) const;

	TMathVectorWrap<Base> operator-() const;

	TMathVectorWrap<Base> operator+(const TMathVectorWrap<Base>& vector) const;
	TMathVectorWrap<Base> operator-(const TMathVectorWrap<Base>& vector) const;
	TMathVectorWrap<Base> operator*(typename Base::ElementType scalar) const;
	TMathVectorWrap<Base> operator/(typename Base::ElementType scalar) const;

	TMathVectorWrap<Base>& operator=(const TMathVectorWrap<Base>& vector);

	TMathVectorWrap<Base>& operator+=(const TMathVectorWrap<Base>& vector);
	TMathVectorWrap<Base>& operator-=(const TMathVectorWrap<Base>& vector);
	TMathVectorWrap<Base>& operator*=(typename Base::ElementType scalar);
	TMathVectorWrap<Base>& operator/=(typename Base::ElementType scalar);

	operator Base() const;
};


// inline constructors

template <typename Base>
inline TMathVectorWrap<Base>::TMathVectorWrap()
{
}


template <typename Base>
inline TMathVectorWrap<Base>::TMathVectorWrap(const Base& vector)
:	BaseClass(vector)
{
}


// inline methods

template <typename Base>
inline void TMathVectorWrap<Base>::Clear()
{
	int elementsCount = BaseClass::GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		BaseClass::SetElement(i, 0.0);
	}
}


template <typename Base>
inline void TMathVectorWrap<Base>::Translate(const TMathVectorWrap<Base>& vector)
{
	int elementsCount = qMin(BaseClass::GetElementsCount(), vector.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
		BaseClass::GetElementRef(i) += vector.GetElement(i);
	}
}


template <typename Base>
TMathVectorWrap<Base> TMathVectorWrap<Base>::GetTranslated(const TMathVectorWrap<Base>& vector)
{
	return *this + vector;
}


template <typename Base>
void TMathVectorWrap<Base>::GetTranslated(const TMathVectorWrap<Base>& vector, TMathVectorWrap<Base>& result)
{
	result = *this + vector;
}


template <typename Base>
inline bool TMathVectorWrap<Base>::IsNull(typename Base::ElementType tolerance) const
{
	return GetLength2() <= tolerance * tolerance;
}


template <typename Base>
inline typename Base::ElementType TMathVectorWrap<Base>::GetDotProduct(const TMathVectorWrap<Base>& vector) const
{
	typename Base::ElementType retVal = 0.0;

	int elementsCount = qMin(BaseClass::GetElementsCount(), vector.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
		retVal +=  BaseClass::GetElement(i) * vector.GetElement(i);
	}

	return retVal;
}


template <typename Base>
inline typename Base::ElementType TMathVectorWrap<Base>::GetLength2() const
{
	return GetDotProduct(*this);
}


template <typename Base>
inline typename Base::ElementType TMathVectorWrap<Base>::GetLength() const
{
	return qSqrt(GetLength2());
}


template <typename Base>
inline typename Base::ElementType TMathVectorWrap<Base>::GetDistance2(const TMathVectorWrap<Base>& vector) const
{
	return (*this - vector).GetLength2();
}


template <typename Base>
inline typename Base::ElementType TMathVectorWrap<Base>::GetDistance(const TMathVectorWrap<Base>& vector) const
{
	return qSqrt(GetDistance2(vector));
}


// operators

template <typename Base>
inline bool TMathVectorWrap<Base>::operator==(const TMathVectorWrap<Base>& vector) const
{
	int elementsCount = BaseClass::GetElementsCount();
	if (elementsCount != vector.GetElementsCount()){
		return false;
	}

	for (int i = 0; i < elementsCount; ++i){
		if (BaseClass::GetElement(i) != vector.GetElement(i)){
			return false;
		}
	}
	return true;
}


template <typename Base>
inline bool TMathVectorWrap<Base>::operator!=(const TMathVectorWrap<Base>& vector) const
{
	return !operator==(vector);
}


template <typename Base>
bool TMathVectorWrap<Base>::operator<(const TMathVectorWrap<Base>& vector) const
{
	int elementsCount = qMin(BaseClass::GetElementsCount(), vector.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
		typename Base::ElementType firstElement = BaseClass::GetElement(i);
		typename Base::ElementType secondElement = vector.GetElement(i);

		if (firstElement > secondElement){
			return false;
		}
		else if (firstElement < secondElement){
			return true;
		}
	}

	return BaseClass::GetElementsCount() < vector.GetElementsCount();
}


template <typename Base>
bool TMathVectorWrap<Base>::operator>(const TMathVectorWrap<Base>& vector) const
{
	int elementsCount = qMin(BaseClass::GetElementsCount(), vector.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
		typename Base::ElementType firstElement = BaseClass::GetElement(i);
		typename Base::ElementType secondElement = vector.GetElement(i);

		if (firstElement > secondElement){
			return true;
		}
		else if (firstElement < secondElement){
			return false;
		}
	}

	return BaseClass::GetElementsCount() > vector.GetElementsCount();
}


template <typename Base>
bool TMathVectorWrap<Base>::operator<=(const TMathVectorWrap<Base>& vector) const
{
	int elementsCount = qMin(BaseClass::GetElementsCount(), vector.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
		typename Base::ElementType firstElement = BaseClass::GetElement(i);
		typename Base::ElementType secondElement = vector.GetElement(i);

		if (firstElement > secondElement){
			return false;
		}
		else if (firstElement < secondElement){
			return true;
		}
	}

	return BaseClass::GetElementsCount() <= vector.GetElementsCount();
}


template <typename Base>
bool TMathVectorWrap<Base>::operator>=(const TMathVectorWrap<Base>& vector) const
{
	int elementsCount = qMin(BaseClass::GetElementsCount(), vector.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
		typename Base::ElementType firstElement = BaseClass::GetElement(i);
		typename Base::ElementType secondElement = vector.GetElement(i);

		if (firstElement > secondElement){
			return true;
		}
		else if (firstElement < secondElement){
			return false;
		}
	}

	return BaseClass::GetElementsCount() >= vector.GetElementsCount();
}


template <typename Base>
inline TMathVectorWrap<Base>& TMathVectorWrap<Base>::operator=(const TMathVectorWrap<Base>& vector)
{
	BaseClass::operator=(vector);

	return *this;
}


template <typename Base>
inline TMathVectorWrap<Base>& TMathVectorWrap<Base>::operator+=(const TMathVectorWrap<Base>& vector)
{
	int elementsCount = qMin(BaseClass::GetElementsCount(), vector.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
		BaseClass::GetElementRef(i) += vector.GetElement(i);
	}

	return *this;
}


template <typename Base>
inline TMathVectorWrap<Base>& TMathVectorWrap<Base>::operator-=(const TMathVectorWrap<Base>& vector)
{
	int elementsCount = qMin(BaseClass::GetElementsCount(), vector.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
		BaseClass::GetElementRef(i) -= vector.GetElement(i);
	}

	return *this;
}


template <typename Base>
inline TMathVectorWrap<Base>& TMathVectorWrap<Base>::operator*=(typename Base::ElementType scalar)
{
	int elementsCount = BaseClass::GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		BaseClass::GetElementRef(i) *= scalar;
	}

	return *this;
}


template <typename Base>
inline TMathVectorWrap<Base>& TMathVectorWrap<Base>::operator/=(typename Base::ElementType scalar)
{
	int elementsCount = BaseClass::GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		BaseClass::GetElementRef(i) /= scalar;
	}

	return *this;
}


template <typename Base>
inline TMathVectorWrap<Base> TMathVectorWrap<Base>::operator-() const
{
	TMathVectorWrap<Base> retVal;

	int elementsCount = BaseClass::GetElementsCount();
	retVal.SetElementsCount(elementsCount);
	for (int i = 0; i < elementsCount; ++i){
		retVal.SetElement(i, -BaseClass::GetElement(i));
	}

	return *this;
}


template <typename Base>
inline TMathVectorWrap<Base> TMathVectorWrap<Base>::operator+(const TMathVectorWrap<Base>& vector) const
{
	TMathVectorWrap<Base> retVal;

	int elementsCount = qMin(BaseClass::GetElementsCount(), vector.GetElementsCount());
	retVal.SetElementsCount(elementsCount);
	for (int i = 0; i < elementsCount; ++i){
		retVal.SetElement(i, BaseClass::GetElement(i) + vector.GetElement(i));
	}

	return retVal;
}


template <typename Base>
inline TMathVectorWrap<Base> TMathVectorWrap<Base>::operator-(const TMathVectorWrap<Base>& vector) const
{
	TMathVectorWrap<Base> retVal;

	int elementsCount = qMin(BaseClass::GetElementsCount(), vector.GetElementsCount());
	retVal.SetElementsCount(elementsCount);
	for (int i = 0; i < elementsCount; ++i){
		retVal.SetElement(i, BaseClass::GetElement(i) - vector.GetElement(i));
	}

	return retVal;
}


template <typename Base>
inline TMathVectorWrap<Base> TMathVectorWrap<Base>::operator*(typename Base::ElementType scalar) const
{
	TMathVectorWrap<Base> retVal;

	int elementsCount = BaseClass::GetElementsCount();
	retVal.SetElementsCount(elementsCount);
	for (int i = 0; i < elementsCount; ++i){
		retVal.SetElement(i, BaseClass::GetElement(i) * scalar);
	}

	return *this;
}


template <typename Base>
inline TMathVectorWrap<Base> TMathVectorWrap<Base>::operator/(typename Base::ElementType scalar) const
{
	TMathVectorWrap<Base> retVal;

	int elementsCount = BaseClass::GetElementsCount();
	retVal.SetElementsCount(elementsCount);
	for (int i = 0; i < elementsCount; ++i){
		retVal.SetElement(i, BaseClass::GetElement(i) / scalar);
	}

	return *this;
}


// public methods

template <typename Base>
bool TMathVectorWrap<Base>::Normalize(typename Base::ElementType length)
{
	typename Base::ElementType isLength = GetLength();

	typename Base::ElementType proportion = isLength / length;

	if (qAbs(proportion) > I_BIG_EPSILON){
		int elementsCount = BaseClass::GetElementsCount();
		for (int i = 0; i < elementsCount; ++i){
			BaseClass::SetElement(i, BaseClass::GetElement(i) / proportion);
		}

		return true;
	}
	else{
		return false;
	}
}


template <typename Base>
bool TMathVectorWrap<Base>::GetNormalized(TMathVectorWrap<Base>& result, typename Base::ElementType length) const
{
	typename Base::ElementType isLength = GetLength();

	typename Base::ElementType proportion = isLength / length;

	if (qAbs(proportion) > I_BIG_EPSILON){
		int elementsCount = BaseClass::GetElementsCount();
		result.SetElementsCount(elementsCount);
		for (int i = 0; i < elementsCount; ++i){
			result.SetElement(i, BaseClass::GetElement(i) / proportion);
		}

		return true;
	}
	else{
		return false;
	}
}


template <typename Base>
bool TMathVectorWrap<Base>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	int elementsCount = BaseClass::GetElementsCount();

	static iser::CArchiveTag elementsTag("Elements", "List of vector element", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag elementTag("Element", "Single vector element", iser::CArchiveTag::TT_LEAF, &elementsTag);

	retVal = retVal && archive.BeginMultiTag(elementsTag, elementTag, elementsCount);

	if (!retVal){
		return false;
	}

	if (!archive.IsStoring()){
		BaseClass::SetElementsCount(elementsCount);
	}

	for (int i = 0; i < elementsCount; ++i){
		retVal = retVal && archive.BeginTag(elementTag);
		retVal = retVal && archive.Process(BaseClass::GetElementRef(i));
		retVal = retVal && archive.EndTag(elementTag);
	}

	retVal = retVal && archive.EndTag(elementsTag);

	return retVal;
}


template <typename Base>
TMathVectorWrap<Base>::operator Base() const
{
	return Base(*this);
}


} // namespace imath


#endif // !imath_TMathVectorWrap_included


