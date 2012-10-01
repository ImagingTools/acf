#include "iview/CInteractiveTubePolylineShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "i2d/CTubePolylineComp.h"

#include "iqt/iqt.h"

#include "iview/IColorShema.h"


namespace iview
{


CInteractiveTubePolylineShape::CInteractiveTubePolylineShape()
{
	m_isOrientationVisible = false;
	m_isAlwaysDraggable = false;
	m_draggedTickerIndex = -1;
	m_draggedTickerType = TT_NONE;
}
 

// protected methods

void CInteractiveTubePolylineShape::DrawCurve(QPainter& drawContext) const
{
	const i2d::CTubePolyline* polylinePtr = dynamic_cast<const i2d::CTubePolyline*>(GetModelPtr());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		const iview::IColorShema& colorShema = GetColorShema();
		int nodesCount = polylinePtr->GetNodesCount();
		if (nodesCount > 1){
			int segmentsCount = polylinePtr->GetSegmentsCount();

			i2d::CVector2d firstKneeVector = polylinePtr->GetKneeVector(0);
			i2d::CVector2d firstPos = polylinePtr->GetNode(0);

			const iview::CScreenTransform& transform = GetLogToScreenTransform();

			bool showOrientation = IsOrientationVisible();

			const i2d::CTubeNode& firstParams = polylinePtr->GetTNodeData(0);
			istd::CRange firstRange = firstParams.GetTubeRange();

			i2d::CVector2d centerPos = firstPos;
			i2d::CVector2d leftPos = firstPos - firstKneeVector * firstRange.GetMinValue();
			i2d::CVector2d rightPos = firstPos - firstKneeVector * firstRange.GetMaxValue();

			istd::CIndex2d prevScreenPoint = transform.GetScreenPosition(centerPos);
			istd::CIndex2d prevLeftScreenPoint = transform.GetScreenPosition(leftPos);
			istd::CIndex2d prevRightScreenPoint = transform.GetScreenPosition(rightPos);

			bool isSelected = IsSelected();
			
			drawContext.save();

			QPen linePen = colorShema.GetPen(isSelected ? iview::IColorShema::SP_SELECTED : iview::IColorShema::SP_NORMAL); 
			QPen tubePen = colorShema.GetPen(isSelected ? iview::IColorShema::SP_SELECTED_DASH : iview::IColorShema::SP_NORMAL_DASH);

			for (int segmentIndex = 0; segmentIndex < segmentsCount; ++segmentIndex){
				int nodeIndex = (segmentIndex + 1) % nodesCount;
				i2d::CVector2d kneeVector = polylinePtr->GetKneeVector(nodeIndex);
				const i2d::CVector2d& nodePosition = polylinePtr->GetNode(nodeIndex);

				istd::CIndex2d screenPoint = transform.GetScreenPosition(nodePosition);

				drawContext.setPen(linePen);
				drawContext.drawLine(iqt::GetQPoint(prevScreenPoint), iqt::GetQPoint(screenPoint));

				const i2d::CTubeNode& nodeData = polylinePtr->GetTNodeData(nodeIndex);
				istd::CRange range = nodeData.GetTubeRange();

				leftPos = nodePosition - kneeVector * range.GetMinValue();
				rightPos = nodePosition - kneeVector * range.GetMaxValue();
				istd::CIndex2d leftScreenPoint = transform.GetScreenPosition(leftPos);
				istd::CIndex2d rightScreenPoint = transform.GetScreenPosition(rightPos);

				drawContext.setPen(tubePen);
				drawContext.drawLine(iqt::GetQPoint(prevLeftScreenPoint), iqt::GetQPoint(leftScreenPoint));
				drawContext.drawLine(iqt::GetQPoint(prevRightScreenPoint), iqt::GetQPoint(rightScreenPoint));

				if ((nodePosition.GetDistance(centerPos) > I_BIG_EPSILON) && showOrientation){
					double arrowWidth = qMin(leftPos.GetDistance(nodePosition), rightPos.GetDistance(nodePosition)) * 0.5;

					i2d::CVector2d delta = (nodePosition - centerPos).GetNormalized(arrowWidth);
					i2d::CVector2d orthogonalized = delta.GetOrthogonal();

					istd::CIndex2d screenArrowBegin = transform.GetScreenPosition(nodePosition);
					istd::CIndex2d screenArrowLeft = transform.GetScreenPosition(nodePosition - delta + orthogonalized);
					istd::CIndex2d screenArrowRight = transform.GetScreenPosition(nodePosition - delta - orthogonalized);

					drawContext.drawLine(iqt::GetQPoint(screenArrowBegin), iqt::GetQPoint(screenArrowLeft));
					drawContext.drawLine(iqt::GetQPoint(screenArrowBegin), iqt::GetQPoint(screenArrowRight));
				}

				prevLeftScreenPoint = leftScreenPoint;
				prevRightScreenPoint = rightScreenPoint;

				prevScreenPoint = screenPoint;
				centerPos = nodePosition;
			}

			drawContext.restore();
		}
	}
}


