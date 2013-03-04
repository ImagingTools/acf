#include "iview/CRectangleShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"

#include "i2d/CLine2d.h"

#include "iqt/iqt.h"

#include "iview/IColorSchema.h"


namespace iview
{


CRectangleShape::CRectangleShape()
	:m_editNode(EN_NONE)
{
}


// reimplemented (iview::CInteractiveShapeBase)

void CRectangleShape::InvalidateBoundingBox()
{
	m_arePointsValid = false;

	BaseClass::InvalidateBoundingBox();
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CRectangleShape::IsTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CRectangle* rectanglePtr = dynamic_cast<const i2d::CRectangle*>(GetModelPtr());
	if (rectanglePtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		if (!m_arePointsValid){
			CalcPoints(*rectanglePtr);
		}

		if (IsSelected()){
			i2d::CRect tbox = colorSchema.GetTickerBox(IColorSchema::TT_MOVE);

			if (			tbox.IsInside(position - m_corners[0][0]) ||
							tbox.IsInside(position - m_corners[0][1]) ||
							tbox.IsInside(position - m_corners[1][0]) ||
							tbox.IsInside(position - m_corners[1][1])){
				return TS_TICKER;
			}
		}

		i2d::CVector2d cp = GetLogPosition(position);

		double proportions = GetViewToScreenTransform().GetDeformMatrix().GetApproxScale();

		double logicalLineWidth = colorSchema.GetLogicalLineWidth();

		const i2d::CRectangle& modelArea = *rectanglePtr;

		bool isEditablePosition = IsEditablePosition();

		i2d::CLine2d line(modelArea.GetLeftTop(), modelArea.GetRightTop());
		if (proportions * line.GetDistance(cp) < logicalLineWidth){
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
		line.SetPoint1(modelArea.GetRightBottom());
		if (proportions * line.GetDistance(cp) < logicalLineWidth){
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
		line.SetPoint2(modelArea.GetLeftBottom());
		if (proportions * line.GetDistance(cp) < logicalLineWidth){
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
		line.SetPoint1(modelArea.GetLeftTop());
		if (proportions * line.GetDistance(cp) < logicalLineWidth){
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
	}

	return TS_NONE;
}


// reimplemented (iview::IShape)

void CRectangleShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CRectangle* framePtr = dynamic_cast<const i2d::CRectangle*>(GetModelPtr());
	if (framePtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();
		if (!m_arePointsValid){
			CalcPoints(*framePtr);
		}

		if (IsSelected()){
			drawContext.save();
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
		}
		else{
			drawContext.save();
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
		}

		drawContext.drawLine(iqt::GetQPoint(m_corners[0][0]), iqt::GetQPoint(m_corners[0][1]));
		drawContext.drawLine(iqt::GetQPoint(m_corners[0][1]), iqt::GetQPoint(m_corners[1][1]));
		drawContext.drawLine(iqt::GetQPoint(m_corners[1][1]), iqt::GetQPoint(m_corners[1][0]));
		drawContext.drawLine(iqt::GetQPoint(m_corners[1][0]), iqt::GetQPoint(m_corners[0][0]));

		if (IsSelected()){
			colorSchema.DrawTicker(drawContext, m_corners[0][0], IColorSchema::TT_MOVE);
			colorSchema.DrawTicker(drawContext, m_corners[0][1], IColorSchema::TT_MOVE);
			colorSchema.DrawTicker(drawContext, m_corners[1][0], IColorSchema::TT_MOVE);
			colorSchema.DrawTicker(drawContext, m_corners[1][1], IColorSchema::TT_MOVE);
		}
		
		drawContext.restore();
	}
}


// reimplemented (imod::IObserver)

bool CRectangleShape::OnAttached(imod::IModel* modelPtr)
{
	Q_ASSERT(dynamic_cast<i2d::CRectangle*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::IMouseActionObserver)

bool CRectangleShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CRectangle* framePtr = dynamic_cast<const i2d::CRectangle*>(GetModelPtr());
	if (framePtr != NULL){
		if (downFlag){
			const IColorSchema& colorSchema = GetColorSchema();
			if (!m_arePointsValid){
				CalcPoints(*framePtr);
			}

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_MOVE);

			if (tickerBox.IsInside(position - m_corners[0][0])){
				m_editNode = EN_CORNER11;
			}
			else if (tickerBox.IsInside(position - m_corners[0][1])){
				m_editNode = EN_CORNER12;
			}
			else if (tickerBox.IsInside(position - m_corners[1][0])){
				m_editNode = EN_CORNER21;
			}
			else if (tickerBox.IsInside(position - m_corners[1][1])){
				m_editNode = EN_CORNER22;
			}
			else{
				EndModelChanges();

				return false;
			}
			m_referencePosition = GetLogPosition(position);

			BeginModelChanges();

			return true;
		}
	}

	EndModelChanges();
	
	return false;
}


bool CRectangleShape::OnMouseMove(istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		i2d::CRectangle& modelArea = *dynamic_cast<i2d::CRectangle*>(modelPtr);
		Q_ASSERT(&modelArea != NULL);

		i2d::CVector2d cp = GetLogPosition(position);

		i2d::CVector2d newPos;
		i2d::CVector2d deltaTranslate = cp - m_referencePosition;
		i2d::CRectangle translatedModel = modelArea.GetTranslated(deltaTranslate);

		switch (m_editNode){
		case EN_CORNER11:
			newPos = modelArea.GetLeftTop() + deltaTranslate;
			if (newPos.GetX() > modelArea.GetRight()){
				newPos.SetX(modelArea.GetRight());
			}
			if (newPos.GetY() > modelArea.GetBottom()){
				newPos.SetY(modelArea.GetBottom());
			}
			cp = newPos + m_referencePosition - modelArea.GetLeftTop();
			modelArea.SetTopLeft(newPos);
			break;

		case EN_CORNER12:
			newPos = modelArea.GetRightTop() + deltaTranslate;
			if (newPos.GetX() < modelArea.GetLeft()){
				newPos.SetX(modelArea.GetLeft());
			}
			if (newPos.GetY() > modelArea.GetBottom()){
				newPos.SetY(modelArea.GetBottom());
			}
			cp = newPos + m_referencePosition - modelArea.GetRightTop();
			modelArea.SetTopRight(newPos);
			break;

		case EN_CORNER21:
			newPos = modelArea.GetLeftBottom() + deltaTranslate;
			if (newPos.GetX() > modelArea.GetRight()){
				newPos.SetX(modelArea.GetRight());
			}
			if (newPos.GetY() < modelArea.GetTop()){
				newPos.SetY(modelArea.GetTop());
			}
			cp = newPos + m_referencePosition - modelArea.GetLeftBottom();
			modelArea.SetBottomLeft(newPos);
			break;

		case EN_CORNER22:
			newPos = modelArea.GetRightBottom() + deltaTranslate;
			if (newPos.GetX() < modelArea.GetLeft()){
				newPos.SetX(modelArea.GetLeft());
			}
			if (newPos.GetY() < modelArea.GetTop()){
				newPos.SetY(modelArea.GetTop());
			}
			cp = newPos + m_referencePosition - modelArea.GetRightBottom();
			modelArea.SetBottomRight(newPos);
			break;

		default:
			return false;
		}

		m_referencePosition = cp;

		UpdateModelChanges();

		return true;
	}
	else{
		return false;
	}
}


// protected methods

void CRectangleShape::CalcPoints(const i2d::CRectangle& rectangle) const
{
	m_corners[0][0] = GetScreenPosition(rectangle.GetLeftTop()).ToIndex2d();
	m_corners[0][1] = GetScreenPosition(rectangle.GetRightTop()).ToIndex2d();
	m_corners[1][0] = GetScreenPosition(rectangle.GetLeftBottom()).ToIndex2d();
	m_corners[1][1] = GetScreenPosition(rectangle.GetRightBottom()).ToIndex2d();

	m_arePointsValid = true;
}


// reimplemented (iview::CInteractiveShapeBase)

void CRectangleShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	m_referencePosition = reference;
}


void CRectangleShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		i2d::CRectangle& rectangle = *dynamic_cast<i2d::CRectangle*>(modelPtr);
		Q_ASSERT(&rectangle != NULL);

		BeginModelChanges();

		rectangle.Translate(position - m_referencePosition);
		m_referencePosition = position;

		EndModelChanges();
	}
}


// reimplemented (iview::CShapeBase)

i2d::CRect CRectangleShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CRectangle* framePtr = dynamic_cast<const i2d::CRectangle*>(GetModelPtr());
	if (framePtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();
		if (!m_arePointsValid){
			CalcPoints(*framePtr);
		}

		i2d::CRect boundingBox(m_corners[0][0], m_corners[0][0]);
		boundingBox.Union(m_corners[0][1]);
		boundingBox.Union(m_corners[1][0]);
		boundingBox.Union(m_corners[1][1]);

		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_MOVE: IColorSchema::TT_INACTIVE);
		
		boundingBox.Expand(tickerBox);
		boundingBox.Expand(i2d::CRect(istd::CIndex2d(-1, -1), istd::CIndex2d(1, 1)));

		return boundingBox;
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview


