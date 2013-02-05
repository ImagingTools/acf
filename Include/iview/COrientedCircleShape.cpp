// copied from CInteractiveCircleShape

#include "iview/COrientedCircleShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"

#include "i2d/COrientedCircle.h"
#include <i2d/CPolylineExtractor.h>

#include "iview/IColorSchema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


COrientedCircleShape::COrientedCircleShape()
{
	m_isEditableRadius = true;
	m_isCenterVisible = true;
}


void COrientedCircleShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetModelPtr();
	const i2d::COrientedCircle& circle = *dynamic_cast<const i2d::COrientedCircle*>(modelPtr);
	Q_ASSERT(&circle != NULL);

	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	i2d::CVector2d screenCenter;
	const i2d::CVector2d& center = circle.GetPosition();
	transform.GetApply(center, screenCenter);

	double radius = circle.GetRadius();

	const i2d::CMatrix2d& deform = transform.GetDeformMatrix();
	i2d::CVector2d scale;
	deform.GetAxesLengths(scale);

	const IColorSchema& colorSchema = GetColorSchema();
	drawContext.save();

	if (IsSelected()){
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
	}
	else{
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
	}

	QBrush emptyBrush(QColor(0, 0, 0), Qt::NoBrush);
	drawContext.setBrush(emptyBrush);

	if (IsCenterVisible()){
#if USE_Z_AXIS_DIRECTION_INDICATOR
		// draw direction indicator according to the right hand rule
		// uses QPointF to avoid rounding errors

		double dirIndicatorRadius = sqrt(radius);
		double dirIndicatorScale = sqrt((scale.GetX() + scale.GetY()) / 2);

		double left = screenCenter.GetX() - dirIndicatorRadius * dirIndicatorScale;
		double right = screenCenter.GetX() + dirIndicatorRadius * dirIndicatorScale;
		double top = screenCenter.GetY() - dirIndicatorRadius * dirIndicatorScale;
		double bottom = screenCenter.GetY() + dirIndicatorRadius * dirIndicatorScale;

		drawContext.drawEllipse(QRectF(QPointF(left, top), QPointF(right, bottom)));

		if (circle.IsOrientedOutside()){
			// draw direction outside SCREEN (inside camera)
			QPen oldPen = drawContext.pen();
			QPen centerDotPen(drawContext.pen());
			centerDotPen.setWidthF(std::max(3.0, dirIndicatorScale));
			centerDotPen.setCapStyle(Qt::RoundCap);
			drawContext.setPen(centerDotPen);
			drawContext.drawPoint(screenCenter);
			drawContext.setPen(oldPen);
		}
		else{
			// move all points towards center
			double crossScale = dirIndicatorScale / sqrt(2.0);
			left = screenCenter.GetX() - dirIndicatorRadius * crossScale;
			right = screenCenter.GetX() + dirIndicatorRadius * crossScale;
			top = screenCenter.GetY() - dirIndicatorRadius * crossScale;
			bottom = screenCenter.GetY() + dirIndicatorRadius * crossScale;

			drawContext.drawLine(QPointF(left, top), QPointF(right, bottom));
			drawContext.drawLine(QPointF(right, top), QPointF(left, bottom));
		}
#else	
		BaseClass::Draw(drawContext);
#endif
	}

	// draw inside or outside direction
	drawContext.drawEllipse(QRect(
			QPoint(int(screenCenter.GetX() - radius * scale.GetX()),
			int(screenCenter.GetY() - radius * scale.GetY())),
			QPoint(int(screenCenter.GetX() + radius * scale.GetX()),
			int(screenCenter.GetY() + radius * scale.GetY()))));
	drawContext.restore();

	if (IsEditableRadius() && IsSelected()){
		istd::CIndex2d ticker1 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()));
		istd::CIndex2d ticker2 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()));
		istd::CIndex2d ticker3 = transform.GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius));
		istd::CIndex2d ticker4 = transform.GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius));

		colorSchema.DrawTicker(drawContext, ticker1, IColorSchema::TT_NORMAL);
		colorSchema.DrawTicker(drawContext, ticker2, IColorSchema::TT_NORMAL);
		colorSchema.DrawTicker(drawContext, ticker3, IColorSchema::TT_NORMAL);
		colorSchema.DrawTicker(drawContext, ticker4, IColorSchema::TT_NORMAL);
	}

	// draw orientation markers as in a polyline
	const QPen& darkPen = colorSchema.GetPen(IColorSchema::SP_ORIENT_DARK);
	const QPen& brightPen = colorSchema.GetPen(IColorSchema::SP_ORIENT_BRIGHT);

	// reduce line opacity for the pens; the border is only used to increase visibility on black/white backgrounds
	QColor brightColor = brightPen.color();
	QBrush brightBrush(brightColor);
	brightColor.setAlphaF(0.25);
	QPen softBrightPen(brightColor);

	QColor darkColor = darkPen.color();
	QBrush darkBrush(darkColor);
	darkColor.setAlphaF(0.25);
	QPen softDarkPen(darkColor);

	i2d::CPolylineExtractor polylineExtractor;
	const int nodesCount = 8;
	i2d::CPolyline polyline = polylineExtractor.CreatePolyline(circle, nodesCount, !circle.IsOrientedOutside(), false);
	i2d::CVector2d firstPoint = transform.GetApply(polyline.GetNode(nodesCount - 1));
	i2d::CLine2d segmentLine;
	segmentLine.SetPoint2(firstPoint);

	for (int pointIndex = 0; pointIndex < nodesCount; ++pointIndex){
		segmentLine.PushEndPoint(transform.GetApply(polyline.GetNode(pointIndex)));
		DrawOrientationMarker(drawContext, softBrightPen, darkBrush, softDarkPen, brightBrush, segmentLine, transform.GetDeformMatrix().GetApproxScale());
	}
}


bool COrientedCircleShape::OnAttached(imod::IModel* modelPtr)
{
	Q_ASSERT(dynamic_cast<i2d::COrientedCircle*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


i2d::CRect COrientedCircleShape::CalcBoundingBox() const
{
	i2d::CRect result = BaseClass::CalcBoundingBox();
	const iview::CScreenTransform& transform = GetLogToScreenTransform();
	const i2d::CMatrix2d& deform = transform.GetDeformMatrix();
	double directionIndicatorWidth = 10 * deform.GetApproxScale();
	result.SetLeft(result.GetLeft() - directionIndicatorWidth);
	result.SetRight(result.GetRight() + directionIndicatorWidth);
	result.SetTop(result.GetTop() - directionIndicatorWidth);
	result.SetBottom(result.GetBottom() + directionIndicatorWidth);

	return result;
}

} // namespace iview


