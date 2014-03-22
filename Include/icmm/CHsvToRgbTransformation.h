#ifndef icmm_CHsvToRgbTransformation_included
#define icmm_CHsvToRgbTransformation_included


// ACF includes
#include "icmm/IColorTransformation.h"


namespace icmm
{


/**
	Implementation of HSV-to-RGB color transformation.
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


