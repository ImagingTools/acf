#ifndef imath_CComplexDoubleManip_included
#define imath_CComplexDoubleManip_included


#include <math.h>
#include <limits>

#include "istd/CRange.h"

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
				const istd::CRange& range = istd::CRange(
							std::numeric_limits<double>::min(),
							std::numeric_limits<double>::max()));

	const istd::CRange& GetRange() const;
	void SetRange(const istd::CRange& range);
	double GetScaleFactor() const;
	void SetScaleFactor(double value);

	// overloaded (imath::CFixedPointManip)
	void SetPrecision(int precision);

	// reimplemented (imath::TIValueManip)
	virtual std::string GetString(const double& value) const;
	virtual bool GetParsed(const std::string& text, double& result) const;

protected:
	// reimplemented (imath::TIValueManip)
	virtual I_SDWORD GetInternalValue(double value) const;

private:
	istd::CRange m_range;
	double m_scaleFactor;

	int m_scaledPrecision;
};


// inline methods

inline const istd::CRange& CComplexDoubleManip::GetRange() const
{
	return m_range;
}


inline void CComplexDoubleManip::SetRange(const istd::CRange& range)
{
	m_range = range;
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


