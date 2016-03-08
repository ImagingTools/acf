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

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

			if (IsEditableRadiusInner()){
				i2d::CVector2d tickers[4] = {
							GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY())),
							GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius)),
							GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY())),
							GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius))
				};
				for (int i = 0; i < 4; ++i){
					if (tickerBox.IsInside(position - tickers[i].ToIndex2d())){
						m_editMode = EM_INNER_RADIUS;

						BeginTickerDrag();

						return true;
					}
				}
			}

			if (IsEditableRadiusOuter()){
				i2d::CVector2d tickers[4] = {
							GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2)),
							GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2)),
							GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2)),
							GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2))
				};
				for (int i = 0; i < 4; ++i){
					if (tickerBox.IsInside(position - tickers[i].ToIndex2d())){
						m_editMode = EM_OUTER_RADIUS;

						BeginTickerDrag();

						return true;
					}
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

	i2d::CAnnulus* annulusPtr = dynamic_cast<i2d::CAnnulus*>(GetObservedModel());
	if (annulusPtr != NULL){
		const i2d::CVector2d& cp = GetLogPosition(position);

		const i2d::CVector2d& center = annulusPtr->GetPosition();
		double radius = center.GetDistance(cp);

		switch (m_editMode){
		case EM_INNER_RADIUS:
			annulusPtr->SetInnerRadius(radius);
			break;

		case EM_OUTER_RADIUS:
			annulusPtr->SetOuterRadius(radius);
			break;

		default:
			return false;
		}

		UpdateModelChanges();
	}

	return true;
}


// reimplemented (iview::IVisualizable)

void CAnnulusShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	if (IsCenterVisible()){
		CPinShape::Draw(drawContext);
	}

	i2d::CAnnulus* annulusPtr = dynamic_cast<i2d::CAnnulus*>(GetObservedModel());
	if (annulusPtr != NULL){
		const i2d::CVector2d& center = annulusPtr->GetPosition();
		double radius = annulusPtr->GetInnerRadius();
		double radius2 = annulusPtr->GetOuterRadius();

		double xyShift2 = radius2 / sqrt(2.0);
		i2d::CVector2d tickers[8] = {
					GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY())),
					GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY())),
					GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2))
		};

		double ctrlShift = radius * 0.5523;
		double xyCtrlShift2 = radius2 * 0.5523 / sqrt(2.0);
		i2d::CVector2d ctrls1[8] = {
					GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY() - ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + ctrlShift, center.GetY() + radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY() + ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - ctrlShift, center.GetY() - radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2 + xyCtrlShift2, center.GetY() + xyShift2 - xyCtrlShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2 + xyCtrlShift2, center.GetY() + xyShift2 + xyCtrlShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2 - xyCtrlShift2, center.GetY() - xyShift2 + xyCtrlShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2 - xyCtrlShift2, center.GetY() - xyShift2 - xyCtrlShift2))
		};
		i2d::CVector2d ctrls2[8] = {
					GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY() + ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - ctrlShift, center.GetY() + radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY() - ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + ctrlShift, center.GetY() - radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2 - xyCtrlShift2, center.GetY() + xyShift2 + xyCtrlShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2 - xyCtrlShift2, center.GetY() + xyShift2 - xyCtrlShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2 + xyCtrlShift2, center.GetY() - xyShift2 - xyCtrlShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2 + xyCtrlShift2, center.GetY() - xyShift2 + xyCtrlShift2))
		};

		const IColorSchema& colorSchema = GetColorSchema();

		if (IsSelected()){
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
			drawContext.setBrush(colorSchema.GetBrush(IColorSchema::SB_HALF_TRANSPARENT));
		}
		else{
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
			drawContext.setBrush(colorSchema.GetBrush(IColorSchema::SB_HALF_TRANSPARENT2));
		}

		QPainterPath painterPatch;

		painterPatch.moveTo(tickers[0]);
		painterPatch.cubicTo(ctrls2[0], ctrls1[1], tickers[1]);
		painterPatch.cubicTo(ctrls2[1], ctrls1[2], tickers[2]);
		painterPatch.cubicTo(ctrls2[2], ctrls1[3], tickers[3]);
		painterPatch.cubicTo(ctrls2[3], ctrls1[0], tickers[0]);
		painterPatch.closeSubpath();

		painterPatch.moveTo(tickers[4]);
		painterPatch.cubicTo(ctrls2[4], ctrls1[5], tickers[5]);
		painterPatch.cubicTo(ctrls2[5], ctrls1[6], tickers[6]);
		painterPatch.cubicTo(ctrls2[6], ctrls1[7], tickers[7]);
		painterPatch.cubicTo(ctrls2[7], ctrls1[4], tickers[4]);
		painterPatch.closeSubpath();

		drawContext.drawPath(painterPatch);

		if (IsEditableRadius() && IsSelected()){
			if (IsEditableRadiusInner()){
				for (int i = 0; i < 4; ++i){
					colorSchema.DrawTicker(drawContext, tickers[i].ToIndex2d(), IColorSchema::TT_NORMAL);
				}
			}

			if (IsEditableRadiusOuter()){
				for (int i = 4; i < 8; ++i){
					colorSchema.DrawTicker(drawContext, tickers[i].ToIndex2d(), IColorSchema::TT_NORMAL);
				}
			}
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

	const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(GetObservedModel());
	if (IsDisplayConnected() && (annulusPtr != NULL)){
		const IColorSchema& colorSchema = GetColorSchema();

		i2d::CVector2d center = annulusPtr->GetPosition();
		double radius = annulusPtr->GetInnerRadius();
		double radius2 = annulusPtr->GetOuterRadius();

		double xyShift2 = radius2 / sqrt(2.0);

		if (IsSelected() && IsEditableRadius()){
			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

			if (IsEditableRadiusInner()){
				i2d::CVector2d tickers[4] = {
							GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY())),
							GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius)),
							GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY())),
							GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius))
				};
				for (int i = 0; i < 4; ++i){
					if (tickerBox.IsInside(position - tickers[i].ToIndex2d())){
						return IInteractiveShape::TS_TICKER;
					}
				}
			}

			if (IsEditableRadiusOuter()){
				i2d::CVector2d tickers[4] = {
							GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2)),
							GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2)),
							GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2)),
							GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2))
				};
				for (int i = 0; i < 4; ++i){
					if (tickerBox.IsInside(position - tickers[i].ToIndex2d())){
						return IInteractiveShape::TS_TICKER;
					}
				}
			}
		}

		double localLineWidth = GetLocalLineWidth(position);

		i2d::CVector2d cp = GetLogPosition(position);
		double distanceToCenter = center.GetDistance(cp);

		if (		(qAbs(distanceToCenter - radius) < localLineWidth) ||
					(qAbs(distanceToCenter - radius2) < localLineWidth)){
			bool isEditablePosition = IsEditablePosition();
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}

		if (IsCenterVisible()){
			return CPinShape::IsTouched(position);
		}
	}

	return TS_NONE;
}


