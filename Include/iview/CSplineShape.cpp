#include <iview/CSplineShape.h>


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <imod/IModel.h>

#include <i2d/CSpline.h>
#include <i2d/CSplineSegment.h>

#include <iqt/iqt.h>

#include <iview/IColorSchema.h>
#include <iview/CScreenTransform.h>


namespace iview
{


// reimplemented (imod::IObserver)

bool CSplineShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CSpline*>(modelPtr) != NULL);

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// protected methods

void CSplineShape::DrawPolyBezier(QPainter& drawContext, const i2d::CVector2d* pointsPtr, int pointsCount) const
{
	Q_ASSERT(pointsPtr != NULL);
	Q_ASSERT(pointsCount > 0);

	drawContext.save();
	drawContext.setBrush(QBrush(QColor(0,0,0,0)));

	if (pointsCount >= 4){
		QPainterPath qtPatch(pointsPtr[0]);

		for (int i = 3; i < pointsCount; i += 3){
			qtPatch.cubicTo(
						pointsPtr[i - 2],
						pointsPtr[i - 1],
						pointsPtr[i]);
		}

		drawContext.drawPath(qtPatch);

		drawContext.restore();
	}
}


// reimplemented (iview::CPolygonShape)

i2d::CVector2d CSplineShape::GetSegmentMiddle(int index) const
{
	const imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		const i2d::CSpline& model = *dynamic_cast<const i2d::CSpline*>(modelPtr);
		Q_ASSERT(&model != NULL);

		const i2d::CSplineSegment& segment = model.GetSplineSegment(index);

		return segment.GetPointInSegment(0.5);
	}
	else{
		return i2d::CVector2d(0, 0);
	}
}


void CSplineShape::DrawCurve(QPainter& drawContext) const
{
	const i2d::CSpline* splinePtr = dynamic_cast<const i2d::CSpline*>(GetObservedModel());
	if (IsDisplayConnected() && (splinePtr != NULL)){
		const IColorSchema& colorSchema = GetColorSchema();

		drawContext.save();
		if (IsSelected()){
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
		}
		else{
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
		}

		int segmentsCount = splinePtr->GetSegmentCount();
		if (segmentsCount > 0){
			i2d::CVector2d points[4];
			points[0] = GetScreenPosition(splinePtr->GetNodePos(0));
			for (int i = 0; i < segmentsCount; i++){
				const i2d::CSplineSegment& segment = splinePtr->GetSplineSegment(i);

				points[1] = GetScreenPosition(segment.GetBezierPointBegin());
				points[2] = GetScreenPosition(segment.GetBezierPointEnd());
				points[3] = GetScreenPosition(segment.GetPointEnd());

				DrawPolyBezier(drawContext, points, 4);

				points[0] = points[3];
			}
		}

		drawContext.restore();
	}
}


// reimplemented (iview::CRectControlledShapeBase)

bool CSplineShape::IsCurveTouched(istd::CIndex2d position) const
{
	const i2d::CSpline* splinePtr = dynamic_cast<const i2d::CSpline*>(GetObservedModel());
	if (IsDisplayConnected() && (splinePtr != NULL)){
		const IColorSchema& colorSchema = GetColorSchema();
		double viewScale = GetViewToScreenTransform().GetDeformMatrix().GetApproxScale();

		double minDistance = colorSchema.GetLogicalLineWidth() / viewScale;
		i2d::CVector2d cp = GetLogPosition(position);

		int segmentCount = splinePtr->GetSegmentCount();
		for (int i = 0; i < segmentCount; i++){
			const i2d::CSplineSegment& segment = splinePtr->GetSplineSegment(i);

			if (segment.IsCloserThan(cp, minDistance)){
				return true;
			}
		}
	}

	return false;
}


// reimplemented (iview::CShapeBase)

i2d::CRect CSplineShape::CalcBoundingBox() const
{
	const i2d::CSpline* splinePtr = dynamic_cast<const i2d::CSpline*>(GetObservedModel());
	if (IsDisplayConnected() && (splinePtr != NULL)){
		const IColorSchema& colorSchema = GetColorSchema();

		int segmentCount = splinePtr->GetSegmentCount();
		if (segmentCount > 0){
			const i2d::CSplineSegment& segment = splinePtr->GetSplineSegment(0);

			istd::CIndex2d sp = GetScreenPosition(segment.GetBezierPointBegin()).ToIndex2d();
			i2d::CRect boundingBox(sp, sp);

			sp = GetScreenPosition(segment.GetBezierPointEnd()).ToIndex2d();
			boundingBox.Union(sp);

			for (int i = 1; i < segmentCount; ++i){
				const i2d::CSplineSegment& segment = splinePtr->GetSplineSegment(i);

				sp = GetScreenPosition(segment.GetBezierPointBegin()).ToIndex2d();
				boundingBox.Union(sp);
				sp = GetScreenPosition(segment.GetBezierPointEnd()).ToIndex2d();
				boundingBox.Union(sp);
			}

			IColorSchema::TickerType tickerType;

			if (IsSelected()){
				int editMode = GetEditMode();
				switch (editMode){
				case ISelectable::EM_NONE:
					boundingBox.Expand(i2d::CRect(-2, -2, 2, 2));
					tickerType = IColorSchema::TT_NORMAL;
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
					tickerType = IColorSchema::TT_INACTIVE;
					break;
				}
			}
			else{
				tickerType = IColorSchema::TT_INACTIVE;
			}

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(tickerType);

			boundingBox.Expand(tickerBox);

			boundingBox.Union(CPolylineShape::CalcBoundingBox());

			return boundingBox;
		}
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview


