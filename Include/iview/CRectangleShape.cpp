#include <iview/CRectangleShape.h>


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <imod/IModel.h>
#include <i2d/CLine2d.h>
#include <iqt/iqt.h>
#include <iview/IColorSchema.h>


namespace iview
{


CRectangleShape::CRectangleShape()
	:m_referencePosition(0.0, 0.0),
	m_editNode(EN_NONE),
	m_arePointsValid(false)
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

	const i2d::CRectangle* rectanglePtr = dynamic_cast<const i2d::CRectangle*>(GetObservedModel());
	if (rectanglePtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		if (!m_arePointsValid){
			CalcPoints(*rectanglePtr);
		}

		if (IsSelected()){
			i2d::CRect tbox = colorSchema.GetTickerBox(IColorSchema::TT_MOVE);

			if (		tbox.IsInside(position - m_corners[0].ToIndex2d()) ||
						tbox.IsInside(position - m_corners[1].ToIndex2d()) ||
						tbox.IsInside(position - m_corners[2].ToIndex2d()) ||
						tbox.IsInside(position - m_corners[3].ToIndex2d())){
				return TS_TICKER;
			}
		}

		double localLineWidth = GetLocalLineWidth(position);

		i2d::CVector2d cp = GetLogPosition(position);

		const i2d::CRectangle& modelArea = *rectanglePtr;

		bool isEditablePosition = IsEditablePosition();

		i2d::CLine2d line(modelArea.GetLeftTop(), modelArea.GetRightTop());
		if (line.GetDistance(cp) < localLineWidth){
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
		line.SetPoint1(modelArea.GetRightBottom());
		if (line.GetDistance(cp) < localLineWidth){
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
		line.SetPoint2(modelArea.GetLeftBottom());
		if (line.GetDistance(cp) < localLineWidth){
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
		line.SetPoint1(modelArea.GetLeftTop());
		if (line.GetDistance(cp) < localLineWidth){
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
	}

	return TS_NONE;
}


// reimplemented (iview::IShape)

void CRectangleShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CRectangle* framePtr = dynamic_cast<const i2d::CRectangle*>(GetObservedModel());
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

		QBrush emptyBrush(QColor(0, 0, 0), Qt::NoBrush);
		drawContext.setBrush(emptyBrush);

		QPainterPath painterPatch;

		painterPatch.moveTo(m_corners[0]);
		painterPatch.cubicTo(m_ctrlsRight[0], m_ctrlsLeft[1], m_corners[1]);
		painterPatch.cubicTo(m_ctrlsRight[1], m_ctrlsLeft[2], m_corners[2]);
		painterPatch.cubicTo(m_ctrlsRight[2], m_ctrlsLeft[3], m_corners[3]);
		painterPatch.cubicTo(m_ctrlsRight[3], m_ctrlsLeft[0], m_corners[0]);
		painterPatch.closeSubpath();

		drawContext.drawPath(painterPatch);

		if (IsSelected()){
			colorSchema.DrawTicker(drawContext, m_corners[0].ToIndex2d(), IColorSchema::TT_MOVE);
			colorSchema.DrawTicker(drawContext, m_corners[1].ToIndex2d(), IColorSchema::TT_MOVE);
			colorSchema.DrawTicker(drawContext, m_corners[2].ToIndex2d(), IColorSchema::TT_MOVE);
			colorSchema.DrawTicker(drawContext, m_corners[3].ToIndex2d(), IColorSchema::TT_MOVE);
		}

		drawContext.restore();
	}
}


// reimplemented (imod::IObserver)

bool CRectangleShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CRectangle*>(modelPtr) != NULL);

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// reimplemented (iview::IMouseActionObserver)

bool CRectangleShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	BaseClass::OnMouseButton(position, buttonType, downFlag);

