#ifndef iview_CInteractiveRectangleShape_included
#define iview_CInteractiveRectangleShape_included


#include "iview/CInteractiveShapeBase.h"


namespace iview
{


/**
	Interactive shape object for iview::CSearchArea.
*/
class CInteractiveRectangleShape: public CInteractiveShapeBase
{
public:
	typedef CInteractiveShapeBase BaseClass;

	CInteractiveRectangleShape();

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void InvalidateBoundingBox();

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

protected:
	void CalcPoints(const i2d::CRectangle& rectangle, const iview::CScreenTransform& transform) const;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void CalcBoundingBox(i2d::CRect& result) const;
	virtual void BeginLogDrag(const i2d::CVector2d& reference);
	virtual void SetLogDragPosition(const i2d::CVector2d& position);

	enum EditNode
	{
		EN_NONE = -1,
		EN_CORNER_FIRST = 0,
		EN_CORNER11 = EN_CORNER_FIRST,
		EN_CORNER12 = 1,
		EN_CORNER21 = 2,
		EN_CORNER22 = 3,
		EN_LAST = EN_CORNER22
	};

	i2d::CVector2d m_referencePosition;
	EditNode m_editNode;

	mutable istd::CIndex2d m_corners[2][2];
	mutable bool m_arePointsValid;
};


} // namespace iview


#endif //iview_CInteractiveRectangleShape_included


