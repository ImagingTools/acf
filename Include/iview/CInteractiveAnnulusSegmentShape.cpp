#include "iview/CInteractiveAnnulusSegmentShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"

#include "i2d/CAnnulusSegment.h"

#include "iqt/iqt.h"

#include "iview/IColorShema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CInteractiveAnnulusSegmentShape::CInteractiveAnnulusSegmentShape()
{
	m_editableAngle = true;

	m_editRadius2Mode = false;
	m_editRadiusMode = false;
	m_editAngle1Mode = false;
	m_editAngle2Mode = false;
}


void CInteractiveAnnulusSegmentShape::SetEditableAngles(bool editable)
{
	if (m_editableAngle != editable){
		m_editableAngle = editable;
		Invalidate(CS_CONSOLE);
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractiveAnnulusSegmentShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CAnnulusSegment* objectPtr = dynamic_cast<const i2d::CAnnulusSegment*>(GetModelPtr());
	if (objectPtr != NULL){
		if (downFlag && IsEditableRadius()){
			const IColorShema& colorShema = GetColorShema();
			i2d::CVector2d center = objectPtr->GetPosition();
			double radius = objectPtr->GetInnerRadius();
			double radius2 = objectPtr->GetOuterRadius();

			i2d::CVector2d tickerDirection;
			tickerDirection.Init((objectPtr->GetBeginAngle() + objectPtr->GetEndAngle()) * 0.5);

			const i2d::ICalibration2d* calibrationPtr = objectPtr->GetCalibration();

			istd::CIndex2d ticker1 = GetScreenPosition(center + tickerDirection * radius, calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker2 = GetScreenPosition(center + tickerDirection * radius2, calibrationPtr).ToIndex2d();

			const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);

			if (tickerBox.IsInside(position - ticker1)){
				m_editRadiusMode = true;

				BeginModelChanges();

				return true;
			}

			if (tickerBox.IsInside(position - ticker2)){
				m_editRadius2Mode = true;

				BeginModelChanges();

				return true;
			}
		}

		m_editRadiusMode = false;
		m_editRadius2Mode = false;

		if (downFlag && IsEditableAngles()){
			const IColorShema& colorShema = GetColorShema();
			i2d::CVector2d center = objectPtr->GetPosition();
			double radius = objectPtr->GetInnerRadius();
			double radius2 = objectPtr->GetOuterRadius();
			double middleRadius = (radius2 + radius) * 0.5;
			i2d::CVector2d delta1;
			delta1.Init(objectPtr->GetBeginAngle(), middleRadius);
			i2d::CVector2d delta2;
			delta2.Init(objectPtr->GetEndAngle(), middleRadius);

			const i2d::ICalibration2d* calibrationPtr = objectPtr->GetCalibration();

			istd::CIndex2d ticker3 = GetScreenPosition(center + delta1, calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker4 = GetScreenPosition(center + delta2, calibrationPtr).ToIndex2d();

			const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);

			if (tickerBox.IsInside(position - ticker3)){
				m_editAngle1Mode = true;

				BeginModelChanges();

				return true;
			}

			if (tickerBox.IsInside(position - ticker4)){
				m_editAngle2Mode = true;

				BeginModelChanges();

				return true;
			}
		}

		m_editAngle1Mode = false;
		m_editAngle2Mode = false;
	}

	return CInteractivePinShape::OnMouseButton(position, buttonType, downFlag);
}


bool CInteractiveAnnulusSegmentShape::OnMouseMove(istd::CIndex2d position)
{
	if (m_editRadiusMode || m_editRadius2Mode || m_editAngle1Mode || m_editAngle2Mode){
		imod::IModel* modelPtr = GetModelPtr();
		i2d::CAnnulusSegment* objectPtr = dynamic_cast<i2d::CAnnulusSegment*>(modelPtr);
		I_ASSERT(objectPtr != NULL);

		const i2d::ICalibration2d* calibrationPtr = objectPtr->GetCalibration();

		i2d::CVector2d cp = GetLogPosition(position, calibrationPtr);

		const i2d::CVector2d& center = objectPtr->GetPosition();

		if (m_editRadiusMode){
			objectPtr->SetInnerRadius(center.GetDistance(cp));

			UpdateModelChanges();

			return true;
		}
		else if (m_editRadius2Mode){
			objectPtr->SetOuterRadius(center.GetDistance(cp));
			
			UpdateModelChanges();

			return true;
		}
		else if (m_editAngle1Mode){
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

			return true;
		}
		else if (m_editAngle2Mode){
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

			return true;
		}
	}

	return CInteractivePinShape::OnMouseMove(position);
}


// reimplemented (iview::IVisualizable)

void CInteractiveAnnulusSegmentShape::Draw(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetModelPtr();
	I_ASSERT(modelPtr != NULL);

	const i2d::CAnnulusSegment* objectPtr = dynamic_cast<const i2d::CAnnulusSegment*>(modelPtr);
	I_ASSERT(objectPtr != NULL);

	const i2d::ICalibration2d* calibrationPtr = objectPtr->GetCalibration();

	const IColorShema& colorShema = GetColorShema();
	const i2d::CVector2d& center = objectPtr->GetPosition();
	i2d::CVector2d screenCenter = GetScreenPosition(center, calibrationPtr);

	double radius = objectPtr->GetInnerRadius();
	double screenRadius = GetScreenPosition(center + i2d::CVector2d(-radius, 0), calibrationPtr).GetDistance(GetScreenPosition(center + i2d::CVector2d(radius, 0), calibrationPtr)) * 0.5;
	double radius2 = objectPtr->GetOuterRadius();
	double screenRadius2 = GetScreenPosition(center + i2d::CVector2d(-radius2, 0), calibrationPtr).GetDistance(GetScreenPosition(center + i2d::CVector2d(radius2, 0), calibrationPtr)) * 0.5;

	if (IsCenterVisible()){
		CInteractivePinShape::Draw(drawContext);
	}

	if (IsSelected()){
		drawContext.save();
		drawContext.setPen(colorShema.GetPen(IColorShema::SP_SELECTED));
		drawContext.setBrush(colorShema.GetBrush(IColorShema::SB_HALF_TRANSPARENT));
	}
	else{
		drawContext.save();
		drawContext.setPen(colorShema.GetPen(IColorShema::SP_NORMAL));
		drawContext.setBrush(colorShema.GetBrush(IColorShema::SB_HALF_TRANSPARENT2));
	}

	DrawArea(	drawContext,
				screenCenter, 
				int(qMin(screenRadius, screenRadius2)), 
				int(qMax(screenRadius, screenRadius2)),
				objectPtr->GetBeginAngle(),
				objectPtr->GetEndAngle());
	
	drawContext.restore();

	if (m_isEditableRadius && IsSelected()){
		i2d::CVector2d tickerDirection;
		tickerDirection.Init((objectPtr->GetBeginAngle() + objectPtr->GetEndAngle()) * 0.5);

		istd::CIndex2d ticker1 = GetScreenPosition(center + tickerDirection * radius, calibrationPtr).ToIndex2d();
		istd::CIndex2d ticker2 = GetScreenPosition(center + tickerDirection * radius2, calibrationPtr).ToIndex2d();

		colorShema.DrawTicker(drawContext, ticker1, IColorShema::TT_NORMAL);
		colorShema.DrawTicker(drawContext, ticker2, IColorShema::TT_NORMAL);
	}

	if (m_editableAngle && IsSelected()){
		double middleRadius = (radius2 + radius) * 0.5;
		i2d::CVector2d delta1;
		delta1.Init(objectPtr->GetBeginAngle(), middleRadius);
		i2d::CVector2d delta2;
		delta2.Init(objectPtr->GetEndAngle(), middleRadius);

		istd::CIndex2d ticker3 = GetScreenPosition(center + delta1, calibrationPtr).ToIndex2d();
		istd::CIndex2d ticker4 = GetScreenPosition(center + delta2, calibrationPtr).ToIndex2d();

		colorShema.DrawTicker(drawContext, ticker3, IColorShema::TT_CHECKBOX_ON);
		colorShema.DrawTicker(drawContext, ticker4, IColorShema::TT_CHECKBOX_ON);
	}
}


// reimplemented (imod::IObserver)

bool CInteractiveAnnulusSegmentShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CAnnulusSegment*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CInteractiveAnnulusSegmentShape::IsTouched(istd::CIndex2d position) const
{
	I_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetModelPtr();
	const i2d::CAnnulusSegment* objectPtr = dynamic_cast<const i2d::CAnnulusSegment*>(modelPtr);
	I_ASSERT(objectPtr != NULL);

	const i2d::ICalibration2d* calibrationPtr = objectPtr->GetCalibration();

	const IColorShema& colorShema = GetColorShema();

	double proportions = GetViewToScreenTransform().GetDeformMatrix().GetApproxScale();

	i2d::CVector2d cp = GetLogPosition(position, calibrationPtr);

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

		istd::CIndex2d ticker1 = GetScreenPosition(center + tickerDirection * radius, calibrationPtr).ToIndex2d();
		istd::CIndex2d ticker2 = GetScreenPosition(center + tickerDirection * radius2, calibrationPtr).ToIndex2d();

		const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_NORMAL);

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

		istd::CIndex2d ticker3 = GetScreenPosition(center + delta1, calibrationPtr).ToIndex2d();
		istd::CIndex2d ticker4 = GetScreenPosition(center + delta2, calibrationPtr).ToIndex2d();

		const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_NORMAL);

		if (		tickerBox.IsInside(position - ticker3) ||
					tickerBox.IsInside(position - ticker4)){
			return IInteractiveShape::TS_TICKER;
		}
	}

	double logicalLineWidth = colorShema.GetLogicalLineWidth();
	double angle = cpToCenterVector.GetAngle();
	if (angle < beginAngle){
		angle += 2 * I_PI;
	}
	if (angle > endAngle){
		angle -= 2 * I_PI;
	}

	if (		(proportions * qAbs(radius - distance) < logicalLineWidth) ||
				(proportions * qAbs(radius2 - distance) < logicalLineWidth)){
		if ((angle >= beginAngle) && (angle <= endAngle)){
			bool isEditablePosition = IsEditablePosition();

			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
	}

	if (		(proportions * middleRadius * qAbs(beginAngle - angle) < logicalLineWidth) ||
				(proportions * middleRadius * qAbs(endAngle - angle) < logicalLineWidth)){
		if ((distance >= radius) && (distance <= radius2)){
			bool isEditablePosition = IsEditablePosition();

			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
	}

	if (IsCenterVisible()){
		return CInteractivePinShape::IsTouched(position);
	}

	return TS_NONE;
}


// protected methods

void CInteractiveAnnulusSegmentShape::DrawArea(
			QPainter& painter,
			i2d::CVector2d center,
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

	double startAngleDeg = 360 - startAngle / I_PI * 180;
	double stopAngleDeg = 360 - stopAngle / I_PI * 180;
	qreal sweepLength = startAngleDeg - stopAngleDeg;

	QRectF minRect = iqt::GetQRect(minBox);
	QRectF maxRect = iqt::GetQRect(maxBox);

	i2d::CVector2d deltaStartAngle;
	deltaStartAngle.Init(startAngle, minRadius);

	QPainterPath painterPath;
	painterPath.moveTo(center + deltaStartAngle);
	painterPath.arcTo(maxRect, startAngleDeg, -sweepLength);
	painterPath.arcTo(minRect, stopAngleDeg, sweepLength);
	painterPath.closeSubpath();

	painter.drawPath(painterPath);
}


} // namespace iview


