#include "imath/CComplexDoubleManip.h"


namespace imath
{


CComplexDoubleManip::CComplexDoubleManip(
			int precision,
			RoundingType roundingType,
			double scaleFactor,
			double minValue,
			double maxValue)
:	BaseClass(precision, roundingType)
{
}


// reimplemented (imath::TIValueManip)

::std::string CComplexDoubleManip::GetString(const double& value) const
{
	return BaseClass::GetString(GetRounded(value) * m_scaleFactor, m_scaledPrecision);
}


bool CComplexDoubleManip::GetParsed(const ::std::string& text, double& result) const
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
	if (value < m_minValue){
		value = m_minValue;
	}

	if (value > m_maxValue){
		value = m_maxValue;
	}

	return BaseClass::GetInternalValue(value);
}


} // namespace imath


