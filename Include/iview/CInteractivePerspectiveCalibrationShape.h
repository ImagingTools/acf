#ifndef iview_CInteractivePerspectiveCalibrationShape_included
#define iview_CInteractivePerspectiveCalibrationShape_included


#include "iview/CCalibrationShapeBase.h"


namespace iview
{


// TODO: Redesign it to ACF transformation concept.

/**
	This shape object, is used to tune bounds of iview::CPerpectiveCalib using user interface.
*/
class CInteractivePerspectiveCalibrationShape: public iview::CCalibrationShapeBase
{
public:
	// reimplemented (iview::IInteractiveShape)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

protected:
	// reimplemented (iview::CInteractiveShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;
	virtual void BeginLogDrag(const i2d::CVector2d& reference);
	virtual void SetLogDragPosition(const i2d::CVector2d& position);

private:
	enum EditBound
	{
		EB_LEFT,
		EB_RIGHT,
		EB_TOP,
		EB_BOTTOM
	} m_editBound;
};


} // namespace iview


#endif // !iview_CInteractivePerspectiveCalibrationShape_included


