#ifndef imath_CComplexDoubleManip_included
#define imath_CComplexDoubleManip_included


// STL includes
#include <limits>
#include <cmath>

// ACF includes
#include <istd/TRange.h>
#include <imath/CFixedPointManip.h>


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
				double offset = 0.0,
				const istd::CRange& range = istd::CRange(
							std::numeric_limits<double>::lowest(),
							std::numeric_limits<double>::max()));

	const istd::CRange& GetRange() const;
	void SetRange(const istd::CRange& range);
	double GetScaleFactor() const;
	void SetScaleFactor(double value);
	double GetOffset() const;
	void SetOffset(double value);

	// overloaded (imath::CFixedPointManip)
	void SetPrecision(int precision);

	// reimplemented (imath::TIValueManip)
	virtual QString GetString(const double& value) const;
	virtual bool GetParsed(const QString& text, double& result) const;

protected:
	// reimplemented (imath::TIValueManip)
	virtual qint32 GetInternalValue(double value) const;

private:
	double m_scaleFactor;
	double m_offset;
	istd::CRange m_range;

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

	m_scaledPrecision = GetPrecision() - int(std::log10(m_scaleFactor));
}


inline double CComplexDoubleManip::GetOffset() const
{
	return m_offset;
}


inline void CComplexDoubleManip::SetOffset(double value)
{
	m_offset = value;
}


// overloaded (imath::CFixedPointManip)

inline void CComplexDoubleManip::SetPrecision(int precision)
{
	BaseClass::SetPrecision(precision);

	m_scaledPrecision = GetPrecision() - int(std::log10(m_scaleFactor));
}


} // namespace imath


#endif // !imath_CComplexDoubleManip_included


