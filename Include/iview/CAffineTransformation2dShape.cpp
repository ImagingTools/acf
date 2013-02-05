#include "CAffineTransformation2dShape.h"

// Acf includes
#include <i2d/CAffineTransformation2d.h>
#include <iview/IColorSchema.h>
#include <iview/CConsoleBase.h>

// Qt includes
#include <QtGui/QPainter>


namespace iview
{


namespace
{

const double baseScale = 100;


/** 
	Structure representing a set of 5 control points of the shape 
 
	Arrow tip points are also generated.
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
		points[0] = i2d::CVector2d(0, 0);
		points[1] = i2d::CVector2d(size, 0);
		points[2] = i2d::CVector2d(0, -size);
		points[3] = points[1] + points[2] - points[0];
		points[4] = (points[3] + points[0]) / 2;

		double xscale = 1;
		double yscale = 1;
		if (transformationPtr != NULL){
			i2d::CVector2d axesLengths = transformationPtr->GetTransformation().GetDeformMatrix().GetAxesLengths();
			if (axesLengths.GetX() != 0){
				xscale = 1 / axesLengths.GetX();
			}
			if (axesLengths.GetY() != 0){
				yscale = 1 / axesLengths.GetY();
			}
		}

		double tipLength1 = baseScale / 20;
		double tipLength2 = 0.7 * tipLength1;
		double tipWidth = 0.3 * tipLength1;

		points[5] = i2d::CVector2d(points[1].GetX() - tipLength1 * xscale, points[1].GetY() - tipWidth * yscale);
		points[6] = i2d::CVector2d(points[1].GetX() - tipLength2 * xscale, points[1].GetY() * yscale);
		points[7] = i2d::CVector2d(points[1].GetX() - tipLength1 * xscale, points[1].GetY() + tipWidth * yscale);
		points[8] = i2d::CVector2d(points[2].GetX() - tipWidth * xscale, points[2].GetY() + tipLength1 * yscale);
		points[9] = i2d::CVector2d(points[2].GetX(), points[2].GetY() + tipLength2 * yscale);
		points[10] = i2d::CVector2d(points[2].GetX() + tipWidth * xscale, points[2].GetY() + tipLength1 * yscale);

		if (transformationPtr != NULL){
			for (int i = 0; i < 11; i++){
				points[i] = transformationPtr->GetValueAt(points[i]);
			}
		}

	}


	/** 
		Transform control points from logical to screen coordinates
		
		\param transform LogToScreenTransform assigned to the view
	 */
	ControlPoints ToScreen(const iview::CScreenTransform & transform)
	{
		ControlPoints result(baseScale);

		for (int i = 0; i < 11; i++){
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
		MovePoint(CAffineTransformation2dShape::NO_POINT, i2d::CVector2d(0, 0), transformationPtr);
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


	/**
		Modify all points to follow a single point movement and store the result 
		in a transformation object.
 
		\param point
			\value POINT1 translate
			\value POINT2 scale in X axis
			\value POINT3 scale in Y axis
			\value POINT4 scale in both axes
			\value POINT5 rotate around POINT1 position
		\param offset movement relative to the current point position
		\param transformationPtr pointer to a transformation object to store changes
	 */
	void MovePoint(int point, i2d::CVector2d offset, i2d::CAffineTransformation2d * transformationPtr)
	{
		i2d::CVector2d translation = points[0];
		i2d::CVector2d relativeVec = points[1] - points[0];
		double angle = atan2(relativeVec.GetY(), relativeVec.GetX());
		i2d::CVector2d scale(points[1].GetDistance(points[0]) / baseScale, points[2].GetDistance(points[0]) / baseScale);

		switch (point){
			case CAffineTransformation2dShape::POINT1:
				translation += offset;
				break;
			case CAffineTransformation2dShape::POINT2:
			{
				i2d::CVector2d newPoint = points[1] + offset;
				scale.SetX(newPoint.GetDistance(points[0]) / baseScale);
			}
				break;
			case CAffineTransformation2dShape::POINT3:
			{
				i2d::CVector2d newPoint = points[2] + offset;
				scale.SetY(newPoint.GetDistance(points[0]) / baseScale);
			}
				break;
			case CAffineTransformation2dShape::POINT4:
			{
				double oldDiameter = points[3].GetDistance(points[0]);
				i2d::CVector2d newPoint = points[3] + offset;
				double newDiameter = newPoint.GetDistance(points[0]);

				scale.SetX(scale.GetX() * newDiameter / oldDiameter);
				scale.SetY(scale.GetY() * newDiameter / oldDiameter);
			}
				break;
			case CAffineTransformation2dShape::POINT5:
			{
				i2d::CVector2d relativeVec = points[4] - points[0];
				double oldAngle = atan2(relativeVec.GetY(), relativeVec.GetX());
				relativeVec += offset;
				double newAngle = atan2(relativeVec.GetY(), relativeVec.GetX());
				angle += newAngle - oldAngle;
			}
				break;
			default:
				return;
		}

		transformationPtr->Reset(translation, angle, scale);
	}

private:
	i2d::CVector2d points[11];
}; // struct CControlPoints


} // namespace


CAffineTransformation2dShape::CAffineTransformation2dShape(void)
: m_activeControlPoints(ALL_POINTS), m_currentPoint(NO_POINT)
{
}


void CAffineTransformation2dShape::SetActiveControlPoints(ControlPoint points)
{
	m_activeControlPoints = points;
	Invalidate(CS_CONSOLE);
}


void CAffineTransformation2dShape::Draw(QPainter & drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const IColorSchema& schema = GetColorSchema();

	QPen baseSystemPen = schema.GetPen(IColorSchema::SP_CYAN);
	QPen transformedSystemPen = schema.GetPen(IColorSchema::SP_BLUE);
	QPen transformationOffsetsPen(QColor("brown"));
	transformationOffsetsPen.setStyle(Qt::DotLine);
	QPen additionalLinesPen = transformedSystemPen;
	additionalLinesPen.setStyle(Qt::DotLine);

	i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetModelPtr());

