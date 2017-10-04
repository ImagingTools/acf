#include <iview/CPolygonShape.h>


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <imod/IModel.h>
#include <i2d/CPolygon.h>
#include <i2d/CAffineTransformation2d.h>
#include <iqt/iqt.h>
#include <iview/IColorSchema.h>
#include <iview/CScreenTransform.h>


namespace iview
{


const istd::IChangeable::ChangeSet s_moveChangeSet(IDisplay::CS_CONSOLE, i2d::IObject2d::CF_OBJECT_POSITION, QObject::tr("Move node"));
const istd::IChangeable::ChangeSet s_moveAllChangeSet(IDisplay::CS_CONSOLE, i2d::IObject2d::CF_OBJECT_POSITION, QObject::tr("Move"));


CPolygonShape::CPolygonShape()
	:m_castAxis(1, 0)
{
	m_referenceIndex = 0;
	m_isFirstVisible = false;
	m_isAlwaysDraggable = false;
}


// reimplemented (iview::IMouseActionObserver)

bool CPolygonShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	if (!ShapeBaseClass::OnMouseButton(position, buttonType, downFlag)){
		imod::IModel* modelPtr = GetObservedModel();
		i2d::CPolygon* polygonPtr = dynamic_cast<i2d::CPolygon*>(modelPtr);
		if (polygonPtr != NULL){
			if (downFlag){
				const IColorSchema& colorSchema = GetColorSchema();

				int nodesCount = polygonPtr->GetNodesCount();

				int editMode = GetEditMode();
				switch (editMode){
				case ISelectable::EM_NONE:
					if (IsSelected() && IsEditablePosition() && CRectControlledShapeBase::OnMouseButton(position, buttonType, downFlag)){
						return true;
					}
					break;

				case ISelectable::EM_MOVE:
					if (IsEditablePosition()){
						const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_MOVE);

						for (int i = nodesCount - 1; i >= 0; --i){
							const i2d::CVector2d& cp = polygonPtr->GetNodePos(i);
							istd::CIndex2d sp = GetScreenPosition(cp).ToIndex2d();

							if (tickerBox.IsInside(position - sp)){
								m_referencePosition = cp - GetLogPosition(position);
								m_referenceIndex = i;

								BeginTickerDrag();

								return true;
							}
						}
					}
					break;

				case ISelectable::EM_ADD:
					{
						i2d::CVector2d cpLast;
						cpLast = GetSegmentMiddle(nodesCount - 1);
						istd::CIndex2d spLast = GetScreenPosition(cpLast).ToIndex2d();

						const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_INSERT);

						if (tickerBox.IsInside(position - spLast)){
							BeginTickerDrag();

							if (polygonPtr->InsertNode(cpLast)){
								m_referencePosition = cpLast - GetLogPosition(position);
								m_referenceIndex = nodesCount;

								UpdateModelChanges();
								return true;
							}
						}
						for (int i = nodesCount - 2; i >= 0; --i){
							i2d::CVector2d middle = GetSegmentMiddle(i);
							istd::CIndex2d sp = GetScreenPosition(middle).ToIndex2d();
							if (tickerBox.IsInside(position - sp)){
								BeginTickerDrag();

								if (polygonPtr->InsertNode(i + 1, middle)){
									m_referencePosition = middle - GetLogPosition(position);
									m_referenceIndex = i + 1;

									UpdateModelChanges();
									return true;
								}
							}
						}
					}
					break;

				case ISelectable::EM_REMOVE:
					if (nodesCount > 2){
						const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_DELETE);

						for (int i = nodesCount - 1; i >= 0; --i){
							const i2d::CVector2d& cp = polygonPtr->GetNodePos(i);
							istd::CIndex2d sp = GetScreenPosition(cp).ToIndex2d();
							if (tickerBox.IsInside(position - sp)){
								BeginTickerDrag();

								polygonPtr->RemoveNode(i);

								EndTickerDrag();

								return true;
							}
						}
					}
					break;
				}
			}
		}

		EndTickerDrag();
	}

	return false;
}


