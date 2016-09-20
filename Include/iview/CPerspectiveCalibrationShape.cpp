#include <iview/CPerspectiveCalibrationShape.h>


// Qt includes
#include <QtCore/qmath.h>
#include <QtGui/QPainter>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <i2d/ICalibration2d.h>
#include <iview/IRuler.h>
#include <iview/IViewRulersAccessor.h>
#include <iview/CCalibratedViewBase.h>



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
	const i2d::ICalibration2d* calibPtr = GetCalibration();
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

		iview::IRuler* leftRulerPtr = NULL;
		iview::IRuler* topRulerPtr = NULL;

		i2d::CAffine2d zeroPosTransform;
		if (calibPtr->GetLocalTransform(i2d::CVector2d::GetZero(), zeroPosTransform)){
			if (rulersAccessorPtr != NULL){
				const i2d::CVector2d& logAxisX = zeroPosTransform.GetDeformMatrix().GetAxisX();

				if (qAbs(logAxisX.GetX()) >= qAbs(logAxisX.GetY())){
					leftRulerPtr = rulersAccessorPtr->GetLeftRulerPtr();
					topRulerPtr = rulersAccessorPtr->GetTopRulerPtr();
				}
				else{
					topRulerPtr = rulersAccessorPtr->GetLeftRulerPtr();
					leftRulerPtr = rulersAccessorPtr->GetTopRulerPtr();
				}
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

				i2d::CRectangle bounds(-100, -100, 200, 200);
				const i2d::CRectangle* argumentAreaPtr = calibPtr->GetArgumentArea();
				if (argumentAreaPtr != NULL){
					bounds = *argumentAreaPtr;
				}

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
					i2d::CLine2d logLine(index * grid, bounds.GetTop(), index * grid, bounds.GetBottom());
					QPointF point1;
					QPointF point2;
					if (!GetLineScreenPosition(*calibPtr, logLine, lastIndex - firstIndex + 1, point1, point2)){
						continue;
					}

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
					i2d::CLine2d logLine(bounds.GetLeft(), index * grid, bounds.GetRight(), index * grid);
					QPointF point1;
					QPointF point2;
					if (!GetLineScreenPosition(*calibPtr, logLine, lastIndex - firstIndex + 1, point1, point2)){
						continue;
					}

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


// protected methods

bool CPerspectiveCalibrationShape::GetLineScreenPosition(const i2d::ICalibration2d& calib, const i2d::CLine2d& logLine, int gridSize, QPointF& point1, QPointF& point2) const
{
	const iview::CScreenTransform& transform = GetViewToScreenTransform();

	int leftGridIndex = 0;
	bool isLeftValid = false;
	i2d::CVector2d leftPosition;

	int rightGridIndex = gridSize;
	bool isRightValid = false;
	i2d::CVector2d rightPosition;

	while (leftGridIndex < rightGridIndex){
		if (!isLeftValid){
			if (calib.GetPositionAt(logLine.GetPositionFromAlpha(double(leftGridIndex) / gridSize), leftPosition)){
				isLeftValid = true;
			}
			else{
				++leftGridIndex;
			}
		}
		if (!isRightValid){
			if (calib.GetPositionAt(logLine.GetPositionFromAlpha(double(rightGridIndex) / gridSize), rightPosition)){
				isRightValid = true;
			}
			else{
				--rightGridIndex;
			}
		}

		if (isLeftValid && isRightValid){
			point1 = transform.GetApply(leftPosition);
			point2 = transform.GetApply(rightPosition);

			return true;
		}

		if (!isLeftValid && !isRightValid){
			break;
		}
	}

	return false;
}


} // namespace iview


