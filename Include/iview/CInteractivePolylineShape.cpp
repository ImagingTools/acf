#include "iview/CInteractivePolylineShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "imod/IModel.h"

#include "i2d/CPolyline.h"

#include "iqt/iqt.h"

#include "iview/CScreenTransform.h"


namespace iview
{


CInteractivePolylineShape::CInteractivePolylineShape()
{
	m_isOrientationVisible = false;
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractivePolylineShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	I_ASSERT(IsDisplayConnected());

	imod::IModel* modelPtr = GetModelPtr();
	i2d::CPolyline* polylinePtr = dynamic_cast<i2d::CPolyline*>(modelPtr);
	if (polylinePtr != NULL){
		if (downFlag){
            const IColorShema& colorShema = GetColorShema();
			const iview::CScreenTransform& transform = GetLogToScreenTransform();

			int nodesCount = polylinePtr->GetNodesCount();

			int editMode = GetEditMode();
			switch (editMode){
			case ISelectable::EM_NONE:
				if (IsSelected() && CInteractiveTransformableRectangleShape::OnMouseButton(position, buttonType, downFlag)){
					return true;
				}
				break;

			case ISelectable::EM_MOVE:
				{
					const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_MOVE);

					for (int i = nodesCount - 1; i >= 0; --i){
						const i2d::CVector2d& cp = polylinePtr->GetNode(i);
						istd::CIndex2d sp = transform.GetScreenPosition(cp);
						if (tickerBox.IsInside(position - sp)){
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
					const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_INSERT);

					i2d::CVector2d cpLast;
					if (!polylinePtr->IsClosed()){
						cpLast = polylinePtr->GetNode(nodesCount - 1);
					}
					else{
						cpLast = GetSegmentMiddle(nodesCount - 1);
					}
					istd::CIndex2d spLast = transform.GetScreenPosition(cpLast);
					if (tickerBox.IsInside(position - spLast)){
						BeginModelChanges();

						if (polylinePtr->InsertNode(cpLast)){
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

							if (polylinePtr->InsertNode(i + 1, middle)){
								m_referencePosition = middle - transform.GetClientPosition(position);
								m_referenceIndex = i + 1;

								UpdateModelChanges();

								return true;
							}
						}
					}
					if (!polylinePtr->IsClosed()){
						i2d::CVector2d cpFirst = polylinePtr->GetNode(0);
						istd::CIndex2d spFirst = transform.GetScreenPosition(cpFirst);
						if (tickerBox.IsInside(position - spFirst)){
							BeginModelChanges();

							if (polylinePtr->InsertNode(0, cpFirst)){
								m_referencePosition = cpFirst - transform.GetClientPosition(position);
								m_referenceIndex = 0;

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
						const i2d::CVector2d& cp = polylinePtr->GetNode(i);
						istd::CIndex2d sp = transform.GetScreenPosition(cp);
						if (tickerBox.IsInside(position - sp)){
							BeginModelChanges();

							polylinePtr->RemoveNode(i);

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


// reimplemented (imod::IObserver)

bool CInteractivePolylineShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CPolyline*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// protected methods

void CInteractivePolylineShape::DrawCurve(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(GetModelPtr());
	if (polylinePtr != NULL){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
        const IColorShema& colorShema = GetColorShema();

		int nodesCount = polylinePtr->GetNodesCount();
		if (nodesCount > 0){
			int secondPointIndex;
			int pointIndex;
			istd::CIndex2d firstPoint;
			istd::CIndex2d point1;

			if (polylinePtr->IsClosed()){
				firstPoint = transform.GetScreenPosition(polylinePtr->GetNode(nodesCount - 1));
				secondPointIndex = 0;
			}
			else{
				firstPoint = transform.GetScreenPosition(polylinePtr->GetNode(0));
				secondPointIndex = 1;
			}
			if (m_isOrientationVisible && IsSelected()){
				const QPen& darkPen = colorShema.GetPen(IColorShema::SP_ORIENT_DARK);
				const QPen& brightPen = colorShema.GetPen(IColorShema::SP_ORIENT_BRIGHT);

				point1 = firstPoint;

				for (pointIndex = secondPointIndex; pointIndex < nodesCount; ++pointIndex){
					istd::CIndex2d point2 = transform.GetScreenPosition(polylinePtr->GetNode(pointIndex));
					istd::CIndex2d delta(point2.GetX() - point1.GetX(), point2.GetY() - point1.GetY());
					if (::abs(delta.GetY()) > ::abs(delta.GetX())){
						if (delta.GetY() > 0){
							drawContext.save();
							drawContext.setPen(darkPen);
							drawContext.drawLine(QPoint(point1.GetX() - 1, point1.GetY()), QPoint(point2.GetX() - 1, point2.GetY()));
							drawContext.restore();
							
							drawContext.save();
							drawContext.setPen(brightPen);
							drawContext.drawLine(QPoint(point1.GetX() + 1, point1.GetY()), QPoint(point2.GetX() + 1, point2.GetY()));
							drawContext.restore();
						}
						else{
							drawContext.save();
							drawContext.setPen(darkPen);
							drawContext.drawLine(QPoint(point1.GetX() + 1, point1.GetY()), QPoint(point2.GetX() + 1, point2.GetY()));
							drawContext.restore();
						
							drawContext.save();
							drawContext.setPen(brightPen);
							drawContext.drawLine(QPoint(point1.GetX() - 1, point1.GetY()), QPoint(point2.GetX() - 1, point2.GetY()));
							drawContext.restore();
						}
					}
					else{
						if (delta.GetX() > 0){
							drawContext.save();
							drawContext.setPen(darkPen);
							drawContext.drawLine(QPoint(point1.GetX(), point1.GetY() + 1), QPoint(point2.GetX(), point2.GetY() + 1));
							drawContext.restore();

							drawContext.save();
							drawContext.setPen(brightPen);
							drawContext.drawLine(QPoint(point1.GetX(), point1.GetY() - 1), QPoint(point2.GetX(), point2.GetY() - 1));
							drawContext.restore();
						}
						else{
							drawContext.save();
							drawContext.setPen(darkPen);
							drawContext.drawLine(QPoint(point1.GetX(), point1.GetY() - 1), QPoint(point2.GetX(), point2.GetY() - 1));
							drawContext.restore();

							drawContext.save();
							drawContext.setPen(brightPen);
							drawContext.drawLine(QPoint(point1.GetX(), point1.GetY() + 1), QPoint(point2.GetX(), point2.GetY() + 1));
							drawContext.restore();
						}
					}
					point1 = point2;
				}
			}

			if (IsSelected()){
				drawContext.save();
				drawContext.setPen(colorShema.GetPen(IColorShema::SP_SELECTED));
			}
			else{
				drawContext.save();
				drawContext.setPen(colorShema.GetPen(IColorShema::SP_NORMAL));
			}

			point1 = firstPoint;
			for (pointIndex = secondPointIndex; pointIndex < nodesCount; ++pointIndex){
				istd::CIndex2d point2 = transform.GetScreenPosition(polylinePtr->GetNode(pointIndex));
				drawContext.drawLine(iqt::GetQPoint(point1), iqt::GetQPoint(point2));

				point1 = point2;
			}

			drawContext.restore();
		}
	}
}


void CInteractivePolylineShape::DrawArea(QPainter&) const
{
}


void CInteractivePolylineShape::DrawSelectionElements(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(GetModelPtr());
	if (polylinePtr != NULL){
        const IColorShema& colorShema = GetColorShema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		istd::CIndex2d sp;
		int nodesCount = polylinePtr->GetNodesCount();
		int editMode = GetEditMode();

		switch (editMode){
			int i;

		case ISelectable::EM_NONE:
			DrawFigure(drawContext);
			DrawTickers(drawContext);
			break;

		case ISelectable::EM_MOVE:
			for (i = 0; i < nodesCount; i++){
				sp = transform.GetScreenPosition(polylinePtr->GetNode(i));

				colorShema.DrawTicker(drawContext, sp, IColorShema::TT_MOVE);
			}
			break;

		case ISelectable::EM_REMOVE:
			if (nodesCount > 2){
				for (i = 0; i < nodesCount; i++){
					sp = transform.GetScreenPosition(polylinePtr->GetNode(i));

					colorShema.DrawTicker(drawContext, sp, IColorShema::TT_DELETE);
				}
			}
			break;

		case ISelectable::EM_ADD:
			for (i = 1; i < nodesCount - 1; i++){
				sp = transform.GetScreenPosition(polylinePtr->GetNode(i));

				colorShema.DrawTicker(drawContext, sp, IColorShema::TT_SELECTED_INACTIVE);
			}
			break;
		}

		if ((nodesCount > 0) && (editMode == ISelectable::EM_ADD)){
			bool isOpened = !polylinePtr->IsClosed();
			if (isOpened){
				sp = transform.GetScreenPosition(polylinePtr->GetNode(0));
				colorShema.DrawTicker(drawContext, sp, IColorShema::TT_INSERT);
			}

			int segmentsCount = polylinePtr->GetSegmentsCount();
			for (int i = 0; i < segmentsCount; i++){
				sp = transform.GetScreenPosition(GetSegmentMiddle(i));

				colorShema.DrawTicker(drawContext, sp, IColorShema::TT_INSERT);
			}

			if (isOpened){
				sp = transform.GetScreenPosition(polylinePtr->GetNode(nodesCount - 1));
				colorShema.DrawTicker(drawContext, sp, IColorShema::TT_INSERT);
			}
		}
	}
}


bool CInteractivePolylineShape::IsCurveTouched(istd::CIndex2d position) const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(GetModelPtr());
	if (polylinePtr != NULL){
        const IColorShema& colorShema = GetColorShema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		double proportions = ::sqrt(transform.GetDeformMatrix().GetDet());

		int nodesCount = polylinePtr->GetNodesCount();

		i2d::CVector2d node1;
		int i;
		if (polylinePtr->IsClosed()){
			node1 = polylinePtr->GetNode(nodesCount - 1);
			i = 0;
		}
		else{
			node1 = polylinePtr->GetNode(0);
			i = 1;
		}

		double logicalLineWidth = colorShema.GetLogicalLineWidth();

		for (; i < nodesCount; i++){
			const i2d::CVector2d& node2 = polylinePtr->GetNode(i);
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

	return false;
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CInteractivePolylineShape::IsTouched(istd::CIndex2d position) const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(GetModelPtr());
	if (polylinePtr != NULL){
        const IColorShema& colorShema = GetColorShema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();

        istd::CIndex2d sp;
        i2d::CVector2d point;
		int nodesCount = polylinePtr->GetNodesCount();

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
				const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_MOVE);
				for (int i = 0; i < nodesCount; i++){
					sp = transform.GetScreenPosition(polylinePtr->GetNode(i));
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
			}
			break;

        case ISelectable::EM_REMOVE:
			{
				const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_DELETE);
				for (int i = 0; i < nodesCount; i++){
					sp = transform.GetScreenPosition(polylinePtr->GetNode(i));
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
			}
			break;

		case ISelectable::EM_ADD:
			{
				const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_INACTIVE);
				int lastIndex;

				bool isOpened = !polylinePtr->IsClosed();

				if (isOpened){
					lastIndex = nodesCount - 2;

					point = polylinePtr->GetNode(0);
					sp = transform.GetScreenPosition(point);
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}

					point = polylinePtr->GetNode(nodesCount - 1);
					sp = transform.GetScreenPosition(point);
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
				else{
					lastIndex = nodesCount - 1;
				}

				for (int i = 0; i <= lastIndex; i++){
					point = (polylinePtr->GetNode(i) + polylinePtr->GetNode((i+1) % nodesCount)) * 0.5;
					sp = transform.GetScreenPosition(point);
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
			}
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


} // namespace iview