	const i2d::CRectangle* framePtr = dynamic_cast<const i2d::CRectangle*>(GetObservedModel());
	if (framePtr != NULL){
		if (downFlag){
			const IColorSchema& colorSchema = GetColorSchema();
			if (!m_arePointsValid){
				CalcPoints(*framePtr);
			}

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_MOVE);

			if (tickerBox.IsInside(position - m_corners[0].ToIndex2d())){
				m_editNode = EN_CORNER11;
			}
			else if (tickerBox.IsInside(position - m_corners[1].ToIndex2d())){
				m_editNode = EN_CORNER12;
			}
			else if (tickerBox.IsInside(position - m_corners[2].ToIndex2d())){
				m_editNode = EN_CORNER22;
			}
			else if (tickerBox.IsInside(position - m_corners[3].ToIndex2d())){
				m_editNode = EN_CORNER21;
			}
			else{
				EndTickerDrag();

				return false;
			}
			m_referencePosition = GetLogPosition(position);

			BeginTickerDrag();

			return true;
		}
	}

	EndTickerDrag();

	return false;
}


bool CRectangleShape::OnMouseMove(istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		i2d::CRectangle& modelArea = *dynamic_cast<i2d::CRectangle*>(modelPtr);
		Q_ASSERT(&modelArea != NULL);

		i2d::CVector2d cp = GetLogPosition(position);

		i2d::CVector2d newPos;
		i2d::CVector2d deltaTranslate = cp - m_referencePosition;

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
	m_corners[0] = GetScreenPosition(rectangle.GetLeftTop());
	m_corners[1] = GetScreenPosition(rectangle.GetRightTop());
	m_corners[2] = GetScreenPosition(rectangle.GetRightBottom());
	m_corners[3] = GetScreenPosition(rectangle.GetLeftBottom());
	m_ctrlsLeft[0] = GetScreenPosition(rectangle.GetLeftTop() * 0.99 + rectangle.GetLeftBottom() * 0.01);
	m_ctrlsLeft[1] = GetScreenPosition(rectangle.GetRightTop() * 0.99 + rectangle.GetLeftTop() * 0.01);
	m_ctrlsLeft[2] = GetScreenPosition(rectangle.GetRightBottom() * 0.99 + rectangle.GetRightTop() * 0.01);
	m_ctrlsLeft[3] = GetScreenPosition(rectangle.GetLeftBottom() * 0.99 + rectangle.GetRightBottom() * 0.01);
	m_ctrlsRight[0] = GetScreenPosition(rectangle.GetLeftTop() * 0.99 + rectangle.GetRightTop() * 0.01);
	m_ctrlsRight[1] = GetScreenPosition(rectangle.GetRightTop() * 0.99 + rectangle.GetRightBottom() * 0.01);
	m_ctrlsRight[2] = GetScreenPosition(rectangle.GetRightBottom() * 0.99 + rectangle.GetLeftBottom() * 0.01);
	m_ctrlsRight[3] = GetScreenPosition(rectangle.GetLeftBottom() * 0.99 + rectangle.GetLeftTop() * 0.01);

	for (int i = 0; i < 4; ++i){
		m_ctrlsLeft[i] += (m_ctrlsLeft[i] - m_corners[i]) * 42;
		m_ctrlsRight[i] += (m_ctrlsRight[i] - m_corners[i]) * 42;
	}

	m_arePointsValid = true;
}


// reimplemented (iview::CInteractiveShapeBase)

void CRectangleShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	m_referencePosition = reference;
}


void CRectangleShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		i2d::CRectangle& rectangle = *dynamic_cast<i2d::CRectangle*>(modelPtr);
		Q_ASSERT(&rectangle != NULL);

		rectangle.Translate(position - m_referencePosition);
		m_referencePosition = position;

		UpdateModelChanges();
	}
}


// reimplemented (iview::CShapeBase)

i2d::CRect CRectangleShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CRectangle* framePtr = dynamic_cast<const i2d::CRectangle*>(GetObservedModel());
	if (framePtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();
		if (!m_arePointsValid){
			CalcPoints(*framePtr);
		}

		i2d::CRect boundingBox(m_corners[0].ToIndex2d(), m_corners[0].ToIndex2d());
		for (int i = 0; i < 4; ++i){
			boundingBox.Union(m_corners[i].ToIndex2d());
			boundingBox.Union(m_ctrlsLeft[i].ToIndex2d());
			boundingBox.Union(m_ctrlsRight[i].ToIndex2d());
		}

		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_MOVE: IColorSchema::TT_INACTIVE);

		boundingBox.Expand(tickerBox);
		boundingBox.Expand(i2d::CRect(istd::CIndex2d(-1, -1), istd::CIndex2d(1, 1)));

		return boundingBox;
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview


