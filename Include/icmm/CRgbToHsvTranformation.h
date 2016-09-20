#ifndef icmm_CRgbToHsvTranformation_included
#define icmm_CRgbToHsvTranformation_included


// ACF includes
#include <icmm/IColorTransformation.h>


namespace icmm
{


/**
	Implementation of HSV-to-RGB color transformation.
*/
class CRgbToHsvTranformation: public icmm::IColorTransformation
{
public:
	// reimplemented (icmm::IColorTransformation)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const;
	virtual ResultType GetValueAt(const ArgumentType& argument) const;
};


} // namespace icmm


#endif // !icmm_CRgbToHsvTranformation_included


