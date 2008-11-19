#include "imath/CComplexDoubleManip.h"


namespace imath
{


CComplexDoubleManip::CComplexDoubleManip(
			int precision,
			RoundingType roundingType,
			double scaleFactor,
			double offset,
			const istd::CRange& range)
:	BaseClass(precision, roundingType),
	m_scaleFactor(scaleFactor),
	m_range(range),
	m_offset(offset)
{
}


// reimplemented (imath::TIValueManip)

std::string CComplexDoubleManip::GetString(const double& value) const
{
	return BaseClass::GetString(GetRounded(value) * m_scaleFactor + m_offset, m_scaledPrecision);
}


bool CComplexDoubleManip::GetParsed(const std::string& text, double& result) const
{
	bool retVal = CDoubleManip::GetParsed(text, result);

	if (retVal){
		result = GetRounded(result / m_scaleFactor - m_offset);
	}

	return retVal;
}


// protected methods

// reimplemented (imath::TIValueManip)

I_SDWORD CComplexDoubleManip::GetInternalValue(double value) const
{
	value = m_range.GetNearestInside(value);

	return BaseClass::GetInternalValue(value);
}


} // namespace imath


