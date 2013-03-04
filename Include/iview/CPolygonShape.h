#ifndef iview_CPolygonShape_included
#define iview_CPolygonShape_included


// Qt includes
#include <QtCore/QVector>
#include <QtGui/QPolygon>

// ACF includes
#include "i2d/CVector2d.h"


#include "iview/CTransformableRectangleShape.h"


namespace iview
{


class CPolygonShape: public CTransformableRectangleShape
{
public:
	typedef CTransformableRectangleShape BaseClass;

	CPolygonShape();

	/**
		Check if first point is always visible as ticker.
		It is used to recognize wchich one point is the first.
	*/
	virtual bool IsFirstVisible() const;
	virtual void SetFirstVisible(bool state = true);

	/**
		Check if in all edit modes is this shape draggable.
		Normally it is possible to drag this object only in EM_NONE mode.
	*/
	virtual bool IsAlwaysMovable() const;

	/**
		Set flag if in all edit modes is this shape draggable.
		\sa IsAlwaysMovable()
	*/
	virtual void SetAlwaysMovable(bool state = true);

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

	// reimplemented (iview::IDraggable)
	virtual bool IsDraggable() const;

protected:
	virtual i2d::CVector2d GetSegmentMiddle(int index) const;
	virtual void DrawCurve(QPainter& drawContext) const;
	virtual void DrawArea(QPainter& drawContext) const;
	virtual void DrawSelectionElements(QPainter& drawContext) const;
	virtual bool IsCurveTouched(istd::CIndex2d position) const;

	// reimplemented (iview::CTransformableRectangleShape)
	virtual void EnsureValidNodes() const;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void BeginLogDrag(const i2d::CVector2d& reference);
	virtual void SetLogDragPosition(const i2d::CVector2d& position);

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

	i2d::CVector2d m_referencePosition;
	int m_referenceIndex;
	QVector<i2d::CVector2d> m_references;

	i2d::CVector2d m_castAxis;
	mutable i2d::CAffine2d m_castTransform;
	mutable QPolygonF m_screenPoints;

private:
	bool m_isFirstVisible;
	bool m_isAlwaysDraggable;
};


inline bool CPolygonShape::IsFirstVisible() const
{
	return m_isFirstVisible;
}


inline void CPolygonShape::SetFirstVisible(bool state)
{
	m_isFirstVisible = state;
}


inline bool CPolygonShape::IsAlwaysMovable() const
{
	return m_isAlwaysDraggable;
}


inline void CPolygonShape::SetAlwaysMovable(bool state)
{
	m_isAlwaysDraggable = state;
}


} // namespace iview


#endif // !iview_CPolygonShape_included


