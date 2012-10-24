#ifndef iview_CInteractivePolylineShape_included
#define iview_CInteractivePolylineShape_included


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "iview/CInteractivePolygonShape.h"


namespace iview
{


class CInteractivePolylineShape: public CInteractivePolygonShape
{
public:
	typedef CInteractivePolygonShape BaseClass;

	CInteractivePolylineShape();

	bool IsOrientationVisible() const;
	void SetOrientationVisible(bool state = true);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

protected:
	virtual void DrawOrientationMarker(
				QPainter& drawContext,
				const QPen& rightPen,
				const QBrush& rightBrush,
				const QPen& leftPen,
				const QBrush& leftBrush,
				const i2d::CLine2d& segmentLine,
				double scale) const;
	// reimplemented (iview::CInteractivePolygonShape)
	virtual void DrawCurve(QPainter& drawContext) const;
	virtual void DrawArea(QPainter& drawContext) const;
	virtual void DrawSelectionElements(QPainter& drawContext) const;
	virtual bool IsCurveTouched(istd::CIndex2d position) const;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

private:
	bool m_isOrientationVisible;
};


inline bool CInteractivePolylineShape::IsOrientationVisible() const
{
	return m_isOrientationVisible;
}


inline void CInteractivePolylineShape::SetOrientationVisible(bool state)
{
	m_isOrientationVisible = state;
}


} // namespace iview


#endif // !iview_CInteractivePolylineShape_included


