#include <iview/CDistanceToolShape.h>


// Qt includes
#include <QPainter>

// ACF includes
#include <iview/IToolsLayerInfo.h>
#include <iview/IColorSchema.h>
#include <iview/ISelectable.h>
#include <iview/CScreenTransform.h>
#include <i2d/CLine2d.h>


namespace iview
{


CDistanceToolShape::CDistanceToolShape()
	:m_measureState(CDistanceToolShape::MS_NONE),
	m_dragNodeIndex(-1),
	m_isSelected(false)
{
}


// reimplemented (iview::IMouseActionObserver)

bool CDistanceToolShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	m_dragNodeIndex = -1;

	if (!IsDistanceToolActive()){
		return false;
	}

	i2d::CLine2d* linePtr = dynamic_cast<i2d::CLine2d*>(GetObservedModel());
	if (linePtr == NULL){
		return false;
	}

	if (buttonType != Qt::LeftButton){
		m_measureState = CDistanceToolShape::MS_NONE;
		m_dragNodeIndex = -1;
		linePtr->GetPoint1Ref().Reset(); 
		linePtr->GetPoint2Ref().Reset();
		EndDrag();
		return false;
	}

	const iview::IColorSchema& colorSchema = GetColorSchema();
	const i2d::CRect& tickerBoxMove = colorSchema.GetTickerBox(iview::IColorSchema::TT_MOVE);

	if (downFlag){
		if(m_measureState == CDistanceToolShape::MS_DONE){
			const i2d::CVector2d& screenPoint1 = GetScreenPosition(linePtr->GetPoint1Ref());
			if (tickerBoxMove.IsInside(position - screenPoint1.ToIndex2d())){
				m_dragNodeIndex = 0;
			}

			const i2d::CVector2d& screenPoint2 = GetScreenPosition(linePtr->GetPoint2Ref());
			if (tickerBoxMove.IsInside(position - screenPoint2.ToIndex2d())){
				m_dragNodeIndex = 1;
			}

			if (m_dragNodeIndex > -1){
				BeginDrag();
				return true;
			}
		}

		if(m_measureState != CDistanceToolShape::MS_START){
			m_screenPos = position;
		}

		BeginDrag();
		return false;
	}
	else{
		if (tickerBoxMove.IsInside(position - m_screenPos)) {
			if (m_measureState != CDistanceToolShape::MS_START){
				const i2d::CVector2d beginPos = GetLogPosition(m_screenPos);
				linePtr->SetPoint1(beginPos);
				linePtr->SetPoint2(beginPos);
				m_measureState = CDistanceToolShape::MS_START;
				return true;
			}

			if (m_measureState == CDistanceToolShape::MS_START){
				const i2d::CVector2d endPos = GetLogPosition(position);
				linePtr->SetPoint2(endPos);
				m_measureState = CDistanceToolShape::MS_DONE;
				EndDrag();
				return true;
			}
		}
	}

	return false;
}

bool CDistanceToolShape::OnMouseMove(istd::CIndex2d position)
{
	if (!IsDistanceToolActive()){
		m_measureState = CDistanceToolShape::MS_NONE;
		m_dragNodeIndex = -1;
		return false;
	}

	i2d::CLine2d* linePtr = dynamic_cast<i2d::CLine2d*>(GetObservedModel());
	if (linePtr == NULL){
		return false;
	}

	if (m_dragNodeIndex > -1){
		if (m_dragNodeIndex == 0){
			linePtr->SetPoint1(GetLogPosition(position));
		}
		else{
			linePtr->SetPoint2(GetLogPosition(position));
		}

		Invalidate();
		return true;
	}

	if (m_measureState != CDistanceToolShape::MS_START){
		return false;
	}

	m_screenPos = position;
	linePtr->SetPoint2(GetLogPosition(position));
	Invalidate();

	return true;
}


// reimplemented (iview::IInteractiveShape)

bool CDistanceToolShape::IsSelected() const
{
	return m_isSelected;
}


void CDistanceToolShape::SetSelected(bool selectFlag)
{
	if (m_isSelected != selectFlag){
		m_isSelected = selectFlag;

		Invalidate();
	
		iview::ISelectable* observerPtr = dynamic_cast<iview::ISelectable*>(GetDisplayPtr());
		if (observerPtr != NULL){
			observerPtr->OnShapeSelected(*this, selectFlag);
		}
	}
}


