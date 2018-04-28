#include <iview/CAffineTransformation2dShape.h>


// ACF includes
#include <iview/IColorSchema.h>
#include <iview/CConsoleBase.h>

// Qt includes
#include <QtGui/QPainter>


namespace iview
{


static const double baseScale = 100;


CAffineTransformation2dShape::CAffineTransformation2dShape(void)
	:m_referencePosition(0.0, 0.0),
	m_activeControlPoints(ALL_POINTS),
	m_currentPoint(NO_POINT)
{
}


void CAffineTransformation2dShape::SetActiveControlPoints(ControlPoint points)
{
	m_activeControlPoints = points;

	Invalidate();
}


void CAffineTransformation2dShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetObservedModel());
	if (transformationPtr == NULL){
		return;
	}

	const IColorSchema& colorSchema = GetColorSchema();

	QPen baseSystemPen = colorSchema.GetPen(IColorSchema::SP_CYAN);
	QPen transformedSystemPen = colorSchema.GetPen(IColorSchema::SP_BLUE);
	QPen transformationOffsetsPen(QColor("brown"));
	transformationOffsetsPen.setStyle(Qt::DotLine);
	QPen additionalLinesPen = transformedSystemPen;
	additionalLinesPen.setStyle(Qt::DotLine);

	drawContext.save();

	const double circleRadius = 5;

	const ControlPoints& cp = GetControlPoints();
	ControlPoints sp;
	GetScreenPoints(cp, sp);

	// draw base coordinate system
	drawContext.setPen(baseSystemPen);
	drawContext.setBrush(baseSystemPen.color());
	drawContext.drawLine(sp[0].GetX(), sp[0].GetY(), sp[1].GetX(), sp[1].GetY());
	QPointF points[4];
	points[0] = sp[1];
	points[1] = sp[5];
	points[2] = sp[6];
	points[3] = sp[7];
	drawContext.drawPolygon(points, 4);

	drawContext.drawLine(sp[0].GetX(), sp[0].GetY(), sp[2].GetX(), sp[2].GetY());
	points[0] = sp[2];
	points[1] = sp[8];
	points[2] = sp[9];
	points[3] = sp[10];
	drawContext.drawPolygon(points, 4);

	if (transformationPtr == NULL){
		drawContext.restore();
		return;
	}

	// draw transformed coordinate system
	drawContext.setPen(transformedSystemPen);
	drawContext.setBrush(transformedSystemPen.color());

	ControlPoints tp;
	GetTransformedPoints(cp, tp, *transformationPtr);

	ControlPoints tsp;
	GetScreenPoints(tp, tsp);

	drawContext.drawLine(tsp[0].GetX(), tsp[0].GetY(), tsp[1].GetX(), tsp[1].GetY());
	points[0] = tsp[1];
	points[1] = tsp[5];
	points[2] = tsp[6];
	points[3] = tsp[7];
	drawContext.drawPolygon(points, 4);

	drawContext.drawLine(tsp[0].GetX(), tsp[0].GetY(), tsp[2].GetX(), tsp[2].GetY());
	points[0] = tsp[2];
	points[1] = tsp[8];
	points[2] = tsp[9];
	points[3] = tsp[10];
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
	for (int i = 0; i < ACTIVE_POINTS_COUNT; i++){
		i2d::CVector2d activePoint = tsp[i];
		istd::CIndex2d currentPoint(activePoint.GetX(), activePoint.GetY());
		ControlPoint pointType = (ControlPoint)(1 << i);
		if ((m_activeControlPoints & pointType) == 0){
			drawContext.setPen(transformedSystemPen);
			drawContext.setBrush(Qt::NoBrush);
			drawContext.drawEllipse(activePoint.GetX() - circleRadius, activePoint.GetY() - circleRadius, circleRadius* 2, circleRadius* 2);
			//			colorSchema.DrawTicker(drawContext, currentPoint, IColorSchema::TT_SELECTED_INACTIVE);
		}
		else if (pointType == POINT5){
			colorSchema.DrawTicker(drawContext, currentPoint, IColorSchema::TT_ROTATE);
		}
		else if (pointType == POINT1){
			colorSchema.DrawTicker(drawContext, currentPoint, IColorSchema::TT_MOVE);
		}
		else{
			colorSchema.DrawTicker(drawContext, currentPoint, IColorSchema::TT_NORMAL);
		}
	}

	drawContext.restore();

}


bool CAffineTransformation2dShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CAffineTransformation2d*>(modelPtr) != NULL);

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


ITouchable::TouchState CAffineTransformation2dShape::IsTouched(istd::CIndex2d position) const
{
	const i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<const i2d::CAffineTransformation2d*>(GetObservedModel());
	if (IsDisplayConnected() && (transformationPtr != NULL)){
		const iview::IColorSchema& colorSchema = GetColorSchema();
		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_NORMAL);

		ControlPoints screenPoints;
		GetTransformedPoints(GetControlPoints(), screenPoints, *transformationPtr);
		for (int i = 0; i < ACTIVE_POINTS_COUNT; i++){
			if (		((m_activeControlPoints & (1 << i)) != 0) &&
						tickerBox.IsInside(position - screenPoints[i].ToIndex2d())){
				return TS_DRAGGABLE;
			}
		}
	}

	return TS_NONE;
}


// protected methods