void CInteractiveTubePolylineShape::DrawSelectionElements(QPainter& drawContext) const
{
	BaseClass::DrawSelectionElements(drawContext);

	int editMode = GetEditMode();
	if (editMode != iview::ISelectable::EM_MOVE){
		return;
	}

	const i2d::CTubePolyline* polylinePtr = dynamic_cast<const i2d::CTubePolyline*>(GetModelPtr());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		const iview::IColorShema& colorShema = GetColorShema();

		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		int nodesCount = polylinePtr->GetNodesCount();

		i2d::CVector2d centerPos;
		i2d::CVector2d leftPos;
		i2d::CVector2d rightPos;

		for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
			i2d::CVector2d kneeVector = polylinePtr->GetKneeVector(nodeIndex);
			const i2d::CVector2d& nodePosition = polylinePtr->GetNode(nodeIndex);

			const i2d::CTubeNode& nodeData = polylinePtr->GetTNodeData(nodeIndex);
			istd::CRange range = nodeData.GetTubeRange();

			istd::CIndex2d screenPoint = transform.GetScreenPosition(nodePosition);

			leftPos = nodePosition - kneeVector * range.GetMinValue();
			rightPos = nodePosition - kneeVector * range.GetMaxValue();
			istd::CIndex2d leftScreenPoint = transform.GetScreenPosition(leftPos);
			istd::CIndex2d rightScreenPoint = transform.GetScreenPosition(rightPos);

			if (true){
				colorShema.DrawTicker(drawContext, leftScreenPoint, iview::IColorShema::TT_MOVE);
				colorShema.DrawTicker(drawContext, rightScreenPoint, iview::IColorShema::TT_MOVE);
				if ((nodeIndex > 0) && (nodeIndex < nodesCount - 1)){
					colorShema.DrawTicker(drawContext, screenPoint, iview::IColorShema::TT_CHECKBOX_ON);
				}
			}
			else{
				colorShema.DrawTicker(drawContext, screenPoint, iview::IColorShema::TT_CHECKBOX_OFF);
			}
		}
	}
}


