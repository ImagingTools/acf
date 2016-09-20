#include <iview/CTubePolylineShape.h>


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <i2d/CTubePolyline.h>
#include <iview/IColorSchema.h>


namespace iview
{


CTubePolylineShape::CTubePolylineShape()
{
	m_isOrientationVisible = false;
	m_isAlwaysDraggable = false;
	m_editedNodeIndex = -1;
	m_editMode = EM_NONE;
}
 

// protected methods

void CTubePolylineShape::DrawCurve(QPainter& drawContext) const
{
	const i2d::CTubePolyline* polylinePtr = dynamic_cast<const i2d::CTubePolyline*>(GetObservedModel());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		const iview::IColorSchema& colorSchema = GetColorSchema();

		int nodesCount = polylinePtr->GetNodesCount();
		if (nodesCount > 1){
			int segmentsCount = polylinePtr->GetSegmentsCount();

			i2d::CVector2d firstKneeVector = polylinePtr->GetKneeVector(0);
			i2d::CVector2d firstPos = polylinePtr->GetNodePos(0);

			bool showOrientation = IsOrientationVisible();

			const i2d::CTubeNode& firstParams = polylinePtr->GetTNodeData(0);
			istd::CRange firstRange = firstParams.GetTubeRange();

			i2d::CVector2d centerPos = firstPos;
			i2d::CVector2d leftPos = firstPos - firstKneeVector * firstRange.GetMinValue();
			i2d::CVector2d rightPos = firstPos - firstKneeVector * firstRange.GetMaxValue();

			i2d::CVector2d prevScreenPoint = GetScreenPosition(centerPos);
			i2d::CVector2d prevLeftScreenPoint = GetScreenPosition(leftPos);
			i2d::CVector2d prevRightScreenPoint = GetScreenPosition(rightPos);

			bool isSelected = IsSelected();
			
			drawContext.save();

			QPen linePen = colorSchema.GetPen(isSelected ? iview::IColorSchema::SP_SELECTED : iview::IColorSchema::SP_NORMAL); 
			QPen tubePen = colorSchema.GetPen(isSelected ? iview::IColorSchema::SP_SELECTED_DASH : iview::IColorSchema::SP_NORMAL_DASH);

			for (int segmentIndex = 0; segmentIndex < segmentsCount; ++segmentIndex){
				int nodeIndex = (segmentIndex + 1) % nodesCount;
				i2d::CVector2d kneeVector = polylinePtr->GetKneeVector(nodeIndex);
				const i2d::CVector2d& nodePosition = polylinePtr->GetNodePos(nodeIndex);

				i2d::CVector2d screenPoint = GetScreenPosition(nodePosition);

				drawContext.setPen(linePen);
				drawContext.drawLine(prevScreenPoint, screenPoint);

				const i2d::CTubeNode& nodeData = polylinePtr->GetTNodeData(nodeIndex);
				istd::CRange range = nodeData.GetTubeRange();

				leftPos = nodePosition - kneeVector * range.GetMinValue();
				rightPos = nodePosition - kneeVector * range.GetMaxValue();

				i2d::CVector2d leftScreenPoint = GetScreenPosition(leftPos);
				i2d::CVector2d rightScreenPoint = GetScreenPosition(rightPos);

				drawContext.setPen(tubePen);
				drawContext.drawLine(prevLeftScreenPoint, leftScreenPoint);
				drawContext.drawLine(prevRightScreenPoint, rightScreenPoint);

				if ((nodePosition.GetDistance(centerPos) > I_BIG_EPSILON) && showOrientation){
					double arrowWidth = qMin(leftPos.GetDistance(nodePosition), rightPos.GetDistance(nodePosition)) * 0.5;

					i2d::CVector2d delta = (nodePosition - centerPos).GetNormalized(arrowWidth);
					i2d::CVector2d orthogonalized = delta.GetOrthogonal();

					i2d::CVector2d screenArrowBegin = GetScreenPosition(nodePosition);
					i2d::CVector2d screenArrowLeft = GetScreenPosition(nodePosition - delta + orthogonalized);
					i2d::CVector2d screenArrowRight = GetScreenPosition(nodePosition - delta - orthogonalized);

					drawContext.drawLine(screenArrowBegin, screenArrowLeft);
					drawContext.drawLine(screenArrowBegin, screenArrowRight);
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


void CTubePolylineShape::DrawSelectionElements(QPainter& drawContext) const
{
	BaseClass::DrawSelectionElements(drawContext);

	int editMode = GetEditMode();
	if (editMode != iview::ISelectable::EM_MOVE){
		return;
	}

	const i2d::CTubePolyline* polylinePtr = dynamic_cast<const i2d::CTubePolyline*>(GetObservedModel());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		const iview::IColorSchema& colorSchema = GetColorSchema();

		int nodesCount = polylinePtr->GetNodesCount();

		for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
			i2d::CVector2d kneeVector = polylinePtr->GetKneeVector(nodeIndex);
			const i2d::CVector2d& nodePosition = polylinePtr->GetNodePos(nodeIndex);

			const i2d::CTubeNode& nodeData = polylinePtr->GetTNodeData(nodeIndex);
			istd::CRange range = nodeData.GetTubeRange();

			i2d::CVector2d screenPoint = GetScreenPosition(nodePosition);

			i2d::CVector2d leftPos = nodePosition - kneeVector * range.GetMinValue();
			i2d::CVector2d rightPos = nodePosition - kneeVector * range.GetMaxValue();
			i2d::CVector2d leftScreenPoint = GetScreenPosition(leftPos);
			i2d::CVector2d rightScreenPoint = GetScreenPosition(rightPos);

			if (true){
				colorSchema.DrawTicker(drawContext, leftScreenPoint.ToIndex2d(), iview::IColorSchema::TT_MOVE);
				colorSchema.DrawTicker(drawContext, rightScreenPoint.ToIndex2d(), iview::IColorSchema::TT_MOVE);

				if ((nodeIndex > 0) && (nodeIndex < nodesCount - 1)){
					colorSchema.DrawTicker(drawContext, screenPoint.ToIndex2d(), iview::IColorSchema::TT_CHECKBOX_ON);
				}
			}
			else{
				colorSchema.DrawTicker(drawContext, screenPoint.ToIndex2d(), iview::IColorSchema::TT_CHECKBOX_OFF);
			}
		}
	}
}


i2d::CRect CTubePolylineShape::CalcBoundingBox() const
{
	i2d::CRect result;
	result.Reset();

	const i2d::CTubePolyline* polylinePtr = dynamic_cast<const i2d::CTubePolyline*>(GetObservedModel());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		int nodesCount = polylinePtr->GetNodesCount();
		if (nodesCount >= 1){
			bool isSelected = IsSelected();

			const iview::IColorSchema& colorSchema = GetColorSchema();

			for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
				i2d::CVector2d kneeVector = polylinePtr->GetKneeVector(nodeIndex);
				const i2d::CVector2d& nodePosition = polylinePtr->GetNodePos(nodeIndex);

				const i2d::CTubeNode& tubeNode = polylinePtr->GetTNodeData(nodeIndex);
				istd::CRange range = tubeNode.GetTubeRange();

				i2d::CVector2d screenPoint = GetScreenPosition(nodePosition);
				result.Union(screenPoint.ToIndex2d());

				if (isSelected){
					i2d::CVector2d leftPos = nodePosition - kneeVector * range.GetMinValue();
					i2d::CVector2d rightPos = nodePosition - kneeVector * range.GetMaxValue();

					i2d::CVector2d leftScreenPoint = GetScreenPosition(leftPos);
					i2d::CVector2d rightScreenPoint = GetScreenPosition(rightPos);

					result.Union(leftScreenPoint.ToIndex2d());
					result.Union(rightScreenPoint.ToIndex2d());
				}
			}
			
			i2d::CRect tickerBox = isSelected ? 
				colorSchema.GetTickerBox(iview::IColorSchema::TT_NORMAL): 
				i2d::CRect(-1, -1, 1, 1);

			result.Expand(tickerBox);
		}
	}

	return result;
}


// reimplemented (iview::IMouseActionObserver)

bool CTubePolylineShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	ShapeBaseClass::OnMouseButton(position, buttonType, downFlag);

