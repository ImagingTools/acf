#include "iview/CInteractiveAnnulusShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"

#include "i2d/CAnnulus.h"

#include "iqt/iqt.h"

#include "iview/IColorShema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CInteractiveAnnulusShape::CInteractiveAnnulusShape()
	:BaseClass()
{
	m_editRadius2Mode = false;
	m_editRadiusMode = false;

	m_isEditableRadius2 = true;
	m_isEditableRadius = true;
}


bool CInteractiveAnnulusShape::IsEditableRadiusInner() const
{
	return m_isEditableRadius;
}


void CInteractiveAnnulusShape::SetEditableRadiusInner(bool editable)
{
	if (m_isEditableRadius != editable){
		m_isEditableRadius = editable;
		Invalidate(CS_CONSOLE);
	}
}


bool CInteractiveAnnulusShape::IsEditableRadiusOuter() const
{
	return m_isEditableRadius2;
}


void CInteractiveAnnulusShape::SetEditableRadiusOuter(bool editable)
{
	if (m_isEditableRadius2 != editable){
		m_isEditableRadius2 = editable;
		Invalidate(CS_CONSOLE);
	}
}


bool CInteractiveAnnulusShape::IsEditableRadius() const
{
	return m_isEditableRadius || m_isEditableRadius2;
}


void CInteractiveAnnulusShape::SetEditableRadius(bool editable)
{
	if (m_isEditableRadius != editable || m_isEditableRadius2 != editable){
		m_isEditableRadius2 = m_isEditableRadius = editable;
		Invalidate(CS_CONSOLE);
	}
}


bool CInteractiveAnnulusShape::IsCenterVisible() const
{
	return m_isCenterVisible;
}


void CInteractiveAnnulusShape::SetCenterVisible(bool state)
{
	if (m_isCenterVisible != state){
		m_isCenterVisible = state;

		Invalidate(CS_CONSOLE);
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractiveAnnulusShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(GetModelPtr());
	if (annulusPtr != NULL){
		const i2d::ITransformation2d* calibrationPtr = annulusPtr->GetCalibration();

		if (downFlag && IsEditableRadius()){
			const IColorShema& colorShema = GetColorShema();
			i2d::CVector2d center = annulusPtr->GetPosition();
			double radius = annulusPtr->GetInnerRadius();
			double xyShift2 = annulusPtr->GetOuterRadius() / sqrt(2.0);

			istd::CIndex2d ticker1 = GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()), calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker2 = GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()), calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker3 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius), calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker4 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius), calibrationPtr).ToIndex2d();

			istd::CIndex2d ticker5 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2), calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker6 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2), calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker7 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2), calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker8 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2), calibrationPtr).ToIndex2d();

			const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);

			if (IsEditableRadiusInner()){
				if (			tickerBox.IsInside(position - ticker1) ||
								tickerBox.IsInside(position - ticker2) ||
								tickerBox.IsInside(position - ticker3) ||
								tickerBox.IsInside(position - ticker4)){
					m_editRadiusMode = true;

					BeginModelChanges();

					return true;
				}
			}

			if (IsEditableRadiusOuter()){
				if (			tickerBox.IsInside(position - ticker5) ||
								tickerBox.IsInside(position - ticker6) ||
								tickerBox.IsInside(position - ticker7) ||
								tickerBox.IsInside(position - ticker8)){
					m_editRadius2Mode = true;

					BeginModelChanges();

					return true;
				}
			}
		}
		m_editRadiusMode = false;
		m_editRadius2Mode = false;
	}

	return CInteractivePinShape::OnMouseButton(position, buttonType, downFlag);
}


bool CInteractiveAnnulusShape::OnMouseMove(istd::CIndex2d position)
{
	if (!m_editRadiusMode && !m_editRadius2Mode){
		return CInteractivePinShape::OnMouseMove(position);
	}

	imod::IModel* modelPtr = GetModelPtr();
	i2d::CAnnulus& annulus = *dynamic_cast<i2d::CAnnulus*>(modelPtr);
	I_ASSERT(&annulus != NULL);

	const i2d::ITransformation2d* calibrationPtr = annulus.GetCalibration();

	const i2d::CVector2d& cp = GetLogPosition(position, calibrationPtr);
	const i2d::CVector2d& center = annulus.GetPosition();
	double radius = center.GetDistance(cp);

	if (m_editRadiusMode){
		annulus.SetInnerRadius(radius);
	}
	else {
		annulus.SetOuterRadius(radius);
	}

	UpdateModelChanges();

	return true;
}


// reimplemented (iview::IVisualizable)

void CInteractiveAnnulusShape::Draw(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetModelPtr();
	I_ASSERT(modelPtr != NULL);

	const i2d::CAnnulus& annulus = *dynamic_cast<const i2d::CAnnulus*>(modelPtr);
	I_ASSERT(&annulus != NULL);

	const i2d::ITransformation2d* calibrationPtr = annulus.GetCalibration();
	const iview::CScreenTransform& viewToScreenTransform = GetViewToScreenTransform();

	const IColorShema& colorShema = GetColorShema();

	const i2d::CVector2d& center = annulus.GetPosition();
	i2d::CVector2d screenCenter = GetScreenPosition(center, calibrationPtr);

	double radius = annulus.GetInnerRadius();
	double radius2 = annulus.GetOuterRadius();

	i2d::CAffine2d transform;
	if ((calibrationPtr != NULL) && calibrationPtr->GetLocalTransform(center, transform)){
		transform.ApplyLeft(viewToScreenTransform);
	}
	else{
		transform = viewToScreenTransform;
	}

	const i2d::CMatrix2d& deform = transform.GetDeformMatrix();
	i2d::CVector2d scale;
	deform.GetAxesLengths(scale);

	if (IsCenterVisible()){
		CInteractivePinShape::Draw(drawContext);
	}

	if (IsSelected()){
		drawContext.save();
		drawContext.setPen(colorShema.GetPen(IColorShema::SP_SELECTED));
		drawContext.save();
		drawContext.setBrush(colorShema.GetBrush(IColorShema::SB_HALF_TRANSPARENT));
	}
	else{
		drawContext.save();
		drawContext.setPen(colorShema.GetPen(IColorShema::SP_NORMAL));
		drawContext.save();
		drawContext.setBrush(colorShema.GetBrush(IColorShema::SB_HALF_TRANSPARENT2));
	}

	DrawAnnulus(drawContext, screenCenter.ToIndex2d(), int(qMin(radius, radius2) * scale.GetX()), int(qMax(radius, radius2) * scale.GetY()));

	drawContext.restore();
	drawContext.restore();

	if (IsEditableRadius() && IsSelected()){
		if (IsEditableRadiusInner()){
			istd::CIndex2d ticker1 = GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()), calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker2 = GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()), calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker3 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius), calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker4 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius), calibrationPtr).ToIndex2d();

			colorShema.DrawTicker(drawContext, ticker1, IColorShema::TT_NORMAL);
			colorShema.DrawTicker(drawContext, ticker2, IColorShema::TT_NORMAL);
			colorShema.DrawTicker(drawContext, ticker3, IColorShema::TT_NORMAL);
			colorShema.DrawTicker(drawContext, ticker4, IColorShema::TT_NORMAL);
		}

		if (IsEditableRadiusOuter()){
			double xyShift2 = radius2 / sqrt(2.0);

			istd::CIndex2d ticker5 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2), calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker6 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2), calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker7 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2), calibrationPtr).ToIndex2d();
			istd::CIndex2d ticker8 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2), calibrationPtr).ToIndex2d();

			colorShema.DrawTicker(drawContext, ticker5, IColorShema::TT_NORMAL);
			colorShema.DrawTicker(drawContext, ticker6, IColorShema::TT_NORMAL);
			colorShema.DrawTicker(drawContext, ticker7, IColorShema::TT_NORMAL);
			colorShema.DrawTicker(drawContext, ticker8, IColorShema::TT_NORMAL);
		}
	}
}


// reimplemented (imod::IObserver)