// protected methods

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

		double xyShift2 = radius2 / sqrt(2.0);
		double ctrlShift = radius * 0.5523;
		double xyCtrlShift2 = radius2 * 0.5523 / sqrt(2.0);
		i2d::CVector2d ctrls1[8] = {
					GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY() - ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + ctrlShift, center.GetY() + radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY() + ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - ctrlShift, center.GetY() - radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2 + xyCtrlShift2, center.GetY() + xyShift2 - xyCtrlShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2 + xyCtrlShift2, center.GetY() + xyShift2 + xyCtrlShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2 - xyCtrlShift2, center.GetY() - xyShift2 + xyCtrlShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2 - xyCtrlShift2, center.GetY() - xyShift2 - xyCtrlShift2))
		};
		i2d::CVector2d ctrls2[8] = {
					GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY() + ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - ctrlShift, center.GetY() + radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY() - ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + ctrlShift, center.GetY() - radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2 - xyCtrlShift2, center.GetY() + xyShift2 + xyCtrlShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2 - xyCtrlShift2, center.GetY() + xyShift2 - xyCtrlShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2 + xyCtrlShift2, center.GetY() - xyShift2 - xyCtrlShift2)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2 + xyCtrlShift2, center.GetY() - xyShift2 + xyCtrlShift2))
		};

		i2d::CRect boundingBox = BaseClass::CalcBoundingBox();

		for (int i = 0; i < 8; ++i){
			boundingBox.Union(ctrls1[i].ToIndex2d());
			boundingBox.Union(ctrls2[i].ToIndex2d());
		}

		if (IsSelected()){
			i2d::CVector2d tickers[8] = {
						GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY())),
						GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius)),
						GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY())),
						GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius)),
						GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() + xyShift2)),
						GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() + xyShift2)),
						GetScreenPosition(i2d::CVector2d(center.GetX() - xyShift2, center.GetY() - xyShift2)),
						GetScreenPosition(i2d::CVector2d(center.GetX() + xyShift2, center.GetY() - xyShift2))
			};

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

			for (int i = 0; i < 8; ++i){
				boundingBox.Union(tickerBox.GetTranslated(tickers[i].ToIndex2d()));
			}
		}

		return boundingBox;
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview


