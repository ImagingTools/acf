#pragma once


// ACF includes
#include <imath/TIMathFunction.h>
#include <icmm/CVarColor.h>


namespace icmm
{


/**
	Basic interface for a color transformation.
*/
class IColorTransformation: virtual public imath::TIMathFunction<icmm::CVarColor, icmm::CVarColor>
{
};


} // namespace icmm


