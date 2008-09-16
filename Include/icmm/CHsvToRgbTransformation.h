#ifndef icmm_CHsvToRgbTransformation_included
#define icmm_CHsvToRgbTransformation_included


#include "icmm/IColorTransformation.h"


namespace icmm
{


/**
*/
class CHsvToRgbTransformation: public icmm::IColorTransformation
{
public:
	// reimplemented (icmm::IColorTransformation)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const;
	virtual ResultType GetValueAt(const ArgumentType& argument) const;
};


} // namespace icmm


#endif // !icmm_CHsvToRgbTransformation_included


