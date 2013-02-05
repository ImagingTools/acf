#include "iview/CPolygonCalibrationShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "imod/TModelWrap.h"

#include "i2d/ITransformation2d.h"
#include "i2d/CPolygon.h"

#include "iqt/iqt.h"

#include "iview/IColorSchema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CPolygonCalibrationShape::CPolygonCalibrationShape()
	:m_castAxis(1, 0)
{
	m_referenceIndex = 0;
	m_isFirstVisible = false;
	m_isAlwaysDraggable = false;
}


// reimplemented (iview::IMouseActionObserver)

bool CPolygonCalibrationShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	imod::IModel* modelPtr = GetModelPtr();
	i2d::CPolygon* polygonPtr = dynamic_cast<i2d::CPolygon*>(modelPtr);
	if (IsDisplayConnected() && (polygonPtr != NULL)){
		const IColorSchema& colorSchema = GetColorSchema();

		if (downFlag){
			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			const i2d::ITransformation2d& calib = GetIsomorphCalib();

			i2d::CVector2d viewMouse = transform.GetClientPosition(position);
			i2d::CVector2d logMouse;
			calib.GetPositionAt(viewMouse, logMouse);

			int nodesCount = polygonPtr->GetNodesCount();

			int editMode = GetEditMode();
			switch (editMode){
			case iview::ISelectable::EM_NONE:
				if (IsSelected() && BaseClass::OnMouseButton(position, buttonType, downFlag)){
					return true;
				}
				break;

			case iview::ISelectable::EM_MOVE:
				{
					const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_MOVE);

					for (int i = nodesCount - 1; i >= 0; --i){
						const i2d::CVector2d& logPos = polygonPtr->GetNode(i);
						i2d::CVector2d viewPos;
						calib.GetInvPositionAt(logPos, viewPos);
						istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

						if (tickerBox.IsInside(screenPos - position)){
							m_referencePosition = logPos - logMouse;
							m_referenceIndex = i;

							BeginModelChanges();
							return true;
						}
					}
				}
				break;

			case iview::ISelectable::EM_ADD:
				{
					i2d::CVector2d logLast = GetSegmentMiddle(nodesCount - 1);
					i2d::CVector2d viewLast;
					calib.GetInvPositionAt(logLast, viewLast);
					istd::CIndex2d screenLast = transform.GetScreenPosition(viewLast);

					const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_INSERT);

					if (tickerBox.IsInside(position - screenLast)){
						BeginModelChanges();
						if (polygonPtr->InsertNode(logLast)){
							m_referencePosition = logLast - logMouse;
							m_referenceIndex = nodesCount;

							UpdateModelChanges();
							return true;
						}
					}
					for (int i = nodesCount - 2; i >= 0; --i){
						i2d::CVector2d logMiddle = GetSegmentMiddle(i);
						i2d::CVector2d viewMiddle;
						calib.GetInvPositionAt(logMiddle, viewMiddle);
						istd::CIndex2d screenMiddle = transform.GetScreenPosition(viewMiddle);
						if (tickerBox.IsInside(position - screenMiddle)){
							BeginModelChanges();
							if (polygonPtr->InsertNode(i + 1, logMiddle)){
								m_referencePosition = logMiddle - logMouse;
								m_referenceIndex = i + 1;

								UpdateModelChanges();
								return true;
							}
						}
					}
				}
				break;

			case iview::ISelectable::EM_REMOVE:
				if (nodesCount > 2){
					const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_DELETE);

					for (int i = nodesCount - 1; i >= 0; --i){
						const i2d::CVector2d& logPos = polygonPtr->GetNode(i);
						i2d::CVector2d viewPos;
						calib.GetInvPositionAt(logPos, viewPos);
						istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
						if (tickerBox.IsInside(position - screenPos)){
							BeginModelChanges();
							polygonPtr->RemoveNode(i);

							UpdateModelChanges();
							return true;
						}
					}
				}
				break;
			}
		}
	}

	EndModelChanges();

	return false;
}


