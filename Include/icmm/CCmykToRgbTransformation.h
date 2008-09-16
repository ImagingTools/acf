#ifndef icmm_CCmykToRgbTransformation_included
#define icmm_CCmykToRgbTransformation_included


#include "icmm/IColorTransformation.h"


namespace icmm
{


/**
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


