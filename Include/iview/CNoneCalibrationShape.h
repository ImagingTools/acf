#ifndef iview_CNoneCalibrationShape_included
#define iview_CNoneCalibrationShape_included


#include "iview/CShapeBase.h"


namespace iview
{


/**
	Shape object for none calibration.
	You need this shape to visualize grid without calibration on the shape view.
*/
class CNoneCalibrationShape: public CShapeBase
{
public:
	typedef CShapeBase BaseClass;

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
	virtual const i2d::ICalibration2d* GetCalibrationPtr() const;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void BeginLogDrag(const i2d::CVector2d& reference);
	virtual void SetLogDragPosition(const i2d::CVector2d& position);

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;
};


} // namespace iview


#endif // !iview_CNoneCalibrationShape_included


