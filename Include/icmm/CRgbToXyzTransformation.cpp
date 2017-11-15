#include <icmm/CRgbToXyzTransformation.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtCore/QtMath>
#else
#include <QtCore/qmath.h>
#endif

// ACF includes
#include <icmm/CRgb.h>


namespace icmm
{


// public methods

// reimplemented (icmm::IColorTransformation)

bool CRgbToXyzTransformation::GetValueAt(const icmm::CVarColor& argument, icmm::CVarColor& result) const
{
	if (argument.GetElementsCount() != icmm::CRgb::GetElementsCount()){
		return false;
	}

	if (result.GetElementsCount() != 3){
		return false;
	}

	double r = argument[icmm::CRgb::CI_RED];
	double g = argument[icmm::CRgb::CI_GREEN];
	double b = argument[icmm::CRgb::CI_BLUE];

	if (r > 0.04045){
		r = pow((r + 0.055) / 1.055, 2.4);
	}
	else{
		r /= 12.92;
	}
		
	if (g > 0.04045){
		g = pow((g + 0.055) / 1.055, 2.4);
	}
	else{
		g /= 12.92;
	}
	
	if (b > 0.04045){
		b = pow((b + 0.055) / 1.055, 2.4);
	}
	else{
		b /= 12.92;
	}

	double X = r * 0.4124 + g * 0.3576 + b * 0.1805;
	double Y = r * 0.2126 + g * 0.7152 + b * 0.0722;
	double Z = r * 0.0193 + g * 0.1192 + b * 0.9505;

	result[0] = X;
	result[1] = Y;
	result[2] = Z;

	return true;
}


icmm::CVarColor CRgbToXyzTransformation::GetValueAt(const icmm::CVarColor& argument) const
{
	icmm::CVarColor result(3);

	GetValueAt(argument, result);

	return result;
}


} // namespace iccm