bool CPolygonShape::OnMouseMove(istd::CIndex2d position)
{
	i2d::CPolygon* polygonPtr = dynamic_cast<i2d::CPolygon*>(GetObservedModel());
	if (polygonPtr != NULL){
		int editMode = GetEditMode();

		if ((editMode == ISelectable::EM_MOVE) || (editMode == ISelectable::EM_ADD)){
			istd::CChangeNotifier notifier(polygonPtr, &s_moveChangeSet);
			Q_UNUSED(notifier);

			polygonPtr->SetNodePos(m_referenceIndex, m_referencePosition + GetLogPosition(position));

			UpdateModelChanges();

			return true;
		}
		else if (editMode == ISelectable::EM_NONE){
			i2d::CVector2d cp = GetLogPosition(position);

			EnsureValidNodes();
			i2d::CAffine2d moveTransform = CalcMoveTransform(cp, m_castTransform);
			m_castAxis = moveTransform.GetDeformMatrix().GetMultiplied(m_castAxis);

			istd::CChangeNotifier notifier(polygonPtr, &s_moveAllChangeSet);
			Q_UNUSED(notifier);

			int nodesCount = polygonPtr->GetNodesCount();
			for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
				const i2d::CVector2d position = polygonPtr->GetNodePos(nodeIndex);
				polygonPtr->SetNodePos(nodeIndex, moveTransform.GetApply(position));
			}

			UpdateModelChanges();

			return true;
		}
	}
	return false;
}


// reimplemented (iview::IVisualizable)

void CPolygonShape::Draw(QPainter& drawContext) const
{
	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetObservedModel());
	if (IsDisplayConnected() && (polygonPtr != NULL)){
		int nodesCount = polygonPtr->GetNodesCount();
		if (nodesCount > 0){
			const IColorSchema& colorSchema = GetColorSchema();

			istd::CIndex2d sp;

			DrawArea(drawContext);
			DrawCurve(drawContext);

			drawContext.save();
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_TICKER));
			drawContext.setBrush(colorSchema.GetBrush(IColorSchema::SB_TICKER));

			if (IsSelected()){
				DrawSelectionElements(drawContext);
			}
			else{
				if (m_isFirstVisible && (nodesCount > 0)){
					sp = GetScreenPosition(polygonPtr->GetNodePos(0)).ToIndex2d();
					colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_INACTIVE);
				}
			}

			drawContext.restore();
		}
	}
}


// reimplemented (imod::IObserver)

bool CPolygonShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CPolygon*>(modelPtr) != NULL);

	return CInteractiveShapeBase::OnModelAttached(modelPtr, changeMask);
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CPolygonShape::IsTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetObservedModel());
	if (polygonPtr != NULL){
		int editMode = GetEditMode();

		const IColorSchema& colorSchema = GetColorSchema();

		int nodesCount = polygonPtr->GetNodesCount();

		switch (editMode){
		case ISelectable::EM_NONE:
			if (IsEditablePosition() && IsSelected()){
				EnsureValidNodes();
				if (BaseClass::IsTickerTouched(position)){
					return TS_TICKER;
				}

				if (BaseClass::IsParallTouched(m_castTransform, position)){
					bool isEditablePosition = IsEditablePosition();

					return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
				}
			}
			break;

		case ISelectable::EM_MOVE:
			if (IsEditablePosition()){
				const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_MOVE: IColorSchema::TT_INACTIVE);
				for (int i = 0; i < nodesCount; i++){
					istd::CIndex2d sp = GetScreenPosition(polygonPtr->GetNodePos(i)).ToIndex2d();
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
			}
			break;

		case ISelectable::EM_REMOVE:
			{
				const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_DELETE: IColorSchema::TT_INACTIVE);
				for (int i = 0; i < nodesCount; i++){
					istd::CIndex2d sp = GetScreenPosition(polygonPtr->GetNodePos(i)).ToIndex2d();
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
			}
			break;

		case ISelectable::EM_ADD:
			{
				const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_INSERT: IColorSchema::TT_INACTIVE);
				for (int i = 0; i < nodesCount; i++){
					istd::CIndex2d sp = GetScreenPosition(GetSegmentMiddle(i)).ToIndex2d();
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
			}
			break;
		}

		if (IsCurveTouched(position)){
			if (IsAlwaysMovable() || (editMode == ISelectable::EM_NONE)){
				bool isEditablePosition = IsEditablePosition();
				return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
			}
			else{
				return TS_INACTIVE;
			}
		}
	}

	return TS_NONE;
}


