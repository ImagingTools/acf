#ifndef icmm_CVarColor_included
#define icmm_CVarColor_included


#include "math.h"

#include "imath/TIValueManip.h"
#include "imath/TVarVector.h"

#include "icmm/icmm.h"


namespace icmm
{


/**
	Generic color implementation.
*/
class CVarColor: public imath::TVarVector<>
{
public:
	typedef imath::TVarVector<> BaseClass;

	/**
		Get color after components value rounding with specified precision.
		\param	manipulator	manipulator object used to round single components.
		\param	result		result object.
	*/
	void GetRounded(const imath::IDoubleManip& manipulator, CVarColor& result);

	/**
		Check if two values are equal after rounding.
		\param	color		second color to comparision.
		\param	manipulator	value manipulator used to round single components.
	*/
	bool IsRoundedEqual(const CVarColor& color, const imath::IDoubleManip& manipulator) const;

	/**
		Allows to compare two colors with tolerance.
	*/
	bool IsSimilar(const CVarColor& color, double tolerance = I_BIG_EPSILON) const;

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
	void GetNormalized(CVarColor& result) const;

	CVarColor operator+(const CVarColor& color) const;
	CVarColor operator-(const CVarColor& color) const;
	CVarColor operator*(const CVarColor& color) const;
	CVarColor operator/(const CVarColor& color) const;

	CVarColor operator*(double value) const;
	CVarColor operator/(double value) const;

	const CVarColor& operator=(const CVarColor& color);
	const CVarColor& operator=(double colorValue);

	const CVarColor& operator+=(const CVarColor& color);
	const CVarColor& operator-=(const CVarColor& color);
	const CVarColor& operator*=(const CVarColor& color);
	const CVarColor& operator/=(const CVarColor& color);

	const CVarColor& operator*=(double value);
	const CVarColor& operator/=(double value);
};


// inline methods

inline bool CVarColor::IsSimilar(const CVarColor& color, double tolerance) const
{
	return GetDistance(color) <= tolerance;
}


inline bool CVarColor::IsNormalized() const
{
	int elementsCount = GetElementsCount();

	for (int i = 0; i < elementsCount; ++i){
		double component = m_elements[i];

		if ((component < 0) && (component > 1)){
			return false;
		}
	}

	return true;
}


inline CVarColor CVarColor::operator+(const CVarColor& color) const
{
	I_ASSERT(GetElementsCount() == color.GetElementsCount());

	CVarColor retVal;

	int elementsCount = GetElementsCount();

	retVal.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		retVal.m_elements[i] = m_elements[i] + color.m_elements[i];
	}

	return retVal;
}


inline CVarColor CVarColor::operator-(const CVarColor& color) const
{
	I_ASSERT(GetElementsCount() == color.GetElementsCount());

	CVarColor retVal;

	int elementsCount = GetElementsCount();

	retVal.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		retVal.m_elements[i] = m_elements[i] - color.m_elements[i];
	}

	return retVal;
}


inline CVarColor CVarColor::operator*(const CVarColor& color) const
{
	I_ASSERT(GetElementsCount() == color.GetElementsCount());

	CVarColor retVal;

	int elementsCount = GetElementsCount();

	retVal.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		retVal.m_elements[i] = m_elements[i] * color.m_elements[i];
	}

	return retVal;
}


inline CVarColor CVarColor::operator/(const CVarColor& color) const
{
	I_ASSERT(GetElementsCount() == color.GetElementsCount());

	CVarColor retVal;

	int elementsCount = GetElementsCount();

	retVal.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		retVal.m_elements[i] = m_elements[i] / color.m_elements[i];
	}

	return retVal;
}


inline CVarColor CVarColor::operator*(double value) const
{
	CVarColor retVal;

	int elementsCount = GetElementsCount();

	retVal.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		retVal.m_elements[i] = m_elements[i] * value;
	}

	return retVal;
}


inline CVarColor CVarColor::operator/(double value) const
{
	CVarColor retVal;

	int elementsCount = GetElementsCount();

	retVal.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		retVal.m_elements[i] = m_elements[i] / value;
	}

	return retVal;
}


inline const CVarColor& CVarColor::operator=(const CVarColor& color)
{
	int elementsCount = color.GetElementsCount();

	SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] = color.m_elements[i];
	}

	return *this;
}


inline const CVarColor& CVarColor::operator+=(const CVarColor& color)
{
	BaseClass::operator+=(color);

	return *this;
}


inline const CVarColor& CVarColor::operator-=(const CVarColor& color)
{
	BaseClass::operator-=(color);

	return *this;
}


inline const CVarColor& CVarColor::operator*=(const CVarColor& color)
{
	I_ASSERT(GetElementsCount() == color.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] *= color.m_elements[i];
	}

	return *this;
}


inline const CVarColor& CVarColor::operator/=(const CVarColor& color)
{
	I_ASSERT(GetElementsCount() == color.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] /= color.m_elements[i];
	}

	return *this;
}


inline const CVarColor& CVarColor::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


inline const CVarColor& CVarColor::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


} // namespace icmm


#endif // !icmm_CVarColor_included


