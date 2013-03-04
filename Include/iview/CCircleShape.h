#ifndef iview_CCircleShape_included
#define iview_CCircleShape_included


#include "iview/CPinShape.h"


namespace iview
{


class CCircleShape: public CPinShape
{
public:
	typedef CPinShape BaseClass;

	CCircleShape();

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

protected:
	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

	bool m_isEditableRadius;
	bool m_isCenterVisible;

	enum EditMode
	{
		EM_NONE,
		EM_RADIUS
	};

	int m_editMode;
};


} // namespace iview


#endif // !iview_CCircleShape_included