bool CPolygonCalibrationShape::OnMouseMove(istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		int editMode = GetEditMode();

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::ITransformation2d& calib = GetIsomorphCalib();

		i2d::CVector2d viewMouse = transform.GetClientPosition(position);
		i2d::CVector2d logMouse;
		calib.GetPositionAt(viewMouse, logMouse);

		if ((editMode == iview::ISelectable::EM_MOVE) || (editMode == iview::ISelectable::EM_ADD)){
			i2d::CPolygon& polygon = *dynamic_cast<i2d::CPolygon*>(modelPtr);
			Q_ASSERT(&polygon != NULL);

			polygon.SetNode(m_referenceIndex, m_referencePosition + logMouse);

			UpdateModelChanges();

			return true;
		}
		else if (editMode == iview::ISelectable::EM_NONE){
			EnsureValidNodes();

			i2d::CAffine2d moveTransform = CalcMoveTransform(logMouse, m_castTransform);
			m_castAxis = moveTransform.GetDeformMatrix().GetMultiplied(m_castAxis);

			i2d::CPolygon& polygon = *dynamic_cast<i2d::CPolygon*>(modelPtr);
			Q_ASSERT(&polygon != NULL);

			int nodesCount = polygon.GetNodesCount();
			for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
				const i2d::CVector2d position = polygon.GetNode(nodeIndex);
				polygon.SetNode(nodeIndex, moveTransform.GetApply(position));
			}

			UpdateModelChanges();

			return true;
		}
	}
	return false;
}


// reimplemented (iview::IVisualizable)

void CPolygonCalibrationShape::Draw(QPainter& drawContext) const
{
	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetModelPtr());
	if (IsDisplayConnected() && (polygonPtr != NULL)){
		int nodesCount = polygonPtr->GetNodesCount();
		if (nodesCount > 0){
			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			const i2d::ITransformation2d& calib = GetIsomorphCalib();

			DrawArea(drawContext);
			DrawCurve(drawContext);

			if (IsSelected()){
				DrawSelectionElements(drawContext);
			}
			else{
				if (m_isFirstVisible && (nodesCount > 0)){
					const IColorSchema& colorSchema = GetColorSchema();

					i2d::CVector2d viewPos;
					calib.GetInvPositionAt(polygonPtr->GetNode(0), viewPos);
					istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
					colorSchema.DrawTicker(drawContext, screenPos, IColorSchema::TT_INACTIVE);
				}
			}
		}
	}
}


// reimplemented (imod::IObserver)

bool CPolygonCalibrationShape::OnAttached(imod::IModel* modelPtr)
{
	Q_ASSERT(dynamic_cast<i2d::CPolygon*>(modelPtr) != NULL);

	return CCalibrationShapeBase::OnAttached(modelPtr);
}


// reimplemented (iview::ITouchable)

iview::ITouchable::TouchState CPolygonCalibrationShape::IsTouched(istd::CIndex2d position) const
{
	if (IsSelected() && IsTickerTouched(position)){
		return TS_TICKER;
	}

	if (IsCurveTouched(position)){
		int editMode = GetEditMode();
		if (IsAlwaysMovable() || (editMode == iview::ISelectable::EM_NONE)){
			bool isEditablePosition = IsEditablePosition();
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
		else{
			return TS_INACTIVE;
		}
	}

	return TS_NONE;
}


// reimplemented (iview::IDraggable)

bool CPolygonCalibrationShape::IsDraggable() const
{
	if (BaseClass::IsDraggable()){
		int editMode = GetEditMode();

		if (IsAlwaysMovable() || (editMode == iview::ISelectable::EM_NONE)){
			return true;
		}
	}
	return false;
}


// protected methods

i2d::CVector2d CPolygonCalibrationShape::GetSegmentMiddle(int index) const
{
	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetModelPtr());
	if (IsDisplayConnected() && (polygonPtr != NULL)){
		if (index + 1 == polygonPtr->GetNodesCount()){
			return (polygonPtr->GetNode(index) + polygonPtr->GetNode(0)) / 2;
		}
		else{
			return (polygonPtr->GetNode(index) + polygonPtr->GetNode(index + 1)) / 2;
		}
	}
	else{
		return i2d::CVector2d(0, 0);
	}
}