	drawContext.save();

	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	const double circleRadius = 5;

	ControlPoints cp(baseScale);
	ControlPoints sp = cp.ToScreen(transform);

	// draw base coordinate system
	drawContext.setPen(baseSystemPen);
	drawContext.setBrush(baseSystemPen.color());
	drawContext.drawLine(sp[0].GetX(), sp[0].GetY(), sp[1].GetX(), sp[1].GetY());
	QPointF points[4];
	points[0] = QPointF(sp[1].GetX(), sp[1].GetY());
	points[1] = QPointF(sp[5].GetX(), sp[5].GetY());
	points[2] = QPointF(sp[6].GetX(), sp[6].GetY());
	points[3] = QPointF(sp[7].GetX(), sp[7].GetY());
	drawContext.drawPolygon(points, 4);

	drawContext.drawLine(sp[0].GetX(), sp[0].GetY(), sp[2].GetX(), sp[2].GetY());
	points[0] = QPointF(sp[2].GetX(), sp[2].GetY());
	points[1] = QPointF(sp[8].GetX(), sp[8].GetY());
	points[2] = QPointF(sp[9].GetX(), sp[9].GetY());
	points[3] = QPointF(sp[10].GetX(), sp[10].GetY());
	drawContext.drawPolygon(points, 4);

	if (transformationPtr == NULL){
		drawContext.restore();
		return;
	}

	// draw transformed coordinate system
	drawContext.setPen(transformedSystemPen);
	drawContext.setBrush(transformedSystemPen.color());
	cp = ControlPoints(baseScale, transformationPtr);
	ControlPoints tsp = cp.ToScreen(transform);
	drawContext.drawLine(tsp[0].GetX(), tsp[0].GetY(), tsp[1].GetX(), tsp[1].GetY());
	points[0] = QPointF(tsp[1].GetX(), tsp[1].GetY());
	points[1] = QPointF(tsp[5].GetX(), tsp[5].GetY());
	points[2] = QPointF(tsp[6].GetX(), tsp[6].GetY());
	points[3] = QPointF(tsp[7].GetX(), tsp[7].GetY());
	drawContext.drawPolygon(points, 4);