// reimplemented (imod::IObserver)

bool CDistanceToolShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CLine2d*>(modelPtr) != NULL);
	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CDistanceToolShape::IsTouched(istd::CIndex2d position) const
{
	i2d::CLine2d* linePtr = dynamic_cast<i2d::CLine2d*>(GetObservedModel());
	if (linePtr == NULL){
		return TS_NONE;
	}

	const iview::IColorSchema& colorSchema = GetColorSchema();
	const i2d::CRect& tickerBoxMove = colorSchema.GetTickerBox(iview::IColorSchema::TT_MOVE);

	i2d::CVector2d screenPoint1 = GetScreenPosition(linePtr->GetPoint1());
	if (tickerBoxMove.IsInside(position - screenPoint1.ToIndex2d())){
		return TS_TICKER;
	}

	i2d::CVector2d screenPoint2 = GetScreenPosition(linePtr->GetPoint2());
	if (tickerBoxMove.IsInside(position - screenPoint2.ToIndex2d())){
		return TS_TICKER;
	}

	return TS_NONE;
}


// protected methods

const i2d::ICalibration2d* CDistanceToolShape::GetCalibration() const
{
	i2d::CLine2d* linePtr = dynamic_cast<i2d::CLine2d*>(GetObservedModel());
	if (linePtr == NULL){
		return NULL;
	}

	const i2d::ICalibration2d* calibrationPtr = linePtr->GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr;
	}

	const iview::IDisplay* displayPtr = GetDisplayPtr();
	while (displayPtr != NULL){
		const i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<const i2d::ICalibrationProvider*>(displayPtr);
		if ((calibrationProviderPtr != NULL)){
			return calibrationProviderPtr->GetCalibration();
		}

		displayPtr = displayPtr->GetParentDisplayPtr();
	}

	return NULL;
}


void CDistanceToolShape::DrawLabel(QPainter& drawContext) const
{
	i2d::CLine2d* linePtr = dynamic_cast<i2d::CLine2d*>(GetObservedModel());
	if (linePtr == NULL){
		return;
	}
	const i2d::CLine2d screenLine(GetScreenPosition(linePtr->GetPoint1()), GetScreenPosition(linePtr->GetPoint2()));
	const i2d::CVector2d diffVect = screenLine.GetDiffVector();

	drawContext.save();
	const int fontSize = 8;
	const int paddings = 5;
	const QPointF paddingDisp(paddings, paddings);
	const QFont font = QFont("Arial", fontSize, QFont::Bold);

	const i2d::ICalibration2d* calibration = GetCalibration();
	const QString label = calibration != NULL ? QObject::tr("%1 mm") : QObject::tr("%1 px");
	double length = linePtr->GetLength();

	if (calibration != NULL && linePtr->GetCalibration() == NULL){
		i2d::CLine2d decalibratedLine;
		if (linePtr->GetInvTransformed(*calibration, decalibratedLine)){
			length = decalibratedLine.GetLength();
		}
	}

	const QString lengthLabel = label.arg(QString::number(length, 'f', 1));

	const QFontMetricsF fm(font);
	const QSizeF pixelsSize = fm.size(Qt::TextSingleLine, lengthLabel);
	const float textRectX = diffVect.GetX() > 0 ? 
		screenLine.GetPoint2().GetX() + 15 + paddings : 
		screenLine.GetPoint2().GetX() - 15 - paddings - pixelsSize.width();
	const QRectF textRect(QPointF(textRectX, screenLine.GetPoint2().GetY() - 0.5*pixelsSize.height()), pixelsSize);
	const QRectF backRect(textRect.topLeft() - paddingDisp, textRect.bottomRight() + paddingDisp);
	drawContext.setBrush(QColor::fromRgba(qRgba(32, 32, 32, 150)));
	QPen rectPen(QColor::fromRgb(20, 20, 20));
	drawContext.setPen(rectPen);

	drawContext.drawRoundRect(backRect, 5, 5);

	drawContext.setBrush(Qt::lightGray);
	drawContext.setPen(Qt::lightGray);
	drawContext.setFont(font);

	drawContext.drawText(textRect, lengthLabel);

	drawContext.restore();
}


// reimplemented (iview::IVisualizable)

