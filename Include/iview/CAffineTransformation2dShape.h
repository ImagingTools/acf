#include "CInteractiveShapeBase.h"


namespace iview
{


/**
	Interactive shape to visualize i2d::CAffineTransformation2d
 */
class CAffineTransformation2dShape: public CInteractiveShapeBase
{
public:
	CAffineTransformation2dShape();


	enum ControlPoint
	{
		NO_POINT = 0, POINT1 = 1, POINT2 = 2, POINT3 = 4,
		POINT4 = 8, POINT5 = 16, ALL_POINTS = POINT1 | POINT2 | POINT3 | POINT4 | POINT5
	};

	/**
		Set control points that can be moved by the user (OR'ed ControlPoints)
	 */
	void SetActiveControlPoints(ControlPoint points = ALL_POINTS);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

protected:
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

}

