#include "iview/CInteractiveRectangleShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "imod/IModel.h"
#include "i2d/CLine2d.h"
#include "iqt/iqt.h"


namespace iview
{


CInteractiveRectangleShape::CInteractiveRectangleShape()
	:m_editNode(EN_NONE)
{
}


// reimplemented (iview::CInteractiveShapeBase)

void CInteractiveRectangleShape::InvalidateBoundingBox()
{
	m_arePointsValid = false;

	BaseClass::InvalidateBoundingBox();
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CInteractiveRectangleShape::IsTouched(istd::CIndex2d position) const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CRectangle* rectanglePtr = dynamic_cast<const i2d::CRectangle*>(GetModelPtr());
	if (rectanglePtr != NULL){
        const IColorShema& colorShema = GetColorShema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		if (!m_arePointsValid){
			CalcPoints(*rectanglePtr, transform);
		}

		if (IsSelected()){
			i2d::CRect tbox = colorShema.GetTickerBox(IColorShema::TT_MOVE);

			if (			tbox.IsInside(position - m_corners[0][0]) ||
							tbox.IsInside(position - m_corners[0][1]) ||
							tbox.IsInside(position - m_corners[1][0]) ||
							tbox.IsInside(position - m_corners[1][1])){
				return TS_TICKER;
			}
		}

		i2d::CVector2d cp = transform.GetClientPosition(position);
		double proportions = ::sqrt(transform.GetDeformMatrix().GetDet());

		double logicalLineWidth = colorShema.GetLogicalLineWidth();

		const i2d::CRectangle& modelArea = *rectanglePtr;

		bool isEditablePosition = IsEditablePosition();

		i2d::CLine2d line(modelArea.GetTopLeft(), modelArea.GetTopRight());
		if (proportions * line.GetDistance(cp) < logicalLineWidth){
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
		line.SetPoint1(modelArea.GetBottomRight());
		if (proportions * line.GetDistance(cp) < logicalLineWidth){
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
		line.SetPoint2(modelArea.GetBottomLeft());
		if (proportions * line.GetDistance(cp) < logicalLineWidth){
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
		line.SetPoint1(modelArea.GetTopLeft());
		if (proportions * line.GetDistance(cp) < logicalLineWidth){
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
	}

	return TS_NONE;
}


// reimplemented (iview::IShape)

void CInteractiveRectangleShape::Draw(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CRectangle* framePtr = dynamic_cast<const i2d::CRectangle*>(GetModelPtr());
	if (framePtr != NULL){
        const IColorShema& colorShema = GetColorShema();
		if (!m_arePointsValid){
			const iview::CScreenTransform& transform = GetLogToScreenTransform();

			CalcPoints(*framePtr, transform);
		}

		if (IsSelected()){
			drawContext.save();
			drawContext.setPen(colorShema.GetPen(IColorShema::SP_SELECTED));
		}
		else{
			drawContext.save();
			drawContext.setPen(colorShema.GetPen(IColorShema::SP_NORMAL));
		}

		drawContext.drawLine(iqt::GetQPoint(m_corners[0][0]), iqt::GetQPoint(m_corners[0][1]));
        drawContext.drawLine(iqt::GetQPoint(m_corners[0][1]), iqt::GetQPoint(m_corners[1][1]));
        drawContext.drawLine(iqt::GetQPoint(m_corners[1][1]), iqt::GetQPoint(m_corners[1][0]));
        drawContext.drawLine(iqt::GetQPoint(m_corners[1][0]), iqt::GetQPoint(m_corners[0][0]));

		if (IsSelected()){
			colorShema.DrawTicker(drawContext, m_corners[0][0], IColorShema::TT_MOVE);
			colorShema.DrawTicker(drawContext, m_corners[0][1], IColorShema::TT_MOVE);
			colorShema.DrawTicker(drawContext, m_corners[1][0], IColorShema::TT_MOVE);
			colorShema.DrawTicker(drawContext, m_corners[1][1], IColorShema::TT_MOVE);
		}
		
		drawContext.restore();
	}
}


// reimplemented (imod::IObserver)

bool CInteractiveRectangleShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CRectangle*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractiveRectangleShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CRectangle* framePtr = dynamic_cast<const i2d::CRectangle*>(GetModelPtr());
	if (framePtr != NULL){
		if (downFlag){
            const IColorShema& colorShema = GetColorShema();
			const iview::CScreenTransform& transform = GetLogToScreenTransform();

			if (!m_arePointsValid){
				CalcPoints(*framePtr, transform);
			}

			const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_MOVE);

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
			m_referencePosition = transform.GetClientPosition(position);

			BeginModelChanges();

			return true;
		}
	}

	EndModelChanges();
	
	return false;
}


bool CInteractiveRectangleShape::OnMouseMove(istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		i2d::CRectangle& modelArea = *dynamic_cast<i2d::CRectangle*>(modelPtr);
		I_ASSERT(&modelArea != NULL);

		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		i2d::CVector2d cp = transform.GetClientPosition(position);

		i2d::CVector2d newPos;
        i2d::CVector2d deltaTranslate = cp - m_referencePosition;
        i2d::CRectangle translatedModel = modelArea.GetTranslated(deltaTranslate);

		switch (m_editNode){
		case EN_CORNER11:
			newPos = modelArea.GetTopLeft() + deltaTranslate;
			if (newPos.GetX() > modelArea.GetRight()){
			    newPos.SetX(modelArea.GetRight());
			}
			if (newPos.GetY() > modelArea.GetBottom()){
			    newPos.SetY(modelArea.GetBottom());
			}
			cp = newPos + m_referencePosition - modelArea.GetTopLeft();
			modelArea.SetTopLeft(newPos);
			break;

		case EN_CORNER12:
			newPos = modelArea.GetTopRight() + deltaTranslate;
			if (newPos.GetX() < modelArea.GetLeft()){
			    newPos.SetX(modelArea.GetLeft());
			}
			if (newPos.GetY() > modelArea.GetBottom()){
			    newPos.SetY(modelArea.GetBottom());
			}
			cp = newPos + m_referencePosition - modelArea.GetTopRight();
			modelArea.SetTopRight(newPos);
			break;

		case EN_CORNER21:
			newPos = modelArea.GetBottomLeft() + deltaTranslate;
			if (newPos.GetX() > modelArea.GetRight()){
			    newPos.SetX(modelArea.GetRight());
			}
			if (newPos.GetY() < modelArea.GetTop()){
			    newPos.SetY(modelArea.GetTop());
			}
			cp = newPos + m_referencePosition - modelArea.GetBottomLeft();
			modelArea.SetBottomLeft(newPos);
			break;

		case EN_CORNER22:
			newPos = modelArea.GetBottomRight() + deltaTranslate;
			if (newPos.GetX() < modelArea.GetLeft()){
			    newPos.SetX(modelArea.GetLeft());
			}
			if (newPos.GetY() < modelArea.GetTop()){
			    newPos.SetY(modelArea.GetTop());
			}
			cp = newPos + m_referencePosition - modelArea.GetBottomRight();
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

void CInteractiveRectangleShape::CalcPoints(const i2d::CRectangle& rectangle, const iview::CScreenTransform& transform) const
{

	m_corners[0][0] = transform.GetScreenPosition(rectangle.GetTopLeft());
	m_corners[0][1] = transform.GetScreenPosition(rectangle.GetTopRight());
	m_corners[1][0] = transform.GetScreenPosition(rectangle.GetBottomLeft());
	m_corners[1][1] = transform.GetScreenPosition(rectangle.GetBottomRight());

	m_arePointsValid = true;
}


// reimplemented (iview::CInteractiveShapeBase)

i2d::CRect CInteractiveRectangleShape::CalcBoundingBox() const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CRectangle* framePtr = dynamic_cast<const i2d::CRectangle*>(GetModelPtr());
	if (framePtr != NULL){
		const IColorShema& colorShema = GetColorShema();
		if (!m_arePointsValid){
			const iview::CScreenTransform& transform = GetLogToScreenTransform();

			CalcPoints(*framePtr, transform);
		}

		i2d::CRect boundingBox(m_corners[0][0], m_corners[0][0]);
		boundingBox.Union(m_corners[0][1]);
		boundingBox.Union(m_corners[1][0]);
		boundingBox.Union(m_corners[1][1]);

		const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_MOVE: IColorShema::TT_INACTIVE);
		
		boundingBox.Expand(tickerBox);
		boundingBox.Expand(i2d::CRect(istd::CIndex2d(-1, -1), istd::CIndex2d(1, 1)));

		return boundingBox;
	}

	return i2d::CRect();
}


void CInteractiveRectangleShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	m_referencePosition = reference;
}


void CInteractiveRectangleShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		i2d::CRectangle& rectangle = *dynamic_cast<i2d::CRectangle*>(modelPtr);
		I_ASSERT(&rectangle != NULL);

		BeginModelChanges();

		rectangle.Translate(position - m_referencePosition);
		m_referencePosition = position;

		EndModelChanges();
	}
}


} // namespace iview