i2d::CRect CInteractiveTubePolylineShape::CalcBoundingBox() const
{
	i2d::CRect result;
	result.Reset();

	const i2d::CTubePolyline* polylinePtr = dynamic_cast<const i2d::CTubePolyline*>(GetModelPtr());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		int nodesCount = polylinePtr->GetNodesCount();
		if (nodesCount >= 1){
			i2d::CVector2d firstPos = polylinePtr->GetNode(0);

			const iview::CScreenTransform& transform = GetLogToScreenTransform();

			bool isSelected = IsSelected();

			const iview::IColorShema& colorShema = GetColorShema();

			for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
				i2d::CVector2d kneeVector = polylinePtr->GetKneeVector(nodeIndex);
				const i2d::CVector2d& nodePosition = polylinePtr->GetNode(nodeIndex);

				const i2d::CTubeNode& tubeNode = polylinePtr->GetTNodeData(nodeIndex);
				istd::CRange range = tubeNode.GetTubeRange();

				istd::CIndex2d screenPoint = transform.GetScreenPosition(nodePosition);
				result.Union(screenPoint);

				if (isSelected){
					i2d::CVector2d leftPos = nodePosition - kneeVector * range.GetMinValue();
					i2d::CVector2d rightPos = nodePosition - kneeVector * range.GetMaxValue();
					istd::CIndex2d leftScreenPoint = transform.GetScreenPosition(leftPos);
					istd::CIndex2d rightScreenPoint = transform.GetScreenPosition(rightPos);

					result.Union(leftScreenPoint);
					result.Union(rightScreenPoint);
				}
			}
			
			i2d::CRect tickerBox = isSelected ? 
				colorShema.GetTickerBox(iview::IColorShema::TT_NORMAL): 
				i2d::CRect(-1, -1, 1, 1);

			result.Expand(tickerBox);
		}
	}

	return result;
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractiveTubePolylineShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	m_draggedTickerType = TT_NONE;

	i2d::CTubePolyline* polylinePtr = dynamic_cast<i2d::CTubePolyline*>(GetModelPtr());

	int editMode = GetEditMode();
	if (editMode == iview::ISelectable::EM_MOVE){
		if (IsDisplayConnected() && (polylinePtr != NULL)){
			const iview::CScreenTransform& transform = GetLogToScreenTransform();

			const iview::IColorShema& colorShema = GetColorShema();
			const i2d::CRect& tickerBoxMove = colorShema.GetTickerBox(iview::IColorShema::TT_MOVE);

			int nodesCount = polylinePtr->GetNodesCount();
			for (int nodeIndex = nodesCount - 1; nodeIndex >= 0; --nodeIndex){
				i2d::CVector2d kneeVector = polylinePtr->GetKneeVector(nodeIndex);
				const i2d::CVector2d& nodePosition = polylinePtr->GetNode(nodeIndex);

				const i2d::CTubeNode& nodeData = polylinePtr->GetTNodeData(nodeIndex);
				istd::CRange range = nodeData.GetTubeRange();

				istd::CIndex2d screenPoint = transform.GetScreenPosition(nodePosition);

				i2d::CVector2d leftPos = nodePosition - kneeVector * range.GetMinValue();
				i2d::CVector2d rightPos = nodePosition - kneeVector * range.GetMaxValue();
				istd::CIndex2d leftScreenPoint = transform.GetScreenPosition(leftPos);
				istd::CIndex2d rightScreenPoint = transform.GetScreenPosition(rightPos);

				if (tickerBoxMove.GetTranslated(rightScreenPoint).IsInside(position)){
					m_draggedTickerType = TT_RIGHT;
					m_draggedTickerIndex = nodeIndex;
					break;
				}

				if (tickerBoxMove.GetTranslated(leftScreenPoint).IsInside(position)){
					m_draggedTickerType = TT_LEFT;
					m_draggedTickerIndex = nodeIndex;
					break;
				}
			}

			if (m_draggedTickerIndex >= 0 && (m_draggedTickerType == TT_LEFT || m_draggedTickerType == TT_RIGHT)){
				if (downFlag){
					BeginModelChanges();

					return true;
				}

				EndModelChanges();

				return false;
			}
		}
	}

	if (BaseClass::OnMouseButton(position, buttonType, downFlag)){
		m_draggedTickerType = TT_BASIC;

		if (editMode == iview::ISelectable::EM_ADD){
			i2d::CTubeNode& nodeData = polylinePtr->GetTNodeDataRef(m_referenceIndex);
			istd::CRange range = nodeData.GetTubeRange();			

			const istd::CRange leftRange = m_referenceIndex > 0 ? 
				polylinePtr->GetTNodeData(m_referenceIndex-1).GetTubeRange() : 
				polylinePtr->GetTNodeData(m_referenceIndex+1).GetTubeRange();

			int nodesCount = polylinePtr->GetNodesCount();
			const istd::CRange rightRange = m_referenceIndex < nodesCount-1 ? 
				polylinePtr->GetTNodeData(m_referenceIndex+1).GetTubeRange() : 
				polylinePtr->GetTNodeData(m_referenceIndex-1).GetTubeRange();

			range.SetInterpolated(leftRange, rightRange, 0.5);

			nodeData.SetTubeRange(range);

			UpdateModelChanges();
		}

		return true;
	}

	EndModelChanges();

	return false;
}


