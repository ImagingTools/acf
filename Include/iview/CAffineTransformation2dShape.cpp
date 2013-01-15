#include "CAffineTransformation2dShape.h"
#include <i2d/CAffineTransformation2d.h>
#include "IColorSchema.h"
#include "CConsoleBase.h"

// Qt includes
#include <QtGui/QPainter>

namespace iview
{


CAffineTransformation2dShape::CAffineTransformation2dShape(void)
: m_activeControlPoints(ALL_POINTS), m_currentPoint(NO_POINT)
{
}


void CAffineTransformation2dShape::SetActiveControlPoints(ControlPoint points)
{
	m_activeControlPoints = points;
}


namespace
{


/** 
	Structure representing a set of 5 control points of the shape 
 
	\todo Arrow tip points are also generated.
 */
struct ControlPoints
{


	/** Generate transformed points using given transformation. 
	 
		If there is no transformation, generate default Cartesian coordinate 
		system corner points: begin, X axis end and Y axis end.
		\param size Length of both axes
		\param transformationPtr initial transformation
	 */
	ControlPoints(int size, const i2d::CAffineTransformation2d * transformationPtr = NULL)
	{
		// points delimiting the original (not transformed) coordinate system
		points[0] = i2d::CVector2d(0, size);
		points[1] = i2d::CVector2d(size, size);
		points[2] = i2d::CVector2d(0, 0);

		if (transformationPtr != NULL){
			points[0] = transformationPtr->GetValueAt(points[0]);
			points[1] = transformationPtr->GetValueAt(points[1]);
			points[2] = transformationPtr->GetValueAt(points[2]);
		}

		points[3] = points[1] + points[2] - points[0];
		points[4] = (points[3] + points[0]) / 2;
	}


	/** 
		Transform control points from logical to screen coordinates
		
		\param transform LogToScreenTransform assigned to the view
	 */
	ControlPoints ToScreen(const iview::CScreenTransform & transform)
	{
		ControlPoints result(100);
		for (int i = 0; i < 5; i++){
			result.points[i] = transform.GetApply(points[i]);
		}

		return result;
	}


	/**
		Calculate transformation from point coordinates.

		\param transformationPtr transformation to be modified
	 */
	void ResetTransformation(i2d::CAffineTransformation2d * transformationPtr)
	{
		i2d::CVector2d translation = points[0];
		double angle = atan2(points[1].GetY(), points[1].GetX());
		i2d::CVector2d scale(points[1].GetDistance(points[0]), points[2].GetDistance(points[0]));

		transformationPtr->Reset(translation, angle, scale);
	}


	/**
		Access any of the points stored in the structure.
	 
		Point indexes:
		[0] - {0,0}, [1] - {1,0}, [2] - {0,1}, [3] - {1,1}, [4] - {0.5,0.5}
		remaining 6 - arrow tips
	 */
	i2d::CVector2d& operator[](int index)
	{
		return points[index];
	}

private:
	i2d::CVector2d points[11];
}; // struct CControlPoints


} // namespace


bool CAffineTransformation2dShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CAffineTransformation2d* transformationPtr =
			dynamic_cast<const i2d::CAffineTransformation2d*>(GetModelPtr());
	if (transformationPtr == NULL){
		return false;
	}

	if (downFlag){
		// use model transformation to transform original points
		ControlPoints points(100, transformationPtr);

		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		ControlPoints screenPoints = points.ToScreen(transform);

		const double pointRadius = 2.5; // pixels
		i2d::CVector2d pos(position.GetX(), position.GetY());

		m_currentPoint = NO_POINT;
		// check whether any of the points or its surrounding was clicked
		for (int i = 0; i < 5; i++){
			ControlPoint point = (ControlPoint)(i + 1);
			if ((m_activeControlPoints & point) != NO_POINT
					&& screenPoints[i].GetDistance(pos) < pointRadius){
				m_currentPoint = point;
				BeginModelChanges();
				return true;
			}
		}
	}
	else{
		m_currentPoint = NO_POINT;
		EndModelChanges();
		return true;
	}

	return false;
}


bool CAffineTransformation2dShape::OnMouseMove(istd::CIndex2d position)
{
	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetModelPtr());

	if (transformationPtr != NULL
			&& m_currentPoint != NO_POINT
			&& m_currentPoint <= POINT5
			&& (m_activeControlPoints & m_currentPoint) != NO_POINT){

		const i2d::CVector2d& cp = transform.GetClientPosition(position);

		ControlPoints points(100, transformationPtr);

		points[m_currentPoint - 1].SetX(cp.GetX());
		points[m_currentPoint - 1].SetY(cp.GetY());

		points.ResetTransformation(transformationPtr);
		UpdateModelChanges();

		return true;
	}

	return false;
}


