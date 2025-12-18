#ifndef iview_CCircleShape_included
#define iview_CCircleShape_included


#include <iview/CPinShape.h>


namespace iview
{


class CCircleShape: public CPinShape
{
public:
	typedef CPinShape BaseClass;
	typedef CInteractiveShapeBase ShapeBaseClass;

	CCircleShape();

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


