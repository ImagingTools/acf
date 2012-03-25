#ifndef i2d_ICalibrationProvider_included
#define i2d_ICalibrationProvider_included


// ACF includes
#include "istd/IChangeable.h"

#include "i2d/ITransformation2d.h"


namespace i2d
{


/**
	Common interface for an object, which delivers a 2D-calibration.
*/
class ICalibrationProvider: virtual public istd::IChangeable
{
public:
	/**
		Get access to transformation object, which transforms a coordinate system to a logical one.
		\return		Pointer to transformation if it is accessible, or NULL.
	*/
	virtual const i2d::ITransformation2d* GetCalibration() const = 0;
};


} // namespace i2d


#endif // !i2d_ICalibrationProvider_included


