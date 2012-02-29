#ifndef iview_ILogicalCoords_included
#define iview_ILogicalCoords_included


#include "iview/CScreenTransform.h"

#include "iview/iview.h"


namespace iview
{


class ILogicalCoords: virtual public istd::IPolymorphic
{
public:
	/**
		Get logical units to view transformation.
		This transformation converts logical units to view coordinate system.
		Please note, that it is only affine transformation, not a calibration.
		You can use it as a calibration only, when linear transformation is enough.
		To get transformation to convert logical units to screen pixel, use GetLogToScreenTransform().
	*/
	virtual const i2d::CAffine2d& GetLogToViewTransform() const = 0;
	
	/**
		Get a direct transformation from logical units to screen pixels.
	*/
	virtual const iview::CScreenTransform& GetLogToScreenTransform() const = 0;
};


} // namespace iview


#endif // !iview_ILogicalCoords_included



