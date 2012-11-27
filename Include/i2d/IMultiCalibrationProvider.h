#ifndef i2d_IMultiCalibrationProvider_included
#define i2d_IMultiCalibrationProvider_included


// ACF includes
#include "istd/IChangeable.h"
#include "i2d/ICalibration2d.h"
#include "iprm/ISelectionConstraints.h"


namespace i2d
{


/**
	Common interface for an object, which delivers a list of 2D-calibration objects.
*/
class IMultiCalibrationProvider: virtual public istd::IChangeable
{
public:
	/**
		Get optional information about calibration objects in form of section contraints.
		It allows to get name and description of each calibration object.
	*/
	virtual const iprm::ISelectionConstraints* GetCalibrationSelectionContraints() const = 0;

	/**
		Get number of available calibrations.
	*/
	virtual int GetCalibrationsCount() const = 0;

	/**
		Get access to transformation object with a given index, which transforms a local logical coordinate system to global one.
		\return		Pointer to transformation if it is accessible, or NULL.
	*/
	virtual const ICalibration2d* GetCalibration(int calibrationIndex) const = 0;
};


} // namespace i2d


#endif // !i2d_IMultiCalibrationProvider_included