void CDistanceToolShape::Draw(QPainter& drawContext) const
{
	if (!IsDisplayConnected()){
		return;
	}

	i2d::CLine2d* linePtr = dynamic_cast<i2d::CLine2d*>(GetObservedModel());
	if (linePtr == NULL){
		return;
	}

	if (!IsDistanceToolActive()){
		linePtr->GetPoint1Ref().Reset(); 
		linePtr->GetPoint2Ref().Reset();
		return;
	}

	if (linePtr->IsNull() || m_measureState == CDistanceToolShape::MS_NONE){
		return;
	}

	// draw 
	drawContext.save();
	const iview::IColorSchema& colorSchema = GetColorSchema();

	const i2d::CLine2d screenLine(GetScreenPosition(linePtr->GetPoint1()), GetScreenPosition(linePtr->GetPoint2()));
	const i2d::CVector2d screenLineDir = screenLine.GetDiffVector().GetNormalized();
	const i2d::CVector2d screenLineOrt = screenLineDir.GetOrthogonal();
	const int tickerSize = 10;

	drawContext.setPen(colorSchema.GetPen(iview::IColorSchema::SP_TICKER));
	drawContext.drawLine(screenLine.GetPoint1(), screenLine.GetPoint1() - tickerSize*screenLineDir);
	drawContext.drawLine(screenLine.GetPoint1() - tickerSize*screenLineOrt, screenLine.GetPoint1() + tickerSize*screenLineOrt);
	drawContext.drawLine(screenLine.GetPoint2(), screenLine.GetPoint2() + tickerSize*screenLineDir);
	drawContext.drawLine(screenLine.GetPoint2() - tickerSize*screenLineOrt, screenLine.GetPoint2() + tickerSize*screenLineOrt);

	const int partLength = 10;
	const double screenLength = screenLine.GetLength();
	const double size = screenLength / partLength;
	const double partAlphaStep = 1.0/size;

	int i = 0;
	double currAlpha = 0;
	i2d::CVector2d firstPos = linePtr->GetPositionFromAlpha(currAlpha);
	i2d::CVector2d& prevPos = firstPos;
	QPen linePen;
	linePen.setWidth(3);

	for (currAlpha = partAlphaStep; currAlpha < 1.0; currAlpha += partAlphaStep, i++){
		linePen.setColor(i%2 ? Qt::lightGray : Qt::black);
		drawContext.setPen(linePen);
		i2d::CVector2d pos = linePtr->GetPositionFromAlpha(currAlpha);
		drawContext.drawLine(GetScreenPosition(prevPos), GetScreenPosition(pos));
		prevPos = pos;
	}

	linePen.setColor(i%2 ? Qt::lightGray : Qt::black);
	drawContext.setPen(linePen);
	drawContext.drawLine(GetScreenPosition(prevPos), GetScreenPosition(linePtr->GetPositionFromAlpha(1.0)));

	drawContext.restore();

	DrawLabel(drawContext);
}


// reimplemented (iview::CShapeBase)

i2d::CRect CDistanceToolShape::CalcBoundingBox() const
{
	return GetClientRect();
}


// private methods

bool CDistanceToolShape::IsDistanceToolActive() const
{
	iview::IToolsLayerInfo* rulerInfoPtr = NULL;
	iview::IDisplay* displayPtr = GetDisplayPtr();
	while (displayPtr != NULL){
		if (rulerInfoPtr == NULL){
			rulerInfoPtr = dynamic_cast<iview::IToolsLayerInfo*>(displayPtr);
		}
		displayPtr = displayPtr->GetParentDisplayPtr();
	}

	if (rulerInfoPtr != NULL && rulerInfoPtr->IsDistanceToolActive()){
		return true;
	}

	return false;
}


void CDistanceToolShape::BeginDrag()
{
	ISelectable* controllerPtr = dynamic_cast<ISelectable*>(GetDisplayPtr());
	if (controllerPtr != NULL){
		controllerPtr->OnShapeFocused(this);
	}
}


void CDistanceToolShape::EndDrag()
{
	Invalidate();

	ISelectable* controllerPtr = dynamic_cast<ISelectable*>(GetDisplayPtr());
	if (controllerPtr != NULL){
		controllerPtr->OnShapeDefocused(this);
	}
}


} // namespace iview


