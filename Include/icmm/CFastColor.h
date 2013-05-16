#ifndef icmm_CFastColor_included
#define icmm_CFastColor_included


// Qt includes
#include <QtCore/qmath.h>

// ACF includes
#include "iser/ISerializable.h"
#include "imath/IDoubleManip.h"
#include "imath/TFastVector.h"
#include "icmm/CVarColor.h"
#include "icmm/CLab.h"


namespace icmm
{


enum{
#ifdef COLOR_COMPONENTS_COUNT
	MAX_COLOR_COMPONENTS_COUNT=COLOR_COMPONENTS_COUNT
#else
	MAX_COLOR_COMPONENTS_COUNT=16
#endif // COLOR_COMPONENTS_COUNT
};


/**
	Color implementation with variable components number and fixed maximal number of components.
	Maximal number of components because to improve performance memory buffer for elements has fixed size.
*/
class CFastColor: public imath::TFastVector<MAX_COLOR_COMPONENTS_COUNT>
{
public:
	typedef imath::TFastVector<MAX_COLOR_COMPONENTS_COUNT> BaseClass;

	/**
		Default constructor.
		It set number of elements to 0.
	*/
	CFastColor();

	/**
		Constructor with explicit initialization of number of elements.
	*/
	explicit CFastColor(int componentsCount, double value = 0);

	/**
		Copy constructor.
	*/
	CFastColor(const CFastColor& color);

	template <int Size>
	CFastColor(const imath::TVector<Size>& vector)
	:	BaseClass(vector)
	{
	}

	CFastColor(const icmm::CVarColor& color);

	icmm::CLab GetAsLab() const;
	void SetAsLab(const icmm::CLab& lab);

	bool EnsureElementsCount(int count, double value = 0);

	/**
		Get color after components value rounding with specified precision.
		\param	manipulator	manipulator object used to round single components.
		\param	result		result object.
	*/
	void GetRounded(const imath::IDoubleManip& manipulator, CFastColor& result);

	/**
		Check if two values are equal after rounding.
		\param	color		second color to comparison.
		\param	manipulator	value manipulator used to round single components.
	*/
	bool IsRoundedEqual(const CFastColor& color, const imath::IDoubleManip& manipulator) const;

	/**
		Allows to compare two colors with tolerance.
	*/
	bool IsSimilar(const CFastColor& color, double tolerance = I_BIG_EPSILON) const;

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
	void GetNormalized(CFastColor& result) const;

	operator icmm::CVarColor() const;

	bool operator==(const CFastColor& vector) const;
	bool operator!=(const CFastColor& vector) const;
	bool operator<(const CFastColor& vector) const;
	bool operator>(const CFastColor& vector) const;
	bool operator<=(const CFastColor& vector) const;
	bool operator>=(const CFastColor& vector) const;

	CFastColor operator+(const CFastColor& color) const;
	CFastColor operator-(const CFastColor& color) const;
	CFastColor operator*(const CFastColor& color) const;
	CFastColor operator/(const CFastColor& color) const;

	CFastColor operator*(double value) const;
	CFastColor operator/(double value) const;

	const CFastColor& operator=(const CFastColor& color);

	const CFastColor& operator+=(const CFastColor& color);
	const CFastColor& operator-=(const CFastColor& color);
	const CFastColor& operator*=(const CFastColor& color);
	const CFastColor& operator/=(const CFastColor& color);

	const CFastColor& operator*=(double value);
	const CFastColor& operator/=(double value);