void CPolygonCalibrationShape::DrawCurve(QPainter& drawContext) const
{
	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetModelPtr());
	if (IsDisplayConnected() && (polygonPtr != NULL)){
		int nodesCount = polygonPtr->GetNodesCount();
		if (nodesCount > 0){
			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			const i2d::ITransformation2d& calib = GetIsomorphCalib();
			const IColorSchema& colorSchema = GetColorSchema();

			drawContext.save();
			
			if (IsSelected()){
				drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
			}
			else{
				drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
			}

			i2d::CVector2d viewFirst;
			calib.GetInvPositionAt(polygonPtr->GetNode(nodesCount - 1), viewFirst);
			istd::CIndex2d point1 = transform.GetScreenPosition(viewFirst);

			for (int i = 0; i < nodesCount; i++){
				i2d::CVector2d viewPos;
				calib.GetInvPositionAt(polygonPtr->GetNode(i), viewPos);
				istd::CIndex2d point2 = transform.GetScreenPosition(viewPos);

				drawContext.drawLine(iqt::GetQPoint(point1), iqt::GetQPoint(point2));

				point1 = point2;
			}

			drawContext.restore();
		}
	}
}



void CPolygonCalibrationShape::DrawArea(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CPolygon& polygon = *dynamic_cast<const i2d::CPolygon*>(modelPtr);
		Q_ASSERT(&polygon != NULL);

		int nodesCount = polygon.GetNodesCount();

		if (nodesCount > 2){
			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			const i2d::ITransformation2d& calib = GetIsomorphCalib();
			const IColorSchema& colorSchema = GetColorSchema();

			if (m_screenPoints.size() != nodesCount){
				m_screenPoints.resize(nodesCount);
			}

			for (int i = 0; i < m_screenPoints.size(); i++){
				i2d::CVector2d viewPos;
				calib.GetInvPositionAt(polygon.GetNode(i), viewPos);
				m_screenPoints[i] = iqt::GetQPoint(transform.GetScreenPosition(viewPos));
			}

			drawContext.save();

			if (IsSelected()){
				drawContext.setBrush(colorSchema.GetBrush(IColorSchema::SB_HALF_TRANSPARENT));
			}
			else{
				drawContext.setBrush(colorSchema.GetBrush(IColorSchema::SB_HALF_TRANSPARENT2));
			}
			
			drawContext.drawPolygon(m_screenPoints);
			
			drawContext.restore();
		}
	}
}


void CPolygonCalibrationShape::DrawSelectionElements(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CPolygon& polygon = *dynamic_cast<const i2d::CPolygon*>(modelPtr);
		Q_ASSERT(&polygon != NULL);

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::ITransformation2d& calib = GetIsomorphCalib();
		istd::CIndex2d screenPos;

		const IColorSchema& colorSchema = GetColorSchema();
		int nodesCount = polygon.GetNodesCount();
		int editMode = GetEditMode();

		switch (editMode){
			int i;

		case iview::ISelectable::EM_NONE:
			DrawFigure(drawContext);
			DrawTickers(drawContext);
			break;

		case iview::ISelectable::EM_MOVE:
			for (i = 0; i < nodesCount; i++){
				i2d::CVector2d viewPos;
				calib.GetInvPositionAt(polygon.GetNode(i), viewPos);
				istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

				colorSchema.DrawTicker(drawContext, screenPos, IColorSchema::TT_MOVE);
			}
			break;

		case iview::ISelectable::EM_REMOVE:
			if (nodesCount > 2){
				for (i = 0; i < nodesCount; i++){
					i2d::CVector2d viewPos;
					calib.GetInvPositionAt(polygon.GetNode(i), viewPos);
					istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

					colorSchema.DrawTicker(drawContext, screenPos, IColorSchema::TT_DELETE);
				}
			}
			break;

		case iview::ISelectable::EM_ADD:
			for (i = 0; i < nodesCount; i++){
				i2d::CVector2d viewPos;
				calib.GetInvPositionAt(polygon.GetNode(i), viewPos);
				istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

				colorSchema.DrawTicker(drawContext, screenPos, IColorSchema::TT_SELECTED_INACTIVE);
			}
			break;
		}

		if ((nodesCount > 0) && (editMode == iview::ISelectable::EM_ADD)){
			for (int i = 0; i < nodesCount; i++){
				i2d::CVector2d viewPos;
				calib.GetInvPositionAt(GetSegmentMiddle(i), viewPos);
				istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

				colorSchema.DrawTicker(drawContext, screenPos, IColorSchema::TT_INSERT);
			}
		}
	}
}


