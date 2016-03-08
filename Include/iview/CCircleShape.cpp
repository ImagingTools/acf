#include "iview/CCircleShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"

#include "i2d/CCircle.h"

#include "iview/IColorSchema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CCircleShape::CCircleShape()
:	m_editMode(EM_NONE)
{
	m_isEditableRadius = true;
	m_isCenterVisible = true;
}


bool CCircleShape::IsEditableRadius() const
{
	return m_isEditableRadius;
}


void CCircleShape::SetEditableRadius(bool editable)
{
	if (m_isEditableRadius != editable){
		m_isEditableRadius = editable;
		Invalidate();
	}
}


bool CCircleShape::IsCenterVisible() const
{
	return m_isCenterVisible;
}


void CCircleShape::SetCenterVisible(bool state)
{
	if (m_isCenterVisible != state){
		m_isCenterVisible = state;

		Invalidate();
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CCircleShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	ShapeBaseClass::OnMouseButton(position, buttonType, downFlag);

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(GetObservedModel());
	if (circlePtr != NULL){
		if (downFlag && m_isEditableRadius){
			const IColorSchema& colorSchema = GetColorSchema();
			i2d::CVector2d center = circlePtr->GetPosition();
			double radius = circlePtr->GetRadius();

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

				i2d::CVector2d tickers[4] = {
							GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY())),
							GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius)),
							GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY())),
							GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius))
				};
				for (int i = 0; i < 4; ++i){
					if (tickerBox.IsInside(position - tickers[i].ToIndex2d())){
						m_editMode = EM_RADIUS;

						BeginTickerDrag();

						return true;
					}
				}
		}

		m_editMode = EM_NONE;

		if (IsCenterVisible()){
			return BaseClass::OnMouseButton(position, buttonType, downFlag);
		}
		else{
			EndTickerDrag();
		}
	}
	return false;
}


bool CCircleShape::OnMouseMove(istd::CIndex2d position)
{
	i2d::CCircle* circlePtr = dynamic_cast<i2d::CCircle*>(GetObservedModel());
	if (circlePtr != NULL){
		if (m_editMode == EM_RADIUS){
			i2d::CVector2d cp = GetLogPosition(position);
			const i2d::CVector2d& center = circlePtr->GetPosition();
		
			double newRadius = center.GetDistance(cp);
		
			if (circlePtr->GetRadius() != newRadius){
				circlePtr->SetRadius(newRadius);

				UpdateModelChanges();
			}
		
			return true;
		}

		if (IsCenterVisible()){
			return BaseClass::OnMouseMove(position);
		}
	}

	return false;
}


// reimplemented (iview::IVisualizable)

void CCircleShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	if (IsCenterVisible()){
		BaseClass::Draw(drawContext);
	}

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(GetObservedModel());
	if (circlePtr != NULL){
		const i2d::CVector2d& center = circlePtr->GetPosition();
		double radius = circlePtr->GetRadius();

		i2d::CVector2d tickers[4] = {
					GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY())),
					GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY())),
					GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius))
		};

		double ctrlShift = radius * 0.5523;
		i2d::CVector2d ctrls1[4] = {
					GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY() - ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + ctrlShift, center.GetY() + radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY() + ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - ctrlShift, center.GetY() - radius))
		};
		i2d::CVector2d ctrls2[4] = {
					GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY() + ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - ctrlShift, center.GetY() + radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY() - ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + ctrlShift, center.GetY() - radius))
		};

		const IColorSchema& colorSchema = GetColorSchema();
		if (IsSelected()){
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
		}
		else{
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
		}

		QBrush emptyBrush(QColor(0, 0, 0), Qt::NoBrush);
		drawContext.setBrush(emptyBrush);

		QPainterPath painterPatch;

		painterPatch.moveTo(tickers[0]);
		painterPatch.cubicTo(ctrls2[0], ctrls1[1], tickers[1]);
		painterPatch.cubicTo(ctrls2[1], ctrls1[2], tickers[2]);
		painterPatch.cubicTo(ctrls2[2], ctrls1[3], tickers[3]);
		painterPatch.cubicTo(ctrls2[3], ctrls1[0], tickers[0]);
		painterPatch.closeSubpath();

		drawContext.drawPath(painterPatch);

		if (IsEditableRadius() && IsSelected()){
			for (int i = 0; i < 4; ++i){
				colorSchema.DrawTicker(drawContext, tickers[i].ToIndex2d(), IColorSchema::TT_NORMAL);
			}
		}
	}
}


// reimplemented (imod::IObserver)

bool CCircleShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CCircle*>(modelPtr) != NULL);

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CCircleShape::IsTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(GetObservedModel());
	if (circlePtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		i2d::CVector2d center = circlePtr->GetPosition();
		double radius = circlePtr->GetRadius();

		i2d::CVector2d screenCenter = GetScreenPosition(center);
		i2d::CVector2d ticker1 = GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()));
		i2d::CVector2d ticker2 = GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()));
		i2d::CVector2d ticker3 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius));
		i2d::CVector2d ticker4 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius));

		if (IsEditableRadius() && IsSelected()){
			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

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

		double localLineWidth = GetLocalLineWidth(position);

		i2d::CVector2d cp = GetLogPosition(position);
		double distanceToCenter = center.GetDistance(cp);

		if (qAbs(distanceToCenter - radius) < localLineWidth){
			bool isEditablePosition = IsEditablePosition();
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}

		if (IsCenterVisible()){
			return BaseClass::IsTouched(position);
		}
	}

	return TS_NONE;
}


// protected methods

// reimplemented (iview::CShapeBase)

i2d::CRect CCircleShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(GetObservedModel());
	if (circlePtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		i2d::CVector2d center = circlePtr->GetPosition();
		double radius = circlePtr->GetRadius();

		double ctrlShift = radius * 0.5523;
		i2d::CVector2d ctrls1[4] = {
					GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY() - ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + ctrlShift, center.GetY() + radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY() + ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - ctrlShift, center.GetY() - radius))
		};
		i2d::CVector2d ctrls2[4] = {
					GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY() + ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - ctrlShift, center.GetY() + radius)),
					GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY() - ctrlShift)),
					GetScreenPosition(i2d::CVector2d(center.GetX() + ctrlShift, center.GetY() - radius))
		};

		i2d::CRect boundingBox = BaseClass::CalcBoundingBox();

		for (int i = 0; i < 4; ++i){
			boundingBox.Union(ctrls1[i].ToIndex2d());
			boundingBox.Union(ctrls2[i].ToIndex2d());
		}

		if (IsSelected()){
			i2d::CVector2d tickers[4] = {
						GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY())),
						GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius)),
						GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY())),
						GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius))
			};

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

			for (int i = 0; i < 4; ++i){
				boundingBox.Union(tickerBox.GetTranslated(tickers[i].ToIndex2d()));
			}
		}

		return boundingBox;
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview


