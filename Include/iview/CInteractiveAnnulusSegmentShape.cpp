#include "iview/CInteractiveAnnulusSegmentShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "imod/IModel.h"

#include "i2d/CAnnulusSegment.h"

#include "iqt/iqt.h"

#include "iview/CScreenTransform.h"


namespace iview
{


CInteractiveAnnulusSegmentShape::CInteractiveAnnulusSegmentShape()
{
	m_editRadius2Mode = false;
	m_editRadiusMode = false;

	m_editableAngle = false;
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

	const i2d::CAnnulusSegment* annulusPtr = dynamic_cast<const i2d::CAnnulusSegment*>(GetModelPtr());
	if (annulusPtr != NULL){
		if (downFlag && IsEditableRadius()){
			const IColorShema& colorShema = GetColorShema();
			i2d::CVector2d center = annulusPtr->GetPosition();
			double radius = annulusPtr->GetInnerRadius();
			double xyShift2 = annulusPtr->GetOuterRadius() / sqrt(2.0);

			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			istd::CIndex2d ticker1 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()));
			istd::CIndex2d ticker2 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()));
			istd::CIndex2d ticker3 = transform.GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius));
			istd::CIndex2d ticker4 = transform.GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius));

			istd::CIndex2d ticker5 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2));
			istd::CIndex2d ticker6 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2));
			istd::CIndex2d ticker7 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2));
			istd::CIndex2d ticker8 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2));

			const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);

			if (			tickerBox.IsInside(position - ticker1) ||
							tickerBox.IsInside(position - ticker2) ||
							tickerBox.IsInside(position - ticker3) ||
							tickerBox.IsInside(position - ticker4)){
				m_editRadiusMode = true;

				BeginModelChanges();

				return true;
			}

			if (			tickerBox.IsInside(position - ticker5) ||
							tickerBox.IsInside(position - ticker6) ||
							tickerBox.IsInside(position - ticker7) ||
							tickerBox.IsInside(position - ticker8)){
				m_editRadius2Mode = true;

				BeginModelChanges();

				return true;
			}
		}

		m_editRadiusMode = false;
		m_editRadius2Mode = false;

		if (downFlag && IsEditableAngles()){
			const IColorShema& colorShema = GetColorShema();
			i2d::CVector2d center = annulusPtr->GetPosition();
			double radius = annulusPtr->GetInnerRadius();
			double radius2 = annulusPtr->GetOuterRadius();
			double r = (radius2 - radius)/2 + radius;
			double x1 = r * qCos(annulusPtr->GetBeginAngle());
			double y1 = r * qSin(annulusPtr->GetBeginAngle());
			double x2 = r * qCos(annulusPtr->GetEndAngle());
			double y2 = r * qSin(annulusPtr->GetEndAngle());

			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			istd::CIndex2d ticker1 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + x1, center.GetY() + y1));
			istd::CIndex2d ticker2 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + x2, center.GetY() + y2));

			const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);

			if (tickerBox.IsInside(position - ticker1)){
				m_editAngle1Mode = true;

				BeginModelChanges();

				return true;
			}

			if (tickerBox.IsInside(position - ticker2)){
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
		i2d::CAnnulusSegment& annulus = *dynamic_cast<i2d::CAnnulusSegment*>(modelPtr);
		I_ASSERT(&annulus != NULL);

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::CVector2d& cp = transform.GetClientPosition(position);
		const i2d::CVector2d& center = annulus.GetPosition();

		if (m_editRadiusMode){
			annulus.SetInnerRadius(center.GetDistance(cp));

			UpdateModelChanges();

			return true;
		}
		else if (m_editRadius2Mode){
			annulus.SetOuterRadius(center.GetDistance(cp));
			
			UpdateModelChanges();

			return true;
		}
		else if (m_editAngle1Mode){
			i2d::CVector2d delta = cp - center;	
			double angle = qAtan2(delta.GetY(), delta.GetX());
			annulus.SetBeginAngle(angle);

			UpdateModelChanges();

			return true;
		}
		else if (m_editAngle2Mode){
			i2d::CVector2d delta = cp - center;	
			double angle = qAtan2(delta.GetY(), delta.GetX());
			annulus.SetEndAngle(angle);

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

	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	const i2d::CAnnulusSegment& annulusSegment = *dynamic_cast<const i2d::CAnnulusSegment*>(modelPtr);
	I_ASSERT(&annulusSegment != NULL);

    const IColorShema& colorShema = GetColorShema();
	i2d::CVector2d screenCenter;
	const i2d::CVector2d& center = annulusSegment.GetPosition();
	transform.GetApply(center, screenCenter);
	double radius = annulusSegment.GetInnerRadius();
	double radius2 = annulusSegment.GetOuterRadius();

	const i2d::CMatrix2d& deform = transform.GetDeformMatrix();
	i2d::CVector2d scale;
	deform.GetAxesLengths(scale);

	const i2d::CRect& clientRect = GetClientRect();
	i2d::CRect bbox = clientRect.GetIntersection(GetBoundingBox());

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

	DrawAnnulusSegment(
				drawContext,
				screenCenter.ToIndex2d(), 
				int(qMin(radius, radius2) * scale.GetX()), 
				int(qMax(radius, radius2) * scale.GetY()),
				annulusSegment.GetBeginAngle(),
				annulusSegment.GetEndAngle());
	
	drawContext.restore();

	if (m_isEditableRadius && IsSelected()){
		double xyShift2 = radius2 / sqrt(2.0);

		istd::CIndex2d ticker1 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()));
		istd::CIndex2d ticker2 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()));
		istd::CIndex2d ticker3 = transform.GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius));
		istd::CIndex2d ticker4 = transform.GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius));

		istd::CIndex2d ticker5 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2));
		istd::CIndex2d ticker6 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2));
		istd::CIndex2d ticker7 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2));
		istd::CIndex2d ticker8 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2));

		colorShema.DrawTicker(drawContext, ticker1, IColorShema::TT_NORMAL);
		colorShema.DrawTicker(drawContext, ticker2, IColorShema::TT_NORMAL);
		colorShema.DrawTicker(drawContext, ticker3, IColorShema::TT_NORMAL);
		colorShema.DrawTicker(drawContext, ticker4, IColorShema::TT_NORMAL);
		colorShema.DrawTicker(drawContext, ticker5, IColorShema::TT_NORMAL);
		colorShema.DrawTicker(drawContext, ticker6, IColorShema::TT_NORMAL);
		colorShema.DrawTicker(drawContext, ticker7, IColorShema::TT_NORMAL);
		colorShema.DrawTicker(drawContext, ticker8, IColorShema::TT_NORMAL);
	}

	if (m_editableAngle && IsSelected()){
		double r = (radius2 - radius)/2 + radius;
		double x1 = r * qCos(annulusSegment.GetBeginAngle());
		double y1 = r * qSin(annulusSegment.GetBeginAngle());
		double x2 = r * qCos(annulusSegment.GetEndAngle());
		double y2 = r * qSin(annulusSegment.GetEndAngle());

		istd::CIndex2d ticker1 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + x1, center.GetY() + y1));
		istd::CIndex2d ticker2 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + x2, center.GetY() + y2));

		colorShema.DrawTicker(drawContext, ticker1, IColorShema::TT_CHECKBOX_ON);
		colorShema.DrawTicker(drawContext, ticker2, IColorShema::TT_CHECKBOX_ON);
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
	const i2d::CAnnulusSegment& annulus = *dynamic_cast<const i2d::CAnnulusSegment*>(modelPtr);
	I_ASSERT(&annulus != NULL);

    const IColorShema& colorShema = GetColorShema();
	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	double proportions = ::sqrt(transform.GetDeformMatrix().GetDet());
	i2d::CVector2d cp = transform.GetClientPosition(position);

	i2d::CVector2d center = annulus.GetPosition();
	double distance = center.GetDistance(cp);

	double radius = annulus.GetInnerRadius();
	double radius2 = annulus.GetOuterRadius();

	if (IsSelected() && IsEditableRadius()){
		double xyShift2 = annulus.GetOuterRadius() / sqrt(2.0);

		istd::CIndex2d ticker1 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()));
		istd::CIndex2d ticker2 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()));
		istd::CIndex2d ticker3 = transform.GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius));
		istd::CIndex2d ticker4 = transform.GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius));

		istd::CIndex2d ticker5 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2));
		istd::CIndex2d ticker6 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2));
		istd::CIndex2d ticker7 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2));
		istd::CIndex2d ticker8 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2));

		const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_NORMAL);

		if (			tickerBox.IsInside(position - ticker1) ||
						tickerBox.IsInside(position - ticker2) ||
						tickerBox.IsInside(position - ticker3) ||
						tickerBox.IsInside(position - ticker4) ||
						tickerBox.IsInside(position - ticker5) ||
						tickerBox.IsInside(position - ticker6) ||
						tickerBox.IsInside(position - ticker7) ||
						tickerBox.IsInside(position - ticker8)){
			return IInteractiveShape::TS_TICKER;
		}
	}

	if (IsSelected() && IsEditableAngles()){
		double r = (radius2 - radius)/2 + radius;
		double x1 = r * qCos(annulus.GetBeginAngle());
		double y1 = r * qSin(annulus.GetBeginAngle());
		double x2 = r * qCos(annulus.GetEndAngle());
		double y2 = r * qSin(annulus.GetEndAngle());

		istd::CIndex2d ticker1 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + x1, center.GetY() + y1));
		istd::CIndex2d ticker2 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + x2, center.GetY() + y2));

		const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_NORMAL);

		if (			tickerBox.IsInside(position - ticker1) ||
						tickerBox.IsInside(position - ticker2)){
			return IInteractiveShape::TS_TICKER;
		}
	}

	double delta = qAbs(radius - distance);
	double delta2 = qAbs(radius2 - distance);

	double logicalLineWidth = colorShema.GetLogicalLineWidth();

	if ((proportions * delta < logicalLineWidth) || (proportions * delta2 < logicalLineWidth)){
		bool isEditablePosition = IsEditablePosition();
		return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
	}

	if (IsCenterVisible()){
		return CInteractivePinShape::IsTouched(position);
	}

	return TS_NONE;
}


