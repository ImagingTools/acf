#ifndef iview_CAnnulusShape_included
#define iview_CAnnulusShape_included


#include <iview/CPinShape.h>


namespace iview
{


class CAnnulusShape: public CPinShape
{
public:
	typedef CPinShape BaseClass;
	typedef CInteractiveShapeBase ShapeBaseClass;

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
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) override;
	virtual bool OnMouseMove(istd::CIndex2d position) override;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;

protected:
	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;

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