bool CPolygonCalibrationShape::IsTickerTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetModelPtr());
	if (IsDisplayConnected() && (polygonPtr != NULL)){
		const IColorSchema& colorSchema = GetColorSchema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::ITransformation2d& calib = GetIsomorphCalib();

		int nodesCount = polygonPtr->GetNodesCount();

		int editMode = GetEditMode();

		switch (editMode){
		case iview::ISelectable::EM_NONE:
			if (IsSelected()){
				EnsureValidNodes();
				if (BaseClass::IsTickerTouched(position)){
					return true;
				}
			}
			break;

		case iview::ISelectable::EM_MOVE:
			{
				const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()?
								IColorSchema::TT_MOVE:
								IColorSchema::TT_INACTIVE);
				for (int i = 0; i < nodesCount; i++){
					i2d::CVector2d viewPos;
					calib.GetInvPositionAt(polygonPtr->GetNode(i), viewPos);
					istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
					if (tickerBox.IsInside(position - screenPos)){
						return true;
					}
				}
			}
			break;

		case iview::ISelectable::EM_REMOVE:
			{
				const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()?
								IColorSchema::TT_DELETE:
								IColorSchema::TT_INACTIVE);
				for (int i = 0; i < nodesCount; i++){
					i2d::CVector2d viewPos;
					calib.GetInvPositionAt(polygonPtr->GetNode(i), viewPos);
					istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
					if (tickerBox.IsInside(position - screenPos)){
						return true;
					}
				}
			}
			break;

		case iview::ISelectable::EM_ADD:
			{
				const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()?
								IColorSchema::TT_INSERT:
								IColorSchema::TT_INACTIVE);
				for (int i = 0; i < nodesCount; i++){
					i2d::CVector2d viewPos;
					calib.GetInvPositionAt(GetSegmentMiddle(i), viewPos);
					istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
					if (tickerBox.IsInside(position - screenPos)){
						return true;
					}
				}
			}
			break;
		}
	}
	return false;
}


bool CPolygonCalibrationShape::IsCurveTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CPolygon& polygon = *dynamic_cast<const i2d::CPolygon*>(modelPtr);
		Q_ASSERT(&polygon != NULL);

		int editMode = GetEditMode();
		if (editMode == iview::ISelectable::EM_NONE){
			if (BaseClass::IsParallTouched(m_castTransform, position)){
				return true;
			}
		}

		const IColorSchema& colorSchema = GetColorSchema();
		int nodesCount = polygon.GetNodesCount();
		if (nodesCount > 0){
			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			const i2d::ITransformation2d& calib = GetIsomorphCalib();

			double proportions = GetViewToScreenTransform().GetDeformMatrix().GetApproxScale();

			i2d::CVector2d node1;
			calib.GetInvPositionAt(polygon.GetNode(nodesCount - 1), node1);

			double logicalLineWidth = colorSchema.GetLogicalLineWidth();

			const i2d::CVector2d& viewMouse = transform.GetClientPosition(position);

			for (int i = 0; i < nodesCount; i++){
				i2d::CVector2d node2;
				calib.GetInvPositionAt(polygon.GetNode(i), node2);

				i2d::CVector2d delta = node2 - node1;

				if ((delta.GetDotProduct(viewMouse - node1) >= 0) && (delta.GetDotProduct(viewMouse - node2) <= 0)){
					i2d::CVector2d ortonormal = delta.GetOrthogonal().GetNormalized();
					double distance = qAbs(ortonormal.GetDotProduct(viewMouse - node1));
					if (proportions * distance < logicalLineWidth){
						return true;
					}
				}

				node1 = node2;
			}
		}
	}

	return false;
}


// reimplemented (iview::CInteractiveTransformableRectangleShape)

