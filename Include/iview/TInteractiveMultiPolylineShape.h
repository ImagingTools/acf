#ifndef iview_TInteractiveMultiPolylineShape_included
#define iview_TInteractiveMultiPolylineShape_included


#include "iview/CInteractivePolylineShape.h"


namespace iview
{


template<class SegmentData>
class TInteractiveMultiPolylineShape: public CInteractivePolylineShape
{
public:
	typedef CInteractivePolylineShape BaseClass;

	TInteractiveMultiPolylineShape();

	/**	Check if in all edit modes is this shape draggable.
	 *		Normally it is possible to drag this object only in EM_NONE mode.
	*/
	virtual bool IsAlwaysMovable() const;
	/**	Set flag if in all edit modes is this shape draggable.
	 *		\sa IsAlwaysMovable()
	*/
	virtual void SetAlwaysMovable(bool state = true);

    // reimplemented (iview::CInteractiveShapeBase)
	virtual void CalcBoundingBox(i2d::CRect& result) const;

    // reimplemented (iview::IInteractiveShape)
    virtual void SetSelected(bool selectFlag);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);

	// reimplemented (iview::ITouchable)
    virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::IShape)
	virtual void OnAttached(imod::IModel* modelPtr);

protected:
	virtual void DrawCurve(QPainter& drawContext) const;
	virtual void DrawSelectionElements(QPainter& drawContext) const;
	virtual bool IsCurveTouched(istd::CIndex2d position) const;

	bool TrySelectSegments(
					iview::TMultiPolyline<SegmentData>& polyline,
					istd::CIndex2d position);
	
	// reimplemented (iview::CInteractiveTransformableRectangleShape)
	virtual void EnsureValidNodes() const;
    
	void CalculateRemovePoints(const iview::TMultiPolyline<SegmentData>& polyline, int segment, i2d::CVector2d& first, i2d::CVector2d& second) const;

private:
	bool m_isAlwaysDraggable;
};



// inline methods

template<class SegmentData>
inline bool TInteractiveMultiPolylineShape<SegmentData>::IsAlwaysMovable() const
{
	return m_isAlwaysDraggable;
}



template<class SegmentData>
inline void TInteractiveMultiPolylineShape<SegmentData>::SetAlwaysMovable(bool state)
{
	m_isAlwaysDraggable = state;
}



// non-inline methods

template<class SegmentData>
TInteractiveMultiPolylineShape<SegmentData>::TInteractiveMultiPolylineShape()
{
	m_isOrientationVisible = false;
	m_isAlwaysDraggable = false;
}



// reimplemented (iview::IMouseActionObserver)

