#ifndef icmm_TComposedColor_included
#define icmm_TComposedColor_included


#include "math.h"

#include "imath/TIValueManip.h"

#include "icmm/icmm.h"


namespace icmm
{


/**
	Generic color implementation.
*/
template <int Count>
class TComposedColor
{
public:
	TComposedColor();
	TComposedColor(const TComposedColor<Count>& color);

	/**
		Get number of components.
	*/
	int GetComponentsCount() const;

	/**
		Get value of single component.
	*/
	double GetComponentValue(int index) const;

	/**
		Get reference to value of single component.
	*/
	double& GetComponentValueRef(int index);

	/**
		Set value of single component.
	*/
	void SetComponentValue(int index, double value);

	/**
		Get color after components value rounding with specified precision.
		\param	manipulator	manipulator object used to round single components.
		\param	result		result object.
	*/
	void GetRounded(const imath::IDoubleManip& manipulator, TComposedColor<Count>& result);

	/**
		Check if two values are equal after rounding.
		\param	color		second color to comparision.
		\param	manipulator	value manipulator used to round single components.
	*/
	bool IsRoundedEqual(const TComposedColor<Count>& color, const imath::IDoubleManip& manipulator) const;

	/**
		Allows to compare two colors with tolerance.
	*/
	bool IsSimilar(const TComposedColor<Count>& color, double tolerance = I_BIG_EPSILON) const;

	/**
		Check if this color value is normalized.
		Normalized values have all components in the range [0, 1].
	*/
	bool IsNormalized() const;

	/**
		Make this color to be normalized CMYK.
		Normalized values have all components in the range [0, 1].
	*/
	void Normalize();

	/**
		Get normalized color.
		Normalized values have all components in the range [0, 1].
	*/
	void GetNormalized(TComposedColor<Count>& result) const;

	/**
		Get euclidian distance to other color.
	*/
	double GetDistance(const TComposedColor<Count>& color) const;

	/**
		Get square of euclidian distance to other color.
	*/
	double GetDistance2(const TComposedColor<Count>& color) const;

	// operators
	bool operator==(const TComposedColor<Count>& color) const;
	bool operator!=(const TComposedColor<Count>& color) const;

	TComposedColor<Count> operator+(const TComposedColor<Count>& color) const;
	TComposedColor<Count> operator-(const TComposedColor<Count>& color) const;
	TComposedColor<Count> operator*(const TComposedColor<Count>& color) const;
	TComposedColor<Count> operator/(const TComposedColor<Count>& color) const;

	TComposedColor<Count> operator*(double value) const;
	TComposedColor<Count> operator/(double value) const;

	const TComposedColor<Count>& operator=(const TComposedColor<Count>& color);

	const TComposedColor<Count>& operator+=(const TComposedColor<Count>& color);
	const TComposedColor<Count>& operator-=(const TComposedColor<Count>& color);
	const TComposedColor<Count>& operator*=(const TComposedColor<Count>& color);
	const TComposedColor<Count>& operator/=(const TComposedColor<Count>& color);

	const TComposedColor<Count>& operator*=(double value);
	const TComposedColor<Count>& operator/=(double value);

private:
	double m_components[Count];
};


// inline methods

template <int Count>
inline TComposedColor<Count>::TComposedColor()
{
}


template <int Count>
inline TComposedColor<Count>::TComposedColor(const TComposedColor<Count>& color)
{
	for (int i = 0; i < Count; ++i){
		m_components[i] = color.m_components[i];
	}
}


template <int Count>
inline int TComposedColor<Count>::GetComponentsCount() const
{
	return Count;
}


template <int Count>
inline double& TComposedColor<Count>::GetComponentValueRef(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < Count);

	return m_components[index];
}


template <int Count>
inline double TComposedColor<Count>::GetComponentValue(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < Count);

	return m_components[index];
}


template <int Count>
inline void TComposedColor<Count>::SetComponentValue(int index, double value)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < Count);

	m_components[index] = value;
}


template <int Count>
inline bool TComposedColor<Count>::IsSimilar(const TComposedColor<Count>& color, double tolerance) const
{
	return GetDistance(color) <= tolerance;
}


template <int Count>
inline bool TComposedColor<Count>::IsNormalized() const
{
	for (int i = 0; i < Count; ++i){
		double component = m_components[i];

		if ((component < 0) && (component > 1)){
			return false;
		}
	}

	return true;
}


template <int Count>
inline TComposedColor<Count> TComposedColor<Count>::operator+(const TComposedColor<Count>& color) const
{
	TComposedColor<Count> retVal;

	for (int i = 0; i < Count; ++i){
		retVal.m_components[i] = m_components[i] + color.m_components[i];
	}

	return retVal;
}


template <int Count>
inline TComposedColor<Count> TComposedColor<Count>::operator-(const TComposedColor<Count>& color) const
{
	TComposedColor<Count> retVal;

	for (int i = 0; i < Count; ++i){
		retVal.m_components[i] = m_components[i] - color.m_components[i];
	}

	return retVal;
}