bool CInteractiveTubePolylineShape::OnMouseMove(istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		int editMode = GetEditMode();
		if (editMode == iview::ISelectable::EM_MOVE){
			if (m_draggedTickerType == TT_LEFT || m_draggedTickerType == TT_RIGHT){
				i2d::CTubePolyline* polylinePtr = dynamic_cast<i2d::CTubePolyline*>(GetModelPtr());
				if (IsDisplayConnected() && (polylinePtr != NULL)){
					i2d::CVector2d kneeVector = polylinePtr->GetKneeVector(m_draggedTickerIndex);
					const i2d::CVector2d& nodePosition = polylinePtr->GetNode(m_draggedTickerIndex);

					i2d::CTubeNode& nodeData = polylinePtr->GetTNodeDataRef(m_draggedTickerIndex);
					istd::CRange range = nodeData.GetTubeRange();

					const iview::CScreenTransform& transform = GetLogToScreenTransform();
					i2d::CVector2d currentPosition = transform.GetClientPosition(position);

					i2d::CVector2d rightPos = nodePosition - kneeVector * range.GetMaxValue();
					i2d::CVector2d leftPos = nodePosition - kneeVector * range.GetMinValue();

					int keysState = GetKeysState();
					bool isSynchronized = ((keysState & Qt::ControlModifier) != 0);

					if (m_draggedTickerType == TT_LEFT){
						i2d::CVector2d newPoint = i2d::CLine2d(leftPos, nodePosition).GetExtendedNearestPoint(currentPosition);
						double newWidth = qMax(0.1, newPoint.GetDistance(nodePosition));

						// check if in the range
						double widthRightNew = rightPos.GetDistance(newPoint);
						double widthRightCenter = rightPos.GetDistance(nodePosition);
						if (widthRightNew + 0.1 < widthRightCenter + newWidth){
							newWidth = 0.1;
						}

						range.SetMinValue(-newWidth);
						if (isSynchronized){
							range.SetMaxValue(newWidth);
						}
					}
					else{
						i2d::CVector2d newPoint = i2d::CLine2d(rightPos, nodePosition).GetExtendedNearestPoint(currentPosition);
						double newWidth = qMax(0.1, newPoint.GetDistance(nodePosition));

						// check if in the range
						double widthLeftNew = leftPos.GetDistance(newPoint);
						double widthLeftCenter = leftPos.GetDistance(nodePosition);
						if (widthLeftNew + 0.1 < widthLeftCenter + newWidth){
							newWidth = 0.1;
						}

						range.SetMaxValue(newWidth);
						if (isSynchronized){
							range.SetMinValue(-newWidth);
						}
					}

					nodeData.SetTubeRange(range);

					UpdateModelChanges();

					return true;
				}

				return false;
			}
		}
	}

	if (m_draggedTickerType == TT_NONE){
		return true;
	}

	if (BaseClass::OnMouseMove(position)){
		return true;
	}

	return false;
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CInteractiveTubePolylineShape::IsTouched(istd::CIndex2d position) const
{
	ITouchable::TouchState isTouched = BaseClass::IsTouched(position);
	if (isTouched != TS_NONE){
		return isTouched;
	}

	int editMode = GetEditMode();
	if (editMode != iview::ISelectable::EM_MOVE){
		return TS_NONE;
	}

	const i2d::CTubePolyline* polylinePtr = dynamic_cast<const i2d::CTubePolyline*>(GetModelPtr());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		const iview::IColorShema& colorShema = GetColorShema();

		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		//const i2d::CRect& tickerBoxCheckboxOn = colorShema.GetTickerBox(iview::IColorShema::TT_CHECKBOX_ON);
		//const i2d::CRect& tickerBoxCheckboxOff = colorShema.GetTickerBox(iview::IColorShema::TT_CHECKBOX_OFF);
		const i2d::CRect& tickerBoxMove = colorShema.GetTickerBox(iview::IColorShema::TT_MOVE);

		int nodesCount = polylinePtr->GetNodesCount();

		for (int nodeIndex = nodesCount - 1; nodeIndex >= 0; --nodeIndex){
			i2d::CVector2d kneeVector = polylinePtr->GetKneeVector(nodeIndex);
			const i2d::CVector2d& nodePosition = polylinePtr->GetNode(nodeIndex);

			const i2d::CTubeNode& nodeData = polylinePtr->GetTNodeData(nodeIndex);
			istd::CRange range = nodeData.GetTubeRange();
			bool isActive = true;

			istd::CIndex2d screenPoint = transform.GetScreenPosition(nodePosition);

			i2d::CVector2d leftPos = nodePosition - kneeVector * range.GetMinValue();
			i2d::CVector2d rightPos = nodePosition - kneeVector * range.GetMaxValue();
			istd::CIndex2d leftScreenPoint = transform.GetScreenPosition(leftPos);
			istd::CIndex2d rightScreenPoint = transform.GetScreenPosition(rightPos);

			if (isActive){
				//if ((nodeIndex > 0) && (nodeIndex < nodesCount - 1)){
				//	if (tickerBoxCheckboxOn.GetTranslated(screenPoint).IsInside(position)){
				//		return TS_TICKER;
				//	}
				//}

				if (tickerBoxMove.GetTranslated(rightScreenPoint).IsInside(position)){
					return TS_TICKER;
				}

				if (tickerBoxMove.GetTranslated(leftScreenPoint).IsInside(position)){
					return TS_TICKER;
				}
			}
			//else{
			//	if (tickerBoxCheckboxOff.GetTranslated(screenPoint).IsInside(position)){
			//		return TS_TICKER;
			//	}
			//}
		}
	}

	return TS_NONE;
}


} // namespace i2d