void CPolygonCalibrationShape::EnsureValidNodes() const
{
	if (!AreNodesValid()){
		const imod::IModel* modelPtr = GetModelPtr();
		if (modelPtr != NULL){
			const i2d::CPolygon& polygon = *dynamic_cast<const i2d::CPolygon*>(modelPtr);
			Q_ASSERT(&polygon != NULL);

			i2d::CVector2d axisX = m_castAxis.GetNormalized();
			i2d::CVector2d axisY = axisX.GetOrthogonal();

			int nodesCount = polygon.GetNodesCount();

			if (nodesCount > 0){
				const i2d::CVector2d& firstNode = polygon.GetNode(0);

				i2d::CVector2d minPosition(axisX.GetDotProduct(firstNode), axisY.GetDotProduct(firstNode));
				i2d::CVector2d maxPosition = minPosition;

				for (int i = 1; i < nodesCount; i++){
					const i2d::CVector2d& viewPos = polygon.GetNode(i);

					i2d::CVector2d castedPosition(axisX.GetDotProduct(viewPos), axisY.GetDotProduct(viewPos));

					if (castedPosition.GetX() < minPosition.GetX()){
						minPosition.SetX(castedPosition.GetX());
					}
					if (castedPosition.GetY() < minPosition.GetY()){
						minPosition.SetY(castedPosition.GetY());
					}

					if (castedPosition.GetX() > maxPosition.GetX()){
						maxPosition.SetX(castedPosition.GetX());
					}
					if (castedPosition.GetY() > maxPosition.GetY()){
						maxPosition.SetY(castedPosition.GetY());
					}
				}
				i2d::CVector2d delta = maxPosition - minPosition;
				m_castTransform = i2d::CAffine2d(
								i2d::CMatrix2d(axisX * delta.GetX(), axisY * delta.GetY()),
								axisX * minPosition.GetX() + axisY * minPosition.GetY());
				CalcNodes(m_castTransform);
				return;
			}
		}

		ResetNodes();
	}
}


// reimplemented (iview::IInteractiveShapeBase)

void CPolygonCalibrationShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CPolygon& polygon = *dynamic_cast<const i2d::CPolygon*>(modelPtr);
		Q_ASSERT(&polygon != NULL);

		int nodesCount = polygon.GetNodesCount();
		m_references.resize(nodesCount);
		for (int i = 0; i < nodesCount; i++){
			m_references[i] = polygon.GetNode(i) - reference;
		}
	}
}


void CPolygonCalibrationShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		i2d::CPolygon& polygon = *dynamic_cast<i2d::CPolygon*>(modelPtr);
		Q_ASSERT(&polygon != NULL);

		BeginModelChanges();

		int nodesCount = polygon.GetNodesCount();
		if (nodesCount == int(m_references.size())){
			for (int i = 0; i < nodesCount; i++){
				polygon.SetNode(i, m_references[i] + position);
			}
		}

		EndModelChanges();
	}
}


// reimplemented (iview::CShapeBase)

i2d::CRect CPolygonCalibrationShape::CalcBoundingBox() const
{
	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetModelPtr());
	if (IsDisplayConnected() && (polygonPtr != NULL)){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::ITransformation2d& calib = GetIsomorphCalib();
		const IColorSchema& colorSchema = GetColorSchema();

		int nodesCount = polygonPtr->GetNodesCount();

		if (nodesCount > 0){
			i2d::CVector2d firstView;
			calib.GetInvPositionAt(polygonPtr->GetNode(0), firstView);
			istd::CIndex2d screenPos = transform.GetScreenPosition(firstView);
			i2d::CRect boundingBox(screenPos, screenPos);
			for (int i = 1; i < nodesCount; i++){
				calib.GetInvPositionAt(polygonPtr->GetNode(i), firstView);
				screenPos = transform.GetScreenPosition(firstView);
				boundingBox.Union(screenPos);
			}

			IColorSchema::TickerType tickerType;

			if (IsSelected()){
				int editMode = GetEditMode();
				switch (editMode){
				case iview::ISelectable::EM_MOVE:
					tickerType = IColorSchema::TT_MOVE;
					break;

				case iview::ISelectable::EM_ADD:
					tickerType = IColorSchema::TT_INSERT;
					break;

				case iview::ISelectable::EM_REMOVE:
					tickerType = IColorSchema::TT_DELETE;
					break;

				default:
					return BaseClass::CalcBoundingBox();
				}
			}
			else{
				tickerType = IColorSchema::TT_INACTIVE;
			}

			return boundingBox.GetExpanded(colorSchema.GetTickerBox(tickerType));
		}
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview



