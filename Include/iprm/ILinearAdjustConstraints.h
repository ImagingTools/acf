#ifndef iprm_ILinearAdjustConstraints_included
#define iprm_ILinearAdjustConstraints_included


#include "istd/IChangeable.h"
#include "istd/CRange.h"

#include "iprm/iprm.h"


namespace iprm
{


/**
	Describes constraints of possible values of iprm::ILinearAdjust.
*/
class ILinearAdjustConstraints: virtual public istd::IChangeable
{
public:
	/**
		Get range of possible scale factor values.
	*/
	virtual istd::CRange GetScaleFactorRange() const = 0;

	/**
		Get range of possible offset factor values.
	*/
	virtual istd::CRange GetOffsetFactorRange() const = 0;
};


} // namespace iprm


#endif // !iprm_ILinearAdjustConstraints_included