// protected methods

void CInteractiveAnnulusSegmentShape::DrawAnnulusSegment(
			QPainter& painter,
			istd::CIndex2d center,
			int minRadius,
			int maxRadius,
			double startAngle,
			double stopAngle,
			bool /*fillFlag*/) const
{
	i2d::CRect maxBox(center.GetX() - maxRadius, center.GetY() - maxRadius, center.GetX() + maxRadius + 1, center.GetY() + maxRadius + 1);
	i2d::CRect minBox(center.GetX() - minRadius, center.GetY() - minRadius, center.GetX() + minRadius + 1, center.GetY() + minRadius + 1);

	QPainterPath painterPath;
	QPainterPath painterLinePath;

	if (startAngle > stopAngle){
		startAngle = startAngle - 2 * I_PI;
	}

	startAngle = startAngle/I_PI * 180.0;
	stopAngle = stopAngle/I_PI * 180;
	qreal sweepLength = stopAngle - startAngle;
	double stopAngle2 = 360-stopAngle;

	QRectF minRect = iqt::GetQRect(minBox);
	QRectF maxRect = iqt::GetQRect(maxBox);

	painterPath.moveTo(minRect.center());
	painterPath.arcTo(minRect, stopAngle2, sweepLength);
	painterPath.moveTo(maxRect.center());
	painterPath.arcTo(maxRect, stopAngle2, sweepLength);
	painterPath.closeSubpath();

	painter.drawPath(painterPath);
}



} // namespace iview



