#ifndef iview_CArcShape_included
#define iview_CArcShape_included


// ACF includes
#include <iview/CPinShape.h>


namespace iview
{


class CArcShape: public CPinShape
{

public:
	typedef CPinShape BaseClass;
	typedef CInteractiveShapeBase ShapeBaseClass;

	CArcShape();

	virtual bool IsEditableRadius() const;
	virtual void SetEditableRadius(bool editable = true);
	virtual bool IsEditableStartAngle() const;
	virtual void SetEditableStartAngle(bool editable = true);
	virtual bool IsEditableAngleWidth() const;
	virtual void SetEditableAngleWidth(bool editable = true);
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

	bool m_isEditableRadius;
	bool m_isCenterVisible;
	bool m_isEditableStartAngle;
	bool m_isEditableAngleWidth;

	enum EditMode
	{
		EM_NONE,
		EM_RADIUS,
		EM_STARTANGLE,
		EM_ANGLEWIDTH
	};

	int m_editMode;
};


} // namespace iview


#endif // !iview_CArcShape_included