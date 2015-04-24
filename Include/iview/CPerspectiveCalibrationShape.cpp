#include "iview/CPerspectiveCalibrationShape.h"


// Qt includes
#include <QtCore/qmath.h>
#include <QtGui/QPainter>

// ACF includes
#include "istd/CChangeNotifier.h"

#include "iqt/iqt.h"

#include "iview/IRuler.h"
#include "iview/IViewRulersAccessor.h"
#include "iview/CCalibratedViewBase.h"
#include "iview/CPerspectiveCalibration.h"



namespace iview
{


// reimplemented (imod::IObserver)

void CPerspectiveCalibrationShape::Invalidate()
{
	BaseClass::Invalidate();

	iview::IDisplay* displayPtr = GetDisplayPtr();
	iview::CViewBase* parentViewPtr = NULL;
	while (displayPtr != NULL){
		iview::CViewBase* viewPtr = dynamic_cast<iview::CViewBase*>(displayPtr);
		if (viewPtr != NULL){
			parentViewPtr = viewPtr;
		}

		displayPtr = displayPtr->GetParentDisplayPtr();
	}

	if (parentViewPtr != NULL){
		static const istd::IChangeable::ChangeSet calibChangeSet(CF_CALIB);
		parentViewPtr->UpdateAllShapes(calibChangeSet);
	}
}


// reimplemented (iview::IVisualizable)

void CPerspectiveCalibrationShape::Draw(QPainter& drawContext) const
{
	const CPerspectiveCalibration* calibPtr = dynamic_cast<const CPerspectiveCalibration*>(GetCalibrationPtr());
	if (calibPtr == NULL){
		BaseClass::Draw(drawContext);

		return;
	}

	if (IsDisplayConnected()){
		i2d::CRect clientRect = GetClientRect();

		iview::IViewRulersAccessor*  rulersAccessorPtr = NULL;
		iview::IVisualCalibrationInfo* calibInfoPtr = NULL;
		iview::IDisplay* displayPtr = GetDisplayPtr();
		while (displayPtr != NULL){
			if (rulersAccessorPtr == NULL){
				rulersAccessorPtr = dynamic_cast<iview::IViewRulersAccessor*>(displayPtr);
			}
			if (calibInfoPtr == NULL){
				calibInfoPtr = dynamic_cast<iview::IVisualCalibrationInfo*>(displayPtr);
			}
			displayPtr = displayPtr->GetParentDisplayPtr();
		}
		if (calibInfoPtr != NULL){
			if (!calibInfoPtr->IsGridInMm()){
				BaseClass::Draw(drawContext);
				return;
			}
		}

		const i3d::CVector3d& logAxisX = calibPtr->GetLogAxisGetX();
		const i2d::CRectangle& bounds = calibPtr->GetBounds();

		iview::IRuler* leftRulerPtr = NULL;
		iview::IRuler* topRulerPtr = NULL;

		if (rulersAccessorPtr != NULL){
			if (qAbs(logAxisX.GetX()) >= qAbs(logAxisX.GetY())){
				leftRulerPtr = rulersAccessorPtr->GetLeftRulerPtr();
				topRulerPtr = rulersAccessorPtr->GetTopRulerPtr();
			}
			else{
				topRulerPtr = rulersAccessorPtr->GetLeftRulerPtr();
				leftRulerPtr = rulersAccessorPtr->GetTopRulerPtr();
			}
		}

		if (calibInfoPtr != NULL){
			i2d::CLine2d topRulerLine;
			i2d::CLine2d leftRulerLine;

			bool isGridVisible = calibInfoPtr->IsGridVisible();
			if (isGridVisible || (leftRulerPtr != NULL) || (topRulerPtr != NULL)){
				istd::CChangeNotifier leftRulerNotifier(leftRulerPtr);
				istd::CChangeNotifier topRulerNotifier(topRulerPtr);

				if (topRulerPtr != NULL){
					if (topRulerPtr->GetLevelsCount() < 3){
						topRulerPtr->SetLevelsCount(3);
					}
					topRulerPtr->Clear();
					topRulerLine = topRulerPtr->GetScreenLine();
				}

				if (leftRulerPtr != NULL){
					if (leftRulerPtr->GetLevelsCount() < 3){
						leftRulerPtr->SetLevelsCount(3);
					}
					leftRulerPtr->Clear();
					leftRulerLine = leftRulerPtr->GetScreenLine();
				}

				const iview::IColorSchema& colorSchema = GetColorSchema();

				i2d::CVector2d logCorners[4];
				logCorners[0] = GetLogPosition(clientRect.GetLeftTop());
				logCorners[1] = GetLogPosition(clientRect.GetRightTop());
				logCorners[2] = GetLogPosition(clientRect.GetLeftBottom());
				logCorners[3] = GetLogPosition(clientRect.GetRightBottom());

				i2d::CVector2d viewLeftTop;
				calibPtr->GetInvPositionAt(bounds.GetLeftTop(), viewLeftTop);
				i2d::CVector2d viewLeftBottom;
				calibPtr->GetInvPositionAt(bounds.GetLeftBottom(), viewLeftBottom);
				i2d::CVector2d viewRightTop;
				calibPtr->GetInvPositionAt(bounds.GetRightTop(), viewRightTop);
				i2d::CVector2d viewRightBottom;
				calibPtr->GetInvPositionAt(bounds.GetRightBottom(), viewRightBottom);

				double perspScale = (
								viewLeftTop.GetDistance(viewRightTop) / bounds.GetWidth() +
								viewLeftBottom.GetDistance(viewRightBottom) / bounds.GetWidth() +
								viewLeftTop.GetDistance(viewLeftBottom) / bounds.GetHeight() +
								viewRightTop.GetDistance(viewRightBottom) / bounds.GetHeight()) * 0.25;

				double viewScale = GetViewToScreenTransform().GetDeformMatrix().GetApproxScale();

				int levels[2];
				double minGridDistance = calibInfoPtr->GetMinGridDistance() / (viewScale * perspScale);
				double grid = qPow(10.0, qCeil(log10(minGridDistance)));
				if (grid * 0.5 < minGridDistance){
					levels[0] = 5;
					levels[1] = 10;
				}
				else{
					grid *= 0.5;
					levels[0] = 2;
					levels[1] = 20;
				}

				int firstIndex = int(qFloor(bounds.GetLeft() / grid));
				int lastIndex = int(qCeil(bounds.GetRight() / grid));

				drawContext.save();
				drawContext.setPen(colorSchema.GetPen(iview::IColorSchema::SP_GUIDELINE3));
				drawContext.setBrush(colorSchema.GetBrush(iview::IColorSchema::SB_TRANSPARENT));
				const QPen& level0Pen = colorSchema.GetPen(iview::IColorSchema::SP_GUIDELINE2);
				const QPen& level1Pen = colorSchema.GetPen(iview::IColorSchema::SP_GUIDELINE1);

				int index;
				for (index = firstIndex; index <= lastIndex; ++index){
					i2d::CVector2d logPos1(index * grid, bounds.GetTop());
					i2d::CVector2d logPos2(index * grid, bounds.GetBottom());
					i2d::CVector2d viewPos1;
					i2d::CVector2d viewPos2;
					calibPtr->GetInvPositionAt(logPos1, viewPos1);
					calibPtr->GetInvPositionAt(logPos2, viewPos2);
					QPointF point1 = GetScreenPosition(viewPos1);
					QPointF point2 = GetScreenPosition(viewPos2);

					int levelIndex = 0;

					if (index % levels[1] == 0){
						if (isGridVisible){
							drawContext.save();
							drawContext.setPen(level1Pen);
							drawContext.drawLine(point1, point2);
							drawContext.restore();
						}
					}
					else if (index % levels[0] == 0){
						if (isGridVisible){
							drawContext.save();
							drawContext.setPen(level0Pen);
							drawContext.drawLine(point1, point2);
							drawContext.restore();
						}

						levelIndex = 1;
					}
					else{
						if (isGridVisible){
							drawContext.drawLine(point1, point2);
						}

						levelIndex = 2;
					}

					if (topRulerPtr != NULL){
						i2d::CLine2d screenLine;
						screenLine.SetPoint1(i2d::CVector2d(point1.x(), point1.y()));
						screenLine.SetPoint2(i2d::CVector2d(point2.x(), point2.y()));
						if (screenLine.IsIntersectedBy(topRulerLine)){
							iview::IRuler::Mark mark(screenLine.GetCutAlpha(topRulerLine), index * grid);
							if ((mark.first >= 0) && (mark.first <= 1)){
								topRulerPtr->InsertMark(levelIndex, mark);
							}
						}
					}
				}

				firstIndex = int(qFloor(bounds.GetTop() / grid));
				lastIndex = int(qCeil(bounds.GetBottom() / grid));

				for (index = firstIndex; index <= lastIndex; ++index){
					i2d::CVector2d logPos1(bounds.GetLeft(), index * grid);
					i2d::CVector2d logPos2(bounds.GetRight(), index * grid);
					i2d::CVector2d viewPos1;
					i2d::CVector2d viewPos2;
					calibPtr->GetInvPositionAt(logPos1, viewPos1);
					calibPtr->GetInvPositionAt(logPos2, viewPos2);

					QPointF point1 = GetScreenPosition(viewPos1);
					QPointF point2 = GetScreenPosition(viewPos2);

					int levelIndex = 0;

					if (index % levels[1] == 0){
						if (isGridVisible){
							drawContext.save();
							drawContext.setPen(level1Pen);
							drawContext.drawLine(point1, point2);
							drawContext.restore();
						}
					}
					else if (index % levels[0] == 0){
						if (isGridVisible){
							drawContext.save();
							drawContext.setPen(level0Pen);
							drawContext.drawLine(point1, point2);
							drawContext.restore();
						}

						levelIndex = 1;
					}
					else{
						if (isGridVisible){
							drawContext.drawLine(point1, point2);
						}

						levelIndex = 2;
					}

					if (leftRulerPtr != NULL){
						i2d::CLine2d screenLine;
						screenLine.SetPoint1(i2d::CVector2d(point1.x(), point1.y()));
						screenLine.SetPoint2(i2d::CVector2d(point2.x(), point2.y()));
						if (screenLine.IsIntersectedBy(leftRulerLine)){
							iview::IRuler::Mark mark(screenLine.GetCutAlpha(leftRulerLine), index * grid);
							if ((mark.first >= 0) && (mark.first <= 1)){
								leftRulerPtr->InsertMark(levelIndex, mark);
							}
						}
					}
				}

				drawContext.restore();
			}
		}
	}
}


} // namespace iview