bool CInteractiveAnnulusShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CAnnulus*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CInteractiveAnnulusShape::IsTouched(istd::CIndex2d position) const
{
	I_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetModelPtr();
	const i2d::CAnnulus& annulus = *dynamic_cast<const i2d::CAnnulus*>(modelPtr);
	I_ASSERT(&annulus != NULL);

	const i2d::ITransformation2d* calibrationPtr = annulus.GetCalibration();

	const IColorShema& colorShema = GetColorShema();

	double proportions = GetViewToScreenTransform().GetDeformMatrix().GetApproxScale();

	i2d::CVector2d cp = GetLogPosition(position,  calibrationPtr);

	i2d::CVector2d center = annulus.GetPosition();
	double distance = center.GetDistance(cp);

	double radius = annulus.GetInnerRadius();
	double radius2 = annulus.GetOuterRadius();

	if (IsSelected() && IsEditableRadius()){
		double xyShift2 = annulus.GetOuterRadius() / sqrt(2.0);

		istd::CIndex2d ticker1 = GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()), calibrationPtr).ToIndex2d();
		istd::CIndex2d ticker2 = GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()), calibrationPtr).ToIndex2d();
		istd::CIndex2d ticker3 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius), calibrationPtr).ToIndex2d();
		istd::CIndex2d ticker4 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius), calibrationPtr).ToIndex2d();

		istd::CIndex2d ticker5 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2), calibrationPtr).ToIndex2d();
		istd::CIndex2d ticker6 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2), calibrationPtr).ToIndex2d();
		istd::CIndex2d ticker7 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2), calibrationPtr).ToIndex2d();
		istd::CIndex2d ticker8 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2), calibrationPtr).ToIndex2d();

		const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);

		if (IsEditableRadiusInner()){
			if (			tickerBox.IsInside(position - ticker1) ||
							tickerBox.IsInside(position - ticker2) ||
							tickerBox.IsInside(position - ticker3) ||
							tickerBox.IsInside(position - ticker4)){
				return IInteractiveShape::TS_TICKER;
			}
		}

		if (IsEditableRadiusOuter()){
			if (			tickerBox.IsInside(position - ticker5) ||
							tickerBox.IsInside(position - ticker6) ||
							tickerBox.IsInside(position - ticker7) ||
							tickerBox.IsInside(position - ticker8)){
				return IInteractiveShape::TS_TICKER;
			}
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

void CInteractiveAnnulusShape::DrawAnnulus(QPainter& painter, istd::CIndex2d center, int minRadius, int maxRadius, bool /*fillFlag*/) const
{
	I_ASSERT(minRadius <= maxRadius);

	i2d::CRect maxBox1(center.GetX() - maxRadius, center.GetY() - maxRadius, center.GetX() + maxRadius + 1, center.GetY() + maxRadius + 1);
	i2d::CRect minBox1(center.GetX() - minRadius, center.GetY() - minRadius, center.GetX() + minRadius + 1, center.GetY() + minRadius + 1);
	i2d::CRect bitmapBox1 = maxBox1.GetIntersection(iqt::GetCRect(painter.clipRegion().boundingRect()));

	QRect tempBitmapRect1(iqt::GetQRect(i2d::CRect(bitmapBox1.GetSize())));
	QPainterPath painterPatch;

	painterPatch.addEllipse(iqt::GetQRect(minBox1));
	painterPatch.addEllipse(iqt::GetQRect(maxBox1));

	painter.drawPath(painterPatch);
}


// reimplemented (iview::CShapeBase)

i2d::CRect CInteractiveAnnulusShape::CalcBoundingBox() const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(GetModelPtr());
	if (annulusPtr != NULL){
		const IColorShema& colorShema = GetColorShema();

		const i2d::ITransformation2d* calibrationPtr = annulusPtr->GetCalibration();
		const iview::CScreenTransform& viewToScreenTransform = GetViewToScreenTransform();

		i2d::CVector2d center = annulusPtr->GetPosition();
		i2d::CVector2d screenCenter = GetScreenPosition(center, calibrationPtr);
		double radius = annulusPtr->GetOuterRadius();

		i2d::CAffine2d transform;
		if ((calibrationPtr != NULL) && calibrationPtr->GetLocalTransform(center, transform)){
			transform.ApplyLeft(viewToScreenTransform);
		}
		else{
			transform = viewToScreenTransform;
		}

		const i2d::CMatrix2d& deform = transform.GetDeformMatrix();
		i2d::CVector2d scale;
		deform.GetAxesLengths(scale);

		const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);

		i2d::CRect boundingBox(
					int(screenCenter.GetX() - radius * scale.GetX() - 1), int(screenCenter.GetY() - radius * scale.GetY() - 1),
					int(screenCenter.GetX() + radius * scale.GetX() + 1), int(screenCenter.GetY() + radius * scale.GetY() + 1));

		boundingBox.Expand(tickerBox);

		return boundingBox;
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview


