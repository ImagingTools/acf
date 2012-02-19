#ifndef iview_TMultiPolylineCalibrationShape_included
#define iview_TMultiPolylineCalibrationShape_included


#include "iview/CPolylineCalibrationShape.h"


namespace iview
{


// TODO: Redesign it to ACF transformation concept.
template<class SegmentData>
class TMultiPolylineCalibrationShape: public CPolylineCalibrationShape
{
public:
	typedef CPolylineCalibrationShape BaseClass;

	TMultiPolylineCalibrationShape();

	/*
		Check if orientation of polyline is visible.
		Orientation is shown as small arrow at the end of polyline.
	*/
	virtual bool IsOrientationVisible() const;
	
	/**
		Set the orientation to be visible or not.
		\param	state	if true, orientation will be visible.
	*/
	virtual void SetOrientationVisible(bool state = true);
	
	/**
		Check if in all edit modes is this shape draggable.
		Normally it is possible to drag this object only in EM_NONE mode.
	*/
	virtual bool IsAlwaysMovable() const;
	
	/**
		Set flag if in all edit modes is this shape draggable.
		\sa IsAlwaysMovable()
	*/
	virtual void SetAlwaysMovable(bool state = true);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

    // reimplemented (iview::ITouchable)
    virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::IInteractiveShape)
    virtual void SetSelected(bool selectFlag);

    // reimplemented (iview::CInteractiveShapeBase)
	virtual void CalcBoundingBox(i2d::CRect& result) const;

protected:
	virtual void DrawCurve(QPainter& drawContext) const;
	virtual void DrawSelectionElements(QPainter& drawContext) const;
	virtual bool IsCurveTouched(istd::CIndex2d position) const;

	bool TrySelectSegments(
					iview::TMultiPolyline<SegmentData>& polyline,
					istd::CIndex2d position);
	
	void CalculateRemovePoints(const iview::TMultiPolyline<SegmentData>& polyline, int segment, i2d::CVector2d& first, i2d::CVector2d& second) const;

	// reimplemented (iview::CInteractiveTransformableRectangleShape)
	virtual void EnsureValidNodes() const;
    
private:
	bool m_isAlwaysDraggable;
	bool m_isOrientationVisible;
};


// inline methods

template<class SegmentData>
inline bool TMultiPolylineCalibrationShape<SegmentData>::IsOrientationVisible() const
{
	return m_isOrientationVisible;
}


template<class SegmentData>
inline void TMultiPolylineCalibrationShape<SegmentData>::SetOrientationVisible(bool state)
{
	if (m_isOrientationVisible != state){
		m_isOrientationVisible = state;
		Invalidate();
	}
}


template<class SegmentData>
inline bool TMultiPolylineCalibrationShape<SegmentData>::IsAlwaysMovable() const
{
	return m_isAlwaysDraggable;
}


template<class SegmentData>
inline void TMultiPolylineCalibrationShape<SegmentData>::SetAlwaysMovable(bool state)
{
	m_isAlwaysDraggable = state;
}


template<class SegmentData>
TMultiPolylineCalibrationShape<SegmentData>::TMultiPolylineCalibrationShape()
{
	m_isOrientationVisible = false;
	m_isAlwaysDraggable = false;
}


// reimplemented (iview::IMouseActionObserver)

template<class SegmentData>
bool TMultiPolylineCalibrationShape<SegmentData>::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	imod::IModel* modelPtr = GetModelPtr();
	iview::TMultiPolyline<SegmentData>* polylinePtr = dynamic_cast<iview::TMultiPolyline<SegmentData>*>(modelPtr);
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		if (downFlag){
			const iview::IColorShema& colorShema = GetColorShema();
			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			const iview::IIsomorphicCalibration& calib = GetIsomorphCalib();

			i2d::CVector2d viewMouse = transform.GetClientPosition(position);
			i2d::CVector2d logMouse;
			calib.GetApplyToLog(viewMouse, logMouse);

			int nodesCount = polylinePtr->GetNodesCount();
			int editMode = GetEditMode();

			switch (editMode){
			case iview::ISelectable::EM_NONE:
				if (IsSelected() && CTransformableRectangleCalibrationShape::OnMouseButton(position, buttonType, downFlag)){
					return true;
				}
				if (TrySelectSegments(*polylinePtr, position)){
					return true;
				}
				break;

			case iview::ISelectable::EM_MOVE:
				{
					const i2d::CRect& tickerBox = colorShema.GetTickerBox(iview::IColorShema::TT_MOVE);

					for (int i = nodesCount - 1; i >= 0; --i){
						const i2d::CVector2d& logPos = polylinePtr->GetNode(i);
						i2d::CVector2d viewPos;
						calib.GetApplyToView(logPos, viewPos);
						istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
						if (tickerBox.IsInside(position - screenPos)){
							m_referencePosition = logPos - logMouse;
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

			case iview::ISelectable::EM_ADD:
				{
					i2d::CVector2d logLast;
					const i2d::CRect& tickerBox = colorShema.GetTickerBox(iview::IColorShema::TT_INSERT);

					if (!polylinePtr->IsClosed()){
						logLast = polylinePtr->GetNode(nodesCount - 1);
					}
					else{
						logLast = GetSegmentMiddle(nodesCount - 1);
					}
					i2d::CVector2d viewLast;
					calib.GetApplyToView(logLast, viewLast);
					istd::CIndex2d screenLast = transform.GetScreenPosition(viewLast);
					if (tickerBox.IsInside(position - screenLast)){
						BeginModelChanges();
						if (polylinePtr->InsertNode(logLast)){
							m_referencePosition = logLast - logMouse;
							m_referenceIndex = nodesCount;

							UpdateModelChanges();
							return true;
						}
					}
					for (int i = nodesCount - 2; i >= 0; --i){
						i2d::CVector2d logPos = GetSegmentMiddle(i);
						i2d::CVector2d viewPos;
						calib.GetApplyToView(logPos, viewPos);
						istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
						if (tickerBox.IsInside(position - screenPos)){
							BeginModelChanges();
							if (polylinePtr->InsertNode(i + 1, logPos)){
								m_referencePosition = logPos - logMouse;
								m_referenceIndex = i + 1;

								UpdateModelChanges();
								return true;
							}
						}
					}
					if (!polylinePtr->IsClosed()){
						i2d::CVector2d logFirst = polylinePtr->GetNode(0);
						i2d::CVector2d viewFirst;
						calib.GetApplyToView(logFirst, viewFirst);
						istd::CIndex2d screenFirst = transform.GetScreenPosition(viewFirst);
						if (tickerBox.IsInside(position - screenFirst)){
							BeginModelChanges();
							if (polylinePtr->InsertNode(0, logFirst)){
								m_referencePosition = logFirst - logMouse;
								m_referenceIndex = 0;

								UpdateModelChanges();
								return true;
							}
						}
					}
				}
				break;

			case iview::ISelectable::EM_REMOVE:
				{
					if (nodesCount > 2){
						const i2d::CRect& tickerBox = colorShema.GetTickerBox(iview::IColorShema::TT_INSERT);

						i2d::CVector2d first, second, viewPos;
						istd::CIndex2d screenPos;
						bool removed = false;

						int segmentsCount = polylinePtr->GetSegmentsCount();
						for (int i = 0; i < segmentsCount; i++){
							CalculateRemovePoints(*polylinePtr, i, first, second);

							calib.GetApplyToView(first, viewPos);
							screenPos = transform.GetScreenPosition(viewPos);
							if (tickerBox.IsInside(position - screenPos)){
								BeginModelChanges();

								polylinePtr->RemoveSegment(i,i);

								EndModelChanges();
								return true;
							}
							calib.GetApplyToView(second, viewPos);
							screenPos = transform.GetScreenPosition(viewPos);
							if (tickerBox.IsInside(position - screenPos)){
								BeginModelChanges();

								polylinePtr->RemoveSegment(i + 1, i);

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


// reimplemented (imod::IObserver)

template<class SegmentData>
bool TMultiPolylineCalibrationShape<SegmentData>::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<iview::TMultiPolyline<SegmentData>*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// protected methods

template<class SegmentData>
void TMultiPolylineCalibrationShape<SegmentData>::DrawCurve(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	BaseClass::DrawCurve(drawContext);

	if (IsSelected()){
		const iview::TMultiPolyline<SegmentData>* polylinePtr =
						dynamic_cast<const iview::TMultiPolyline<SegmentData>*>(GetModelPtr());
		if (polylinePtr != NULL){
			const iview::IColorShema& colorShema = GetColorShema();
			int nodesCount = polylinePtr->GetNodesCount();
			if (nodesCount > 1){
				int segmentsCount = polylinePtr->GetSegmentsCount();
				int parallelsCount = polylinePtr->GetParallelsCount();

				i2d::CVector2d firstKneeVector = polylinePtr->GetKneeVector(0);
				i2d::CVector2d firstPoint = polylinePtr->GetNode(0);

				const iview::CScreenTransform& transform = GetLogToScreenTransform();
				const iview::IIsomorphicCalibration& calib = GetIsomorphCalib();

				bool showOrientation = IsOrientationVisible() && (parallelsCount > 0);

				for (int segmentIndex = 0; segmentIndex < segmentsCount; ++segmentIndex){
					int nodeIndex = (segmentIndex + 1) % nodesCount;
					i2d::CVector2d secondKneeVector = polylinePtr->GetKneeVector(nodeIndex);
					const i2d::CVector2d& secondPoint = polylinePtr->GetNode(nodeIndex);

					double parallelAverage = 0;

					for (int parallelIndex = 0; parallelIndex < parallelsCount; ++parallelIndex){
						double parallel = polylinePtr->GetParallel(segmentIndex, parallelIndex);
						
						parallelAverage += ::fabs(parallel);

						i2d::CVector2d viewPos;
						calib.GetApplyToView(firstPoint + firstKneeVector * parallel, viewPos);
						istd::CIndex2d screenPoint1 = transform.GetScreenPosition(viewPos);
						calib.GetApplyToView(secondPoint + secondKneeVector * parallel, viewPos);
						istd::CIndex2d screenPoint2 = transform.GetScreenPosition(viewPos);

						if (polylinePtr->IsSegmentSelected(segmentIndex)){
							drawContext.PushPen(colorShema.GetPen(iview::IColorShema::SP_SELECTED_DASH));
						}
						else{
							drawContext.PushPen(colorShema.GetPen(iview::IColorShema::SP_NORMAL_DASH));
						}

						drawContext.drawLine(screenPoint1, screenPoint2);

						drawContext.restore();
					}
					if ((firstPoint.GetDistance(secondPoint) > I_EPSILON) && showOrientation){
						parallelAverage /= parallelsCount;

						i2d::CVector2d delta = (secondPoint - firstPoint).GetNormalized(parallelAverage);

						i2d::CVector2d orthogonalized = delta.GetOrthogonal();

						i2d::CVector2d viewArrowBegin;
						i2d::CVector2d viewArrowLeft;
						i2d::CVector2d viewArrowRight;
						calib.GetApplyToView(firstPoint, viewArrowBegin);
						calib.GetApplyToView(firstPoint + delta + orthogonalized, viewArrowLeft);
						calib.GetApplyToView(firstPoint + delta - orthogonalized, viewArrowRight);
						istd::CIndex2d screenArrowBegin = transform.GetScreenPosition(viewArrowBegin);
						istd::CIndex2d screenArrowLeft = transform.GetScreenPosition(viewArrowLeft);
						istd::CIndex2d screenArrowRight = transform.GetScreenPosition(viewArrowRight);

						drawContext.PushPen(colorShema.GetPen(iview::IColorShema::SP_SELECTED));
						
						drawContext.drawLine(screenArrowBegin, screenArrowLeft);
						drawContext.drawLine(screenArrowBegin, screenArrowRight);

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
void TMultiPolylineCalibrationShape<SegmentData>::DrawSelectionElements(QPainter& drawContext) const
{
	const iview::TMultiPolyline<SegmentData>* polylinePtr =
						dynamic_cast<const iview::TMultiPolyline<SegmentData>*>(GetModelPtr());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
        const iview::IColorShema& colorShema = GetColorShema();
		int nodesCount = polylinePtr->GetNodesCount();
		int editMode = GetEditMode();

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const iview::IIsomorphicCalibration& calib = GetIsomorphCalib();

		for (int i = 0; i < nodesCount; i++){
			i2d::CVector2d viewPos;
			calib.GetApplyToView(polylinePtr->GetNode(i), viewPos);
			istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

			if ((editMode == iview::ISelectable::EM_MOVE)){
				colorShema.DrawTicker(drawContext, screenPos, iview::IColorShema::TT_MOVE);
			}
			else if ((editMode != iview::ISelectable::EM_ADD) || ((i != 0) && (i != nodesCount - 1))){
				colorShema.DrawTicker(drawContext, screenPos, iview::IColorShema::TT_SELECTED_INACTIVE);
			}
		}
		if (nodesCount > 0){
			switch (editMode){
			case iview::ISelectable::EM_NONE:
				DrawFigure(drawContext);
				DrawTickers(drawContext);
				break;

			case iview::ISelectable::EM_ADD:
				{
					istd::CIndex2d screenPos;
					i2d::CVector2d viewPos;
					if (!polylinePtr->IsClosed()){
						calib.GetApplyToView(polylinePtr->GetNode(0), viewPos);
						screenPos = transform.GetScreenPosition(viewPos);
						colorShema.DrawTicker(drawContext, screenPos, iview::IColorShema::TT_INSERT);
					}

					int segmentsCount = polylinePtr->GetSegmentsCount();
					for (int i = 0; i < segmentsCount; i++){
						calib.GetApplyToView(GetSegmentMiddle(i), viewPos);
						screenPos = transform.GetScreenPosition(viewPos);

						colorShema.DrawTicker(drawContext, screenPos, iview::IColorShema::TT_INSERT);
					}

					if (!polylinePtr->IsClosed()){
						calib.GetApplyToView(polylinePtr->GetNode(nodesCount - 1), viewPos);
						screenPos = transform.GetScreenPosition(viewPos);
						colorShema.DrawTicker(drawContext, screenPos, iview::IColorShema::TT_INSERT);
					}
				}
				break;

			case iview::ISelectable::EM_REMOVE:
				{
					if (nodesCount > 2){
						i2d::CVector2d first, second;

						int segmentsCount = polylinePtr->GetSegmentsCount();
						for (int i = 0; i < segmentsCount; i++){
							CalculateRemovePoints(*polylinePtr, i, first, second);

							i2d::CVector2d viewPos;
							calib.GetApplyToView(first, viewPos);
							istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
							colorShema.DrawTicker(drawContext, screenPos, iview::IColorShema::TT_DELETE);
							calib.GetApplyToView(second, viewPos);
							screenPos = transform.GetScreenPosition(viewPos);
							colorShema.DrawTicker(drawContext, screenPos, iview::IColorShema::TT_DELETE);
						}
					}
				}
				break;
			}
		}
	}
}


template<class SegmentData>
bool TMultiPolylineCalibrationShape<SegmentData>::IsCurveTouched(istd::CIndex2d position) const
{
	I_ASSERT(IsDisplayConnected());

	const iview::TMultiPolyline<SegmentData>* polylinePtr =
					dynamic_cast<const iview::TMultiPolyline<SegmentData>*>(GetModelPtr());
	if (polylinePtr != NULL){
        const iview::IColorShema& colorShema = GetColorShema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const iview::IIsomorphicCalibration& calib = GetIsomorphCalib();

		int nodesCount = polylinePtr->GetNodesCount();

		i2d::CVector2d node1;
		int i;
		if (polylinePtr->IsClosed()){
			calib.GetApplyToView(polylinePtr->GetNode(nodesCount - 1), node1);
			i = 0;
		}
		else{
			calib.GetApplyToView(polylinePtr->GetNode(0), node1);
			i = 1;
		}

		double logicalLineWidth = colorShema.GetLogicalLineWidth();

		double proportions = ::sqrt(transform.GetDeformMatrix().GetDet());
		const i2d::CVector2d& viewMouse = transform.GetClientPosition(position);

		for (; i < nodesCount; i++){
			i2d::CVector2d node2;
			calib.GetApplyToView(polylinePtr->GetNode(i), node2);
			
			i2d::CVector2d delta = node2 - node1;

			if ((delta.GetDotProduct(viewMouse - node1) >= 0) && (delta.GetDotProduct(viewMouse - node2) <= 0)){
				i2d::CVector2d ortonormal = delta.GetOrthogonal().GetNormalized();
				double distance = ::fabs(ortonormal.GetDotProduct(viewMouse - node1));
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
bool TMultiPolylineCalibrationShape<SegmentData>::TrySelectSegments(
				iview::TMultiPolyline<SegmentData>& polyline,
				istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (IsDisplayConnected() && (modelPtr != NULL)){
		const iview::IColorShema& colorShema = GetColorShema();

		int nodesCount = polyline.GetNodesCount();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const iview::IIsomorphicCalibration& calib = GetIsomorphCalib();

		double proportions = ::sqrt(transform.GetDeformMatrix().GetDet());

		int nodeIndex;
		i2d::CVector2d node1;
		if (polyline.IsClosed()){
			calib.GetApplyToView(polyline.GetNode(nodesCount - 1), node1);
			nodeIndex = 0;
		}
		else{
			calib.GetApplyToView(polyline.GetNode(0), node1);
			nodeIndex = 1;
		}

		double logicalLineWidth = colorShema.GetLogicalLineWidth();

		for (; nodeIndex < nodesCount; nodeIndex++){
			i2d::CVector2d node2;
			calib.GetApplyToView(polyline.GetNode(nodeIndex), node2);
			const i2d::CVector2d& viewMouse = transform.GetClientPosition(position);

			i2d::CVector2d delta = node2 - node1;

			if ((delta.GetDotProduct(viewMouse - node1) >= 0) && (delta.GetDotProduct(viewMouse - node2) <= 0)){
				i2d::CVector2d ortonormal = delta.GetOrthogonal().GetNormalized();
				double distance = ::fabs(ortonormal.GetDotProduct(viewMouse - node1));
				if (proportions * distance < logicalLineWidth){
					int selectedSegment = nodeIndex - 1;
					bool selected = true;
					iview::int keysState = GetKeysState();

					if ((keysState & iview::ISelectable::KS_CONTROL) != 0){
						selected = !polyline.IsSegmentSelected(selectedSegment);
					}
					else if ((keysState & iview::ISelectable::KS_SHIFT) != 0){
						int firstSegment = selectedSegment - 1;
						int lastSegment = selectedSegment + 1;

						while ((firstSegment >= 0) && !polyline.IsSegmentSelected(firstSegment)){
							firstSegment--;
						}
						if (firstSegment < 0){
							firstSegment = selectedSegment;
						}

						int segmentsCount = polyline.GetSegmentsCount();
						while (lastSegment < segmentsCount && !polyline.IsSegmentSelected(lastSegment)){
							lastSegment++;
						}
						if (lastSegment >= segmentsCount){
							lastSegment = selectedSegment;
						}

						for (int currSegment = firstSegment; currSegment < lastSegment; currSegment++){
							polyline.SelectSegment(currSegment, true);
						}
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
	}
	return false;
}


template<class SegmentData>
iview::ITouchable::TouchState TMultiPolylineCalibrationShape<SegmentData>::IsTouched(istd::CIndex2d position) const
{
	const iview::TMultiPolyline<SegmentData>* polylinePtr =
					dynamic_cast<const iview::TMultiPolyline<SegmentData>*>(GetModelPtr());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
        const iview::IColorShema& colorShema = GetColorShema();

		int nodesCount = polylinePtr->GetNodesCount();

		int editMode = GetEditMode();

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const iview::IIsomorphicCalibration& calib = GetIsomorphCalib();

		switch (editMode){
		case iview::ISelectable::EM_NONE:
			if (IsSelected()){
				CTransformableRectangleCalibrationShape::EnsureValidNodes();
				if (CTransformableRectangleCalibrationShape::IsTickerTouched(position)){
					return TS_TICKER;
				}

				if (CTransformableRectangleCalibrationShape::IsParallTouched(m_castTransform, position)){
					bool isEditablePosition = IsEditablePosition();
					return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
				}
			}
			break;

		case iview::ISelectable::EM_MOVE:
			{
				const i2d::CRect& tickerBox = colorShema.GetTickerBox(iview::IColorShema::TT_MOVE);

				for (int i = 0; i < nodesCount; i++){
					i2d::CVector2d viewPos;
					calib.GetApplyToView(polylinePtr->GetNode(i), viewPos);
					istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
					if (tickerBox.IsInside(position - screenPos)){
						return TS_TICKER;
					}
				}
			}
			break;

		case iview::ISelectable::EM_ADD:
			{
				const i2d::CRect& tickerBox = colorShema.GetTickerBox(iview::IColorShema::TT_INSERT);

				for (int i = 0; i < nodesCount; i++){
					istd::CIndex2d screenPos;
					i2d::CVector2d viewPos;
					if (i == 0 && !polylinePtr->IsClosed()){
						i2d::CVector2d viewPos;
						calib.GetApplyToView(polylinePtr->GetNode(i), viewPos);
						screenPos = transform.GetScreenPosition(viewPos);
						if (tickerBox.IsInside(position - screenPos)){
							return TS_TICKER;
						}
					}

					if (i == (nodesCount - 1) && !polylinePtr->IsClosed()){
						calib.GetApplyToView(polylinePtr->GetNode(i), viewPos);
						screenPos = transform.GetScreenPosition(viewPos);
						if (tickerBox.IsInside(position - screenPos)){
							return TS_TICKER;
						}
					}

					calib.GetApplyToView((polylinePtr->GetNode(i) + polylinePtr->GetNode((i+1) % nodesCount)) * 0.5, viewPos);
					screenPos = transform.GetScreenPosition(viewPos);
					if (tickerBox.IsInside(position - screenPos)){
						return TS_TICKER;
					}
				}
			}
			break;

		case iview::ISelectable::EM_REMOVE:
			{
				i2d::CVector2d first, second;

				const i2d::CRect& tickerBox = colorShema.GetTickerBox(iview::IColorShema::TT_DELETE);

				int segmentsCount = polylinePtr->GetSegmentsCount();
				for (int i = 0; i < segmentsCount; i++){
					CalculateRemovePoints(*polylinePtr, i, first, second);

					i2d::CVector2d viewPos;
					calib.GetApplyToView(first, viewPos);
					istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
					if (tickerBox.IsInside(position - screenPos)){
						return TS_TICKER;
					}
					calib.GetApplyToView(second, viewPos);
					screenPos = transform.GetScreenPosition(viewPos);
					if (tickerBox.IsInside(position - screenPos)){
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
			else if ((editMode == iview::ISelectable::EM_NONE) || (editMode == iview::ISelectable::EM_MOVE)){
				return TS_OTHER;
			}
			else{
				return TS_INACTIVE;
			}
		}
	}

	return TS_NONE;
}


template<class SegmentData>
void  TMultiPolylineCalibrationShape<SegmentData>::SetSelected(bool selectFlag)
{
	iview::TMultiPolyline<SegmentData>* polylinePtr =
					dynamic_cast<iview::TMultiPolyline<SegmentData>*>(GetModelPtr());
	if (polylinePtr != NULL){
		if (IsSelected() != selectFlag){
			if (selectFlag){
				polylinePtr->SelectAllSegments(true);
			}
			else{
				polylinePtr->SelectAllSegments(false);
			}
		}
	}
    BaseClass::SetSelected(selectFlag);
}


template<class SegmentData>
void TMultiPolylineCalibrationShape<SegmentData>::CalcBoundingBox(i2d::CRect& result) const
{
	const iview::TMultiPolyline<SegmentData>* polylinePtr =
					dynamic_cast<const iview::TMultiPolyline<SegmentData>*>(GetModelPtr());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		int nodesCount = polylinePtr->GetNodesCount();
		int segmentsCount = polylinePtr->GetSegmentsCount();
		
		int parallelCount = polylinePtr->GetParallelsCount();

		BaseClass::CalcBoundingBox(result);

		if (segmentsCount > 0){
			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			const iview::IIsomorphicCalibration& calib = GetIsomorphCalib();

			for (int i = 0; i < nodesCount; i++){
				const i2d::CVector2d& position = polylinePtr->GetNode(i);
				const i2d::CVector2d& knee = polylinePtr->GetKneeVector(i);
				int segmentIndex = i % segmentsCount;
				for (int parallelIndex = 0; parallelIndex < parallelCount; ++parallelIndex){
					double parallelValue = polylinePtr->GetParallel(segmentIndex, parallelIndex);
					i2d::CVector2d edgePos = position + knee * parallelValue;
					i2d::CVector2d edgeView;
					calib.GetApplyToView(edgePos, edgeView);
					result.Union(transform.GetScreenPosition(edgeView));
				}
				segmentIndex = (i + 1) % segmentsCount;
				for (parallelIndex = 0; parallelIndex < parallelCount; ++parallelIndex){
					double parallelValue = polylinePtr->GetParallel(segmentIndex, parallelIndex);
					i2d::CVector2d edgePos = position + knee * parallelValue;
					i2d::CVector2d edgeView;
					calib.GetApplyToView(edgePos, edgeView);
					result.Union(transform.GetScreenPosition(edgeView));
				}
			}
		}
		
		result.SetBottom(result.GetBottom() + 1);
		result.SetRight(result.GetRight() + 1);

		return;
	}

	result.Reset(); 
}


// protected methods

template<class SegmentData>
void TMultiPolylineCalibrationShape<SegmentData>::CalculateRemovePoints(const iview::TMultiPolyline<SegmentData>& polyline, int segment, i2d::CVector2d& first, i2d::CVector2d& second) const
{
	int nodesCount = polyline.GetNodesCount();
    i2d::CVector2d line = polyline.GetNode((segment+1) % nodesCount) - polyline.GetNode(segment);
    first = polyline.GetNode(segment) + line * 0.2;
    second = polyline.GetNode(segment) + line * 0.8;
}


// reimplemented (iview::CInteractiveTransformableRectangleShape)

template<class SegmentData>
void TMultiPolylineCalibrationShape<SegmentData>::EnsureValidNodes() const
{
	if (!AreNodesValid()){
		const iview::TMultiPolyline<SegmentData>* polylinePtr =
						dynamic_cast<const iview::TMultiPolyline<SegmentData>*>(GetModelPtr());
		if (polylinePtr != NULL){
			i2d::CVector2d axisX = m_castAxis.GetNormalized();
			i2d::CVector2d axisY = axisX.GetOrthogonal();

			int nodesCount = polylinePtr->GetNodesCount();
			int segmentsCount = polylinePtr->GetSegmentsCount();
			if (nodesCount > 0){
				const i2d::CVector2d& firstNode = polylinePtr->GetNode(0);

				i2d::CVector2d minPosition(axisX.GetDotProduct(firstNode), axisY.GetDotProduct(firstNode));
				i2d::CVector2d maxPosition = minPosition;

				int parallelCount = polylinePtr->GetParallelsCount();
				for (int i = 0; i < nodesCount; i++){
					const i2d::CVector2d& position = polylinePtr->GetNode(i);

					i2d::CVector2d castedPosition(axisX.GetDotProduct(position), axisY.GetDotProduct(position));

					if (castedPosition.GetSetX() < minPosition.GetSetX()){
						minPosition.SetSetX(castedPosition.GetSetX());
					}
					if (castedPosition.GetSetY() < minPosition.GetSetY()){
						minPosition.SetSetY(castedPosition.GetSetY());
					}

					if (castedPosition.GetSetX() > maxPosition.GetSetX()){
						maxPosition.SetSetX(castedPosition.GetSetX());
					}
					if (castedPosition.GetSetY() > maxPosition.GetSetY()){
						maxPosition.SetY(castedPosition.GetSetY());
					}

					const i2d::CVector2d& knee = polylinePtr->GetKneeVector(i);
					int segmentIndex = i % segmentsCount;
					for (int parallelIndex = 0; parallelIndex < parallelCount; ++parallelIndex){
						double parallelValue = polylinePtr->GetParallel(segmentIndex, parallelIndex);
						const i2d::CVector2d& edgePos = polylinePtr->GetNode(i) + knee * parallelValue;

						castedPosition = i2d::CVector2d(axisX.GetDotProduct(edgePos), axisY.GetDotProduct(edgePos));

						if (castedPosition.GetSetX() < minPosition.GetSetX()){
							minPosition.SetX(castedPosition.GetSetX());
						}
						if (castedPosition.GetSetY() < minPosition.GetSetY()){
							minPosition.SetY(castedPosition.GetSetY());
						}

						if (castedPosition.GetSetX() > maxPosition.GetSetX()){
							maxPosition.SetX(castedPosition.GetSetX());
						}
						if (castedPosition.GetSetY() > maxPosition.GetSetY()){
							maxPosition.SetY(castedPosition.GetSetY());
						}
					}
					segmentIndex = (i + 1) % segmentsCount;
					for (parallelIndex = 0; parallelIndex < parallelCount; ++parallelIndex){
						double parallelValue = polylinePtr->GetParallel(segmentIndex, parallelIndex);
						const i2d::CVector2d& edgePos = polylinePtr->GetNode(i) + knee * parallelValue;

						castedPosition = i2d::CVector2d(axisX.GetDotProduct(edgePos), axisY.GetDotProduct(edgePos));

						if (castedPosition.GetSetX() < minPosition.GetSetX()){
							minPosition.SetX(castedPosition.GetSetX());
						}
						if (castedPosition.GetSetY() < minPosition.GetSetY()){
							minPosition.SetY(castedPosition.GetSetY());
						}

						if (castedPosition.GetSetX() > maxPosition.GetSetX()){
							maxPosition.SetX(castedPosition.GetSetX());
						}
						if (castedPosition.GetSetY() > maxPosition.GetSetY()){
							maxPosition.SetY(castedPosition.GetSetY());
						}
					}
				}
				i2d::CVector2d delta = maxPosition - minPosition;
				m_castTransform = i2d::CAffine2d(
								i2d::CMatrix2d(axisX * delta.GetSetX(), axisY * delta.GetSetY()),
								axisX * minPosition.GetSetX() + axisY * minPosition.GetSetY());

				CalcNodes(m_castTransform);
				return;
			}
		}
		ResetNodes();
	}
}


} // namespace iview


#endif // !iview_TMultiPolylineCalibrationShape_included