	// not reimplemented (iser::ISerializable)
	bool Serialize(iser::IArchive& archive);
};


// inline methods

inline CFastColor::CFastColor()
{
#ifdef COLOR_COMPONENTS_COUNT
	for (int i = 0; i < COLOR_COMPONENTS_COUNT; ++i){
		m_elements[i] = 0;
	}
#endif // COLOR_COMPONENTS_COUNT
}


inline CFastColor::CFastColor(int componentsCount, double value)
#ifndef COLOR_COMPONENTS_COUNT
:	BaseClass(componentsCount, value)
#endif // !COLOR_COMPONENTS_COUNT
{
#ifdef COLOR_COMPONENTS_COUNT
	m_elementsCount = componentsCount;

	for (int i = 0; i < COLOR_COMPONENTS_COUNT; ++i){
		m_elements[i] = value;
	}
#endif // COLOR_COMPONENTS_COUNT
}


inline CFastColor::CFastColor(const CFastColor& color)
#ifndef COLOR_COMPONENTS_COUNT
:	BaseClass(color)
#endif // !COLOR_COMPONENTS_COUNT
{
#ifdef COLOR_COMPONENTS_COUNT
	m_elementsCount = color.m_elementsCount;

	for (int i = 0; i < COLOR_COMPONENTS_COUNT; ++i){
		m_elements[i] = color.m_elements[i];
	}
#endif // COLOR_COMPONENTS_COUNT
}


inline bool CFastColor::EnsureElementsCount(int count, double value)
{
	if (count > GetElementsCount()){
		return SetElementsCount(count, value);
	}

	return true;
}


inline bool CFastColor::IsSimilar(const CFastColor& color, double tolerance) const
{
	return GetDistance(color) <= tolerance;
}


inline bool CFastColor::IsNormalized() const
{
	int elementsCount = GetElementsCount();

	for (int i = 0; i < elementsCount; ++i){
		double component = m_elements[i];

		if ((component < 0) || (component > 1)){
			return false;
		}
	}

	return true;
}


inline CFastColor::operator icmm::CVarColor() const
{
	int elementsCount = GetElementsCount();

	icmm::CVarColor retVal(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		retVal[i] = GetElement(i);
	}

	return retVal;
}


inline bool CFastColor::operator==(const CFastColor& vector) const
{
	int elementsCount = GetElementsCount();

	if (elementsCount != vector.GetElementsCount()){
		return false;
	}

	for (int i = 0; i < elementsCount; ++i){
		if (qAbs(operator[](i) - vector[i]) > I_BIG_EPSILON){
			return false;
		}
	}

	return true;
}


inline bool CFastColor::operator!=(const CFastColor& vector) const
{
	return !operator==(vector);
}


inline bool CFastColor::operator<(const CFastColor& vector) const
{
	int commonSize = qMin(GetElementsCount(), vector.GetElementsCount());

	for (int i = 0; i < commonSize; ++i){
		if (operator[](i) > vector[i] + I_BIG_EPSILON){
			return false;
		}
		else if (operator[](i) < vector[i] - I_BIG_EPSILON){
			return true;
		}
	}

	return GetElementsCount() < vector.GetElementsCount();
}


inline bool CFastColor::operator>(const CFastColor& vector) const
{
	int commonSize = qMin(GetElementsCount(), vector.GetElementsCount());
	for (int i = 0; i < commonSize; ++i){
		if (operator[](i) > vector[i] + I_BIG_EPSILON){
			return true;
		}
		else if (operator[](i) < vector[i] - I_BIG_EPSILON){
			return false;
		}
	}

	return GetElementsCount() > vector.GetElementsCount();
}


inline bool CFastColor::operator<=(const CFastColor& vector) const
{
	int commonSize = qMin(GetElementsCount(), vector.GetElementsCount());
	for (int i = 0; i < commonSize; ++i){
		if (operator[](i) > vector[i] + I_BIG_EPSILON){
			return false;
		}
		else if (operator[](i) < vector[i] - I_BIG_EPSILON){
			return true;
		}
	}

	return GetElementsCount() <= vector.GetElementsCount();
}


inline bool CFastColor::operator>=(const CFastColor& vector) const
{
	int commonSize = qMin(GetElementsCount(), vector.GetElementsCount());
	for (int i = 0; i < commonSize; ++i){
		if (operator[](i) > vector[i] + I_BIG_EPSILON){
			return true;
		}
		else if (operator[](i) < vector[i] - I_BIG_EPSILON){
			return false;
		}
	}

	return GetElementsCount() >= vector.GetElementsCount();
}


inline CFastColor CFastColor::operator+(const CFastColor& color) const
{
	CFastColor retVal(*this);

	retVal += color;

	return retVal;
}


inline CFastColor CFastColor::operator-(const CFastColor& color) const
{
	CFastColor retVal(*this);

	retVal -= color;

	return retVal;
}


inline CFastColor CFastColor::operator*(const CFastColor& color) const
{
	CFastColor retVal(*this);

	retVal *= color;

	return retVal;
}


inline CFastColor CFastColor::operator/(const CFastColor& color) const
{
	CFastColor retVal(*this);

	retVal /= color;

	return retVal;
}


inline CFastColor CFastColor::operator*(double value) const
{
	int elementsCount = GetElementsCount();

	CFastColor retVal(elementsCount);

#ifdef COLOR_COMPONENTS_COUNT
	for (int i = 0; i < COLOR_COMPONENTS_COUNT; ++i){
#else
	for (int i = 0; i < elementsCount; ++i){
#endif // COLOR_COMPONENTS_COUNT
		retVal.m_elements[i] = m_elements[i] * value;
	}

	return retVal;
}


inline CFastColor CFastColor::operator/(double value) const
{
	int elementsCount = GetElementsCount();

	CFastColor retVal(elementsCount);

#ifdef COLOR_COMPONENTS_COUNT
	for (int i = 0; i < COLOR_COMPONENTS_COUNT; ++i){
#else
	for (int i = 0; i < elementsCount; ++i){
#endif // COLOR_COMPONENTS_COUNT
		retVal.m_elements[i] = m_elements[i] / value;
	}

	return retVal;
}


inline const CFastColor& CFastColor::operator=(const CFastColor& color)
{
#ifdef COLOR_COMPONENTS_COUNT
	m_elementsCount = color.m_elementsCount;
	Q_ASSERT(m_elementsCount <= COLOR_COMPONENTS_COUNT);

	for (int i = 0; i < COLOR_COMPONENTS_COUNT; ++i){
		m_elements[i] = color.m_elements[i];
	}
#else
	BaseClass::operator=(color);
#endif // COLOR_COMPONENTS_COUNT

	return *this;
}


inline const CFastColor& CFastColor::operator+=(const CFastColor& color)
{
#ifdef COLOR_COMPONENTS_COUNT
	m_elementsCount = qMax(m_elementsCount, color.m_elementsCount);
	Q_ASSERT(m_elementsCount <= COLOR_COMPONENTS_COUNT);

	for (int i = 0; i < COLOR_COMPONENTS_COUNT; ++i){
		m_elements[i] += color.m_elements[i];
	}
#else
	BaseClass::operator+=(color);
#endif // COLOR_COMPONENTS_COUNT

	return *this;
}


inline const CFastColor& CFastColor::operator-=(const CFastColor& color)
{
#ifdef COLOR_COMPONENTS_COUNT
	m_elementsCount = qMax(m_elementsCount, color.m_elementsCount);
	Q_ASSERT(m_elementsCount <= COLOR_COMPONENTS_COUNT);

	for (int i = 0; i < COLOR_COMPONENTS_COUNT; ++i){
		m_elements[i] -= color.m_elements[i];
	}
#else
	BaseClass::operator-=(color);
#endif // COLOR_COMPONENTS_COUNT

	return *this;
}


inline const CFastColor& CFastColor::operator*=(const CFastColor& color)
{
#ifdef COLOR_COMPONENTS_COUNT
	for (int i = 0; i < COLOR_COMPONENTS_COUNT; ++i){
#else
	int elementsCount = qMin(GetElementsCount(), color.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
#endif // COLOR_COMPONENTS_COUNT
		m_elements[i] *= color.m_elements[i];
	}

	return *this;
}


inline const CFastColor& CFastColor::operator/=(const CFastColor& color)
{
#ifdef COLOR_COMPONENTS_COUNT
	for (int i = 0; i < COLOR_COMPONENTS_COUNT; ++i){
#else
	int elementsCount = qMin(GetElementsCount(), color.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
#endif // COLOR_COMPONENTS_COUNT
		m_elements[i] /= color.m_elements[i];
	}

	return *this;
}


inline const CFastColor& CFastColor::operator*=(double value)
{
#ifdef COLOR_COMPONENTS_COUNT
	for (int i = 0; i < COLOR_COMPONENTS_COUNT; ++i){
		m_elements[i] *= value;
	}
#else
	BaseClass::operator*=(value);
#endif // COLOR_COMPONENTS_COUNT

	return *this;
}


inline const CFastColor& CFastColor::operator/=(double value)
{
#ifdef COLOR_COMPONENTS_COUNT
	for (int i = 0; i < COLOR_COMPONENTS_COUNT; ++i){
		m_elements[i] /= value;
	}
#else
	BaseClass::operator/=(value);
#endif // COLOR_COMPONENTS_COUNT

	return *this;
}


} // namespace icmm


#endif // !icmm_CFastColor_included


