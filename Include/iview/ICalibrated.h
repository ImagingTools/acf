#ifndef iview_ICalibrated_included
#define iview_ICalibrated_included


#include "istd/IPolymorphic.h"

#include "iview/ICalibration.h"


namespace iview
{

/**
	Common interface for all objects which allow access to calibration info.
*/
class ICalibrated: virtual public istd::IPolymorphic
{
public:
	/**
		Get access to the calibration object.
	*/
	virtual const ICalibration& GetCalibration() const = 0;
};


} // namespace iview


#endif // !iview_ICalibrated_included


