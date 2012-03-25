#include "iview/CInteractivePerspectiveCalibrationShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "iqt/iqt.h"

#include "iview/CCalibratedViewBase.h"
#include "iview/CPerspectiveCalibration.h"


namespace iview
{


// reimplemented (iview::IInteractiveShape)

iview::ITouchable::TouchState CInteractivePerspectiveCalibrationShape::IsTouched(istd::CIndex2d position) const
{
	I_ASSERT(IsDisplayConnected());

	const CPerspectiveCalibration* calibPtr = dynamic_cast<const CPerspectiveCalibration*>(GetModelPtr());
	if (calibPtr != NULL){
		const i2d::CRectangle& bounds = calibPtr->GetBounds();
		i2d::CVector2d viewLeftTop, viewRightTop, viewLeftBottom, viewRightBottom;

		calibPtr->GetInvPositionAt(bounds.GetTopLeft(), viewLeftTop);
		calibPtr->GetInvPositionAt(bounds.GetTopRight(), viewRightTop);
		calibPtr->GetInvPositionAt(bounds.GetBottomLeft(), viewLeftBottom);
		calibPtr->GetInvPositionAt(bounds.GetBottomRight(), viewRightBottom);

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const iview::IColorShema& colorShema = GetColorShema();

		i2d::CVector2d viewPos = transform.GetClientPosition(position);

		i2d::CLine2d viewLeft(viewLeftTop, viewLeftBottom);
		i2d::CLine2d viewRight(viewRightTop, viewRightBottom);
		i2d::CLine2d viewTop(viewLeftTop, viewRightTop);
		i2d::CLine2d viewBottom(viewLeftBottom, viewRightBottom);

        double proportions = ::sqrt(transform.GetDeformMatrix().GetDet());
        double maxDistance = colorShema.GetLogicalLineWidth() / proportions;

		if (			(viewLeft.GetDistance(viewPos) < maxDistance) ||
						(viewRight.GetDistance(viewPos) < maxDistance) ||
						(viewTop.GetDistance(viewPos) < maxDistance) ||
						(viewBottom.GetDistance(viewPos) < maxDistance)){
			return TS_TICKER;
		}
	}

	return TS_NONE;
}


// reimplemented (iview::IVisualizable)

void CInteractivePerspectiveCalibrationShape::Draw(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	const CPerspectiveCalibration* calibPtr = dynamic_cast<const CPerspectiveCalibration*>(GetModelPtr());
	if (calibPtr != NULL){
		const i2d::CRectangle& bounds = calibPtr->GetBounds();
		i2d::CVector2d viewLeftTop, viewRightTop, viewLeftBottom, viewRightBottom;

		calibPtr->GetInvPositionAt(bounds.GetTopLeft(), viewLeftTop);
		calibPtr->GetInvPositionAt(bounds.GetTopRight(), viewRightTop);
		calibPtr->GetInvPositionAt(bounds.GetBottomLeft(), viewLeftBottom);
		calibPtr->GetInvPositionAt(bounds.GetBottomRight(), viewRightBottom);

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const iview::IColorShema& colorShema = GetColorShema();

		bool isSelected = IsSelected();
		drawContext.save();
		drawContext.setPen(colorShema.GetPen(isSelected? iview::IColorShema::SP_SELECTED: iview::IColorShema::SP_NORMAL));

		QPoint screenLeftTop = iqt::GetQPoint(transform.GetScreenPosition(viewLeftTop));
		QPoint screenLeftBottom = iqt::GetQPoint(transform.GetScreenPosition(viewLeftBottom));
		QPoint screenRightTop = iqt::GetQPoint(transform.GetScreenPosition(viewRightTop));
		QPoint screenRightBottom = iqt::GetQPoint(transform.GetScreenPosition(viewRightBottom));

		drawContext.drawLine(screenLeftTop, screenRightTop);
		drawContext.drawLine(screenRightTop, screenRightBottom);
		drawContext.drawLine(screenRightBottom, screenLeftBottom);
		drawContext.drawLine(screenLeftBottom, screenLeftTop);
		
		drawContext.restore();
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractivePerspectiveCalibrationShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
{
	I_ASSERT(IsDisplayConnected());

	if (!IsEditablePosition()){
		return false;
	}

	if (downFlag){
		const CPerspectiveCalibration* calibPtr = dynamic_cast<const CPerspectiveCalibration*>(GetModelPtr());
		if (calibPtr != NULL){
			const i2d::CRectangle& bounds = calibPtr->GetBounds();
			i2d::CVector2d viewLeftTop, viewRightTop, viewLeftBottom, viewRightBottom;

			calibPtr->GetInvPositionAt(bounds.GetTopLeft(), viewLeftTop);
			calibPtr->GetInvPositionAt(bounds.GetTopRight(), viewRightTop);
			calibPtr->GetInvPositionAt(bounds.GetBottomLeft(), viewLeftBottom);
			calibPtr->GetInvPositionAt(bounds.GetBottomRight(), viewRightBottom);

			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			const iview::IColorShema& colorShema = GetColorShema();

			i2d::CVector2d viewPos = transform.GetClientPosition(position);

			i2d::CLine2d viewLeft(viewLeftTop, viewLeftBottom);
			i2d::CLine2d viewRight(viewRightTop, viewRightBottom);
			i2d::CLine2d viewTop(viewLeftTop, viewRightTop);
			i2d::CLine2d viewBottom(viewLeftBottom, viewRightBottom);

			double proportions = ::sqrt(transform.GetDeformMatrix().GetDet());
			double maxDistance = colorShema.GetLogicalLineWidth() / proportions;

			if (viewLeft.GetDistance(viewPos) < maxDistance){
				m_editBound = EB_LEFT;
				BeginModelChanges();
				return true;
			}
			if (viewRight.GetDistance(viewPos) < maxDistance){
				m_editBound = EB_RIGHT;
				BeginModelChanges();
				return true;
			}
			if (viewTop.GetDistance(viewPos) < maxDistance){
				m_editBound = EB_TOP;
				BeginModelChanges();
				return true;
			}
			if (viewBottom.GetDistance(viewPos) < maxDistance){
				m_editBound = EB_BOTTOM;
				BeginModelChanges();
				return true;
			}
		}
	}

	EndModelChanges();

	return false;
}


bool CInteractivePerspectiveCalibrationShape::OnMouseMove(istd::CIndex2d position)
{
	if (!IsEditablePosition()){
		return false;
	}

	imod::IModel* modelPtr = GetModelPtr();
	CPerspectiveCalibration* calibPtr = dynamic_cast<CPerspectiveCalibration*>(modelPtr);
	if (calibPtr != NULL){
		i2d::CRectangle bounds = calibPtr->GetBounds();

		iview::CScreenTransform transform = GetLogToScreenTransform();
		i2d::CVector2d viewPos = transform.GetClientPosition(position);

		i2d::CVector2d logPos;
		if (calibPtr->GetPositionAt(viewPos, logPos)){
			bool isChanged = false;

			switch (m_editBound){
			case EB_LEFT:
				if (logPos.GetX() < bounds.GetRight()){
					bounds.SetLeft(logPos.GetX());
					isChanged = true;
				}
				break;

			case EB_RIGHT:
				if (logPos.GetX() > bounds.GetLeft()){
					bounds.SetRight(logPos.GetX());
					isChanged = true;
				}
				break;

			case EB_TOP:
				if (logPos.GetY() < bounds.GetBottom()){
					bounds.SetTop(logPos.GetY());
					isChanged = true;
				}
				break;

			case EB_BOTTOM:
				if (logPos.GetY() > bounds.GetTop()){
					bounds.SetBottom(logPos.GetY());
					isChanged = true;
				}
				break;
			}
			if (isChanged){
				calibPtr->SetBounds(bounds);
				UpdateModelChanges();
			}
		}
		return true;
	}
	else{
		return false;
	}
}


// protected methods

// reimplemented (iview::CInteractiveShapeBase)

void CInteractivePerspectiveCalibrationShape::CalcBoundingBox(i2d::CRect& result) const
{
	result.Reset();
	iview::IDisplay* displayPtr = GetDisplayPtr();
	if (displayPtr != NULL){
		result = displayPtr->GetClientRect();
	}
}


void CInteractivePerspectiveCalibrationShape::BeginLogDrag(const i2d::CVector2d& /*reference*/)
{
}


void CInteractivePerspectiveCalibrationShape::SetLogDragPosition(const i2d::CVector2d& /*position*/)
{
}


} // namespace iview



