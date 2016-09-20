#ifndef icmm_CRgbToCmykTransformation_included
#define icmm_CRgbToCmykTransformation_included


// ACF includes
#include <icmm/IColorTransformation.h>


namespace icmm
{


/**
	Implementation of RGB-to-CMYK color transformation.
*/
class CRgbToCmykTransformation: public icmm::IColorTransformation
{
public:
	// reimplemented (icmm::IColorTransformation)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const;
	virtual ResultType GetValueAt(const ArgumentType& argument) const;
};


} // namespace icmm


#endif // !icmm_CRgbToCmykTransformation_included