void CAffineTransformation2dShape::Draw(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	///\todo use ColorSchema?
	QPen baseSystemPen(QColor("cyan"));
	QPen transformedSystemPen(QColor("blue"));
	QPen transformationOffsetsPen(QColor("brown"));
	transformationOffsetsPen.setStyle(Qt::DotLine);
	QPen additionalLinesPen(QColor("blue"));
	additionalLinesPen.setStyle(Qt::DotLine);

	i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetModelPtr());

	drawContext.save();

	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	double tipLength1 = 5 * sqrt(sqrt(transform.GetDeformMatrix().GetApproxScale()));
	double tipLength2 = 0.7 * tipLength1;
	double tipWidth = 0.3 * tipLength1;
	double circleRadius = 2 * tipWidth;

	ControlPoints cp(100);
	ControlPoints sp = cp.ToScreen(transform);
	i2d::CVector2d arrowPoint1(cp[1].GetX() - tipLength1, cp[1].GetY() - tipWidth);
	i2d::CVector2d arrowPoint2(cp[1].GetX() - tipLength2, cp[1].GetY());
	i2d::CVector2d arrowPoint3(cp[1].GetX() - tipLength1, cp[1].GetY() + tipWidth);
	i2d::CVector2d arrowPoint4(cp[2].GetX() - tipWidth, cp[2].GetY() + tipLength1);
	i2d::CVector2d arrowPoint5(cp[2].GetX(), cp[2].GetY() + tipLength2);
	i2d::CVector2d arrowPoint6(cp[2].GetX() + tipWidth, cp[2].GetY() + tipLength1);

	// draw base coordinate system
	drawContext.setPen(baseSystemPen);
	drawContext.setBrush(baseSystemPen.color());
	i2d::CVector2d sap1 = transform.GetApply(arrowPoint1);
	i2d::CVector2d sap2 = transform.GetApply(arrowPoint2);
	i2d::CVector2d sap3 = transform.GetApply(arrowPoint3);
	drawContext.drawLine(sp[0].GetX(), sp[0].GetY(), sp[1].GetX(), sp[1].GetY());
	QPointF points[4];
	points[0] = QPointF(sp[1].GetX(), sp[1].GetY());
	points[1] = QPointF(sap1.GetX(), sap1.GetY());
	points[2] = QPointF(sap2.GetX(), sap2.GetY());
	points[3] = QPointF(sap3.GetX(), sap3.GetY());
	drawContext.drawPolygon(points, 4);

	drawContext.drawLine(sp[0].GetX(), sp[0].GetY(), sp[2].GetX(), sp[2].GetY());
	i2d::CVector2d sap4 = transform.GetApply(arrowPoint4);
	i2d::CVector2d sap5 = transform.GetApply(arrowPoint5);
	i2d::CVector2d sap6 = transform.GetApply(arrowPoint6);
	points[0] = QPointF(sp[2].GetX(), sp[2].GetY());
	points[1] = QPointF(sap4.GetX(), sap4.GetY());
	points[2] = QPointF(sap5.GetX(), sap5.GetY());
	points[3] = QPointF(sap6.GetX(), sap6.GetY());
	drawContext.drawPolygon(points, 4);

	if (transformationPtr == NULL){
		drawContext.restore();
		return;
	}

	// draw transformed coordinate system
	drawContext.setPen(transformedSystemPen);
	drawContext.setBrush(transformedSystemPen.color());
	cp = ControlPoints(100, transformationPtr);
	ControlPoints tsp = cp.ToScreen(transform);
	arrowPoint1 = transformationPtr->GetValueAt(arrowPoint1);
	arrowPoint2 = transformationPtr->GetValueAt(arrowPoint2);
	arrowPoint3 = transformationPtr->GetValueAt(arrowPoint3);
	arrowPoint4 = transformationPtr->GetValueAt(arrowPoint4);
	arrowPoint5 = transformationPtr->GetValueAt(arrowPoint5);
	arrowPoint6 = transformationPtr->GetValueAt(arrowPoint6);
	drawContext.drawLine(tsp[0].GetX(), tsp[0].GetY(), tsp[1].GetX(), tsp[1].GetY());
	sap1 = transform.GetApply(arrowPoint1);
	sap2 = transform.GetApply(arrowPoint2);
	sap3 = transform.GetApply(arrowPoint3);
	points[0] = QPointF(tsp[1].GetX(), tsp[1].GetY());
	points[1] = QPointF(sap1.GetX(), sap1.GetY());
	points[2] = QPointF(sap2.GetX(), sap2.GetY());
	points[3] = QPointF(sap3.GetX(), sap3.GetY());
	drawContext.drawPolygon(points, 4);
	drawContext.drawLine(tsp[0].GetX(), tsp[0].GetY(), tsp[2].GetX(), tsp[2].GetY());
	sap4 = transform.GetApply(arrowPoint4);
	sap5 = transform.GetApply(arrowPoint5);
	sap6 = transform.GetApply(arrowPoint6);
	points[0] = QPointF(tsp[2].GetX(), tsp[2].GetY());
	points[1] = QPointF(sap4.GetX(), sap4.GetY());
	points[2] = QPointF(sap5.GetX(), sap5.GetY());
	points[3] = QPointF(sap6.GetX(), sap6.GetY());
	drawContext.drawPolygon(points, 4);

	// draw lines connecting base system corner points with their transformed counterparts
	drawContext.setPen(transformationOffsetsPen);
	drawContext.drawLine(sp[0].GetX(), sp[0].GetY(), tsp[0].GetX(), tsp[0].GetY());
	drawContext.drawLine(sp[1].GetX(), sp[1].GetY(), tsp[1].GetX(), tsp[1].GetY());
	drawContext.drawLine(sp[2].GetX(), sp[2].GetY(), tsp[2].GetX(), tsp[2].GetY());

	// draw lines connecting transformed corner points with point 4 (and 5)
	drawContext.setPen(additionalLinesPen);
	drawContext.drawLine(tsp[0].GetX(), tsp[0].GetY(), tsp[3].GetX(), tsp[3].GetY());
	drawContext.drawLine(tsp[1].GetX(), tsp[1].GetY(), tsp[3].GetX(), tsp[3].GetY());
	drawContext.drawLine(tsp[2].GetX(), tsp[2].GetY(), tsp[3].GetX(), tsp[3].GetY());

	// draw full and empty circles representing control points
	drawContext.setPen(transformedSystemPen);
	for (int i = 0; i < 5; i++){
		if ((m_activeControlPoints & (1 << i)) != NO_POINT){
			drawContext.setBrush(transformedSystemPen.color());
		}
		else{
			drawContext.setBrush(Qt::NoBrush);
		}
		i2d::CVector2d currentPoint = tsp[i];
		drawContext.drawEllipse(currentPoint.GetX() - circleRadius, currentPoint.GetY() - circleRadius, circleRadius * 2, circleRadius * 2);
	}

	drawContext.restore();

}


