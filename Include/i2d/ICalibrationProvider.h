#ifndef i2d_ICalibrationProvider_included
#define i2d_ICalibrationProvider_included


// ACF includes
#include "istd/IChangeable.h"

#include "i2d/ICalibration2d.h"


namespace i2d
{


/**
	Common interface for an object, which delivers a 2D-calibration.
*/
class ICalibrationProvider: virtual public istd::IChangeable
{
public:
	/**
		Get access to transformation object, which transforms a local logical coordinate system to global one.
		\return		Pointer to transformation if it is accessible, or NULL.
	*/
	virtual const ICalibration2d* GetCalibration() const = 0;
};


} // namespace i2d


#endif // !i2d_ICalibrationProvider_included