template <int Count>
inline TComposedColor<Count> TComposedColor<Count>::operator*(const TComposedColor<Count>& color) const
{
	TComposedColor<Count> retVal;

	for (int i = 0; i < Count; ++i){
		retVal.m_components[i] = m_components[i] * color.m_components[i];
	}

	return retVal;
}


template <int Count>
inline TComposedColor<Count> TComposedColor<Count>::operator/(const TComposedColor<Count>& color) const
{
	TComposedColor<Count> retVal;

	for (int i = 0; i < Count; ++i){
		retVal.m_components[i] = m_components[i] / color.m_components[i];
	}

	return retVal;
}


template <int Count>
inline TComposedColor<Count> TComposedColor<Count>::operator*(double value) const
{
	TComposedColor<Count> retVal;

	for (int i = 0; i < Count; ++i){
		retVal.m_components[i] = m_components[i] * value;
	}

	return retVal;
}


template <int Count>
inline TComposedColor<Count> TComposedColor<Count>::operator/(double value) const
{
	TComposedColor<Count> retVal;

	for (int i = 0; i < Count; ++i){
		retVal.m_components[i] = m_components[i] / value;
	}

	return retVal;
}


template <int Count>
inline bool TComposedColor<Count>::operator==(const TComposedColor<Count>& color) const
{
	for (int i = 0; i < Count; ++i){
		if (m_components[i] != color.m_components[i]){
			return false;
		}
	}

	return true;
}


template <int Count>
inline bool TComposedColor<Count>::operator!=(const TComposedColor<Count>& color) const
{
	for (int i = 0; i < Count; ++i){
		if (m_components[i] == color.m_components[i]){
			return false;
		}
	}

	return true;
}


template <int Count>
inline const TComposedColor<Count>& TComposedColor<Count>::operator=(const TComposedColor<Count>& color)
{
	for (int i = 0; i < Count; ++i){
		m_components[i] = color.m_components[i];
	}

	return *this;
}


template <int Count>
inline const TComposedColor<Count>& TComposedColor<Count>::operator+=(const TComposedColor<Count>& color)
{
	for (int i = 0; i < Count; ++i){
		m_components[i] += color.m_components[i];
	}

	return *this;
}


template <int Count>
inline const TComposedColor<Count>& TComposedColor<Count>::operator-=(const TComposedColor<Count>& color)
{
	for (int i = 0; i < Count; ++i){
		m_components[i] -= color.m_components[i];
	}

	return *this;
}


template <int Count>
inline const TComposedColor<Count>& TComposedColor<Count>::operator*=(const TComposedColor<Count>& color)
{
	for (int i = 0; i < Count; ++i){
		m_components[i] *= color.m_components[i];
	}

	return *this;
}


template <int Count>
inline const TComposedColor<Count>& TComposedColor<Count>::operator/=(const TComposedColor<Count>& color)
{
	for (int i = 0; i < Count; ++i){
		m_components[i] /= color.m_components[i];
	}

	return *this;
}


template <int Count>
inline const TComposedColor<Count>& TComposedColor<Count>::operator*=(double value)
{
	for (int i = 0; i < Count; ++i){
		m_components[i] *= value;
	}

	return *this;
}


template <int Count>
inline const TComposedColor<Count>& TComposedColor<Count>::operator/=(double value)
{
	for (int i = 0; i < Count; ++i){
		m_components[i] /= value;
	}

	return *this;
}


// public normal methods

template <int Count>
void TComposedColor<Count>::GetRounded(const imath::IDoubleManip& manipulator, TComposedColor<Count>& result)
{
	for (int i = 0; i < Count; ++i){
		result.m_components[i] = manipulator.GetRounded(m_components[i]);
	}
}


template <int Count>
bool TComposedColor<Count>::IsRoundedEqual(const TComposedColor<Count>& color, const imath::IDoubleManip& manipulator) const
{
	for (int i = 0; i < Count; ++i){
		if (!manipulator.IsEqual(m_components[i], color.m_components[i])){
			return false;
		}
	}

	return true;
}


template <int Count>
void TComposedColor<Count>::Normalize()
{
	for (int i = 0; i < Count; ++i){
		if (m_components[i] < 0.0){
			m_components[i] = 0.0;
		}

		if (m_components[i] > 1.0){
			m_components[i] = 1.0;
		}
	}
}


template <int Count>
void TComposedColor<Count>::GetNormalized(TComposedColor<Count>& result) const
{
	result = *this;

	retVal.Normalize();
}


template <int Count>
double TComposedColor<Count>::GetDistance(const TComposedColor<Count>& color) const
{
	return ::sqrt(GetDistance2(color));
}


template <int Count>
double TComposedColor<Count>::GetDistance2(const TComposedColor<Count>& color) const
{
	double retVal = 0.0;

	for (int i = 0; i < Count; ++i){
		double difference = m_components[i] - color.m_components[i];

		retVal += difference * difference;
	}

	return retVal;
}


} // namespace icmm


#endif // !icmm_TComposedColor_included


