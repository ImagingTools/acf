#ifndef iview_CInteractiveTubePolylineShape_included
#define iview_CInteractiveTubePolylineShape_included


#include "iview/CInteractivePolylineShape.h"


namespace iview
{


/**
	Graphical representation of tube region.
*/
class CInteractiveTubePolylineShape: public iview::CInteractivePolylineShape
{
public:
	typedef iview::CInteractivePolylineShape BaseClass;

	CInteractiveTubePolylineShape();

	/**
		Check if orientation of polyline is visible.
		Orientation is shown as small arrow at the end of polyline.
	*/
	virtual bool IsOrientationVisible() const;
	
	/**
		Set the orientation to be visible or not.
		@param	state	if true, orientation will be visible.
	*/
	virtual void SetOrientationVisible(bool state = true);

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

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

protected:
	// reimplemented (iview::CInteractivePolygonShape)
	virtual void DrawCurve(QPainter& drawContext) const;
	virtual void DrawSelectionElements(QPainter& drawContext) const;
    
private:
	enum TickerType{
		TT_NONE,
		TT_BASIC,
		TT_LEFT,
		TT_RIGHT
	};

	bool m_isAlwaysDraggable;
	bool m_isOrientationVisible;

	int m_draggedTickerType;
	int m_draggedTickerIndex;
};


// inline methods

inline bool CInteractiveTubePolylineShape::IsOrientationVisible() const
{
	return m_isOrientationVisible;
}


inline void CInteractiveTubePolylineShape::SetOrientationVisible(bool state)
{
	if (m_isOrientationVisible != state){
		m_isOrientationVisible = state;
		Invalidate(iview::CS_CONSOLE);
	}
}


inline bool CInteractiveTubePolylineShape::IsAlwaysMovable() const
{
	return m_isAlwaysDraggable;
}


inline void CInteractiveTubePolylineShape::SetAlwaysMovable(bool state)
{
	m_isAlwaysDraggable = state;
}


} // namespace qto


#endif // !iview_CInteractiveTubePolylineShape_included



