#include "iview/CInteractivePolygonShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "imod/IModel.h"

#include "i2d/CPolygon.h"

#include "iqt/iqt.h"

#include "iview/IColorShema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CInteractivePolygonShape::CInteractivePolygonShape()
	:m_castAxis(1, 0)
{
	m_referenceIndex = 0;
	m_isFirstVisible = false;
	m_isAlwaysDraggable = false;
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractivePolygonShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	I_ASSERT(IsDisplayConnected());

	imod::IModel* modelPtr = GetModelPtr();
	i2d::CPolygon* polygonPtr = dynamic_cast<i2d::CPolygon*>(modelPtr);
	if (polygonPtr != NULL){
		if (downFlag){
            const IColorShema& colorShema = GetColorShema();
			const iview::CScreenTransform& transform = GetLogToScreenTransform();

			int nodesCount = polygonPtr->GetNodesCount();
			int editMode = GetEditMode();

			switch (editMode){
			case ISelectable::EM_NONE:
				if (IsSelected() && BaseClass::OnMouseButton(position, buttonType, downFlag)){
					return true;
				}
				break;

			case ISelectable::EM_MOVE:
				{
					const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_MOVE);

					for (int i = nodesCount - 1; i >= 0; --i){
						const i2d::CVector2d& cp = polygonPtr->GetNode(i);
						istd::CIndex2d sp = transform.GetScreenPosition(cp);

						if (tickerBox.IsInside(sp - position)){
							m_referencePosition = cp - transform.GetClientPosition(position);
							m_referenceIndex = i;

							BeginModelChanges();

							return true;
						}
					}
				}
				break;

			case ISelectable::EM_ADD:
				{
					i2d::CVector2d cpLast;
					cpLast = GetSegmentMiddle(nodesCount - 1);
					istd::CIndex2d spLast = transform.GetScreenPosition(cpLast);

					const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_INSERT);

					if (tickerBox.IsInside(position - spLast)){
						BeginModelChanges();

						if (polygonPtr->InsertNode(cpLast)){
							m_referencePosition = cpLast - transform.GetClientPosition(position);
							m_referenceIndex = nodesCount;

							UpdateModelChanges();
							return true;
						}
					}
					for (int i = nodesCount - 2; i >= 0; --i){
						i2d::CVector2d middle = GetSegmentMiddle(i);
						istd::CIndex2d sp = transform.GetScreenPosition(middle);
						if (tickerBox.IsInside(position - sp)){
							BeginModelChanges();

							if (polygonPtr->InsertNode(i + 1, middle)){
								m_referencePosition = middle - transform.GetClientPosition(position);
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
					const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_DELETE);

					for (int i = nodesCount - 1; i >= 0; --i){
						const i2d::CVector2d& cp = polygonPtr->GetNode(i);
						istd::CIndex2d sp = transform.GetScreenPosition(cp);
						if (tickerBox.IsInside(position - sp)){
							BeginModelChanges();

							polygonPtr->RemoveNode(i);

							EndModelChanges();
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


bool CInteractivePolygonShape::OnMouseMove(istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		int editMode = GetEditMode();

		if ((editMode == ISelectable::EM_MOVE) || (editMode == ISelectable::EM_ADD)){
			i2d::CPolygon& polygon = *dynamic_cast<i2d::CPolygon*>(modelPtr);
			I_ASSERT(&polygon != NULL);

			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			polygon.SetNode(m_referenceIndex, m_referencePosition + transform.GetClientPosition(position));

			UpdateModelChanges();

			return true;
		}
		else if (editMode == ISelectable::EM_NONE){
			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			i2d::CVector2d cp = transform.GetClientPosition(position);

			EnsureValidNodes();
			i2d::CAffine2d moveTransform = CalcMoveTransform(cp, m_castTransform);
			m_castAxis = moveTransform.GetDeformMatrix().GetMultiplied(m_castAxis);

			i2d::CPolygon& polygon = *dynamic_cast<i2d::CPolygon*>(modelPtr);
			I_ASSERT(&polygon != NULL);

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

void CInteractivePolygonShape::Draw(QPainter& drawContext) const
{
	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetModelPtr());
	if (IsDisplayConnected() && (polygonPtr != NULL)){
        int nodesCount = polygonPtr->GetNodesCount();
        if (nodesCount > 0){
            const iview::CScreenTransform transform = GetLogToScreenTransform();
	        const IColorShema& colorShema = GetColorShema();

            istd::CIndex2d sp;

            DrawArea(drawContext);
            DrawCurve(drawContext);

			drawContext.save();
            drawContext.setPen(colorShema.GetPen(IColorShema::SP_TICKER));
			drawContext.save();
            drawContext.setBrush(colorShema.GetBrush(IColorShema::SB_TICKER));

            if (IsSelected()){
                DrawSelectionElements(drawContext);
            }
            else{
                if (m_isFirstVisible && (nodesCount > 0)){
                    sp = transform.GetScreenPosition(polygonPtr->GetNode(0));
                    colorShema.DrawTicker(drawContext, sp, IColorShema::TT_INACTIVE);
                }
            }

            drawContext.restore();
            drawContext.restore();
        }
    }
}


// reimplemented (imod::IObserver)

bool CInteractivePolygonShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CPolygon*>(modelPtr) != NULL);

	return CInteractiveShapeBase::OnAttached(modelPtr);
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CInteractivePolygonShape::IsTouched(istd::CIndex2d position) const
{
	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetModelPtr());
	if (IsDisplayConnected() && (polygonPtr != NULL)){
        const IColorShema& colorShema = GetColorShema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		int nodesCount = polygonPtr->GetNodesCount();

		int editMode = GetEditMode();

		switch (editMode){
		case ISelectable::EM_NONE:
			if (IsSelected()){
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
			{
				const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_MOVE: IColorShema::TT_INACTIVE);
				for (int i = 0; i < nodesCount; i++){
					istd::CIndex2d sp = transform.GetScreenPosition(polygonPtr->GetNode(i));
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
			}
			break;

		case ISelectable::EM_REMOVE:
			{
				const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_DELETE: IColorShema::TT_INACTIVE);
				for (int i = 0; i < nodesCount; i++){
					istd::CIndex2d sp = transform.GetScreenPosition(polygonPtr->GetNode(i));
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
			}
			break;

        case ISelectable::EM_ADD:
			{
				const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_INSERT: IColorShema::TT_INACTIVE);
				for (int i = 0; i < nodesCount; i++){
					istd::CIndex2d sp = transform.GetScreenPosition(GetSegmentMiddle(i));
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

bool CInteractivePolygonShape::IsDraggable() const
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

i2d::CVector2d CInteractivePolygonShape::GetSegmentMiddle(int index) const
{
	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CPolygon& polygon = *dynamic_cast<const i2d::CPolygon*>(modelPtr);
		I_ASSERT(&polygon != NULL);

		if (index + 1 == polygon.GetNodesCount()){
			return (polygon.GetNode(index) + polygon.GetNode(0)) / 2;
		}
		else{
			return (polygon.GetNode(index) + polygon.GetNode(index + 1)) / 2;
		}
	}
	else{
		return i2d::CVector2d(0, 0);
	}
}


void CInteractivePolygonShape::DrawCurve(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CPolygon& polygon = *dynamic_cast<const i2d::CPolygon*>(modelPtr);
		I_ASSERT(&polygon != NULL);

		const IColorShema& colorShema = GetColorShema();
		if (IsSelected()){
			drawContext.save();
			drawContext.setPen(colorShema.GetPen(IColorShema::SP_SELECTED));
		}
		else{
			drawContext.save();
			drawContext.setPen(colorShema.GetPen(IColorShema::SP_NORMAL));
		}

		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		int nodesCount = polygon.GetNodesCount();
		if (nodesCount > 0){
			QPoint point1 = iqt::GetQPoint(transform.GetScreenPosition(polygon.GetNode(nodesCount - 1)));

			for (int i = 0; i < nodesCount; i++){
				QPoint point2 = iqt::GetQPoint(transform.GetScreenPosition(polygon.GetNode(i)));

				drawContext.drawLine(point1, point2);

				point1 = point2;
			}
		}
	}

	drawContext.restore();
}


void CInteractivePolygonShape::DrawArea(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetModelPtr());
	if (polygonPtr != NULL){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const IColorShema& colorShema = GetColorShema();

		int nodesCount = polygonPtr->GetNodesCount();

		if (nodesCount > 2){
			if (m_screenPoints.size() != nodesCount){
				m_screenPoints.resize(nodesCount);
			}

			for (int i = 0; i < m_screenPoints.size(); i++){
				m_screenPoints[i] = iqt::GetQPoint(transform.GetScreenPosition(polygonPtr->GetNode(i)));
			}

			if (IsSelected()){
				drawContext.save();
				drawContext.setBrush(colorShema.GetBrush(IColorShema::SB_HALF_TRANSPARENT));
			}
			else{
				drawContext.save();
				drawContext.setBrush(colorShema.GetBrush(IColorShema::SB_HALF_TRANSPARENT2));
			}
			
			drawContext.drawPolygon(m_screenPoints);
			
			drawContext.restore();
		}
	}
}


void CInteractivePolygonShape::DrawSelectionElements(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetModelPtr());
	if (polygonPtr != NULL){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
        const IColorShema& colorShema = GetColorShema();

		int nodesCount = polygonPtr->GetNodesCount();
		int editMode = GetEditMode();
		istd::CIndex2d sp;

		switch (editMode){
			int i;

		case ISelectable::EM_NONE:
			DrawFigure(drawContext);
			DrawTickers(drawContext);
			break;

		case ISelectable::EM_MOVE:
			for (i = 0; i < nodesCount; i++){
				sp = transform.GetScreenPosition(polygonPtr->GetNode(i));

				colorShema.DrawTicker(drawContext, sp, IColorShema::TT_MOVE);
			}
			break;

		case ISelectable::EM_REMOVE:
			if (nodesCount > 2){
				for (i = 0; i < nodesCount; i++){
					sp = transform.GetScreenPosition(polygonPtr->GetNode(i));

					colorShema.DrawTicker(drawContext, sp, IColorShema::TT_DELETE);
				}
			}
			break;

		case ISelectable::EM_ADD:
			for (i = 0; i < nodesCount; i++){
				sp = transform.GetScreenPosition(polygonPtr->GetNode(i));

				colorShema.DrawTicker(drawContext, sp, IColorShema::TT_SELECTED_INACTIVE);
			}
			break;
		}

		if ((nodesCount > 0) && (editMode == ISelectable::EM_ADD)){
			for (int i = 0; i < nodesCount; i++){
				sp = transform.GetScreenPosition(GetSegmentMiddle(i));

				colorShema.DrawTicker(drawContext, sp, IColorShema::TT_INSERT);
			}
		}
	}
}


bool CInteractivePolygonShape::IsCurveTouched(istd::CIndex2d position) const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetModelPtr());
	if (polygonPtr != NULL){
        const IColorShema& colorShema = GetColorShema();
        int nodesCount = polygonPtr->GetNodesCount();
        if (nodesCount > 0){
            const iview::CScreenTransform& transform = GetLogToScreenTransform();

            double proportions = ::sqrt(transform.GetDeformMatrix().GetDet());

            i2d::CVector2d node1;
            int i;
            node1 = polygonPtr->GetNode(nodesCount - 1);

            double logicalLineWidth = colorShema.GetLogicalLineWidth();

            for (i = 0; i < nodesCount; i++){
                const i2d::CVector2d& node2 = polygonPtr->GetNode(i);
                const i2d::CVector2d& cp = transform.GetClientPosition(position);

                i2d::CVector2d delta = node2 - node1;

                if ((delta.GetDotProduct(cp - node1) >= 0) && (delta.GetDotProduct(cp - node2) <= 0)){
                    i2d::CVector2d ortonormal = delta.GetOrthogonal().GetNormalized();
                    double distance = qAbs(ortonormal.GetDotProduct(cp - node1));
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

void CInteractivePolygonShape::EnsureValidNodes() const
{
	if (!AreNodesValid()){
		const imod::IModel* modelPtr = GetModelPtr();
		if (modelPtr != NULL){
			const i2d::CPolygon& polygon = *dynamic_cast<const i2d::CPolygon*>(modelPtr);
			I_ASSERT(&polygon != NULL);

			i2d::CVector2d axisX = m_castAxis.GetNormalized();
			i2d::CVector2d axisY = axisX.GetOrthogonal();

			int nodesCount = polygon.GetNodesCount();
			const iview::CScreenTransform& logToViewTransform = GetLogToScreenTransform();
			if (nodesCount > 0){
				const i2d::CVector2d& firstNode = polygon.GetNode(0);

				i2d::CVector2d minPosition(axisX.GetDotProduct(firstNode), axisY.GetDotProduct(firstNode));
				i2d::CVector2d maxPosition = minPosition;

				for (int i = 1; i < nodesCount; i++){
					const i2d::CVector2d& position = polygon.GetNode(i);

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
				i2d::CAffine2d composedTransform = logToViewTransform.GetApply(m_castTransform);

				CalcNodes(composedTransform);

				return;
			}
		}

		ResetNodes();
	}
}


// reimplemented (iview::IInteractiveShapeBase)

void CInteractivePolygonShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CPolygon& polygon = *dynamic_cast<const i2d::CPolygon*>(modelPtr);
		I_ASSERT(&polygon != NULL);

		int nodesCount = polygon.GetNodesCount();
		m_references.resize(nodesCount);
		for (int i = 0; i < nodesCount; i++){
			m_references[i] = polygon.GetNode(i) - reference;
		}
	}
}


void CInteractivePolygonShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		i2d::CPolygon& polygon = *dynamic_cast<i2d::CPolygon*>(modelPtr);
		I_ASSERT(&polygon != NULL);

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

i2d::CRect CInteractivePolygonShape::CalcBoundingBox() const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(GetModelPtr());
	if (polygonPtr != NULL){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
        const IColorShema& colorShema = GetColorShema();

		int nodesCount = polygonPtr->GetNodesCount();

		if (nodesCount > 0){
			istd::CIndex2d sp = transform.GetScreenPosition(polygonPtr->GetNode(0));
			i2d::CRect boundingBox(sp, sp);
			for (int i = 1; i < nodesCount; i++){
				sp = transform.GetScreenPosition(polygonPtr->GetNode(i));
				boundingBox.Union(sp);
			}

			boundingBox.Expand(i2d::CRect(-1,-1, 1, 1));

			IColorShema::TickerType tickerType;

			if (IsSelected()){
				int editMode = GetEditMode();
				switch (editMode){
				case ISelectable::EM_MOVE:
					tickerType = IColorShema::TT_MOVE;
					break;

				case ISelectable::EM_ADD:
					tickerType = IColorShema::TT_INSERT;
					break;

				case ISelectable::EM_REMOVE:
					tickerType = IColorShema::TT_DELETE;
					break;

				default:
					return BaseClass::CalcBoundingBox();
				}
			}
			else{
				tickerType = IColorShema::TT_INACTIVE;
			}

			return boundingBox.GetExpanded(colorShema.GetTickerBox(tickerType));
		}
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview


