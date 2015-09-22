#include "iview/CAnnulusShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"

#include "i2d/CAnnulus.h"

#include "iqt/iqt.h"

#include "iview/IColorSchema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CAnnulusShape::CAnnulusShape()
:	BaseClass(),
	m_isCenterVisible(true),
	m_editMode(EM_NONE)
{
	m_isEditableRadius2 = true;
	m_isEditableRadius = true;
}


bool CAnnulusShape::IsEditableRadiusInner() const
{
	return m_isEditableRadius;
}


void CAnnulusShape::SetEditableRadiusInner(bool editable)
{
	if (m_isEditableRadius != editable){
		m_isEditableRadius = editable;
		Invalidate();
	}
}


bool CAnnulusShape::IsEditableRadiusOuter() const
{
	return m_isEditableRadius2;
}


void CAnnulusShape::SetEditableRadiusOuter(bool editable)
{
	if (m_isEditableRadius2 != editable){
		m_isEditableRadius2 = editable;

		Invalidate();
	}
}


bool CAnnulusShape::IsEditableRadius() const
{
	return m_isEditableRadius || m_isEditableRadius2;
}


void CAnnulusShape::SetEditableRadius(bool editable)
{
	if (m_isEditableRadius != editable || m_isEditableRadius2 != editable){
		m_isEditableRadius2 = m_isEditableRadius = editable;

		Invalidate();
	}
}


bool CAnnulusShape::IsCenterVisible() const
{
	return m_isCenterVisible;
}


void CAnnulusShape::SetCenterVisible(bool state)
{
	if (m_isCenterVisible != state){
		m_isCenterVisible = state;

		Invalidate();
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CAnnulusShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	ShapeBaseClass::OnMouseButton(position, buttonType, downFlag);

	const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(GetObservedModel());
	if (annulusPtr != NULL){
		if (downFlag && IsEditableRadius()){
			const IColorSchema& colorSchema = GetColorSchema();
			i2d::CVector2d center = annulusPtr->GetPosition();
			double radius = annulusPtr->GetInnerRadius();
			double xyShift2 = annulusPtr->GetOuterRadius() / sqrt(2.0);

			istd::CIndex2d ticker1 = GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY())).ToIndex2d();
			istd::CIndex2d ticker2 = GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY())).ToIndex2d();
			istd::CIndex2d ticker3 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius)).ToIndex2d();
			istd::CIndex2d ticker4 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius)).ToIndex2d();

			istd::CIndex2d ticker5 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2)).ToIndex2d();
			istd::CIndex2d ticker6 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2)).ToIndex2d();
			istd::CIndex2d ticker7 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2)).ToIndex2d();
			istd::CIndex2d ticker8 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2)).ToIndex2d();

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

			if (IsEditableRadiusInner()){
				if (			tickerBox.IsInside(position - ticker1) ||
								tickerBox.IsInside(position - ticker2) ||
								tickerBox.IsInside(position - ticker3) ||
								tickerBox.IsInside(position - ticker4)){
					m_editMode = EM_INNER_RADIUS;

					BeginTickerDrag();

					return true;
				}
			}

			if (IsEditableRadiusOuter()){
				if (			tickerBox.IsInside(position - ticker5) ||
								tickerBox.IsInside(position - ticker6) ||
								tickerBox.IsInside(position - ticker7) ||
								tickerBox.IsInside(position - ticker8)){
					m_editMode = EM_OUTER_RADIUS;

					BeginTickerDrag();

					return true;
				}
			}
		}

		m_editMode = EM_NONE;
	}

	return CPinShape::OnMouseButton(position, buttonType, downFlag);
}


bool CAnnulusShape::OnMouseMove(istd::CIndex2d position)
{
	if (m_editMode == EM_NONE){
		return BaseClass::OnMouseMove(position);
	}

	imod::IModel* modelPtr = GetObservedModel();
	i2d::CAnnulus& annulus = *dynamic_cast<i2d::CAnnulus*>(modelPtr);
	Q_ASSERT(&annulus != NULL);

	const i2d::CVector2d& cp = GetLogPosition(position);

	const i2d::CVector2d& center = annulus.GetPosition();
	double radius = center.GetDistance(cp);

	switch (m_editMode){
	case EM_INNER_RADIUS:
		annulus.SetInnerRadius(radius);
		break;

	case EM_OUTER_RADIUS:
		annulus.SetOuterRadius(radius);
		break;

	default:
		return false;
	}

	UpdateModelChanges();

	return true;
}


