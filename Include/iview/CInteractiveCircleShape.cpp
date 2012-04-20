#include "iview/CInteractiveCircleShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"

#include "i2d/CCircle.h"

#include "iview/CScreenTransform.h"


namespace iview
{


CInteractiveCircleShape::CInteractiveCircleShape()
{
	m_isEditableRadius = true;
	m_isCenterVisible = true;
}


bool CInteractiveCircleShape::IsEditableRadius() const
{
	return m_isEditableRadius;
}


void CInteractiveCircleShape::SetEditableRadius(bool editable)
{
	if (m_isEditableRadius != editable){
		m_isEditableRadius = editable;
		Invalidate(CS_CONSOLE);
	}
}


bool CInteractiveCircleShape::IsCenterVisible() const
{
	return m_isCenterVisible;
}


void CInteractiveCircleShape::SetCenterVisible(bool state)
{
	if (m_isCenterVisible != state){
		m_isCenterVisible = state;

		Invalidate(CS_CONSOLE);
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractiveCircleShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(GetModelPtr());
	if (circlePtr != NULL){
		if (downFlag && m_isEditableRadius){
			const IColorShema& colorShema = GetColorShema();
			i2d::CVector2d center = circlePtr->GetPosition();
			double radius = circlePtr->GetRadius();

			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			istd::CIndex2d ticker1 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()));
			istd::CIndex2d ticker2 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()));
			istd::CIndex2d ticker3 = transform.GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius));
			istd::CIndex2d ticker4 = transform.GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius));

			const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);

			if (			tickerBox.IsInside(position - ticker1) ||
							tickerBox.IsInside(position - ticker2) ||
							tickerBox.IsInside(position - ticker3) ||
							tickerBox.IsInside(position - ticker4)){
				m_editRadiusMode = true;

				BeginModelChanges();

				return true;
			}
		}
		m_editRadiusMode = false;

		if (m_isCenterVisible){
			return BaseClass::OnMouseButton(position, buttonType, downFlag);
		}
		else{
			EndModelChanges();
		}
	}
	return false;
}


bool CInteractiveCircleShape::OnMouseMove(istd::CIndex2d position)
{
	if (m_editRadiusMode){
		imod::IModel* modelPtr = GetModelPtr();
		i2d::CCircle& circle = *dynamic_cast<i2d::CCircle*>(modelPtr);
		I_ASSERT(&circle != NULL);

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::CVector2d& cp = transform.GetClientPosition(position);
		const i2d::CVector2d& center = circle.GetPosition();
		
		double newRaduis = center.GetDistance(cp);
		
		if (circle.GetRadius() != newRaduis){
			circle.SetRadius(center.GetDistance(cp));

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

void CInteractiveCircleShape::Draw(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	if (IsCenterVisible()){
		BaseClass::Draw(drawContext);
	}

	const imod::IModel* modelPtr = GetModelPtr();
	const i2d::CCircle& circle = *dynamic_cast<const i2d::CCircle*>(modelPtr);
	I_ASSERT(&circle != NULL);

	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	i2d::CVector2d screenCenter;
	const i2d::CVector2d& center = circle.GetPosition();
	transform.GetApply(center, screenCenter);
	double radius = circle.GetRadius();

	const i2d::CMatrix2d& deform = transform.GetDeformMatrix();
	i2d::CVector2d scale;
	deform.GetAxesLengths(scale);

    const IColorShema& colorShema = GetColorShema();
	drawContext.save();

	if (IsSelected()){
		drawContext.setPen(colorShema.GetPen(IColorShema::SP_SELECTED));
	}
	else{
		drawContext.setPen(colorShema.GetPen(IColorShema::SP_NORMAL));
	}

	QBrush emptyBrush(QColor(0, 0, 0), Qt::NoBrush);
	drawContext.setBrush(emptyBrush);

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

		colorShema.DrawTicker(drawContext, ticker1, IColorShema::TT_NORMAL);
		colorShema.DrawTicker(drawContext, ticker2, IColorShema::TT_NORMAL);
		colorShema.DrawTicker(drawContext, ticker3, IColorShema::TT_NORMAL);
		colorShema.DrawTicker(drawContext, ticker4, IColorShema::TT_NORMAL);
	}
}


// reimplemented (imod::IObserver)

bool CInteractiveCircleShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CCircle*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CInteractiveCircleShape::IsTouched(istd::CIndex2d position) const
{
	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(GetModelPtr());
	if (IsDisplayConnected() && (circlePtr != NULL)){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const IColorShema& colorShema = GetColorShema();

		double proportions = ::sqrt(transform.GetDeformMatrix().GetDet());
		i2d::CVector2d cp = transform.GetClientPosition(position);

		i2d::CVector2d center = circlePtr->GetPosition();
		double radius = circlePtr->GetRadius();

		if (m_isEditableRadius && IsSelected()){
			istd::CIndex2d ticker1 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() + radius, center.GetY()));
			istd::CIndex2d ticker2 = transform.GetScreenPosition(i2d::CVector2d(center.GetX() - radius, center.GetY()));
			istd::CIndex2d ticker3 = transform.GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() + radius));
			istd::CIndex2d ticker4 = transform.GetScreenPosition(i2d::CVector2d(center.GetX(), center.GetY() - radius));

			const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);

			if (			tickerBox.IsInside(position - ticker1) ||
							tickerBox.IsInside(position - ticker2) ||
							tickerBox.IsInside(position - ticker3) ||
							tickerBox.IsInside(position - ticker4)){
				return TS_TICKER;
			}
		}

		double distance = center.GetDistance(cp);
		double delta = qAbs(radius - distance);

		double logicalLineWidth = colorShema.GetLogicalLineWidth();

		if (proportions * delta < logicalLineWidth){
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


// reimplemented (iview::CInteractiveShapeBase)

i2d::CRect CInteractiveCircleShape::CalcBoundingBox() const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(GetModelPtr());
	if (circlePtr != NULL){
		const IColorShema& colorShema = GetColorShema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		i2d::CVector2d center;
		const i2d::CAffine2d& test = transform;
		test.GetApply(circlePtr->GetPosition(), center);
		double radius = circlePtr->GetRadius();

		const i2d::CMatrix2d& deform = transform.GetDeformMatrix();
		i2d::CVector2d scale;
		deform.GetAxesLengths(scale);

		const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);

		i2d::CRect boundingBox(
					int(center.GetX() - radius * scale.GetX() - 1), int(center.GetY() - radius * scale.GetY() + 1),
					int(center.GetX() + radius * scale.GetX() - 1), int(center.GetY() + radius * scale.GetY() + 1));

		boundingBox.Expand(tickerBox);

		return boundingBox;
	}

	return i2d::CRect();
}


} // namespace iview


