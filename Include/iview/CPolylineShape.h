#ifndef iview_CPolylineShape_included
#define iview_CPolylineShape_included


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <iview/CPolygonShape.h>


namespace iview
{


class CPolylineShape: public CPolygonShape
{
public:
	typedef CPolygonShape BaseClass;
	typedef CInteractiveShapeBase ShapeBaseClass;

	CPolylineShape();

	bool IsOrientationVisible() const;
	void SetOrientationVisible(bool state = true);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;
	virtual QString GetShapeDescriptionAt(istd::CIndex2d position) const;

	// static methods
	/**
		Draw an orientation marker consisting of two triangles (black for 
		outside and white for inside)
	*/
	static void DrawOrientationMarker(
				QPainter& drawContext,
				const QPen& rightPen,
				const QBrush& rightBrush,
				const QPen& leftPen,
				const QBrush& leftBrush,
				const i2d::CLine2d& segmentLine,
				double scale);

	static void DrawOrientationMarker(
				QPainter& drawContext,
				const i2d::CVector2d& point,
				const i2d::CVector2d& direction,
				double maxWidth,
				const iview::IColorSchema& colorSchema);
protected:
	// reimplemented (iview::CPolygonShape)
	virtual void DrawCurve(QPainter& drawContext) const;
	virtual void DrawArea(QPainter& drawContext) const;
	virtual void DrawSelectionElements(QPainter& drawContext) const;

	// reimplemented (iview::CRectControlledShapeBase)
	virtual bool IsCurveTouched(istd::CIndex2d position) const;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

private:
	bool m_isOrientationVisible;
};


inline bool CPolylineShape::IsOrientationVisible() const
{
	return m_isOrientationVisible;
}


inline void CPolylineShape::SetOrientationVisible(bool state)
{
	m_isOrientationVisible = state;
}


} // namespace iview


#endif // !iview_CPolylineShape_included


