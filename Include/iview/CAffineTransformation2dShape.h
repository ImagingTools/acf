#ifndef iview_CAffineTransformation2dShape_included
#define iview_CAffineTransformation2dShape_included


// ACF includes
#include <i2d/CAffineTransformation2d.h>
#include "iview/CInteractiveShapeBase.h"


namespace iview
{


/**
	Interactive shape to visualize i2d::CAffineTransformation2d
 */
class CAffineTransformation2dShape: public CInteractiveShapeBase
{
public:


	enum ControlPoint
	{
		NO_POINT = 0,
		POINT1 = 1,
		POINT2 = 2,
		POINT3 = 4,
		POINT4 = 8,
		POINT5 = 16,
		ALL_POINTS = POINT1 | POINT2 | POINT3 | POINT4 | POINT5
	};

	CAffineTransformation2dShape();

	/**
		Set control points that can be moved by the user (OR'ed ControlPoints).
	 */
	void SetActiveControlPoints(ControlPoint points = ALL_POINTS);

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

protected:
	enum{
		CONTROL_POINTS_COUNT = 11,
		ACTIVE_POINTS_COUNT = 5
	};
	typedef i2d::CVector2d ControlPoints[CONTROL_POINTS_COUNT];

	/** Generate transformed points using given transformation. 
	 
		If there is no transformation, generate default Cartesian coordinate 
		system corner points: begin, X axis end and Y axis end.
		\param size Length of both axes
	*/
	static const ControlPoints& GetControlPoints();
	/** 
		Transform control points from logical to screen coordinates
		
		\param transform LogToScreenTransform assigned to the view
	*/
	void GetScreenPoints(const ControlPoints& points, ControlPoints& result) const;
	static void GetTransformedPoints(const ControlPoints& points, ControlPoints& result, const i2d::CAffineTransformation2d& transformation);

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void BeginLogDrag(const i2d::CVector2d& reference);
	virtual void SetLogDragPosition(const i2d::CVector2d& position);

private:
	i2d::CVector2d m_referencePosition;
	int m_activeControlPoints;

	ControlPoint m_currentPoint; ///< point pressed by mouse
};


} // namespace iview


#endif // !iview_CAffineTransformation2dShape_included


