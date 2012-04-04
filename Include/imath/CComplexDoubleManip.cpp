#include "imath/CComplexDoubleManip.h"


namespace imath
{


// public methods

CComplexDoubleManip::CComplexDoubleManip(
			int precision,
			RoundingType roundingType,
			double scaleFactor,
			double offset,
			const istd::CRange& range)
:	BaseClass(precision, roundingType),
	m_scaleFactor(scaleFactor),
	m_offset(offset),
	m_range(range)
{
}


// reimplemented (imath::TIValueManip)

QString CComplexDoubleManip::GetString(const double& value) const
{
	return BaseClass::GetString(GetRounded(value) * m_scaleFactor + m_offset, m_scaledPrecision);
}


bool CComplexDoubleManip::GetParsed(const QString& text, double& result) const
{
	bool retVal = CDoubleManip::GetParsed(text, result);

	if (retVal){
		result = GetRounded(result / m_scaleFactor - m_offset);
	}

	return retVal;
}


// protected methods

// reimplemented (imath::TIValueManip)

qint32 CComplexDoubleManip::GetInternalValue(double value) const
{
	value = m_range.GetNearestInside(value);

	return BaseClass::GetInternalValue(value);
}


} // namespace imath


