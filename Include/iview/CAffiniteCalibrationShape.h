#ifndef iview_CAffiniteCalibrationShape_included
#define iview_CAffiniteCalibrationShape_included


#include <iview/CNoneCalibrationShape.h>


namespace iview
{


// TODO: Redesign it to ACF transformation concept.

/**
	Shape object for affinite calibration.
	You may use this shape to visualize affine calibration object on display console.
	\sa simple implementation of affine transformation: \c i2d::CAffineTransformation2d.
*/
class CAffiniteCalibrationShape: public iview::CNoneCalibrationShape
{
public:
	typedef iview::CNoneCalibrationShape BaseClass;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;
};


} // namespace iview


#endif // !iview_CAffiniteCalibrationShape_included