	m_editMode = EM_NONE;

	i2d::CTubePolyline* polylinePtr = dynamic_cast<i2d::CTubePolyline*>(GetObservedModel());

	int editMode = GetEditMode();
	if (editMode == iview::ISelectable::EM_MOVE){
		if (IsDisplayConnected() && (polylinePtr != NULL)){
			const iview::IColorSchema& colorSchema = GetColorSchema();

			const i2d::CRect& tickerBoxMove = colorSchema.GetTickerBox(iview::IColorSchema::TT_MOVE);

			int nodesCount = polylinePtr->GetNodesCount();
			for (int nodeIndex = nodesCount - 1; nodeIndex >= 0; --nodeIndex){
				i2d::CVector2d kneeVector = polylinePtr->GetKneeVector(nodeIndex);
				const i2d::CVector2d& nodePosition = polylinePtr->GetNodePos(nodeIndex);

				const i2d::CTubeNode& nodeData = polylinePtr->GetTNodeData(nodeIndex);
				istd::CRange range = nodeData.GetTubeRange();

				i2d::CVector2d leftPos = nodePosition - kneeVector * range.GetMinValue();
				i2d::CVector2d rightPos = nodePosition - kneeVector * range.GetMaxValue();

				i2d::CVector2d leftScreenPoint = GetScreenPosition(leftPos);
				i2d::CVector2d rightScreenPoint = GetScreenPosition(rightPos);

				if (tickerBoxMove.IsInside(position - rightScreenPoint.ToIndex2d())){
					m_editMode = EM_RIGHT;
					m_editedNodeIndex = nodeIndex;

					break;
				}

				if (tickerBoxMove.IsInside(position - leftScreenPoint.ToIndex2d())){
					m_editMode = EM_LEFT;
					m_editedNodeIndex = nodeIndex;

					break;
				}
			}

			if (m_editedNodeIndex >= 0 && (m_editMode == EM_LEFT || m_editMode == EM_RIGHT)){
				if (downFlag){
					BeginTickerDrag();

					return true;
				}

				EndTickerDrag();

				return false;
			}
		}
	}

