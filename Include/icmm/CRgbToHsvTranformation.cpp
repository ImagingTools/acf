#include <icmm/CRgbToHsvTranformation.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtCore/QtMath>
#else
#include <QtCore/qmath.h>
#endif

// ACF includes
#include <icmm/CHsv.h>
#include <icmm/CRgb.h>


namespace icmm
{


// public methods

// reimplemented (icmm::IColorTransformation)

bool CRgbToHsvTranformation::GetValueAt(const icmm::CVarColor& argument, icmm::CVarColor& result) const
{
	if (argument.GetElementsCount() != icmm::CRgb::GetElementsCount()){
		return false;
	}

	if (result.GetElementsCount() != icmm::CHsv::GetElementsCount()){
		return false;
	}

	double red = argument.GetElement(icmm::CRgb::CI_RED);
	double green = argument.GetElement(icmm::CRgb::CI_GREEN);
	double blue = argument.GetElement(icmm::CRgb::CI_BLUE);

	double hue = 0;
	double saturation = 0;
	double value = 0;

	double maxRgb = qMax(qMax(red, green), blue);
	double minRgb = qMin(qMin(red, green), blue);
	double delta = maxRgb - minRgb;

	if (fabs(delta) <= I_EPSILON){
		hue = 0;
		saturation = 0.0;
	}
	else{
		saturation = (delta) / maxRgb;

		if (qFuzzyCompare(maxRgb, red)){
			hue = (green - blue) / delta;
		}
		else if (qFuzzyCompare(maxRgb,green)){
			hue = (2 + (blue - red) / delta);
		}
		else if (qFuzzyCompare(maxRgb, blue)){
			hue = (4 + (red - green) / delta);
		}

		hue *= 60;
	}

	if (hue < 0){
		hue += 360.0;
	}

	value = maxRgb;

	result.SetElement(icmm::CHsv::CI_HUE, hue);
	result.SetElement(icmm::CHsv::CI_SATURATION, saturation);
	result.SetElement(icmm::CHsv::CI_VALUE, value);

	return true;
}


icmm::CVarColor CRgbToHsvTranformation::GetValueAt(const icmm::CVarColor& argument) const
{
	icmm::CVarColor result(3);

	GetValueAt(argument, result);

	return result;
}


} // namespace iccm


