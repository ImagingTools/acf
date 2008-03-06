#include "imath/CFixedPointManip.h"


#include "math.h"
#include <vector>


namespace imath{


CFixedPointManip::CFixedPointManip(int precision, RoundingType roundingType)
	:	m_pointChar('.'), m_roundingType(roundingType)
{
	SetPrecision(precision);
}


void CFixedPointManip::SetPrecision(int precision)
{
	m_precision = precision;
	m_scaleToIntFactor = ::pow(10.0, m_precision);
}


void CFixedPointManip::SetRoundingType(RoundingType roundingType)
{
	m_roundingType = roundingType;
}


::std::string CFixedPointManip::GetString(const double& value) const
{
	return GetString(value, m_precision);
}



bool CFixedPointManip::GetParsed(const ::std::string& text, double& result) const
{
	bool retVal = GetParsedUnrounded(text, result);

	if (retVal){
		result = GetRounded(result);
	}

	return retVal;
}


// protected methods

::std::string CFixedPointManip::GetString(const double& value, int /*precision*/) const
{
	typedef ::std::vector<char> CharacterList;
	CharacterList rotatedString;

	I_SDWORD intValue = GetInternalValue(value);
	I_DWORD intAbsValue = (intValue > 0)? I_DWORD(intValue): I_DWORD(-intValue);

	if (m_precision > 0){
		for (int i = 0; i < m_precision; ++i){
			rotatedString.push_back('0' + char(intAbsValue % 10));
			intAbsValue /= 10;
		}
		rotatedString.push_back(m_pointChar);
	}
	else{
		for (int i = 0; i < -m_precision; ++i){
			rotatedString.push_back('0');
		}
	}

	do{
		rotatedString.push_back('0' + char(intAbsValue % 10));
		intAbsValue /= 10;
	} while (intAbsValue != 0);

	if (intValue < 0){
		rotatedString.push_back('-');
	}

	int stringLength = int(rotatedString.size());

	::std::string retVal(rotatedString.size(), ' ');

	for (int i = 0; i < stringLength; ++i){
		retVal[i] = rotatedString[stringLength - i - 1];
	}

	return retVal;
}



bool CFixedPointManip::GetParsedUnrounded(const ::std::string& text, double& result) const
{
	double sign = 0.0;
	double divider = 0.0;
	I_SDWORD intValueResult = 0;

	int stringLength = int(text.size());
	for (int i = 0; i < stringLength; ++i){
		char c = text[i];
		if (::isdigit(c)){
			divider *= 10;

			intValueResult = intValueResult * 10 + c - '0';
		}
		else if (c == m_pointChar){
			if (divider != 0){
				return false;
			}

			divider = 1.0;
		}
		else if (c == '-'){
			if (sign != 0.0){
				return false;	// two minus or plus signs are not allowed
			}

			sign = -1.0;
		}
		else if (c == '+'){
			if (sign != 0.0){
				return false;	// two minus or plus signs are not allowed
			}

			sign = 1.0;
		}
	}

	if (sign == 0.0){
		sign = 1.0;
	}

	if (divider == 0.0){
		divider = 1.0;
	}

	result = intValueResult * sign / divider;

	return true;
}


// static protected methods

double CFixedPointManip::NormalRoundFuntion(double value)
{
	return ::floor(value + 0.5);
}


// static attributes

CFixedPointManip::RoundingFuntionPtr CFixedPointManip::m_roundingFuntionsPtr[RT_LAST + 1] = {
			CFixedPointManip::NormalRoundFuntion,
			::floor,
			::ceil};


} // namespace imath



