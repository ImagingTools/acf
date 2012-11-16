// copied from CInteractiveCircleShape

#include "iview/COrientedCircleShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"

#include "i2d/COrientedCircle.h"

#include "iview/IColorShema.h"
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
	I_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetModelPtr();
	const i2d::COrientedCircle& circle = *dynamic_cast<const i2d::COrientedCircle*>(modelPtr);
	I_ASSERT(&circle != NULL);

	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	i2d::CVector2d screenCenter;
	const i2d::CVector2d& center = circle.GetPosition();
	transform.GetApply(center, screenCenter);

	double radius = circle.GetRadius();

	const i2d::CMatrix2d& deform = transform.GetDeformMatrix();
	i2d::CVector2d scale;
	deform.GetAxesLengths(scale);

	const IColorShema& colorSchema = GetColorShema();
	drawContext.save();

	if (IsSelected()){
		drawContext.setPen(colorSchema.GetPen(IColorShema::SP_SELECTED));
	}
	else{
		drawContext.setPen(colorSchema.GetPen(IColorShema::SP_NORMAL));
	}

	QBrush emptyBrush(QColor(0, 0, 0), Qt::NoBrush);
	drawContext.setBrush(emptyBrush);

	// draw direction indicator - uses QPointF to avoid rounding errors
	if (IsCenterVisible()){
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

		// draw inside or outside direction
	}

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

		colorSchema.DrawTicker(drawContext, ticker1, IColorShema::TT_NORMAL);
		colorSchema.DrawTicker(drawContext, ticker2, IColorShema::TT_NORMAL);
		colorSchema.DrawTicker(drawContext, ticker3, IColorShema::TT_NORMAL);
		colorSchema.DrawTicker(drawContext, ticker4, IColorShema::TT_NORMAL);
	}
}


bool COrientedCircleShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::COrientedCircle*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


} // namespace iview


