#ifndef iview_CInteractiveCenterPinShape_included
#define iview_CInteractiveCenterPinShape_included


#include "i2d/CVector2d.h"
#include "i2d/CPosition2d.h"

#include "iview/CInteractiveShapeBase.h"
#include "iview/CInteractivePinShape.h"


namespace iview
{


class CInteractiveCenterPinShape: public CInteractivePinShape
{
public:
	typedef CInteractivePinShape BaseClass;

	CInteractiveCenterPinShape();

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

protected:
	// reimplemented (iview::CInteractiveShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

	i2d::CVector2d m_referencePosition;
};


} // namespace iview


#endif // !iview_CInteractivePinShape_included


