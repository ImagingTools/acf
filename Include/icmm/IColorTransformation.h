#ifndef icmm_IColorTransformation_included
#define icmm_IColorTransformation_included


#include "imath/TIMathFunction.h"

#include "icmm/CVarColor.h"


namespace icmm
{


/**
*/
class IColorTransformation: virtual public imath::TIMathFunction<icmm::CVarColor, icmm::CVarColor>
{
};


} // namespace icmm


#endif // !icmm_IColorTransformation_included


