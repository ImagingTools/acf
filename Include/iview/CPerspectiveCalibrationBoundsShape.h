#ifndef iview_CPerspectiveCalibrationBoundsShape_included
#define iview_CPerspectiveCalibrationBoundsShape_included


#include "iview/CInteractiveShapeBase.h"


namespace iview
{


/**
	This shape object, is used to tune bounds of iview::CPerpectiveCalib using user interface.
*/
class CPerspectiveCalibrationBoundsShape: public CInteractiveShapeBase
{
public:
	typedef CInteractiveShapeBase BaseClass;

	// reimplemented (iview::IInteractiveShape)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

protected:
	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

	// reimplemented (iview::CInteractiveShapeBase)
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


#endif // !iview_CPerspectiveCalibrationBoundsShape_included


