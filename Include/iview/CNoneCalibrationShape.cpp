#include "iview/CNoneCalibrationShape.h"


// Qt includes
#include <QtCore/qmath.h>
#include <QtGui/QPainter>

// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/iqt.h"

#include "iview/IRuler.h"
#include "iview/IViewRulersAccessor.h"
#include "iview/CCalibratedViewBase.h"


namespace iview
{


// public methods

// reimplemented (iview::IVisualizable)

void CNoneCalibrationShape::Draw(QPainter& drawContext) const
{
	if (IsDisplayConnected()){
		i2d::CRect clientRect = GetClientRect();
		if (clientRect.IsEmpty()){
			return;
		}

		istd::TChangeNotifier<iview::IRuler> leftRulerPtr(NULL);
		istd::TChangeNotifier<iview::IRuler> topRulerPtr(NULL);

		iview::IViewRulersAccessor* rulersAccessorPtr = NULL;
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

		if (rulersAccessorPtr != NULL){
			leftRulerPtr.SetPtr(rulersAccessorPtr->GetLeftRulerPtr());
			topRulerPtr.SetPtr(rulersAccessorPtr->GetTopRulerPtr());
		}

		if (calibInfoPtr != NULL){
			bool isGridVisible = calibInfoPtr->IsGridVisible();
			if (isGridVisible || leftRulerPtr.IsValid() || topRulerPtr.IsValid()){
				if (topRulerPtr.IsValid()){
					if (topRulerPtr->GetLevelsCount() < 3){
						topRulerPtr->SetLevelsCount(3);
					}
					topRulerPtr->Clear();
				}

				if (leftRulerPtr.IsValid()){
					if (leftRulerPtr->GetLevelsCount() < 3){
						leftRulerPtr->SetLevelsCount(3);
					}
					leftRulerPtr->Clear();
				}

				const iview::IColorShema& colorShema = GetColorShema();
				const iview::CScreenTransform& transform = GetViewToScreenTransform();

				i2d::CVector2d logCorners[4];
				logCorners[0] = transform.GetClientPosition(clientRect.GetLeftTop());
				logCorners[1] = transform.GetClientPosition(clientRect.GetRightTop());
				logCorners[2] = transform.GetClientPosition(clientRect.GetLeftBottom());
				logCorners[3] = transform.GetClientPosition(clientRect.GetRightBottom());

				double left = qMin(qMin(logCorners[0].GetX(), logCorners[1].GetX()), qMin(logCorners[2].GetX(), logCorners[3].GetX()));
				double top = qMin(qMin(logCorners[0].GetY(), logCorners[1].GetY()), qMin(logCorners[2].GetY(), logCorners[3].GetY()));
				double right = qMax(qMax(logCorners[0].GetX(), logCorners[1].GetX()), qMax(logCorners[2].GetX(), logCorners[3].GetX()));
				double bottom = qMax(qMax(logCorners[0].GetY(), logCorners[1].GetY()), qMax(logCorners[2].GetY(), logCorners[3].GetY()));

				i2d::CRectangle boundRectangle(left, top, right-left, bottom-top);

				double scale = transform.GetDeformMatrix().GetAxisX().GetLength();

				int levels[2];
				double minGridDistance = calibInfoPtr->GetMinGridDistance() / scale;
				double grid = qPow(10.0, int(ceil(log10(minGridDistance))));
				if (grid * 0.5 < minGridDistance){
					levels[0] = 5;
					levels[1] = 10;
				}
				else{
					grid *= 0.5;
					levels[0] = 2;
					levels[1] = 20;
				}

				int firstIndex = int(::floor(boundRectangle.GetLeft() / grid)) + 1;
				int lastIndex = int(::ceil(boundRectangle.GetRight() / grid));

				drawContext.save();
				drawContext.setPen(colorShema.GetPen(iview::IColorShema::SP_GUIDELINE3));
				drawContext.setBrush(colorShema.GetBrush(iview::IColorShema::SB_TRANSPARENT));
				const QPen& level0Pen = colorShema.GetPen(iview::IColorShema::SP_GUIDELINE2);
				const QPen& level1Pen = colorShema.GetPen(iview::IColorShema::SP_GUIDELINE1);

				int index;
				for (index = firstIndex; index <= lastIndex; ++index){
					i2d::CVector2d position1(index * grid, boundRectangle.GetTop());
					i2d::CVector2d position2(index * grid, boundRectangle.GetBottom());
					QPoint point1 = iqt::GetQPoint(transform.GetScreenPosition(position1));
					QPoint point2 = iqt::GetQPoint(transform.GetScreenPosition(position2));

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

					if ((topRulerPtr != NULL) && (point1.y() != point2.y())){
						int rulerX = point1.x() - (point2.x() - point1.x()) * point1.y() / (point2.y() - point1.y());
						iview::IRuler::Mark mark(double(rulerX) / clientRect.GetWidth(), index * grid);
						if ((mark.first >= 0) && (mark.first <= 1)){
							topRulerPtr->InsertMark(levelIndex, mark);
						}
					}
				}

				firstIndex = int(::floor(boundRectangle.GetTop() / grid)) + 1;
				lastIndex = int(::ceil(boundRectangle.GetBottom() / grid));

				for (index = firstIndex; index <= lastIndex; ++index){
					i2d::CVector2d position1(boundRectangle.GetLeft(), index * grid);
					i2d::CVector2d position2(boundRectangle.GetRight(), index * grid);

					QPoint point1 = iqt::GetQPoint(transform.GetScreenPosition(position1));
					QPoint point2 = iqt::GetQPoint(transform.GetScreenPosition(position2));

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

					if ((leftRulerPtr.IsValid()) && (point1.x() != point2.x())){
						int rulerY = point1.y() - (point2.y() - point1.y()) * point1.x() / (point2.x() - point1.x());
						iview::IRuler::Mark mark(double(rulerY) / clientRect.GetHeight(), index * grid);
						if ((mark.first >= 0) && (mark.first <= 1)){
							leftRulerPtr->InsertMark(levelIndex, mark);
						}
					}
				}

				drawContext.restore();
			}
		}
	}
}


// reimplemented (iview::IInteractiveShape)

iview::ITouchable::TouchState CNoneCalibrationShape::IsTouched(istd::CIndex2d /*position*/) const
{
	return TS_NONE;
}


// reimplemented (iview::IMouseActionObserver)

bool CNoneCalibrationShape::OnMouseButton(istd::CIndex2d /*position*/, Qt::MouseButton /*buttonType*/, bool /*downFlag*/)
{
	return false;
}


bool CNoneCalibrationShape::OnMouseMove(istd::CIndex2d /*position*/)
{
	return false;
}


// protected methods

const i2d::ITransformation2d* CNoneCalibrationShape::GetCalibrationPtr() const
{
	const i2d::ITransformation2d* calibrationPtr = dynamic_cast<const i2d::ITransformation2d*>(GetModelPtr());
	if (calibrationPtr == NULL){
		const iview::IDisplay* displayPtr = GetDisplayPtr();
		while (displayPtr != NULL){
			const i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<const i2d::ICalibrationProvider*>(displayPtr);
			if ((calibrationProviderPtr != NULL)){
				return calibrationProviderPtr->GetCalibration();
			}

			displayPtr = displayPtr->GetParentDisplayPtr();
		}
	}

	return calibrationPtr;
}


// reimplemented (iview::CInteractiveShapeBase)

void CNoneCalibrationShape::CalcBoundingBox(i2d::CRect& result) const
{
	iview::IDisplay* displayPtr = GetDisplayPtr();
	if (displayPtr != NULL){
		result = displayPtr->GetClientRect();
	}
	else{
		result.Reset();
	}
}
 

void CNoneCalibrationShape::BeginLogDrag(const i2d::CVector2d& /*reference*/)
{
}


void CNoneCalibrationShape::SetLogDragPosition(const i2d::CVector2d& /*position*/)
{
}


} // namespace iview


