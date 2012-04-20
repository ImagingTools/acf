#ifndef iview_CInteractiveAnnulusShape_included
#define iview_CInteractiveAnnulusShape_included


#include "iview/CInteractivePinShape.h"


namespace iview
{


class CInteractiveAnnulusShape: public CInteractivePinShape
{
public:
	typedef CInteractivePinShape BaseClass;

	CInteractiveAnnulusShape();

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

	// reimplemented (iview::CInteractiveShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

protected:
	virtual void DrawAnnulus(QPainter& painter, istd::CIndex2d center, int minRadius1, int maxRadius1, bool fillFlag = true) const;

protected:

	bool m_isCenterVisible;

	bool m_editRadiusMode;
	bool m_isEditableRadius;

	// true when the outer radius is being edited
	bool m_editRadius2Mode;
	// true when the outer radius is editable
	bool m_isEditableRadius2;
};


} // namespace iview


#endif // !iview_CInteractiveAnnulusShape_included



