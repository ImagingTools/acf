#ifndef iview_CNoneCalibrationShape_included
#define iview_CNoneCalibrationShape_included


#include "iview/CCalibrationShapeBase.h"


namespace iview
{


// TODO: Redesign it to ACF transformation concept.

/**
	Shape object for none calibration.
	You need this shape to visualize grid without calibration on the shape view.
*/
class CNoneCalibrationShape: public CCalibrationShapeBase
{
public:
	typedef iview::CCalibrationShapeBase BaseClass;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (iview::IInteractiveShape)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

protected:
	/**
		Get access to calibration.
	*/
	virtual const i2d::ITransformation2d* GetCalibrationPtr() const;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void CalcBoundingBox(i2d::CRect& result) const;
	virtual void BeginLogDrag(const i2d::CVector2d& reference);
	virtual void SetLogDragPosition(const i2d::CVector2d& position);
};


} // namespace iview


#endif // !iview_CNoneCalibrationShape_included


