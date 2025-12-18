#ifndef iview_CCenterPinShape_included
#define iview_CCenterPinShape_included


#include <i2d/CVector2d.h>
#include <i2d/CPosition2d.h>

#include <iview/CInteractiveShapeBase.h>
#include <iview/CPinShape.h>


namespace iview
{


class CCenterPinShape: public CPinShape
{
public:
	typedef CPinShape BaseClass;
	typedef CInteractiveShapeBase ShapeBaseClass;

	CCenterPinShape();

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const override;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) override;
	virtual bool OnMouseMove(istd::CIndex2d position) override;

protected:
	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;
};


} // namespace iview


#endif // !iview_CPinShape_included


