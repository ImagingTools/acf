#include "imath/CDoubleManip.h"


// STL includes
#include <cmath>
#include <sstream>


namespace imath{


CDoubleManip::CDoubleManip()
:	m_pointChar('.')
{
}


// reimplemented (imath::IDoubleManip)

int CDoubleManip::GetPrecision() const
{
	return 15;	// approximated precision of type double with the leading digit before the point
}


// reimplemented (imath::TIValueManip)

std::string CDoubleManip::GetString(const double& value) const
{
	std::ostringstream stream;

	stream << value;

	return stream.str();
}


bool CDoubleManip::GetParsed(const std::string& text, double& result) const
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


} // namespace imath


