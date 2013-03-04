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
		Invalidate(CS_CONSOLE);
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

		Invalidate(CS_CONSOLE);
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CCircleShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(GetModelPtr());
	if (circlePtr != NULL){
		if (downFlag && m_isEditableRadius){
			const IColorSchema& colorSchema = GetColorSchema();
			i2d::CVector2d center = circlePtr->GetPosition();
			double radius = circlePtr->GetRadius();

			i2d::CVector2d ticker1 = GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()));
			i2d::CVector2d ticker2 = GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()));
			i2d::CVector2d ticker3 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius));
			i2d::CVector2d ticker4 = GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius));

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

			if (			tickerBox.IsInside(position - ticker1.ToIndex2d()) ||
							tickerBox.IsInside(position - ticker2.ToIndex2d()) ||
							tickerBox.IsInside(position - ticker3.ToIndex2d()) ||
							tickerBox.IsInside(position - ticker4.ToIndex2d())){
				m_editMode = EM_RADIUS;

				BeginModelChanges();

				return true;
			}
		}
		m_editMode = EM_NONE;

		if (m_isCenterVisible){
			return BaseClass::OnMouseButton(position, buttonType, downFlag);
		}
		else{
			EndModelChanges();
		}
	}
	return false;
}


bool CCircleShape::OnMouseMove(istd::CIndex2d position)
{
	if (m_editMode == EM_RADIUS){
		imod::IModel* modelPtr = GetModelPtr();
		i2d::CCircle& circle = *dynamic_cast<i2d::CCircle*>(modelPtr);
		Q_ASSERT(&circle != NULL);

		i2d::CVector2d cp = GetLogPosition(position);
		const i2d::CVector2d& center = circle.GetPosition();
		
		double newRadius = center.GetDistance(cp);
		
		if (circle.GetRadius() != newRadius){
			circle.SetRadius(newRadius);

			UpdateModelChanges();
		}
		
		return true;
	}

	if (m_isCenterVisible){
		return BaseClass::OnMouseMove(position);
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

	const imod::IModel* modelPtr = GetModelPtr();
	const i2d::CCircle& circle = *dynamic_cast<const i2d::CCircle*>(modelPtr);
	Q_ASSERT(&circle != NULL);

	const i2d::CVector2d& center = circle.GetPosition();
	double radius = circle.GetRadius();

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

	const IColorSchema& colorSchema = GetColorSchema();

	if (IsSelected()){
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
	}
	else{
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
	}

	QBrush emptyBrush(QColor(0, 0, 0), Qt::NoBrush);
	drawContext.setBrush(emptyBrush);

	drawContext.drawEllipse(QRect(
				QPoint(int(screenCenter.GetX() - screenRadius), int(screenCenter.GetY() - screenRadius)),
				QPoint(int(screenCenter.GetX() + screenRadius), int(screenCenter.GetY() + screenRadius))));

	if (IsEditableRadius() && IsSelected()){
		colorSchema.DrawTicker(drawContext, ticker1.ToIndex2d(), IColorSchema::TT_NORMAL);
		colorSchema.DrawTicker(drawContext, ticker2.ToIndex2d(), IColorSchema::TT_NORMAL);
		colorSchema.DrawTicker(drawContext, ticker3.ToIndex2d(), IColorSchema::TT_NORMAL);
		colorSchema.DrawTicker(drawContext, ticker4.ToIndex2d(), IColorSchema::TT_NORMAL);
	}
}


// reimplemented (imod::IObserver)

bool CCircleShape::OnAttached(imod::IModel* modelPtr)
{
	Q_ASSERT(dynamic_cast<i2d::CCircle*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CCircleShape::IsTouched(istd::CIndex2d position) const
{
	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(GetModelPtr());
	if (IsDisplayConnected() && (circlePtr != NULL)){
		const IColorSchema& colorSchema = GetColorSchema();

		i2d::CVector2d center = circlePtr->GetPosition();
		double radius = circlePtr->GetRadius();

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

		if (m_isEditableRadius && IsSelected()){
			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

			if (			tickerBox.IsInside(position - ticker1.ToIndex2d()) ||
							tickerBox.IsInside(position - ticker2.ToIndex2d()) ||
							tickerBox.IsInside(position - ticker3.ToIndex2d()) ||
							tickerBox.IsInside(position - ticker4.ToIndex2d())){
				return TS_TICKER;
			}
		}

		double screenDistance = screenCenter.GetDistance(i2d::CVector2d(position));
		double logicalLineWidth = colorSchema.GetLogicalLineWidth();

		if (qAbs(screenRadius - screenDistance) < logicalLineWidth){
			bool isEditablePosition = IsEditablePosition();
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
		else{
			if (m_isCenterVisible){
				return BaseClass::IsTouched(position);
			}
		}
	}

	return TS_NONE;
}


// protected methods

// reimplemented (iview::CShapeBase)

i2d::CRect CCircleShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(GetModelPtr());
	if (circlePtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		const i2d::CVector2d& center = circlePtr->GetPosition();
		double radius = circlePtr->GetRadius();

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

		i2d::CRect boundingBox(
					int(screenCenter.GetX() - screenRadius - 1), int(screenCenter.GetY() - screenRadius - 1),
					int(screenCenter.GetX() + screenRadius + 1), int(screenCenter.GetY() + screenRadius + 1));

		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

		boundingBox.Union(tickerBox.GetTranslated(ticker1.ToIndex2d()));
		boundingBox.Union(tickerBox.GetTranslated(ticker2.ToIndex2d()));
		boundingBox.Union(tickerBox.GetTranslated(ticker3.ToIndex2d()));
		boundingBox.Union(tickerBox.GetTranslated(ticker4.ToIndex2d()));

		return boundingBox;
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview


