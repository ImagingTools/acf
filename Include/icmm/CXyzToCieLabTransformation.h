#ifndef icmm_CXyzToCieLabTransformation_included
#define icmm_CXyzToCieLabTransformation_included


// ACF includes
#include <icmm/IColorTransformation.h>


namespace icmm
{


/**
	Implementation of XYZ-to-CIE-Lab color transformation.
	2° Observer and D65-illumination are used.
*/
class CXyzToCieLabTransformation: public icmm::IColorTransformation
{
public:
	// reimplemented (icmm::IColorTransformation)
	virtual bool GetValueAt(const icmm::CVarColor& argument, icmm::CVarColor& result) const;
	virtual icmm::CVarColor GetValueAt(const icmm::CVarColor& argument) const;
};


} // namespace icmm


#endif // !icmm_CXyzToCieLabTransformation_included