// reimplemented (iview::IDraggable)

bool CPolygonShape::IsDraggable() const
{
	if (BaseClass::IsDraggable()){
		int editMode = GetEditMode();

		if (IsAlwaysMovable() || (editMode == ISelectable::EM_NONE)){
			return true;
		}
	}
	return false;
}


// protected methods

i2d::CVector2d CPolygonShape::GetSegmentMiddle(int index) const
{
	const imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		const i2d::CPolygon& polygon = *dynamic_cast<const i2d::CPolygon*>(modelPtr);
		Q_ASSERT(&polygon != NULL);

		if (index + 1 == polygon.GetNodesCount()){
			return (polygon.GetNodePos(index) + polygon.GetNodePos(0)) / 2;
		}
		else{
			return (polygon.GetNodePos(index) + polygon.GetNodePos(index + 1)) / 2;
		}
	}
	else{
		return i2d::CVector2d(0, 0);
	}
}


void CPolygonShape::DrawCurve(QPainter& /*drawContext*/) const
{
	// not needed because QPainter::drawPolygon (see DrawArea) draws the borders as well...
}


void CPolygonShape::DrawArea(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetObservedModel());
	if (polygonPtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		int nodesCount = polygonPtr->GetNodesCount();

		if (nodesCount > 2){
			if (m_screenPoints.size() != nodesCount){
				m_screenPoints.resize(nodesCount);
			}

			for (int i = 0; i < m_screenPoints.size(); i++){
				m_screenPoints[i] = GetScreenPosition(polygonPtr->GetNodePos(i));
			}

			drawContext.save();

			if (IsSelected()){
				drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
				drawContext.setBrush(colorSchema.GetBrush(IColorSchema::SB_HALF_TRANSPARENT));
			}
			else{
				drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
				drawContext.setBrush(colorSchema.GetBrush(IColorSchema::SB_HALF_TRANSPARENT2));
			}
			
			drawContext.drawPolygon(m_screenPoints);
			
			drawContext.restore();
		}
	}
}


void CPolygonShape::DrawSelectionElements(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetObservedModel());
	if (polygonPtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		int nodesCount = polygonPtr->GetNodesCount();
		int editMode = GetEditMode();
		istd::CIndex2d sp;

		switch (editMode){
			int i;

		case ISelectable::EM_NONE:
			if (IsEditablePosition()){
				CRectControlledShapeBase::DrawFigure(drawContext);
				CRectControlledShapeBase::DrawTickers(drawContext);
			}
			break;

		case ISelectable::EM_MOVE:
			if (IsEditablePosition()){
				for (i = 0; i < nodesCount; i++){
					sp = GetScreenPosition(polygonPtr->GetNodePos(i)).ToIndex2d();

					colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_MOVE);
				}
			}
			break;

		case ISelectable::EM_REMOVE:
			if (nodesCount > 2){
				for (i = 0; i < nodesCount; i++){
					sp = GetScreenPosition(polygonPtr->GetNodePos(i)).ToIndex2d();

					colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_DELETE);
				}
			}
			break;

		case ISelectable::EM_ADD:
			for (i = 0; i < nodesCount; i++){
				sp = GetScreenPosition(polygonPtr->GetNodePos(i)).ToIndex2d();

				colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_SELECTED_INACTIVE);
			}
			break;
		}

		if ((nodesCount > 0) && (editMode == ISelectable::EM_ADD)){
			for (int i = 0; i < nodesCount; i++){
				sp = GetScreenPosition(GetSegmentMiddle(i)).ToIndex2d();

				colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_INSERT);
			}
		}
	}
}


// reimplemented (iview::CRectControlledShapeBase)

