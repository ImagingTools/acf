#ifndef imath_CComplexDoubleManip_included
#define imath_CComplexDoubleManip_included


#include <math.h>
#include <limits>

#include "imath/CFixedPointManip.h"


namespace imath
{


class CComplexDoubleManip: public CFixedPointManip
{
public:
	typedef CFixedPointManip BaseClass;

	CComplexDoubleManip(
				int precision = 1,
				RoundingType roundingType = RT_NORMAL,
				double scaleFactor = 1.0,
				double minValue = ::std::numeric_limits<double>::min(),
				double maxValue = ::std::numeric_limits<double>::max());

	double GetMinValue() const;
	void SetMinValue(double value);
	double GetMaxValue() const;
	void SetMaxValue(double value);
	double GetScaleFactor() const;
	void SetScaleFactor(double value);

	// overloaded (imath::CFixedPointManip)
	void SetPrecision(int precision);

	// reimplemented (imath::TIValueManip)
	virtual ::std::string GetString(const double& value) const;
	virtual bool GetParsed(const ::std::string& text, double& result) const;

protected:
	// reimplemented (imath::TIValueManip)
	virtual I_SDWORD GetInternalValue(double value) const;

private:
	double m_minValue;
	double m_maxValue;
	double m_scaleFactor;

	int m_scaledPrecision;
};


// inline methods

inline double CComplexDoubleManip::GetMinValue() const
{
	return m_minValue;
}


inline void CComplexDoubleManip::SetMinValue(double value)
{
	m_minValue = value;
}


inline double CComplexDoubleManip::GetMaxValue() const
{
	return m_maxValue;
}


inline void CComplexDoubleManip::SetMaxValue(double value)
{
	m_maxValue = value;
}


inline double CComplexDoubleManip::GetScaleFactor() const
{
	return m_scaleFactor;
}


inline void CComplexDoubleManip::SetScaleFactor(double value)
{
	m_scaleFactor = value;

	m_scaledPrecision = GetPrecision() - int(::log10(m_scaleFactor));
}


// overloaded (imath::CFixedPointManip)

inline void CComplexDoubleManip::SetPrecision(int precision)
{
	BaseClass::SetPrecision(precision);

	m_scaledPrecision = GetPrecision() - int(::log10(m_scaleFactor));
}


} // namespace imath


#endif // !imath_CComplexDoubleManip_included


