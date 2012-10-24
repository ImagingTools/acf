#include "iview/CInteractivePolylineShape.h"


// ACF includes
#include "imod/IModel.h"
#include "i2d/CPolyline.h"
#include "iqt/iqt.h"
#include "iview/IColorShema.h"
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


namespace
{


} // namespace


// protected methods

void CInteractivePolylineShape::DrawOrientationMarker(
			QPainter& drawContext,
			const QPen& rightPen,
			const QBrush& rightBrush,
			const QPen& leftPen,
			const QBrush& leftBrush,
			const i2d::CLine2d& segmentLine,
			double scale) const
{
	double lineLength = segmentLine.GetLength();
	if (lineLength < I_BIG_EPSILON){
		return;
	}

	scale = sqrt(scale);

	// arbitrary marker dimensions in pixels, multiplied by scale
	double markerSizeX = 8 * scale;
	double markerSizeY = 30 * scale;

	double maxMarkerLength = qMin(20.0, lineLength * 0.2);

	// limit marker size to half line length
	if (markerSizeY > maxMarkerLength){
		markerSizeX *= maxMarkerLength / markerSizeY;
		markerSizeY = maxMarkerLength;
	}

	i2d::CVector2d lineCenter = segmentLine.GetCenter();

	i2d::CVector2d segmentDelta = segmentLine.GetDiffVector();
	i2d::CVector2d segmentBaseX = segmentDelta.GetNormalized(markerSizeX);
	i2d::CVector2d segmentBaseY = segmentDelta.GetOrthogonal().GetNormalized(markerSizeY);
	// two points common to both markers
	i2d::CVector2d markerPoint1 = lineCenter - segmentBaseX;
	i2d::CVector2d markerPoint2 = lineCenter + segmentBaseX;
	i2d::CVector2d markerPoint3 = lineCenter - segmentBaseY;
	i2d::CVector2d markerPoint4 = lineCenter + segmentBaseY;
 
	drawContext.save();

	// draw the markers
	QPolygonF markerPolygon(3);
	markerPolygon[0] = markerPoint1;
	markerPolygon[1] = markerPoint2;
	markerPolygon[2] = markerPoint3;
	drawContext.setPen(leftPen);
	drawContext.setBrush(leftBrush);
	drawContext.drawPolygon(markerPolygon);

	markerPolygon[2] = markerPoint4;

	drawContext.setPen(rightPen);
	drawContext.setBrush(rightBrush);
	drawContext.drawPolygon(markerPolygon);

	drawContext.restore();
}


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
			i2d::CVector2d firstPoint;

			i2d::CLine2d segmentLine;

			if (polylinePtr->IsClosed()){
				firstPoint = transform.GetApply(polylinePtr->GetNode(nodesCount - 1));
				secondPointIndex = 0;
			}
			else{
				firstPoint = transform.GetApply(polylinePtr->GetNode(0));
				secondPointIndex = 1;
			}
			if (m_isOrientationVisible && IsSelected()){
				const QPen& darkPen = colorShema.GetPen(IColorShema::SP_ORIENT_DARK);
				const QPen& brightPen = colorShema.GetPen(IColorShema::SP_ORIENT_BRIGHT);

				// reduce line opacity for the pens; the border is only used to increase visibility on black/white backgrounds
				QColor brightColor = brightPen.color();
				QBrush brightBrush(brightColor);
				brightColor.setAlphaF(0.25);
				QPen softBrightPen(brightColor);

				QColor darkColor = darkPen.color();
				QBrush darkBrush(darkColor);
				darkColor.setAlphaF(0.25);
				QPen softDarkPen(darkColor);

				segmentLine.SetPoint2(firstPoint);

				for (int pointIndex = secondPointIndex; pointIndex < nodesCount; ++pointIndex){
					segmentLine.PushEndPoint(transform.GetApply(polylinePtr->GetNode(pointIndex)));

					DrawOrientationMarker(drawContext, softBrightPen, darkBrush, softDarkPen, brightBrush, segmentLine, transform.GetDeformMatrix().GetApproxScale());
				}
			}

			// draw the polygon
			drawContext.save();

			if (IsSelected()){
				drawContext.setPen(colorShema.GetPen(IColorShema::SP_SELECTED));
			}
			else{
				drawContext.setPen(colorShema.GetPen(IColorShema::SP_NORMAL));
			}

			segmentLine.SetPoint2(firstPoint);

			for (int pointIndex = secondPointIndex; pointIndex < nodesCount; ++pointIndex){
				segmentLine.PushEndPoint(transform.GetApply(polylinePtr->GetNode(pointIndex)));

				drawContext.drawLine(segmentLine.GetPoint1(), segmentLine.GetPoint2());
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
		int nodesCount = polylinePtr->GetNodesCount();
		if (nodesCount < 2){
			return false;
		}

		const IColorShema& colorShema = GetColorShema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		double logicalLineWidth = colorShema.GetLogicalLineWidth();

		i2d::CLine2d segmentLine;
		int i;
		if (polylinePtr->IsClosed()){
			segmentLine.SetPoint2(transform.GetScreenPosition(polylinePtr->GetNode(nodesCount - 1)));
			i = 0;
		}
		else{
			segmentLine.SetPoint2(transform.GetScreenPosition(polylinePtr->GetNode(0)));
			i = 1;
		}

		i2d::CVector2d screenPosition(position);

		for (; i < nodesCount; i++){
			segmentLine.PushEndPoint(transform.GetScreenPosition(polylinePtr->GetNode(i)));

			if (segmentLine.GetDistance(screenPosition) < logicalLineWidth){
				return true;
			}
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
					point = (polylinePtr->GetNode(i) + polylinePtr->GetNode((i + 1) % nodesCount)) * 0.5;
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


// reimplemented (iview::CShapeBase)

i2d::CRect CInteractivePolylineShape::CalcBoundingBox() const
{
	i2d::CRect retVal = BaseClass::CalcBoundingBox();

	if (m_isOrientationVisible){
		retVal.Expand(i2d::CRect(-21, -21, 21, 21));
	}

	return retVal;
}


} // namespace iview