const CAffineTransformation2dShape::ControlPoints& CAffineTransformation2dShape::GetControlPoints()
{
	static ControlPoints result;
	static bool arePointsCalculated = false;

	if (!arePointsCalculated){
		// points delimiting the original (not transformed) coordinate system
		result[0] = i2d::CVector2d(0, 0);
		result[1] = i2d::CVector2d(baseScale, 0);
		result[2] = i2d::CVector2d(0, -baseScale);
		result[3] = result[1] + result[2] - result[0];
		result[4] = (result[3] + result[0]) / 2;

		double tipLength1 = baseScale / 20;
		double tipLength2 = 0.7 * tipLength1;
		double tipWidth = 0.3 * tipLength1;

		result[5] = i2d::CVector2d(result[1].GetX() - tipLength1, result[1].GetY() - tipWidth);
		result[6] = i2d::CVector2d(result[1].GetX() - tipLength2, result[1].GetY());
		result[7] = i2d::CVector2d(result[1].GetX() - tipLength1, result[1].GetY() + tipWidth);
		result[8] = i2d::CVector2d(result[2].GetX() - tipWidth, result[2].GetY() + tipLength1);
		result[9] = i2d::CVector2d(result[2].GetX(), result[2].GetY() + tipLength2);
		result[10] = i2d::CVector2d(result[2].GetX() + tipWidth, result[2].GetY() + tipLength1);

		arePointsCalculated = true;
	}

	return result;
}


void CAffineTransformation2dShape::GetScreenPoints(
			const ControlPoints& points,
			ControlPoints& result) const
{
	for (int i = 0; i < CONTROL_POINTS_COUNT; ++i){
		result[i] = GetScreenPosition(points[i]);
	}
}


void CAffineTransformation2dShape::GetTransformedPoints(
			const ControlPoints& points,
			ControlPoints& result,
			const i2d::CAffineTransformation2d& transformation)
{
	for (int i = 0; i < CONTROL_POINTS_COUNT; ++i){
		result[i] = transformation.GetValueAt(points[i]);
	}
}


i2d::CRect CAffineTransformation2dShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetObservedModel());
	if (transformationPtr == NULL){
		return i2d::CRect::GetEmpty();
	}

	const ControlPoints& cp = GetControlPoints();

	ControlPoints screenPoints;
	GetScreenPoints(cp, screenPoints);

	ControlPoints transformedPoints;
	GetTransformedPoints(cp, transformedPoints, *transformationPtr);

	ControlPoints transformedScreenPoints;
	GetScreenPoints(transformedPoints, transformedScreenPoints);

	i2d::CRect boundingBox(screenPoints[0].GetX(), screenPoints[0].GetY(), screenPoints[0].GetX(), screenPoints[0].GetY());

	for (int i = 2; i < ACTIVE_POINTS_COUNT; i++){
		boundingBox.Union(istd::CIndex2d(screenPoints[i].GetX(), screenPoints[i].GetY()));
	}

	for (int i = 0; i < ACTIVE_POINTS_COUNT; i++){
		boundingBox.Union(istd::CIndex2d(transformedScreenPoints[i].GetX(), transformedScreenPoints[i].GetY()));
	}

	const i2d::CAffine2d& viewTransform = GetViewToScreenTransform();
	double pointMaxRadius = ceil(5 * viewTransform.GetDeformMatrix().GetApproxScale());

	boundingBox.Expand(i2d::CRect(-pointMaxRadius, -pointMaxRadius, pointMaxRadius, pointMaxRadius));

	return boundingBox;
}


void CAffineTransformation2dShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	m_referencePosition = reference;

	const i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<const i2d::CAffineTransformation2d*>(GetObservedModel());
	if (transformationPtr == NULL){
		return;
	}

	// use model transformation to transform original points
	const ControlPoints& cp = GetControlPoints();
	ControlPoints points;
	GetTransformedPoints(cp, points, *transformationPtr);
	const double pointRadius = 2.5;

	m_currentPoint = NO_POINT;
	// check whether any of the points or its surrounding was clicked
	for (int i = 0; i < ACTIVE_POINTS_COUNT; i++){
		ControlPoint point = (ControlPoint)(1 << i);
		if (		((m_activeControlPoints & point) != 0) &&
					points[i].GetDistance(reference) < pointRadius){
			m_currentPoint = point;
		}
	}
}


void CAffineTransformation2dShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	if (!IsEditablePosition()){
		return;
	}

	i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetObservedModel());
	if (transformationPtr == NULL){
		return;
	}

	i2d::CVector2d offset = position - m_referencePosition;
	const ControlPoints& cp = GetControlPoints();
	ControlPoints points;
	GetTransformedPoints(cp, points, *transformationPtr);

	i2d::CVector2d translation = points[0];
	i2d::CVector2d relativeVec = points[1] - points[0];
	double angle = atan2(relativeVec.GetY(), relativeVec.GetX());
	i2d::CVector2d scale(points[1].GetDistance(points[0]) / baseScale, points[2].GetDistance(points[0]) / baseScale);

	switch (m_currentPoint){
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

			scale.SetX(scale.GetX()* newDiameter / oldDiameter);
			scale.SetY(scale.GetY()* newDiameter / oldDiameter);
		}
		break;

	case CAffineTransformation2dShape::POINT5:
		{
			relativeVec = points[4] - points[0];
			relativeVec += offset;

			double oldAngle = atan2(relativeVec.GetY(), relativeVec.GetX());
			double newAngle = atan2(relativeVec.GetY(), relativeVec.GetX());
			angle += newAngle - oldAngle;
		}
		break;

	default:
		break;
	}

	transformationPtr->Reset(translation, angle, scale);

	UpdateModelChanges();

	m_referencePosition = position;
}


} // namespace iview