// reimplemented (iview::IVisualizable)

void CAnnulusShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetObservedModel();
	Q_ASSERT(modelPtr != NULL);

	const i2d::CAnnulus& annulus = *dynamic_cast<const i2d::CAnnulus*>(modelPtr);
	Q_ASSERT(&annulus != NULL);

	const IColorSchema& colorSchema = GetColorSchema();

	const i2d::CVector2d& center = annulus.GetPosition();
	double radius = annulus.GetInnerRadius();
	double radius2 = annulus.GetOuterRadius();

	i2d::CVector2d screenCenter = GetScreenPosition(center);
	i2d::CVector2d ticker1 = GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()));
	i2d::CVector2d ticker2 = GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()));
	i2d::CVector2d ticker3 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius));
	i2d::CVector2d ticker4 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius));

	double screenRadius = (
				screenCenter.GetDistance(ticker1) +
				screenCenter.GetDistance(ticker2) +
				screenCenter.GetDistance(ticker3) +
				screenCenter.GetDistance(ticker4)) * 0.25;

	double xyShift2 = radius2 / sqrt(2.0);
	i2d::CVector2d ticker5 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2));
	i2d::CVector2d ticker6 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2));
	i2d::CVector2d ticker7 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2));
	i2d::CVector2d ticker8 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2));

	double screenRadius2 = (
				screenCenter.GetDistance(ticker5) +
				screenCenter.GetDistance(ticker6) +
				screenCenter.GetDistance(ticker7) +
				screenCenter.GetDistance(ticker8)) * 0.25;

	if (IsCenterVisible()){
		CPinShape::Draw(drawContext);
	}

	drawContext.save();
	if (IsSelected()){
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
		drawContext.setBrush(colorSchema.GetBrush(IColorSchema::SB_HALF_TRANSPARENT));
	}
	else{
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
		drawContext.setBrush(colorSchema.GetBrush(IColorSchema::SB_HALF_TRANSPARENT2));
	}

	DrawAnnulus(drawContext, screenCenter.ToIndex2d(), qMin(screenRadius, screenRadius2), qMax(screenRadius, screenRadius2));

	drawContext.restore();

	if (IsEditableRadius() && IsSelected()){
		if (IsEditableRadiusInner()){
			colorSchema.DrawTicker(drawContext, ticker1.ToIndex2d(), IColorSchema::TT_NORMAL);
			colorSchema.DrawTicker(drawContext, ticker2.ToIndex2d(), IColorSchema::TT_NORMAL);
			colorSchema.DrawTicker(drawContext, ticker3.ToIndex2d(), IColorSchema::TT_NORMAL);
			colorSchema.DrawTicker(drawContext, ticker4.ToIndex2d(), IColorSchema::TT_NORMAL);
		}

		if (IsEditableRadiusOuter()){
			colorSchema.DrawTicker(drawContext, ticker5.ToIndex2d(), IColorSchema::TT_NORMAL);
			colorSchema.DrawTicker(drawContext, ticker6.ToIndex2d(), IColorSchema::TT_NORMAL);
			colorSchema.DrawTicker(drawContext, ticker7.ToIndex2d(), IColorSchema::TT_NORMAL);
			colorSchema.DrawTicker(drawContext, ticker8.ToIndex2d(), IColorSchema::TT_NORMAL);
		}
	}
}


// reimplemented (imod::IObserver)

