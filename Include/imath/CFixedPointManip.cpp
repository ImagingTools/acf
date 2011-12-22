#include "imath/CFixedPointManip.h"


#include "math.h"
#include <vector>


namespace imath{


CFixedPointManip::CFixedPointManip(int precision, RoundingType roundingType)
:	m_roundingType(roundingType)
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


// reimplemented (imath::IDoubleManip)

int CFixedPointManip::GetPrecision() const
{
	return m_precision;
}


// reimplemented (imath::TIValueManip)

std::string CFixedPointManip::GetString(const double& value) const
{
	return GetString(value, m_precision);
}


bool CFixedPointManip::GetParsed(const std::string& text, double& result) const
{
	bool retVal = BaseClass::GetParsed(text, result);

	if (retVal){
		result = GetRounded(result);
	}

	return retVal;
}


// protected methods

std::string CFixedPointManip::GetString(const double& value, int /*precision*/) const
{
	typedef std::vector<char> CharacterList;
	CharacterList rotatedString;

	I_SDWORD intValue = GetInternalValue(value);
	I_DWORD intAbsValue = (intValue > 0)? I_DWORD(intValue): I_DWORD(-intValue);

	if (m_precision > 0){
		for (int i = 0; i < m_precision; ++i){
			rotatedString.push_back('0' + char(intAbsValue % 10));
			intAbsValue /= 10;
		}
		rotatedString.push_back(GetPointChar());
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

	std::string retVal(rotatedString.size(), ' ');

	for (int i = 0; i < stringLength; ++i){
		retVal[i] = rotatedString[stringLength - i - 1];
	}

	return retVal;
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


