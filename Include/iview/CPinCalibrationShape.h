#ifndef iview_CPinCalibrationShape_included
#define iview_CPinCalibrationShape_included


#include "i2d/CVector2d.h"
#include "i2d/CPosition2d.h"

#include "iview/CCalibrationShapeBase.h"


namespace iview
{


// TODO: Redesign it to ACF transformation concept.
class CPinCalibrationShape: public CCalibrationShapeBase
{
public:
	typedef CCalibrationShapeBase BaseClass;

	CPinCalibrationShape();

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

protected:
	// reimplemented (iview::CInteractiveShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;
	virtual void BeginLogDrag(const i2d::CVector2d& reference);
	virtual void SetLogDragPosition(const i2d::CVector2d& position);

	i2d::CVector2d m_referencePosition;
};


} // namespace iview


#endif // !iview_CPinCalibrationShape_included


