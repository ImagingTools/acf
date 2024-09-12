#pragma once


// ACF includes
#include <icmm/CRgb.h>
#include <icmm/CRgbColor.h>
#include <icmm/ITristimulusSpecification.h>


namespace icmm
{


/**
	This class implements "regular" RGB Color class, BUT:
	makes it so that the ColorSpec is ALWAYS D65/2 (per the spec for SRGB)
*/
class CSRgbColor: virtual public CRgbColor
{
public:
	CSRgbColor(const icmm::CRgb& rgb);
};


} // namespace icmm


