#ifndef iview_IVisualCalibrationInfo_included
#define iview_IVisualCalibrationInfo_included


#include "i2d/ICalibrationProvider.h"


namespace iview
{


/**
	Common interface for all objects which allow access to calibration info.
*/
class IVisualCalibrationInfo: virtual public i2d::ICalibrationProvider
{
public:
	/**
		Get minimal distance between grid lines.
	*/
	virtual double GetMinGridDistance() const = 0;
	
	/**
		Check if grid is visible.
	*/
	virtual bool IsGridVisible() const = 0;
	
	/**
		Check if grid should be shown in milimeter.
	*/
	virtual bool IsGridInMm() const = 0;
};


} // namespace iview


#endif // !iview_IVisualCalibrationInfo_included


