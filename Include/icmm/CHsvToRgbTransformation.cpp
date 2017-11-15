#include <icmm/CHsvToRgbTransformation.h>


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

bool CHsvToRgbTransformation::GetValueAt(const ArgumentType& argument, ResultType& result) const
{
	if (argument.GetElementsCount() != icmm::CHsv::GetElementsCount()){
		return false;
	}

	if (result.GetElementsCount() != icmm::CRgb::GetElementsCount()){
		return false;
	}

	double hue = argument.GetElement(icmm::CHsv::CI_HUE) * 360.0;
	double saturation = argument.GetElement(icmm::CHsv::CI_SATURATION);
	double value = argument.GetElement(icmm::CHsv::CI_VALUE);

	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;

	// color is on black-and-white center line 
	if (saturation == 0.0f){
		red = value;		// achromatic:shades of gray 
		green = value;	// supposedly invalid for hue=0 but who cares 
		blue = value;
	}
	// chromatic color 
	else{
		double hTemp;
		if (hue == 360.0){		// 360 degrees same as 0 degrees 
			hTemp = 0.0;
		}
		else{ 
			hTemp = hue;
		}

		hTemp = hTemp / 60.0;		// hue is now in [0,6) 
		int intHue = qFloor(hTemp);	// largest integer <= hue 
		double f = hTemp - intHue;		// fractional part of hue 
			
		double p = value * (1.0 - saturation); 
		double q = value * (1.0 - (saturation * f)); 
		double t = value * (1.0 - (saturation * (1.0 - f))); 
		
		switch (intHue){
			case 0:
				red = value; 
				green = t;
				blue = p; 
				break;
			
			case 1:
				red = q; 
				green = value; 
				blue = p;
				break;
			
			case 2:
				red = p; 
				green = value; 
				blue = t; 
				break;

			case 3:
				red = p; 
				green = q; 
				blue = value; 
				break;

			case 4:
				red = t; 
				green = p; 
				blue = value; 
				break;

			case 5:
				red = value; 
				green = p; 
				blue = value; 
				break;
		}
	}

	result.SetElement(icmm::CRgb::CI_RED, red);
	result.SetElement(icmm::CRgb::CI_GREEN, green);
	result.SetElement(icmm::CRgb::CI_BLUE, blue);

	return true;
}


CHsvToRgbTransformation::ResultType CHsvToRgbTransformation::GetValueAt(const ArgumentType& argument) const
{
	CHsvToRgbTransformation::ResultType result(3);

	GetValueAt(argument, result);

	return result;
}


} // namespace iccm


