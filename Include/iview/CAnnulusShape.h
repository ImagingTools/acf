#ifndef iview_CAnnulusShape_included
#define iview_CAnnulusShape_included


#include "iview/CPinShape.h"


namespace iview
{


class CAnnulusShape: public CPinShape
{
public:
	typedef CPinShape BaseClass;

	CAnnulusShape();

	virtual bool IsEditableRadiusInner() const;
	virtual void SetEditableRadiusInner(bool editable = true);
	virtual bool IsEditableRadiusOuter() const;
	virtual void SetEditableRadiusOuter(bool editable = true);

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
	virtual void DrawAnnulus(QPainter& painter, istd::CIndex2d center, int minRadius1, int maxRadius1, bool fillFlag = true) const;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

	bool m_isCenterVisible;

	bool m_isEditableRadius;
	bool m_isEditableRadius2;

	enum EditMode
	{
		EM_NONE,
		EM_INNER_RADIUS,
		EM_OUTER_RADIUS
	};

	int m_editMode;
};


} // namespace iview


#endif // !iview_CAnnulusShape_included