bool CAnnulusShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CAnnulus*>(modelPtr) != NULL);

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CAnnulusShape::IsTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetObservedModel();
	const i2d::CAnnulus& annulus = *dynamic_cast<const i2d::CAnnulus*>(modelPtr);
	Q_ASSERT(&annulus != NULL);

	const IColorSchema& colorSchema = GetColorSchema();

	i2d::CVector2d center = annulus.GetPosition();
	double radius = annulus.GetInnerRadius();
	double radius2 = annulus.GetOuterRadius();

	i2d::CVector2d screenCenter = GetScreenPosition(center);
	i2d::CVector2d ticker1 = GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()));
	i2d::CVector2d ticker2 = GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()));
	i2d::CVector2d ticker3 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius));
	i2d::CVector2d ticker4 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius));

	double screenRadius = (
				screenCenter.GetDistance(ticker1) +
				screenCenter.GetDistance(ticker2) +
				screenCenter.GetDistance(ticker3) +
				screenCenter.GetDistance(ticker4)) * 0.25;

	double xyShift2 = radius2 / sqrt(2.0);
	i2d::CVector2d ticker5 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2));
	i2d::CVector2d ticker6 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2));
	i2d::CVector2d ticker7 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2));
	i2d::CVector2d ticker8 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2));

	double screenRadius2 = (
				screenCenter.GetDistance(ticker5) +
				screenCenter.GetDistance(ticker6) +
				screenCenter.GetDistance(ticker7) +
				screenCenter.GetDistance(ticker8)) * 0.25;

	if (IsSelected() && IsEditableRadius()){
		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

		if (IsEditableRadiusInner()){
			if (			tickerBox.IsInside(position - ticker1.ToIndex2d()) ||
							tickerBox.IsInside(position - ticker2.ToIndex2d()) ||
							tickerBox.IsInside(position - ticker3.ToIndex2d()) ||
							tickerBox.IsInside(position - ticker4.ToIndex2d())){
				return IInteractiveShape::TS_TICKER;
			}
		}

		if (IsEditableRadiusOuter()){
			if (			tickerBox.IsInside(position - ticker5.ToIndex2d()) ||
							tickerBox.IsInside(position - ticker6.ToIndex2d()) ||
							tickerBox.IsInside(position - ticker7.ToIndex2d()) ||
							tickerBox.IsInside(position - ticker8.ToIndex2d())){
				return IInteractiveShape::TS_TICKER;
			}
		}
	}

	double logicalLineWidth = colorSchema.GetLogicalLineWidth();

	double screenDistanceToCenter = screenCenter.GetDistance(i2d::CVector2d(position));
	if (		(qAbs(screenDistanceToCenter - screenRadius) < logicalLineWidth) ||
				(qAbs(screenDistanceToCenter - screenRadius2) < logicalLineWidth)){
		bool isEditablePosition = IsEditablePosition();
		return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
	}

	if (IsCenterVisible()){
		return CPinShape::IsTouched(position);
	}

	return TS_NONE;
}


// protected methods

void CAnnulusShape::DrawAnnulus(QPainter& painter, istd::CIndex2d center, int minRadius, int maxRadius, bool /*fillFlag*/) const
{
	Q_ASSERT(minRadius <= maxRadius);

	i2d::CRect maxBox1(center.GetX() - maxRadius, center.GetY() - maxRadius, center.GetX() + maxRadius + 1, center.GetY() + maxRadius + 1);
	i2d::CRect minBox1(center.GetX() - minRadius, center.GetY() - minRadius, center.GetX() + minRadius + 1, center.GetY() + minRadius + 1);

	QPainterPath painterPatch;

	painterPatch.addEllipse(iqt::GetQRect(minBox1));
	painterPatch.addEllipse(iqt::GetQRect(maxBox1));

	painter.drawPath(painterPatch);
}


// reimplemented (iview::CShapeBase)

i2d::CRect CAnnulusShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(GetObservedModel());
	if (annulusPtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		i2d::CVector2d center = annulusPtr->GetPosition();
		double radius = annulusPtr->GetInnerRadius();
		double radius2 = annulusPtr->GetOuterRadius();

		i2d::CVector2d screenCenter = GetScreenPosition(center);
		i2d::CVector2d ticker1 = GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()));
		i2d::CVector2d ticker2 = GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()));
		i2d::CVector2d ticker3 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius));
		i2d::CVector2d ticker4 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius));

		double screenRadius = (
					screenCenter.GetDistance(ticker1) +
					screenCenter.GetDistance(ticker2) +
					screenCenter.GetDistance(ticker3) +
					screenCenter.GetDistance(ticker4)) * 0.25;

		double xyShift2 = radius2 / sqrt(2.0);
		i2d::CVector2d ticker5 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2));
		i2d::CVector2d ticker6 = GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2));
		i2d::CVector2d ticker7 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2));
		i2d::CVector2d ticker8 = GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2));

		double screenRadius2 = (
					screenCenter.GetDistance(ticker5) +
					screenCenter.GetDistance(ticker6) +
					screenCenter.GetDistance(ticker7) +
					screenCenter.GetDistance(ticker8)) * 0.25;

		double maxScreenRadius = qMax(screenRadius, screenRadius2);

		i2d::CRect boundingBox(
					int(screenCenter.GetX() - maxScreenRadius - 1), int(screenCenter.GetY() - maxScreenRadius - 1),
					int(screenCenter.GetX() + maxScreenRadius + 1), int(screenCenter.GetY() + maxScreenRadius + 1));

		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);
		boundingBox.Expand(tickerBox);

		return boundingBox;
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview


