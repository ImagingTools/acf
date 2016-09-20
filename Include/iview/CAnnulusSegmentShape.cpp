#include <iview/CAnnulusSegmentShape.h>


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <imod/IModel.h>
#include <i2d/CAnnulusSegment.h>
#include <iqt/iqt.h>
#include <iview/IColorSchema.h>
#include <iview/CScreenTransform.h>


namespace iview
{


CAnnulusSegmentShape::CAnnulusSegmentShape()
{
	m_editableAngle = true;
}


void CAnnulusSegmentShape::SetEditableAngles(bool editable)
{
	if (m_editableAngle != editable){
		m_editableAngle = editable;
		Invalidate();
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CAnnulusSegmentShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	ShapeBaseClass::OnMouseButton(position, buttonType, downFlag);

	const i2d::CAnnulusSegment* objectPtr = dynamic_cast<const i2d::CAnnulusSegment*>(GetObservedModel());
	if (objectPtr != NULL){
		if (downFlag && IsEditableRadius()){
			const IColorSchema& colorSchema = GetColorSchema();
			i2d::CVector2d center = objectPtr->GetPosition();
			double radius = objectPtr->GetInnerRadius();
			double radius2 = objectPtr->GetOuterRadius();

			i2d::CVector2d tickerDirection;
			tickerDirection.Init((objectPtr->GetBeginAngle() + objectPtr->GetEndAngle()) * 0.5);

			istd::CIndex2d ticker1 = GetScreenPosition(center + tickerDirection * radius).ToIndex2d();
			istd::CIndex2d ticker2 = GetScreenPosition(center + tickerDirection * radius2).ToIndex2d();

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

			if (tickerBox.IsInside(position - ticker1)){
				m_editMode = EM_INNER_RADIUS;

				BeginTickerDrag();

				return true;
			}

			if (tickerBox.IsInside(position - ticker2)){
				m_editMode = EM_OUTER_RADIUS;

				BeginTickerDrag();

				return true;
			}
		}

		if (downFlag && IsEditableAngles()){
			const IColorSchema& colorSchema = GetColorSchema();
			i2d::CVector2d center = objectPtr->GetPosition();
			double radius = objectPtr->GetInnerRadius();
			double radius2 = objectPtr->GetOuterRadius();
			double middleRadius = (radius2 + radius) * 0.5;
			i2d::CVector2d delta1;
			delta1.Init(objectPtr->GetBeginAngle(), middleRadius);
			i2d::CVector2d delta2;
			delta2.Init(objectPtr->GetEndAngle(), middleRadius);

			istd::CIndex2d ticker3 = GetScreenPosition(center + delta1).ToIndex2d();
			istd::CIndex2d ticker4 = GetScreenPosition(center + delta2).ToIndex2d();

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

			if (tickerBox.IsInside(position - ticker3)){
				m_editMode = EM_ANGLE1;

				BeginTickerDrag();

				return true;
			}

			if (tickerBox.IsInside(position - ticker4)){
				m_editMode = EM_ANGLE2;

				BeginTickerDrag();

				return true;
			}
		}

		m_editMode = EM_NONE;
	}

	return CPinShape::OnMouseButton(position, buttonType, downFlag);
}


bool CAnnulusSegmentShape::OnMouseMove(istd::CIndex2d position)
{
	if (m_editMode == EM_NONE){
		return CPinShape::OnMouseMove(position);
	}

	imod::IModel* modelPtr = GetObservedModel();
	i2d::CAnnulusSegment* objectPtr = dynamic_cast<i2d::CAnnulusSegment*>(modelPtr);
	Q_ASSERT(objectPtr != NULL);

	i2d::CVector2d cp = GetLogPosition(position);

	const i2d::CVector2d& center = objectPtr->GetPosition();

	switch (m_editMode){
	case EM_INNER_RADIUS:
		objectPtr->SetInnerRadius(center.GetDistance(cp));

		UpdateModelChanges();

		return true;

	case EM_OUTER_RADIUS:
		objectPtr->SetOuterRadius(center.GetDistance(cp));

		UpdateModelChanges();

		return true;

	case EM_ANGLE1:
		{
			i2d::CVector2d delta = cp - center;
			double beginAngle = qAtan2(delta.GetY(), delta.GetX());
			double endAngle = objectPtr->GetEndAngle();
			if (beginAngle > endAngle){
				endAngle += 2 * I_PI;
			}
			else if (beginAngle + 2 * I_PI < endAngle){
				endAngle -= 2 * I_PI;
			}
			objectPtr->SetBeginAngle(beginAngle);
			objectPtr->SetEndAngle(endAngle);

			UpdateModelChanges();
		}

		return true;

	case EM_ANGLE2:
		{
			i2d::CVector2d delta = cp - center;
			double beginAngle = objectPtr->GetBeginAngle();
			double endAngle = qAtan2(delta.GetY(), delta.GetX());
			if (beginAngle > endAngle){
				beginAngle -= 2 * I_PI;
			}
			else if (beginAngle + 2 * I_PI < endAngle){
				beginAngle += 2 * I_PI;
			}
			objectPtr->SetBeginAngle(beginAngle);
			objectPtr->SetEndAngle(endAngle);

			UpdateModelChanges();
		}

		return true;

	default:
		return false;
	}
}


// reimplemented (iview::IVisualizable)

void CAnnulusSegmentShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetObservedModel();
	Q_ASSERT(modelPtr != NULL);

	const i2d::CAnnulusSegment* objectPtr = dynamic_cast<const i2d::CAnnulusSegment*>(modelPtr);
	Q_ASSERT(objectPtr != NULL);

	const IColorSchema& colorSchema = GetColorSchema();
	const i2d::CVector2d& center = objectPtr->GetPosition();
	i2d::CVector2d screenCenter = GetScreenPosition(center);

	double radius = objectPtr->GetInnerRadius();
	double screenRadius = GetScreenPosition(center + i2d::CVector2d(-radius, 0)).GetDistance(GetScreenPosition(center + i2d::CVector2d(radius, 0))) * 0.5;
	double radius2 = objectPtr->GetOuterRadius();
	double screenRadius2 = GetScreenPosition(center + i2d::CVector2d(-radius2, 0)).GetDistance(GetScreenPosition(center + i2d::CVector2d(radius2, 0))) * 0.5;

	if (IsCenterVisible()){
		CPinShape::Draw(drawContext);
	}

	if (IsSelected()){
		drawContext.save();
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
		drawContext.setBrush(colorSchema.GetBrush(IColorSchema::SB_HALF_TRANSPARENT));
	}
	else{
		drawContext.save();
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
		drawContext.setBrush(colorSchema.GetBrush(IColorSchema::SB_HALF_TRANSPARENT2));
	}

	DrawArea(	drawContext,
				center,
				screenCenter,
				qMin(radius,radius2),
				qMax(radius,radius2),
				int(qMin(screenRadius, screenRadius2)),
				int(qMax(screenRadius, screenRadius2)),
				objectPtr->GetBeginAngle(),
				objectPtr->GetEndAngle());

	drawContext.restore();

	if (m_isEditableRadius && IsSelected()){
		i2d::CVector2d tickerDirection;
		tickerDirection.Init((objectPtr->GetBeginAngle() + objectPtr->GetEndAngle()) * 0.5);

		istd::CIndex2d ticker1 = GetScreenPosition(center + tickerDirection * radius).ToIndex2d();
		istd::CIndex2d ticker2 = GetScreenPosition(center + tickerDirection * radius2).ToIndex2d();

		colorSchema.DrawTicker(drawContext, ticker1, IColorSchema::TT_NORMAL);
		colorSchema.DrawTicker(drawContext, ticker2, IColorSchema::TT_NORMAL);
	}

	if (m_editableAngle && IsSelected()){
		double middleRadius = (radius2 + radius) * 0.5;
		i2d::CVector2d delta1;
		delta1.Init(objectPtr->GetBeginAngle(), middleRadius);
		i2d::CVector2d delta2;
		delta2.Init(objectPtr->GetEndAngle(), middleRadius);

		istd::CIndex2d ticker3 = GetScreenPosition(center + delta1).ToIndex2d();
		istd::CIndex2d ticker4 = GetScreenPosition(center + delta2).ToIndex2d();

		colorSchema.DrawTicker(drawContext, ticker3, IColorSchema::TT_CHECKBOX_ON);
		colorSchema.DrawTicker(drawContext, ticker4, IColorSchema::TT_CHECKBOX_ON);
	}
}


// reimplemented (imod::IObserver)

bool CAnnulusSegmentShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CAnnulusSegment*>(modelPtr) != NULL);

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CAnnulusSegmentShape::IsTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetObservedModel();
	const i2d::CAnnulusSegment* objectPtr = dynamic_cast<const i2d::CAnnulusSegment*>(modelPtr);
	Q_ASSERT(objectPtr != NULL);

	const IColorSchema& colorSchema = GetColorSchema();

	double viewScale = GetViewToScreenTransform().GetDeformMatrix().GetApproxScale();

	i2d::CVector2d cp = GetLogPosition(position);

	i2d::CVector2d center = objectPtr->GetPosition();
	i2d::CVector2d cpToCenterVector = cp - center;
	double distance = cpToCenterVector.GetLength();

	double radius = objectPtr->GetInnerRadius();
	double radius2 = objectPtr->GetOuterRadius();
	double beginAngle = objectPtr->GetBeginAngle();
	double endAngle = objectPtr->GetEndAngle();
	double middleRadius = (radius2 - radius)/2 + radius;

	if (IsSelected() && IsEditableRadius()){
		i2d::CVector2d tickerDirection;
		tickerDirection.Init((beginAngle + endAngle) * 0.5);

		istd::CIndex2d ticker1 = GetScreenPosition(center + tickerDirection * radius).ToIndex2d();
		istd::CIndex2d ticker2 = GetScreenPosition(center + tickerDirection * radius2).ToIndex2d();

		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_NORMAL);

		if (		tickerBox.IsInside(position - ticker1) ||
					tickerBox.IsInside(position - ticker2)){
			return IInteractiveShape::TS_TICKER;
		}
	}

	if (IsSelected() && IsEditableAngles()){
		i2d::CVector2d delta1;
		delta1.Init(beginAngle, middleRadius);
		i2d::CVector2d delta2;
		delta2.Init(endAngle, middleRadius);

		istd::CIndex2d ticker3 = GetScreenPosition(center + delta1).ToIndex2d();
		istd::CIndex2d ticker4 = GetScreenPosition(center + delta2).ToIndex2d();

		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_NORMAL);

		if (		tickerBox.IsInside(position - ticker3) ||
					tickerBox.IsInside(position - ticker4)){
			return IInteractiveShape::TS_TICKER;
		}
	}

	double viewportLineWidth = colorSchema.GetLogicalLineWidth();
	double angle = cpToCenterVector.GetAngle();
	if (angle < beginAngle){
		angle += 2 * I_PI;
	}
	if (angle > endAngle){
		angle -= 2 * I_PI;
	}

	if (		(viewScale * qAbs(radius - distance) < viewportLineWidth) ||
				(viewScale * qAbs(radius2 - distance) < viewportLineWidth)){
		if ((angle >= beginAngle) && (angle <= endAngle)){
			bool isEditablePosition = IsEditablePosition();

			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
	}

	if (		(viewScale * middleRadius * qAbs(beginAngle - angle) < viewportLineWidth) ||
				(viewScale * middleRadius * qAbs(endAngle - angle) < viewportLineWidth)){
		if ((distance >= radius) && (distance <= radius2)){
			bool isEditablePosition = IsEditablePosition();

			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
	}

	if (IsCenterVisible()){
		return CPinShape::IsTouched(position);
	}

	return TS_NONE;
}


// protected methods

void CAnnulusSegmentShape::DrawArea(
			QPainter& painter,
			i2d::CVector2d realCenter,
			i2d::CVector2d center,
			double realMinRadius,
			double realMaxRadius,
			int minRadius,
			int maxRadius,
			double startAngle,
			double stopAngle,
			bool /*fillFlag*/) const
{
	i2d::CRect maxBox(center.GetX() - maxRadius, center.GetY() - maxRadius, center.GetX() + maxRadius + 1, center.GetY() + maxRadius + 1);
	i2d::CRect minBox(center.GetX() - minRadius, center.GetY() - minRadius, center.GetX() + minRadius + 1, center.GetY() + minRadius + 1);

	if (startAngle > stopAngle){
		startAngle = startAngle - 2 * I_PI;
	}

	QRectF minRect = iqt::GetQRect(minBox);
	QRectF maxRect = iqt::GetQRect(maxBox);

	i2d::CVector2d deltaStartAngle;
	deltaStartAngle.Init(startAngle, realMinRadius);
	QPointF startScreenPos = GetScreenPosition(realCenter + deltaStartAngle);
	
	i2d::CVector2d temp;
	temp.Init(startAngle);
	double startAngleDeg = GetDegreeAndleOfPoint(center, GetScreenPosition(realCenter + temp));

	i2d::CVector2d deltaEndAngle;
	deltaEndAngle.Init(stopAngle, realMaxRadius);
	
	temp.Init(stopAngle);
	double stopAngleDeg = GetDegreeAndleOfPoint(center, GetScreenPosition(realCenter + temp));

	qreal sweepLength = startAngleDeg - stopAngleDeg;
	if (sweepLength <= 0)
		sweepLength += 360;

	// debug only
	//painter.drawEllipse(startScreenPos, 10, 10);
	//QPointF stopScreenPos = GetScreenPosition(realCenter + deltaEndAngle);
	//painter.drawEllipse(stopScreenPos, 10, 10);

	QPainterPath painterPath;
	painterPath.moveTo(startScreenPos);
	painterPath.arcTo(maxRect, startAngleDeg, -sweepLength);
	painterPath.arcTo(minRect, stopAngleDeg, sweepLength);
	painterPath.closeSubpath();

	painter.drawPath(painterPath);
}


double CAnnulusSegmentShape::GetDegreeAndleOfPoint(const i2d::CVector2d& center, const QPointF& point) const
{
	// check sector
	if (point.x() <= center.GetX() && point.y() <= center.GetY()){
		// sector 2
		QPointF fallPos(point.x(), center.GetY());
		double oppositeLength = fallPos.y() - point.y();
		double adjancedLength = center.GetX() - fallPos.x();
		double tangent = adjancedLength ? oppositeLength / adjancedLength : 0;
		double arcTan = atan(tangent);
		return 180 - arcTan / I_PI * 180;
	}
	else if (point.x() <= center.GetX() && point.y() >= center.GetY()){
		// sector 3
		QPointF fallPos(point.x(), center.GetY());
		double oppositeLength = point.y() - fallPos.y();
		double adjancedLength = center.GetX() - fallPos.x();
		double tangent = adjancedLength ? oppositeLength / adjancedLength : 0;
		double arcTan = atan(tangent);
		return 180 + arcTan / I_PI * 180;	
	}

	// sector 1 or 4
	QPointF fallPos(point.x(), center.GetY());
	double oppositeLength = fallPos.y() - point.y();
	double adjancedLength = fallPos.x() - center.GetX();
	double tangent = adjancedLength ? oppositeLength / adjancedLength : 0;
	double arcTan = atan(tangent);
	return arcTan / I_PI * 180;
}


} // namespace iview