void CPolygonShape::EnsureValidNodes() const
{
	if (!AreNodesValid()){
		const imod::IModel* modelPtr = GetObservedModel();
		if (modelPtr != NULL){
			const i2d::CPolygon& polygon = *dynamic_cast<const i2d::CPolygon*>(modelPtr);
			Q_ASSERT(&polygon != NULL);

			i2d::CVector2d axisX = m_castAxis.GetNormalized();
			i2d::CVector2d axisY = axisX.GetOrthogonal();

			int nodesCount = polygon.GetNodesCount();
			if (nodesCount > 0){
				const i2d::CVector2d& firstNode = polygon.GetNodePos(0);

				i2d::CVector2d minPosition(axisX.GetDotProduct(firstNode), axisY.GetDotProduct(firstNode));
				i2d::CVector2d maxPosition = minPosition;

				for (int i = 1; i < nodesCount; i++){
					const i2d::CVector2d& position = polygon.GetNodePos(i);

					i2d::CVector2d castedPosition(axisX.GetDotProduct(position), axisY.GetDotProduct(position));

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


bool CPolygonShape::IsCurveTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetObservedModel());
	if (polygonPtr != NULL){
		int nodesCount = polygonPtr->GetNodesCount();
		if (nodesCount < 2){
			return false;
		}

		const IColorSchema& colorSchema = GetColorSchema();
		double logicalLineWidth = colorSchema.GetLogicalLineWidth();

		i2d::CLine2d segmentLine;
		segmentLine.SetPoint2(GetScreenPosition(polygonPtr->GetNodePos(nodesCount - 1)));

		i2d::CVector2d screenPosition(position);

		for (int i = 0; i < nodesCount; i++){
			segmentLine.PushEndPointQuiet(GetScreenPosition(polygonPtr->GetNodePos(i)));

			if (segmentLine.GetDistance(screenPosition) < logicalLineWidth){
				return true;
			}
		}
	}

	return false;
}


// reimplemented (iview::IInteractiveShapeBase)

void CPolygonShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	const imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		const i2d::CPolygon& polygon = *dynamic_cast<const i2d::CPolygon*>(modelPtr);
		Q_ASSERT(&polygon != NULL);

		int nodesCount = polygon.GetNodesCount();
		m_references.resize(nodesCount);
		for (int i = 0; i < nodesCount; i++){
			m_references[i] = polygon.GetNodePos(i) - reference;
		}
	}
}


void CPolygonShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	i2d::CPolygon* polygonPtr = dynamic_cast<i2d::CPolygon*>(GetObservedModel());
	if (polygonPtr != NULL){
		istd::CChangeNotifier notifier(polygonPtr, &s_moveAllChangeSet);
		Q_UNUSED(notifier);

		int nodesCount = polygonPtr->GetNodesCount();
		if (nodesCount == int(m_references.size())){
			for (int i = 0; i < nodesCount; i++){
				polygonPtr->SetNodePos(i, m_references[i] + position);
			}
		}

		UpdateModelChanges();
	}
}


// reimplemented (iview::CShapeBase)

i2d::CRect CPolygonShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetObservedModel());
	if (polygonPtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		int nodesCount = polygonPtr->GetNodesCount();

		if (nodesCount > 0){
			istd::CIndex2d sp = GetScreenPosition(polygonPtr->GetNodePos(0)).ToIndex2d();
			i2d::CRect boundingBox(sp, sp);
			for (int i = 1; i < nodesCount; i++){
				sp = GetScreenPosition(polygonPtr->GetNodePos(i)).ToIndex2d();
				boundingBox.Union(sp);
			}

			boundingBox.Expand(i2d::CRect(-1,-1, 1, 1));

			IColorSchema::TickerType tickerType = IColorSchema::TT_INACTIVE;

			if (IsSelected()){
				int editMode = GetEditMode();
				switch (editMode){
				case ISelectable::EM_NONE:
					if (IsEditablePosition()){
						return boundingBox.GetUnion(BaseClass::CalcBoundingBox());
					}
					break;

				case ISelectable::EM_MOVE:
					tickerType = IColorSchema::TT_MOVE;
					break;

				case ISelectable::EM_ADD:
					tickerType = IColorSchema::TT_INSERT;
					break;

				case ISelectable::EM_REMOVE:
					tickerType = IColorSchema::TT_DELETE;
					break;

				default:
					break;
				}
			}

			return boundingBox.GetExpanded(colorSchema.GetTickerBox(tickerType));
		}
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview


