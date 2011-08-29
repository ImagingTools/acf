#ifndef iprm_ILinearAdjustParams_included
#define iprm_ILinearAdjustParams_included


#include "iser/ISerializable.h"


namespace iprm
{


class ILinearAdjustConstraints;


/**
	Get parameter of linear adjust.
	Linear adjust is simple linear transformation in form y = x * scale + offset.
*/
class ILinearAdjustParams: virtual public iser::ISerializable
{
public:
	/**
		Get access to constraints object.
		This object describes for example range of possible parameter values.
	*/
	virtual const ILinearAdjustConstraints* GetAdjustConstraints() const = 0;

	/**
		Get value of scale factor.
	*/
	virtual double GetScaleFactor() const = 0;
	/**
		Set value of scale factor.
	*/
	virtual void SetScaleFactor(double scale) = 0;
	/**
		Get value of offset factor.
	*/
	virtual double GetOffsetFactor() const = 0;
	/**
		Set value of offset factor.
	*/
	virtual void SetOffsetFactor(double offset) = 0;
};


} // namespace iprm


#endif // !iprm_ILinearAdjustParams_included