bool CAffineTransformation2dShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CAffineTransformation2d*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


ITouchable::TouchState CAffineTransformation2dShape::IsTouched(istd::CIndex2d position) const
{
	const i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<const i2d::CAffineTransformation2d*>(GetModelPtr());
	if (IsDisplayConnected() && (transformationPtr != NULL)){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		i2d::CVector2d cp = transform.GetClientPosition(position);

		const double pointRadius = 2.5 * sqrt(sqrt(transform.GetDeformMatrix().GetApproxScale())); // pixels

		ControlPoints movablePoints(100, transformationPtr);
		for (int i = 0; i < 5; i++){
			if ((m_activeControlPoints & (1 << i)) == NO_POINT){
				continue;
			}

			if (movablePoints[i].GetDistance(cp) < pointRadius){
				return TS_DRAGGABLE;
			}
		}
	}

	return TS_NONE;
}


i2d::CRect CAffineTransformation2dShape::CalcBoundingBox() const
{
	I_ASSERT(IsDisplayConnected());

	i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetModelPtr());
	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	ControlPoints screenPoints = ControlPoints(100).ToScreen(transform);
	ControlPoints transformedScreenPoints = ControlPoints(100, transformationPtr).ToScreen(transform);

	i2d::CRect result(screenPoints[0].GetX(), screenPoints[0].GetY(), screenPoints[0].GetX(), screenPoints[0].GetY());

	for (int i = 2; i < 5; i++){
		result.Union(istd::CIndex2d(screenPoints[i].GetX(), screenPoints[i].GetY()));
	}

	for (int i = 0; i < 5; i++){
		result.Union(istd::CIndex2d(transformedScreenPoints[i].GetX(), transformedScreenPoints[i].GetY()));
	}

	double pointMaxRadius = ceil(5 * transform.GetDeformMatrix().GetApproxScale());

	result.SetLeft(result.GetLeft() - pointMaxRadius);
	result.SetRight(result.GetRight() + pointMaxRadius);
	result.SetTop(result.GetRight() - pointMaxRadius);
	result.SetBottom(result.GetRight() + pointMaxRadius);

	return result;
}


}

