#include "imath/CComplexDoubleManip.h"


namespace imath
{


CComplexDoubleManip::CComplexDoubleManip(
			int precision,
			RoundingType roundingType,
			double scaleFactor,
			const istd::CRange& range)
:	BaseClass(precision, roundingType), m_scaleFactor(scaleFactor), m_range(range)
{
}


// reimplemented (imath::TIValueManip)

std::string CComplexDoubleManip::GetString(const double& value) const
{
	return BaseClass::GetString(GetRounded(value) * m_scaleFactor, m_scaledPrecision);
}


bool CComplexDoubleManip::GetParsed(const std::string& text, double& result) const
{
	bool retVal = GetParsedUnrounded(text, result);

	if (retVal){
		result = GetRounded(result / m_scaleFactor);
	}

	return retVal;
}


// protected methods

// reimplemented (imath::TIValueManip)

I_SDWORD CComplexDoubleManip::GetInternalValue(double value) const
{
	value = m_range.GetNearestInRange(value);

	return BaseClass::GetInternalValue(value);
}


} // namespace imath


