#include "iview/CInteractiveSplineShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "imod/IModel.h"
#include "i2d/CSpline.h"
#include "i2d/CSplineSegment.h"
#include "iqt/iqt.h"



#include "iview/CScreenTransform.h"


namespace iview
{


// reimplemented (imod::IObserver)

bool CInteractiveSplineShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CSpline*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


i2d::CRect CInteractiveSplineShape::CalcBoundingBox() const
{
	const i2d::CSpline* splinePtr = dynamic_cast<const i2d::CSpline*>(GetModelPtr());
	if (IsDisplayConnected() && (splinePtr != NULL)){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
        const IColorShema& colorShema = GetColorShema();

		int segmentCount = splinePtr->GetSegmentCount();
		if (segmentCount > 0){
			const i2d::CSplineSegment& segment = splinePtr->GetSplineSegment(0);

			istd::CIndex2d sp = transform.GetScreenPosition(segment.GetBezierPointBegin());
			i2d::CRect boundingBox(sp, sp);

			sp = transform.GetScreenPosition(segment.GetBezierPointEnd());
			boundingBox.Union(sp);

			for (int i = 1; i < segmentCount; ++i){
				const i2d::CSplineSegment& segment = splinePtr->GetSplineSegment(i);

				sp = transform.GetScreenPosition(segment.GetBezierPointBegin());
				boundingBox.Union(sp);
				sp = transform.GetScreenPosition(segment.GetBezierPointEnd());
				boundingBox.Union(sp);
			}

			IColorShema::TickerType tickerType;

			if (IsSelected()){
				int editMode = GetEditMode();
				switch (editMode){
				case ISelectable::EM_NONE:
					boundingBox.Expand(i2d::CRect(-2, -2, 2, 2));
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
					tickerType = IColorShema::TT_INACTIVE;
					break;
				}
			}
			else{
				tickerType = IColorShema::TT_INACTIVE;
			}

			const i2d::CRect& tickerBox = colorShema.GetTickerBox(tickerType);

			boundingBox.Expand(tickerBox);

			boundingBox.Union(CInteractivePolylineShape::CalcBoundingBox());

			return boundingBox;
		}
	}

	return i2d::CRect();
}


void CInteractiveSplineShape::DrawPolyBezier(QPainter& drawContext, const istd::CIndex2d* pointsPtr, int pointsCount) const
{
	I_ASSERT(pointsPtr != NULL);
	I_ASSERT(pointsCount > 0);

	drawContext.save();
	drawContext.setBrush(QBrush(QColor(0,0,0,0)));

	if (pointsCount >= 4){
		QPainterPath qtPatch(iqt::GetQPointF(i2d::CVector2d(pointsPtr[0])));
		for (int i = 3; i < pointsCount; i += 3){
			qtPatch.cubicTo(iqt::GetQPoint(pointsPtr[i - 2]),
							iqt::GetQPoint(pointsPtr[i - 1]),
							iqt::GetQPoint(pointsPtr[i]));
		}

		drawContext.drawPath(qtPatch);

		drawContext.restore();
	}
}


// reimplemented (iview::CInteractivePolygonShape)

i2d::CVector2d CInteractiveSplineShape::GetSegmentMiddle(int index) const
{
	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CSpline& model = *dynamic_cast<const i2d::CSpline*>(modelPtr);
		I_ASSERT(&model != NULL);

		const i2d::CSplineSegment& segment = model.GetSplineSegment(index);

		return segment.GetPointInSegment(0.5);
	}
	else{
		return i2d::CVector2d(0, 0);
	}
}


void CInteractiveSplineShape::DrawCurve(QPainter& drawContext) const
{
	const i2d::CSpline* splinePtr = dynamic_cast<const i2d::CSpline*>(GetModelPtr());
	if (IsDisplayConnected() && (splinePtr != NULL)){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
        const IColorShema& colorShema = GetColorShema();

		drawContext.save();
		if (IsSelected()){
			drawContext.setPen(colorShema.GetPen(IColorShema::SP_SELECTED));
		}
		else{
			drawContext.setPen(colorShema.GetPen(IColorShema::SP_NORMAL));
		}

		int segmentsCount = splinePtr->GetSegmentCount();
		if (segmentsCount > 0){
			istd::CIndex2d points[4];
			points[0] = transform.GetScreenPosition(splinePtr->GetNode(0));
			for (int i = 0; i < segmentsCount; i++){
				const i2d::CSplineSegment& segment = splinePtr->GetSplineSegment(i);

				points[1] = transform.GetScreenPosition(segment.GetBezierPointBegin());
				points[2] = transform.GetScreenPosition(segment.GetBezierPointEnd());
				points[3] = transform.GetScreenPosition(segment.GetPointEnd());

				DrawPolyBezier(drawContext, points, 4);

				points[0] = points[3];
			}
		}

		drawContext.restore();
	}
}


bool CInteractiveSplineShape::IsCurveTouched(istd::CIndex2d position) const
{
	const i2d::CSpline* splinePtr = dynamic_cast<const i2d::CSpline*>(GetModelPtr());
	if (IsDisplayConnected() && (splinePtr != NULL)){
        const IColorShema& colorShema = GetColorShema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		double proportions = ::sqrt(transform.GetDeformMatrix().GetDet());

		double minDistance = colorShema.GetLogicalLineWidth() / proportions;
		i2d::CVector2d cp = transform.GetClientPosition(position);

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


} // namespace iview


