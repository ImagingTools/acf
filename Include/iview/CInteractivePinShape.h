#ifndef iview_CInteractivePinShape_included
#define iview_CInteractivePinShape_included


#include "i2d/CVector2d.h"
#include "i2d/CPosition2d.h"

#include "iview/CInteractiveShapeBase.h"


namespace iview
{


class CInteractivePinShape: public CInteractiveShapeBase
{
public:
	typedef CInteractiveShapeBase BaseClass;

	CInteractivePinShape();

	// reimplemented (iview::IInteractiveShape)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

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

	i2d::CVector2d m_referencePosition;
};


} // namespace iview


#endif // !iview_CInteractivePinShape_included


