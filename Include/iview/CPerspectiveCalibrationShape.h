#ifndef iview_CPerspectiveCalibrationShape_included
#define iview_CPerspectiveCalibrationShape_included


#include <iview/CNoneCalibrationShape.h>


namespace iview
{


/**
	Shape object for perspective calibration.
	You need this shape to visualize calibration of type iview::CPerspectiveCalibration on the shape view.
*/
class CPerspectiveCalibrationShape: public iview::CNoneCalibrationShape
{
public:
	typedef iview::CNoneCalibrationShape BaseClass;

	// reimplemented (imod::IObserver)
	virtual void Invalidate();

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

protected:
	bool GetLineScreenPosition(const i2d::ICalibration2d& calib, const i2d::CLine2d& logLine, int gridSize, QPointF& point1, QPointF& point2) const;
};


} // namespace iview


#endif // !iview_CPerspectiveCalibrationShape_included


