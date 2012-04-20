#ifndef iview_CInteractiveCircleShape_included
#define iview_CInteractiveCircleShape_included


#include "iview/CInteractivePinShape.h"


namespace iview
{


class CInteractiveCircleShape: public CInteractivePinShape
{
public:
	typedef CInteractivePinShape BaseClass;

	CInteractiveCircleShape();

	virtual bool IsEditableRadius() const;
	virtual void SetEditableRadius(bool editable = true);
	virtual bool IsCenterVisible() const;
	virtual void SetCenterVisible(bool state = true);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	
	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

protected:
	bool m_isEditableRadius;
	bool m_isCenterVisible;

	bool m_editRadiusMode;
};


} // namespace iview


#endif // !iview_CInteractiveCircleShape_included