	drawContext.drawLine(tsp[0].GetX(), tsp[0].GetY(), tsp[2].GetX(), tsp[2].GetY());
	points[0] = QPointF(tsp[2].GetX(), tsp[2].GetY());
	points[1] = QPointF(tsp[8].GetX(), tsp[8].GetY());
	points[2] = QPointF(tsp[9].GetX(), tsp[9].GetY());
	points[3] = QPointF(tsp[10].GetX(), tsp[10].GetY());
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
	for (int i = 0; i < 5; i++){
		i2d::CVector2d cp = tsp[i];
		istd::CIndex2d currentPoint(cp.GetX(), cp.GetY());
		ControlPoint pointType = (ControlPoint)(1 << i);
		if ((m_activeControlPoints & pointType) == NO_POINT){
			drawContext.setPen(transformedSystemPen);
			drawContext.setBrush(Qt::NoBrush);
			drawContext.drawEllipse(cp.GetX() - circleRadius, cp.GetY() - circleRadius, circleRadius * 2, circleRadius * 2);
			//			schema.DrawTicker(drawContext, currentPoint, IColorSchema::TT_SELECTED_INACTIVE);
		}
		else if (pointType == POINT5){
			schema.DrawTicker(drawContext, currentPoint, IColorSchema::TT_ROTATE);
		}
		else if (pointType == POINT1){
			schema.DrawTicker(drawContext, currentPoint, IColorSchema::TT_MOVE);
		}
		else{
			schema.DrawTicker(drawContext, currentPoint, IColorSchema::TT_NORMAL);
		}
	}

	drawContext.restore();

}


bool CAffineTransformation2dShape::OnAttached(imod::IModel * modelPtr)
{
	Q_ASSERT(dynamic_cast<i2d::CAffineTransformation2d*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


ITouchable::TouchState CAffineTransformation2dShape::IsTouched(istd::CIndex2d position) const
{
	const i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<const i2d::CAffineTransformation2d*>(GetModelPtr());
	if (IsDisplayConnected() && (transformationPtr != NULL)){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		i2d::CVector2d cp = transform.GetClientPosition(position);

		const double pointRadius = 2.5 * sqrt(sqrt(transform.GetDeformMatrix().GetApproxScale())); // pixels

		ControlPoints movablePoints(baseScale, transformationPtr);
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
	Q_ASSERT(IsDisplayConnected());

	i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetModelPtr());
	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	ControlPoints screenPoints = ControlPoints(baseScale).ToScreen(transform);
	ControlPoints transformedScreenPoints = ControlPoints(baseScale, transformationPtr).ToScreen(transform);

	i2d::CRect boundingBox(screenPoints[0].GetX(), screenPoints[0].GetY(), screenPoints[0].GetX(), screenPoints[0].GetY());

	for (int i = 2; i < 5; i++){
		boundingBox.Union(istd::CIndex2d(screenPoints[i].GetX(), screenPoints[i].GetY()));
	}

	for (int i = 0; i < 5; i++){
		boundingBox.Union(istd::CIndex2d(transformedScreenPoints[i].GetX(), transformedScreenPoints[i].GetY()));
	}

	double pointMaxRadius = ceil(5 * transform.GetDeformMatrix().GetApproxScale());

	// expand bounding box to include control point circles
	boundingBox.SetLeft(boundingBox.GetLeft() - pointMaxRadius);
	boundingBox.SetRight(boundingBox.GetRight() + pointMaxRadius);
	boundingBox.SetTop(boundingBox.GetTop() - pointMaxRadius);
	boundingBox.SetBottom(boundingBox.GetBottom() + pointMaxRadius);

	return boundingBox;
}


void CAffineTransformation2dShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	m_referencePosition = reference;

	const i2d::CAffineTransformation2d* transformationPtr =
			dynamic_cast<const i2d::CAffineTransformation2d*>(GetModelPtr());
	if (transformationPtr == NULL){
		return;
	}

	// use model transformation to transform original points
	ControlPoints points(baseScale, transformationPtr);
	const double pointRadius = 2.5;

	m_currentPoint = NO_POINT;
	// check whether any of the points or its surrounding was clicked
	for (int i = 0; i < 5; i++){
		ControlPoint point = (ControlPoint)(1 << i);
		if ((m_activeControlPoints & point) != NO_POINT
				&& points[i].GetDistance(reference) < pointRadius){
			m_currentPoint = point;
		}
	}
}


void CAffineTransformation2dShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	if (!IsEditablePosition()){
		return;
	}

	i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetModelPtr());
	if (transformationPtr == NULL){
		return;
	}

	BeginModelChanges();

	i2d::CVector2d offset = position - m_referencePosition;
	ControlPoints points(baseScale, transformationPtr);

	switch (m_currentPoint){
		case POINT1:
		case POINT2:
		case POINT3:
		case POINT4:
		case POINT5:
			points.MovePoint(m_currentPoint, offset, transformationPtr);
			break;
		default:
			break;
	}

	EndModelChanges();

	m_referencePosition = position;
}


} // namespace iview

