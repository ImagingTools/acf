#ifndef icmm_CCmykToRgbTransformation_included
#define icmm_CCmykToRgbTransformation_included


// ACF includes
#include <icmm/IColorTransformation.h>


namespace icmm
{


/**
	Implementation of CMYK-to-RGB color transformation.
*/
class CCmykToRgbTransformation: public icmm::IColorTransformation
{
public:
	// reimplemented (icmm::IColorTransformation)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const;
	virtual ResultType GetValueAt(const ArgumentType& argument) const;
};


} // namespace icmm


#endif // !icmm_CCmykToRgbTransformation_included


