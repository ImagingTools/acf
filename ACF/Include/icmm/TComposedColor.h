#ifndef icmm_TComposedColor_included
#define icmm_TComposedColor_included


#include "math.h"

#include "iser/ISerializable.h"
#include "iser/CArchiveTag.h"

#include "imath/TIValueManip.h"
#include "imath/TVector.h"
#include "imath/TVarVector.h"

#include "icmm/icmm.h"


namespace icmm
{


/**
	Generic color implementation.
*/
template <int Size>
class TComposedColor: public imath::TVector<Size>, public iser::ISerializable
{
public:
	typedef imath::TVector<Size> BaseClass;

	TComposedColor();
	TComposedColor(const BaseClass& value);
	explicit TComposedColor(const imath::TVarVector<double>& vector);

	/**
		Get color after components value rounding with specified precision.
		\param	manipulator	manipulator object used to round single components.
		\param	result		result object.
	*/
	void GetRounded(const imath::IDoubleManip& manipulator, TComposedColor<Size>& result);

	/**
		Check if two values are equal after rounding.
		\param	color		second color to comparision.
		\param	manipulator	value manipulator used to round single components.
	*/
	bool IsRoundedEqual(const TComposedColor<Size>& color, const imath::IDoubleManip& manipulator) const;

	/**
		Allows to compare two colors with tolerance.
	*/
	bool IsSimilar(const TComposedColor<Size>& color, double tolerance = I_BIG_EPSILON) const;

	/**
		Check if this color value is normalized.
		Normalized values have all components in the range [0, 1].
	*/
	bool IsNormalized() const;

	/**
		Make this color to be normalized.
		Normalized values have all components in the range [0, 1].
	*/
	void Normalize();

	/**
		Get normalized color.
		Normalized values have all components in the range [0, 1].
	*/
	void GetNormalized(TComposedColor<Size>& result) const;

	TComposedColor<Size> operator+(const TComposedColor<Size>& color) const;
	TComposedColor<Size> operator-(const TComposedColor<Size>& color) const;
	TComposedColor<Size> operator*(const TComposedColor<Size>& color) const;
	TComposedColor<Size> operator/(const TComposedColor<Size>& color) const;

	TComposedColor<Size> operator*(double value) const;
	TComposedColor<Size> operator/(double value) const;

	const TComposedColor<Size>& operator=(const TComposedColor<Size>& color);
	const TComposedColor<Size>& operator=(double colorValue);

	const TComposedColor<Size>& operator+=(const TComposedColor<Size>& color);
	const TComposedColor<Size>& operator-=(const TComposedColor<Size>& color);
	const TComposedColor<Size>& operator*=(const TComposedColor<Size>& color);
	const TComposedColor<Size>& operator/=(const TComposedColor<Size>& color);

	const TComposedColor<Size>& operator*=(double value);
	const TComposedColor<Size>& operator/=(double value);

	// reimplemented (iser::ISerializable)
	bool Serialize(iser::IArchive& archive);
};


// inline methods

template <int Size>
inline TComposedColor<Size>::TComposedColor()
{
}


template <int Size>
inline TComposedColor<Size>::TComposedColor(const BaseClass& value)
:	BaseClass(value)
{
}


template <int Size>
inline TComposedColor<Size>::TComposedColor(const imath::TVarVector<double>& vector)
{
	int commonSize = istd::Min(vector.GetElementsCount(), Size);
	for (int copyIndex = 0; copyIndex < commonSize; ++copyIndex){
		m_elements[copyIndex] = vector[copyIndex];
	}

	for (int resetIndex = commonSize; resetIndex < Size; ++resetIndex){
		m_elements[resetIndex] = 0.0;
	}
}


template <int Size>
inline bool TComposedColor<Size>::IsSimilar(const TComposedColor<Size>& color, double tolerance) const
{
	return GetDistance(color) <= tolerance;
}


template <int Size>
inline bool TComposedColor<Size>::IsNormalized() const
{
	for (int i = 0; i < Size; ++i){
		double component = m_elements[i];

		if ((component < 0) && (component > 1)){
			return false;
		}
	}

	return true;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator+(const TComposedColor<Size>& color) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] + color.m_elements[i];
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator-(const TComposedColor<Size>& color) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] - color.m_elements[i];
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator*(const TComposedColor<Size>& color) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] * color.m_elements[i];
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator/(const TComposedColor<Size>& color) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] / color.m_elements[i];
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator*(double value) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] * value;
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator/(double value) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] / value;
	}

	return retVal;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator=(const TComposedColor<Size>& color)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] = color.m_elements[i];
	}

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator=(double colorValue)
{
	memset(m_elements, 0.0, sizeof(double) * Size);

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator+=(const TComposedColor<Size>& color)
{
	BaseClass::operator+=(color);

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator-=(const TComposedColor<Size>& color)
{
	BaseClass::operator-=(color);

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator*=(const TComposedColor<Size>& color)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] *= color.m_elements[index];
	}

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator/=(const TComposedColor<Size>& color)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] /= color.m_elements[index];
	}

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


// public normal methods

template <int Size>
void TComposedColor<Size>::GetRounded(const imath::IDoubleManip& manipulator, TComposedColor<Size>& result)
{
	for (int i = 0; i < Size; ++i){
		result.m_elements[i] = manipulator.GetRounded(m_elements[i]);
	}
}


template <int Size>
bool TComposedColor<Size>::IsRoundedEqual(const TComposedColor<Size>& color, const imath::IDoubleManip& manipulator) const
{
	for (int i = 0; i < Size; ++i){
		if (!manipulator.IsEqual(m_elements[i], color.m_elements[i])){
			return false;
		}
	}

	return true;
}


template <int Size>
void TComposedColor<Size>::Normalize()
{
	for (int i = 0; i < Size; ++i){
		if (m_elements[i] < 0.0){
			m_elements[i] = 0.0;
		}

		if (m_elements[i] > 1.0){
			m_elements[i] = 1.0;
		}
	}
}


template <int Size>
void TComposedColor<Size>::GetNormalized(TComposedColor<Size>& result) const
{
	result = *this;

	result.Normalize();
}


// reimplemented (iser::ISerializable)

template <int Size>
bool TComposedColor<Size>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag colorComponentsTag("ColorComponents", "List of color components");
	static iser::CArchiveTag componentTag("Component", "Single component");

	int elementsCount = GetElementsCount();

	retVal = retVal && archive.BeginMultiTag(colorComponentsTag, componentTag, elementsCount);

	if ((!retVal) || (!archive.IsStoring() && (elementsCount != Size))){
		return false;
	}

    for (int i = 0; i < elementsCount; ++i){
		retVal = retVal && archive.BeginTag(componentTag);
		retVal = retVal && archive.Process(m_elements[i]);
		retVal = retVal && archive.EndTag(componentTag);
	}

	retVal = retVal && archive.EndTag(colorComponentsTag);

	return retVal;
}


} // namespace icmm


#endif // !icmm_TComposedColor_included