template<class SegmentData>
bool TInteractiveMultiPolylineShape<SegmentData>::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	I_ASSERT(IsDisplayConnected());

	iview::TMultiPolyline<SegmentData>* polylinePtr = dynamic_cast<iview::TMultiPolyline<SegmentData>*>(GetModelPtr());
	if (polylinePtr != NULL){
		if (downFlag){
			const IColorShema& colorShema = GetColorShema();

			int nodesCount = polylinePtr->GetNodesCount();
			int editMode = GetEditMode();

			const iview::CScreenTransform& transform = GetLogToScreenTransform();

			switch (editMode){
			case ISelectable::EM_NONE:
				if (IsSelected() && CInteractiveTransformableRectangleShape::OnMouseButton(position, buttonType, downFlag)){
					return true;
				}
				if (TrySelectSegments(*polylinePtr, position)){
					return true;
				}
				break;

			case ISelectable::EM_MOVE:
				{
					const i2d::CRect& tickerBox = colorShemaPtr->GetTickerBox(IColorShema::TT_MOVE);

					for (int i = nodesCount - 1; i >= 0; --i){
						const i2d::CVector2d& cp = polyline->GetNode(i);
						istd::CIndex2d sp = transform.GetScreenPosition(cp);
						if (tickerBox.IsInside(position - sp)){
							m_referencePosition = cp - transform.GetClientPosition(position);
							m_referenceIndex = i;

							BeginModelChanges();
							return true;
						}
					}
				}
				if (TrySelectSegments(*polylinePtr, position)){
					return true;
				}
				break;

			case ISelectable::EM_ADD:
				{
					i2d::CVector2d cpLast;
					const i2d::CRect& tickerBox = colorShemaPtr->GetTickerBox(IColorShema::TT_INSERT);

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
				{
					if (nodesCount > 2){
						const i2d::CRect& tickerBox = colorShemaPtr->GetTickerBox(IColorShema::TT_INSERT);

						i2d::CVector2d first, second;
						istd::CIndex2d sp;

						int segmentsCount = polylinePtr->GetSegmentsCount();
						for (int i = 0; i < segmentsCount; i++){
							CalculateRemovePoints(*polylinePtr, i, first, second);

							sp = transform.GetScreenPosition(first);
							if (tickerBox.IsInside(position - sp)){
								BeginModelChanges();

								polylinePtr->RemoveSegment(i,i);

								EndModelChanges();
								return true;
							}
							sp = transform.GetScreenPosition(second);
							if (tickerBox.IsInside(position - sp)){
								BeginModelChanges();

								polylinePtr->RemoveSegment(i + 1,i);

								EndModelChanges();
								return true;
							}
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



// reimplemented (iview::IShape)

template<class SegmentData>
void TInteractiveMultiPolylineShape<SegmentData>::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<iview::TMultiPolyline<SegmentData>*>(modelPtr) != NULL);

	BaseClass::OnAttached(modelPtr);
}



// reimplemented (iview::ITouchable)

template<class SegmentData>
ITouchable::TouchState TInteractiveMultiPolylineShape<SegmentData>::IsTouched(istd::CIndex2d position) const
{
	I_ASSERT(IsDisplayConnected());

	iview::TMultiPolyline<SegmentData>* polylinePtr = dynamic_cast<iview::TMultiPolyline<SegmentData>*>(GetModelPtr());
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
				if (colorShemaPtr != NULL){
					CInteractiveTransformableRectangleShape::EnsureValidNodes();
					if (CInteractiveTransformableRectangleShape::IsTickerTouched(position)){
						return TS_TICKER;
					}

					if (CInteractiveTransformableRectangleShape::IsParallTouched(m_castTransform, position)){
						bool isEditablePosition = IsEditablePosition();
						return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
					}
				}
			}
			break;

		case ISelectable::EM_MOVE:
			{
				const i2d::CRect& tickerBox = colorShemaPtr->GetTickerBox(IColorShema::TT_MOVE);

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
				const i2d::CRect& tickerBox = colorShemaPtr->GetTickerBox(IColorShema::TT_INSERT);

				for (int i = 0; i < nodesCount; i++){
					if(i == 0 && !polylinePtr->IsClosed()){
						point = polylinePtr->GetNode(i);
						sp = transform.GetScreenPosition(point);
						if (tickerBox.IsInside(position - sp)){
							return TS_TICKER;
						}
					}

					if(i == (nodesCount - 1) && !polylinePtr->IsClosed()){
						point = polylinePtr->GetNode(i);
						sp = transform.GetScreenPosition(point);
						if (tickerBox.IsInside(position - sp)){
							return TS_TICKER;
						}
					}

					point = (polylinePtr->GetNode(i) + polylinePtr->GetNode((i+1) % nodesCount)) * 0.5;
					sp = transform.GetScreenPosition(point);
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
			}
			break;

		case ISelectable::EM_REMOVE:
			{
				i2d::CVector2d first, second;

				const i2d::CRect& tickerBox = colorShemaPtr->GetTickerBox(IColorShema::TT_DELETE);

				int segmentsCount = polylinePtr->GetSegmentsCount();
				for (int i = 0; i < segmentsCount; i++){
					CalculateRemovePoints(*polylinePtr, i, first, second);

					sp = transform.GetScreenPosition(first);
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
					sp = transform.GetScreenPosition(second);
					if (tickerBox.IsInside(position - sp)){
						return TS_TICKER;
					}
				}
			}
			break;
		}

		if (IsCurveTouched(position)){
			if (IsAlwaysMovable()){
				return TS_DRAGGABLE;
			}
			else if ((editMode == ISelectable::EM_NONE) || (editMode == ISelectable::EM_MOVE)){
				return TS_OTHER;
			}
			else{
				return TS_INACTIVE;
			}
		}
	}

	return TS_NONE;
}



// protected methods

template<class SegmentData>
void TInteractiveMultiPolylineShape<SegmentData>::DrawCurve(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	BaseClass::DrawCurve(drawContext);

	if (IsSelected()){
		iview::TMultiPolyline<SegmentData>* polylinePtr = dynamic_cast<iview::TMultiPolyline<SegmentData>*>(GetModelPtr());
		if (polylinePtr != NULL){
			const IColorShema& colorShema = GetColorShema();
			int nodesCount = polylinePtr->GetNodesCount();
			if (nodesCount > 1){
				int segmentsCount = polylinePtr->GetSegmentsCount();
				int parallelsCount = polylinePtr->GetParallelsCount();

				i2d::CVector2d firstKneeVector = polylinePtr->GetKneeVector(0);
				i2d::CVector2d firstPoint = polylinePtr->GetNode(0);

				const iview::CScreenTransform& transform = GetLogToScreenTransform();

				for (int segmentIndex = 0; segmentIndex < segmentsCount; ++segmentIndex){
					int nodeIndex = (segmentIndex + 1) % nodesCount;
					i2d::CVector2d secondKneeVector = polylinePtr->GetKneeVector(nodeIndex);
					const i2d::CVector2d& secondPoint = polylinePtr->GetNode(nodeIndex);

					for (int parallelIndex = 0; parallelIndex < parallelsCount; ++parallelIndex){
						double parallel = polylinePtr->GetParallel(segmentIndex, parallelIndex);

						istd::CIndex2d screenPoint1 = transform.GetScreenPosition(firstPoint + firstKneeVector * parallel);
						istd::CIndex2d screenPoint2 = transform.GetScreenPosition(secondPoint + secondKneeVector * parallel);

						if (polylinePtr->IsSegmentSelected(segmentIndex)){
							drawContext.PushPen(colorShemaPtr->GetPen(IColorShema::SP_SELECTED_DASH));
						}
						else{
							drawContext.PushPen(colorShemaPtr->GetPen(IColorShema::SP_NORMAL_DASH));
						}

						drawContext.drawLine(screenPoint1, screenPoint2);

						drawContext.restore();
					}
					firstPoint = secondPoint;
					firstKneeVector = secondKneeVector;
				}
			}
		}
	}
}



template<class SegmentData>
void TInteractiveMultiPolylineShape<SegmentData>::DrawSelectionElements(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	iview::TMultiPolyline<SegmentData>* polylinePtr = dynamic_cast<iview::TMultiPolyline<SegmentData>*>(GetModelPtr());
	if (polylinePtr != NULL){
		istd::CIndex2d sp;

        const IColorShema& colorShema = GetColorShema();
		int nodesCount = polylinePtr->GetNodesCount();
		int editMode = GetEditMode();

		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		for (int i = 0; i < nodesCount; i++){
			sp = transform.GetScreenPosition(polylinePtr->GetNode(i));

			if ((editMode == ISelectable::EM_MOVE)){
				colorShemaPtr->DrawTicker(drawContext, sp, IColorShema::TT_MOVE);
			}
			else if ((editMode != ISelectable::EM_ADD) || ((i != 0) && (i != nodesCount - 1))){
				colorShemaPtr->DrawTicker(drawContext, sp, IColorShema::TT_SELECTED_INACTIVE);
			}
		}
		if (nodesCount > 0){
			switch (editMode){
			case ISelectable::EM_NONE:
				DrawFigure(drawContext);
				DrawTickers(drawContext);
				break;

			case ISelectable::EM_ADD:
				{
					if (!polylinePtr->IsClosed()){
						sp = transform.GetScreenPosition(polylinePtr->GetNode(0));
						colorShemaPtr->DrawTicker(drawContext, sp, IColorShema::TT_INSERT);
					}

					int segmentsCount = polylinePtr->GetSegmentsCount();
					for (int i = 0; i < segmentsCount; i++){
						sp = transform.GetScreenPosition(GetSegmentMiddle(i));

						colorShemaPtr->DrawTicker(drawContext, sp, IColorShema::TT_INSERT);
					}

					if (!polylinePtr->IsClosed()){
						sp = transform.GetScreenPosition(polylinePtr->GetNode(nodesCount - 1));
						colorShemaPtr->DrawTicker(drawContext, sp, IColorShema::TT_INSERT);
					}
				}
				break;

			case ISelectable::EM_REMOVE:
				{
					if (nodesCount > 2){
						i2d::CVector2d first, second;

						int segmentsCount = polylinePtr->GetSegmentsCount();
						for (int i = 0; i < segmentsCount; i++){
							CalculateRemovePoints(*polylinePtr, i, first, second);

							sp = transform.GetScreenPosition(first);
							colorShemaPtr->DrawTicker(drawContext, sp, IColorShema::TT_DELETE);
							sp = transform.GetScreenPosition(second);
							colorShemaPtr->DrawTicker(drawContext, sp, IColorShema::TT_DELETE);
						}
					}
				}
				break;
			}
		}
	}
}



template<class SegmentData>
bool TInteractiveMultiPolylineShape<SegmentData>::IsCurveTouched(istd::CIndex2d position) const
{
	I_ASSERT(IsDisplayConnected());

	iview::TMultiPolyline<SegmentData>* polylinePtr = dynamic_cast<iview::TMultiPolyline<SegmentData>*>(GetModelPtr());
	if (polylinePtr != NULL){
        const IColorShema& colorShema = GetColorShema();

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

		double logicalLineWidth = colorShemaPtr->GetLogicalLineWidth();

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		double proportions = ::sqrt(transform.GetDeformMatrix().GetDet());
		const i2d::CVector2d& cp = transform.GetClientPosition(position);

		for (; i < nodesCount; i++){
			const i2d::CVector2d& node2 = polylinePtr->GetNode(i);
			i2d::CVector2d delta = node2 - node1;

			if ((delta.GetDotProduct(cp - node1) >= 0) && (delta.GetDotProduct(cp - node2) <= 0)){
				i2d::CVector2d ortonormal = delta.GetOrthogonal().GetNormalized();
				double distance = ::fabs(ortonormal.GetDotProduct(cp - node1));
				if (proportions * distance < logicalLineWidth){
					return true;
				}
			}

			node1 = node2;
		}
	}

	return false;
}



template<class SegmentData>
bool TInteractiveMultiPolylineShape<SegmentData>::TrySelectSegments(
				iview::TMultiPolyline<SegmentData>& polyline,
				istd::CIndex2d position)
{
	I_ASSERT(IsDisplayConnected());

	imod::IModel* modelPtr = GetModelPtr();
	const IColorShema& colorShema = GetColorShema();
	I_ASSERT(colorShemaPtr != NULL);

	int nodesCount = polyline.GetNodesCount();
	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	double proportions = ::sqrt(transform.GetDeformMatrix().GetDet());

	int nodeIndex;
	i2d::CVector2d node1;
	if (polyline.IsClosed()){
		node1 = polyline.GetNode(nodesCount - 1);
		nodeIndex = 0;
	}
	else{
		node1 = polyline.GetNode(0);
		nodeIndex = 1;
	}

	double logicalLineWidth = colorShemaPtr->GetLogicalLineWidth();

	for (; nodeIndex < nodesCount; nodeIndex++){
		const i2d::CVector2d& node2 = polyline.GetNode(nodeIndex);
		const i2d::CVector2d& cp = transform.GetClientPosition(position);

		i2d::CVector2d delta = node2 - node1;

		if ((delta.GetDotProduct(cp - node1) >= 0) && (delta.GetDotProduct(cp - node2) <= 0)){
			i2d::CVector2d ortonormal = delta.GetOrthogonal().GetNormalized();
			double distance = ::fabs(ortonormal.GetDotProduct(cp - node1));
			if (proportions * distance < logicalLineWidth){
				int selectedSegment = nodeIndex - 1;
				bool selected = true;
				int keysState = GetKeysState();

				if (keysState & iview::IShapeView::KS_CONTROL != 0){
					selected = !polyline.IsSegmentSelected(selectedSegment);
				}
				else if(keysState & iview::IShapeView::KS_SHIFT != 0){
					int firstSegment = selectedSegment - 1;
					int lastSegment = selectedSegment + 1;

					while(firstSegment >= 0 && !polyline.IsSegmentSelected(firstSegment))
						firstSegment--;
					if(firstSegment < 0)
						firstSegment = selectedSegment;

					int segmentsCount = polyline.GetSegmentsCount();
					while(lastSegment < segmentsCount && !polyline.IsSegmentSelected(lastSegment))
						lastSegment++;
					if(lastSegment >= segmentsCount)
						lastSegment = selectedSegment;

					for(int currSegment = firstSegment; currSegment < lastSegment; currSegment++)
						polyline.SelectSegment(currSegment, true);
				}
				else{
					polyline.SelectAllSegments(false);
				}
				polyline.SelectSegment(nodeIndex - 1, selected);

				UpdateModelChanges();

				return true;
			}
		}
		node1 = node2;
	}
	return false;
}



// reimplemented (iview::CInteractiveTransformableRectangleShape)

template<class SegmentData>
void TInteractiveMultiPolylineShape<SegmentData>::EnsureValidNodes() const
{
	if (!AreNodesValid()){
		const imod::IModel* modelPtr = GetModelPtr();
		if (modelPtr != NULL){
			const iview::TMultiPolyline<SegmentData>& polyline =
						*dynamic_cast<const iview::TMultiPolyline<SegmentData>*>(modelPtr);
			I_ASSERT(&polyline != NULL);

			i2d::CVector2d axisX = m_castAxis.GetNormalized();
			i2d::CVector2d axisY = axisX.GetOrthogonal();

			int nodesCount = polyline.GetNodesCount();
			int segmentsCount = polyline.GetSegmentsCount();
			if (nodesCount > 0){
				const i2d::CVector2d& firstNode = polyline.GetNode(0);

				i2d::CVector2d minPosition(axisX.GetDotProduct(firstNode), axisY.GetDotProduct(firstNode));
				i2d::CVector2d maxPosition = minPosition;

				int parallelCount = polyline.GetParallelsCount();
				for (int i = 0; i < nodesCount; i++){
					const i2d::CVector2d& position = polyline.GetNode(i);

					i2d::CVector2d castedPosition(axisX.GetDotProduct(position), axisY.GetDotProduct(position));

					if (castedPosition.GetSetX() < minPosition.GetSetX()){
						minPosition.SetX(castedPosition.GetSetX());
					}
					if (castedPosition.GetY() < minPosition.GetY()){
						minPosition.SetY(castedPosition.GetY());
					}

					if (castedPosition.GetSetX() > maxPosition.GetSetX()){
						maxPosition.SetX(castedPosition.GetSetX());
					}
					if (castedPosition.GetY() > maxPosition.GetY()){
						maxPosition.SetY(castedPosition.GetY());
					}

					const i2d::CVector2d& knee = polyline.GetKneeVector(i);
					int segmentIndex = i % segmentsCount;
					for (int parallelIndex = 0; parallelIndex < parallelCount; ++parallelIndex){
						double parallelValue = polyline.GetParallel(segmentIndex, parallelIndex);
						const i2d::CVector2d& edgePos = polyline.GetNode(i) + knee * parallelValue;

						castedPosition = i2d::CVector2d(axisX.GetDotProduct(edgePos), axisY.GetDotProduct(edgePos));

						if (castedPosition.GetSetX() < minPosition.GetSetX()){
							minPosition.SetX(castedPosition.GetSetX());
						}
						if (castedPosition.GetY() < minPosition.GetY()){
							minPosition.SetY(castedPosition.GetY());
						}

						if (castedPosition.GetSetX() > maxPosition.GetSetX()){
							maxPosition.SetX(castedPosition.GetSetX());
						}
						if (castedPosition.GetY() > maxPosition.GetY()){
							maxPosition.SetY(castedPosition.GetY());
						}
					}
					segmentIndex = (i + 1) % segmentsCount;
					for (parallelIndex = 0; parallelIndex < parallelCount; ++parallelIndex){
						double parallelValue = polyline.GetParallel(segmentIndex, parallelIndex);
						const i2d::CVector2d& edgePos = polyline.GetNode(i) + knee * parallelValue;

						castedPosition = i2d::CVector2d(axisX.GetDotProduct(edgePos), axisY.GetDotProduct(edgePos));

						if (castedPosition.GetSetX() < minPosition.GetSetX()){
							minPosition.SetX(castedPosition.GetSetX());
						}
						if (castedPosition.GetY() < minPosition.GetY()){
							minPosition.SetY(castedPosition.GetY());
						}

						if (castedPosition.GetSetX() > maxPosition.GetSetX()){
							maxPosition.SetX(castedPosition.GetSetX());
						}
						if (castedPosition.GetY() > maxPosition.GetY()){
							maxPosition.SetY(castedPosition.GetY());
						}
					}
				}
				i2d::CVector2d delta = maxPosition - minPosition;
				m_castTransform = i2d::CAffine2d(
								i2d::CMatrix2d(axisX * delta.GetSetX(), axisY * delta.GetY()),
								axisX * minPosition.GetSetX() + axisY * minPosition.GetY());

				const iview::CScreenTransform& logToViewTransform = GetLogToScreenTransform();
				i2d::CAffine2d composedTransform = logToViewTransform.GetApply(m_castTransform);
				i2d::CVector2d& positionRef = composedTransform.GetPositionRef();
				i2d::CVector2d& axisXRef = composedTransform.GetDeformMatrixRef()[0];
				i2d::CVector2d& axisYRef = composedTransform.GetDeformMatrixRef()[1];
				i2d::CVector2d axisXNorm = axisXRef.GetNormalized(4);
				i2d::CVector2d axisYNorm = axisYRef.GetNormalized(4);
				positionRef -= axisXNorm + axisYNorm;
				axisXRef += axisXNorm * 2;
				axisYRef += axisYNorm * 2;
				CalcNodes(composedTransform);
				return;
			}
		}
		ResetNodes();
	}
}



template<class SegmentData>
void TInteractiveMultiPolylineShape<SegmentData>::CalculateRemovePoints(const iview::TMultiPolyline<SegmentData>& polyline, int segment, i2d::CVector2d& first, i2d::CVector2d& second) const
{
	int nodesCount = polyline.GetNodesCount();
    i2d::CVector2d line = polyline.GetNode((segment+1) % nodesCount) - polyline.GetNode(segment);
    first = polyline.GetNode(segment) + line * 0.2;
    second = polyline.GetNode(segment) + line * 0.8;
}



// reimplemented (iview::TShapeBase)

template<class SegmentData>
void TInteractiveMultiPolylineShape<SegmentData>::CalcBoundingBox(i2d::CRect& result) const
{
	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const iview::TMultiPolyline<SegmentData>& polyline =
						*dynamic_cast<const iview::TMultiPolyline<SegmentData>*>(modelPtr);
        I_ASSERT(&polyline != NULL);

		int nodesCount = polyline.GetNodesCount();
		int segmentsCount = polyline.GetSegmentsCount();
		
		int parallelCount = polyline.GetParallelsCount();

		BaseClass::CalcBoundingBox(result);

		if (segmentsCount > 0){
			const iview::CScreenTransform& transform = GetLogToScreenTransform();

			for (int i = 0; i < nodesCount; i++){
				const i2d::CVector2d& position = polyline.GetNode(i);
				const i2d::CVector2d& knee = polyline.GetKneeVector(i);
				int segmentIndex = i % segmentsCount;
				for (int parallelIndex = 0; parallelIndex < parallelCount; ++parallelIndex){
					double parallelValue = polyline.GetParallel(segmentIndex, parallelIndex);
					i2d::CVector2d edgePos = position + knee * parallelValue;
					result.Union(transform.GetScreenPosition(edgePos));
				}
				segmentIndex = (i + 1) % segmentsCount;
				for (parallelIndex = 0; parallelIndex < parallelCount; ++parallelIndex){
					double parallelValue = polyline.GetParallel(segmentIndex, parallelIndex);
					i2d::CVector2d edgePos = position + knee * parallelValue;
					result.Union(transform.GetScreenPosition(edgePos));
				}
			}
		}
		result.GetBottom()++;
		result.GetRight()++;
        return;
	}

	result.Reset(); 
}



// reimplemented (iview::IInteractiveShape)

template<class SegmentData>
void  TInteractiveMultiPolylineShape<SegmentData>::SetSelected(bool selectFlag)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		iview::TMultiPolyline<SegmentData>& polyline =
						*dynamic_cast<iview::TMultiPolyline<SegmentData>*>(modelPtr);
		I_ASSERT(&polyline != NULL);

		if(IsSelected() != selectFlag)
			if(selectFlag)
				polyline.SelectAllSegments(true);
			else
				polyline.SelectAllSegments(false);
	}
    BaseClass::SetSelected(selectFlag);
}



} // namespace iview



#endif // !iview_TInteractiveMultiPolylineShape_included