	if (BaseClass::OnMouseButton(position, buttonType, downFlag)){
		m_editMode = EM_BASIC;

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

	EndTickerDrag();

	return false;
}


bool CTubePolylineShape::OnMouseMove(istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		int editMode = GetEditMode();
		if (editMode == iview::ISelectable::EM_MOVE){
			if (m_editMode == EM_LEFT || m_editMode == EM_RIGHT){
				i2d::CTubePolyline* polylinePtr = dynamic_cast<i2d::CTubePolyline*>(GetObservedModel());
				if (IsDisplayConnected() && (polylinePtr != NULL)){
					i2d::CVector2d kneeVector = polylinePtr->GetKneeVector(m_editedNodeIndex);
					const i2d::CVector2d& nodePosition = polylinePtr->GetNodePos(m_editedNodeIndex);

					i2d::CTubeNode& nodeData = polylinePtr->GetTNodeDataRef(m_editedNodeIndex);
					istd::CRange range = nodeData.GetTubeRange();

					i2d::CVector2d currentPosition = GetLogPosition(position);

					double newWidth = qFabs(kneeVector.GetDotProduct(currentPosition - nodePosition) / kneeVector.GetLength2());

					bool isSynchronized = ((GetKeysState() & Qt::ControlModifier) != 0);
					if (isSynchronized){
						range.SetMinValue(-newWidth);
						range.SetMaxValue(newWidth);
					}
					else if (m_editMode == EM_LEFT){
						range.SetMinValue(-newWidth);
					}
					else{
						range.SetMaxValue(newWidth);
					}

					istd::CChangeNotifier changePtr(polylinePtr);

					nodeData.SetTubeRange(range);

					UpdateModelChanges();

					return true;
				}

				return false;
			}
		}
	}

	if (m_editMode == EM_NONE){
		return true;
	}

	if (BaseClass::OnMouseMove(position)){
		return true;
	}

	return false;
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CTubePolylineShape::IsTouched(istd::CIndex2d position) const
{
	ITouchable::TouchState isTouched = BaseClass::IsTouched(position);
	if (isTouched != TS_NONE){
		return isTouched;
	}

	int editMode = GetEditMode();
	if (editMode != iview::ISelectable::EM_MOVE){
		return TS_NONE;
	}

	const i2d::CTubePolyline* polylinePtr = dynamic_cast<const i2d::CTubePolyline*>(GetObservedModel());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		const iview::IColorSchema& colorSchema = GetColorSchema();

		const i2d::CRect& tickerBoxMove = colorSchema.GetTickerBox(iview::IColorSchema::TT_MOVE);

		int nodesCount = polylinePtr->GetNodesCount();

		for (int nodeIndex = nodesCount - 1; nodeIndex >= 0; --nodeIndex){
			i2d::CVector2d kneeVector = polylinePtr->GetKneeVector(nodeIndex);
			const i2d::CVector2d& nodePosition = polylinePtr->GetNodePos(nodeIndex);

			const i2d::CTubeNode& nodeData = polylinePtr->GetTNodeData(nodeIndex);
			istd::CRange range = nodeData.GetTubeRange();
			bool isActive = true;

			i2d::CVector2d leftPos = nodePosition - kneeVector * range.GetMinValue();
			i2d::CVector2d rightPos = nodePosition - kneeVector * range.GetMaxValue();

			i2d::CVector2d leftScreenPoint = GetScreenPosition(leftPos);
			i2d::CVector2d rightScreenPoint = GetScreenPosition(rightPos);

			if (isActive){
				if (tickerBoxMove.IsInside(position - rightScreenPoint.ToIndex2d())){
					return TS_TICKER;
				}

				if (tickerBoxMove.IsInside(position - leftScreenPoint.ToIndex2d())){
					return TS_TICKER;
				}
			}
		}
	}

	return TS_NONE;
}


} // namespace i2d



