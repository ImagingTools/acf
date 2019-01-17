#include <iview/CPolylineShape.h>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <imod/IModel.h>
#include <i2d/CPolyline.h>
#include <iqt/iqt.h>
#include <iview/IColorSchema.h>
#include <iview/CScreenTransform.h>


namespace iview
{


CPolylineShape::CPolylineShape()
{
	m_isOrientationVisible = false;
}


// reimplemented (iview::IMouseActionObserver)

bool CPolylineShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	if (!ShapeBaseClass::OnMouseButton(position, buttonType, downFlag)){
		imod::IModel* modelPtr = GetObservedModel();
		i2d::CPolyline* polylinePtr = dynamic_cast<i2d::CPolyline*>(modelPtr);
		if (polylinePtr != NULL){
			if (downFlag){
				const IColorSchema& colorSchema = GetColorSchema();

				int nodesCount = polylinePtr->GetNodesCount();

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
							const i2d::CVector2d& cp = polylinePtr->GetNodePos(i);

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
						if (!polylinePtr->IsClosed()){
							cpLast = polylinePtr->GetNodePos(nodesCount - 1);
						}
						else{
							cpLast = GetSegmentMiddle(nodesCount - 1);
						}
						istd::CIndex2d spLast = GetScreenPosition(cpLast).ToIndex2d();
						const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_INSERT);

						if (tickerBox.IsInside(position - spLast)){
							BeginTickerDrag();

							if (polylinePtr->InsertNode(cpLast)){
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

								if (polylinePtr->InsertNode(i + 1, middle)){
									m_referencePosition = middle - GetLogPosition(position);
									m_referenceIndex = i + 1;

									UpdateModelChanges();

									return true;
								}
							}
						}
						if (!polylinePtr->IsClosed()){
							i2d::CVector2d cpFirst = polylinePtr->GetNodePos(0);
							istd::CIndex2d spFirst = GetScreenPosition(cpFirst).ToIndex2d();
							if (tickerBox.IsInside(position - spFirst)){
								BeginTickerDrag();

								if (polylinePtr->InsertNode(0, cpFirst)){
									m_referencePosition = cpFirst - GetLogPosition(position);
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
						const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_DELETE);

						for (int i = nodesCount - 1; i >= 0; --i){
							const i2d::CVector2d& cp = polylinePtr->GetNodePos(i);
							istd::CIndex2d sp = GetScreenPosition(cp).ToIndex2d();
							if (tickerBox.IsInside(position - sp)){
								BeginTickerDrag();

								// check the button
								if (buttonType == Qt::RightButton){	// right button: open/close
									if (i > 0){
										std::vector<i2d::CVector2d> nodesVector(nodesCount);
										for (int j = 0; j < nodesCount; ++j){
											nodesVector[j] = polylinePtr->GetNodePos(j);
										}

										std::rotate(nodesVector.begin(), nodesVector.begin()+i, nodesVector.end());

										istd::CChangeNotifier changeNotifier(polylinePtr);

										for (int j = 0; j < nodesCount; ++j){
											polylinePtr->SetNodePos(j, nodesVector.at(j));
										}
									}

									polylinePtr->SetClosed(false);
								}
								else{	// default: remove the node
									polylinePtr->RemoveNode(i);
								}

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


// reimplemented (imod::IObserver)

bool CPolylineShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CPolyline*>(modelPtr) != NULL);

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CPolylineShape::IsTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(GetObservedModel());
	if (polylinePtr != NULL){
		int editMode = GetEditMode();

		const IColorSchema& colorSchema = GetColorSchema();

		istd::CIndex2d sp;
		i2d::CVector2d point;
		int nodesCount = polylinePtr->GetNodesCount();

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
				const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_MOVE);
				for (int i = 0; i < nodesCount; i++){
					sp = GetScreenPosition(polylinePtr->GetNodePos(i)).ToIndex2d();
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
			}
			break;

		case ISelectable::EM_REMOVE:
			{
				const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_DELETE);
				for (int i = 0; i < nodesCount; i++){
					sp = GetScreenPosition(polylinePtr->GetNodePos(i)).ToIndex2d();
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
			}
			break;

		case ISelectable::EM_ADD:
			{
				const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_INACTIVE);
				int lastIndex;

				bool isOpened = !polylinePtr->IsClosed();

				if (isOpened){
					lastIndex = nodesCount - 2;

					point = polylinePtr->GetNodePos(0);
					sp = GetScreenPosition(point).ToIndex2d();
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}

					point = polylinePtr->GetNodePos(nodesCount - 1);
					sp = GetScreenPosition(point).ToIndex2d();
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
				else{
					lastIndex = nodesCount - 1;
				}

				for (int i = 0; i <= lastIndex; i++){
					point = (polylinePtr->GetNodePos(i) + polylinePtr->GetNodePos((i + 1) % nodesCount)) * 0.5;
					sp = GetScreenPosition(point).ToIndex2d();
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


QString CPolylineShape::GetShapeDescriptionAt(istd::CIndex2d position) const
{
	QString result = BaseClass::GetShapeDescriptionAt(position);

	if (GetEditMode() == ISelectable::EM_REMOVE){
		if (IsTouched(position) == TS_TICKER){
			return QObject::tr("Left click: remove selected node, right click: break at the selected node");
		}
	}

	return result;
}


// static methods

void CPolylineShape::DrawOrientationMarker(
			QPainter& drawContext,
			const QPen& rightPen,
			const QBrush& rightBrush,
			const QPen& leftPen,
			const QBrush& leftBrush,
			const i2d::CLine2d& segmentLine,
			double scale)
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

void CPolylineShape::DrawOrientationMarker(
			QPainter& drawContext,
			const i2d::CVector2d& point,
			const i2d::CVector2d& direction,
			double maxWidth,
			const iview::IColorSchema& colorSchema)
{
	const i2d::CRect& tickerBox = colorSchema.GetTickerBox(iview::IColorSchema::TT_CHECKBOX_ON);
	const double sizeBase = qMin(maxWidth, double(tickerBox.GetHeight() + tickerBox.GetWidth()));

	const i2d::CVector2d directionNormal = direction.GetNormalized(sizeBase);
	i2d::CVector2d orthogonal = directionNormal.GetOrthogonal();

	QPolygonF arrow = QPolygonF();
	arrow.push_back(point + orthogonal * 0.5);
	arrow.push_back(point + directionNormal);
	arrow.push_back(point - orthogonal * 0.5);

	drawContext.save();
	drawContext.setPen(colorSchema.GetPen(iview::IColorSchema::SP_YELLOW));
	drawContext.setBrush(QBrush(QColor(255, 255, 0)));
	drawContext.drawPolygon(arrow);
	drawContext.restore();
}

// protected methods

void CPolylineShape::DrawCurve(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(GetObservedModel());
	if (polylinePtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		int nodesCount = polylinePtr->GetNodesCount();
		if (nodesCount > 0){
			int secondPointIndex;
			i2d::CVector2d firstPoint;

			i2d::CLine2d segmentLine;

			if (polylinePtr->IsClosed()){
				firstPoint = GetScreenPosition(polylinePtr->GetNodePos(nodesCount - 1));
				secondPointIndex = 0;
			}
			else{
				firstPoint = GetScreenPosition(polylinePtr->GetNodePos(0));
				secondPointIndex = 1;
			}

			if (m_isOrientationVisible){
				const QPen& darkPen = colorSchema.GetPen(IColorSchema::SP_ORIENT_DARK);
				const QPen& brightPen = colorSchema.GetPen(IColorSchema::SP_ORIENT_BRIGHT);

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

				double viewScale = GetViewToScreenTransform().GetDeformMatrix().GetApproxScale();

				for (int pointIndex = secondPointIndex; pointIndex < nodesCount; ++pointIndex){
					segmentLine.PushEndPointQuiet(GetScreenPosition(polylinePtr->GetNodePos(pointIndex)));

					DrawOrientationMarker(
								drawContext,
								softBrightPen, darkBrush,
								softDarkPen, brightBrush,
								segmentLine,
								viewScale);
				}
			}

			// draw the polygon
			drawContext.save();

			if (IsSelected()){
				drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
			}
			else{
				drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
			}

			QPolygonF polyline;
			polyline.append(firstPoint);

			for (int pointIndex = secondPointIndex; pointIndex < nodesCount; ++pointIndex){
				polyline.append(GetScreenPosition(polylinePtr->GetNodePos(pointIndex)));
			}

			drawContext.drawPolyline(polyline);

			drawContext.restore();
		}
	}
}


void CPolylineShape::DrawArea(QPainter&) const
{
	// CPolylineShape has no filled area
}


void CPolylineShape::DrawSelectionElements(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(GetObservedModel());
	if (polylinePtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		istd::CIndex2d sp;
		int nodesCount = polylinePtr->GetNodesCount();
		int editMode = GetEditMode();

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
					sp = GetScreenPosition(polylinePtr->GetNodePos(i)).ToIndex2d();

					colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_MOVE);
				}
			}
			break;

		case ISelectable::EM_REMOVE:
			if (nodesCount > 2){
				for (i = 0; i < nodesCount; i++){
					sp = GetScreenPosition(polylinePtr->GetNodePos(i)).ToIndex2d();

					colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_DELETE);
				}
			}
			break;

		case ISelectable::EM_ADD:
			for (i = 1; i < nodesCount - 1; i++){
				sp = GetScreenPosition(polylinePtr->GetNodePos(i)).ToIndex2d();

				colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_SELECTED_INACTIVE);
			}
			break;
		}

		if ((nodesCount > 0) && (editMode == ISelectable::EM_ADD)){
			bool isOpened = !polylinePtr->IsClosed();
			if (isOpened){
				sp = GetScreenPosition(polylinePtr->GetNodePos(0)).ToIndex2d();

				colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_INSERT);
			}

			int segmentsCount = polylinePtr->GetSegmentsCount();
			for (int i = 0; i < segmentsCount; i++){
				sp = GetScreenPosition(GetSegmentMiddle(i)).ToIndex2d();

				colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_INSERT);
			}

			if (isOpened){
				sp = GetScreenPosition(polylinePtr->GetNodePos(nodesCount - 1)).ToIndex2d();

				colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_INSERT);
			}
		}
	}
}


// reimplemented (iview::CRectControlledShapeBase)

bool CPolylineShape::IsCurveTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(GetObservedModel());
	if (polylinePtr != NULL){
		int nodesCount = polylinePtr->GetNodesCount();
		if (nodesCount < 2){
			return false;
		}

		const IColorSchema& colorSchema = GetColorSchema();
		double logicalLineWidth = colorSchema.GetLogicalLineWidth();

		i2d::CLine2d segmentLine;
		int i;
		if (polylinePtr->IsClosed()){
			segmentLine.SetPoint2(GetScreenPosition(polylinePtr->GetNodePos(nodesCount - 1)));
			i = 0;
		}
		else{
			segmentLine.SetPoint2(GetScreenPosition(polylinePtr->GetNodePos(0)));
			i = 1;
		}

		i2d::CVector2d screenPosition(position);

		for (; i < nodesCount; i++){
			segmentLine.PushEndPointQuiet(GetScreenPosition(polylinePtr->GetNodePos(i)));

			if (segmentLine.GetDistance(screenPosition) < logicalLineWidth){
				return true;
			}
		}
	}

	return false;
}


// reimplemented (iview::CShapeBase)

i2d::CRect CPolylineShape::CalcBoundingBox() const
{
	i2d::CRect retVal = BaseClass::CalcBoundingBox();

	if (m_isOrientationVisible){
		retVal.Expand(i2d::CRect(-21, -21, 21, 21));
	}

	return retVal;
}


} // namespace iview


