#include "iview/CPinCalibrationShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "imod/TModelWrap.h"

#include "i2d/CPosition2d.h"

#include "iview/IColorShema.h"
#include "iview/CPinCalibrationShape.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CPinCalibrationShape::CPinCalibrationShape()
{
}


// reimplemented (iview::ITouchable)

iview::ITouchable::TouchState CPinCalibrationShape::IsTouched(istd::CIndex2d position) const
{
	i2d::CRect boundingBox = CPinCalibrationShape::CalcBoundingBox();
	if (boundingBox.IsInside(position) != 0){
		return TS_TICKER;
	}
	else{
		return TS_NONE;
	}
}


// reimplemented (iview::IShape)

void CPinCalibrationShape::Draw(QPainter& drawContext) const
{
	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetModelPtr());
	if (IsDisplayConnected() && (pinPtr != NULL)){
	    const IColorShema& colorShema = GetColorShema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::ITransformation2d& calib = GetIsomorphCalib();

		i2d::CVector2d viewPos;
		calib.GetInvPositionAt(pinPtr->GetPosition(), viewPos);
		istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

		if (IsSelected()){
			if (IsEditablePosition()){
				colorShema.DrawTicker(drawContext, screenPos, IColorShema::TT_MOVE);
			}
			else{
				colorShema.DrawTicker(drawContext, screenPos, IColorShema::TT_SELECTED_INACTIVE);
			}
		}
		else{
			colorShema.DrawTicker(drawContext, screenPos, IColorShema::TT_INACTIVE);
		}
	}
}


// reimplemented (imod::IObserver)

bool CPinCalibrationShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CPosition2d*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::IMouseActionObserver)

bool CPinCalibrationShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
{
	I_ASSERT(IsDisplayConnected());

	if (!IsEditablePosition()){
		EndModelChanges();
		return false;
	}

	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetModelPtr());
	if (IsDisplayConnected() && (pinPtr != NULL)){
		if (downFlag){
			const IColorShema& colorShema = GetColorShema();
			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			const i2d::ITransformation2d& calib = GetIsomorphCalib();

			const i2d::CVector2d& logPos = pinPtr->GetPosition();
			i2d::CVector2d viewPos;
			calib.GetInvPositionAt(logPos, viewPos);
			istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
			const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_MOVE);
			if (tickerBox.IsInside(position - screenPos)){
				i2d::CVector2d logMouse;
				calib.GetPositionAt(transform.GetClientPosition(position), logMouse);
				m_referencePosition = logPos - logMouse;
				BeginModelChanges();

				return true;
			}
		}
	}

	EndModelChanges();

	return false;
}


bool CPinCalibrationShape::OnMouseMove(istd::CIndex2d position)
{
	if (!IsEditablePosition()){
		return false;
	}

	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		i2d::CPosition2d& pin = *dynamic_cast<i2d::CPosition2d*>(modelPtr);
		I_ASSERT(&pin != NULL);

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::ITransformation2d& calib = GetIsomorphCalib();

		i2d::CVector2d logMouse;
		calib.GetPositionAt(transform.GetClientPosition(position), logMouse);

		pin.SetPosition(m_referencePosition + logMouse);

		UpdateModelChanges();

		return true;
	}
	else{
		return false;
	}
}


// protected methods

// reimplemented (iview::CInteractiveShapeBase)

void CPinCalibrationShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CPosition2d& pin = *dynamic_cast<const i2d::CPosition2d*>(modelPtr);
		I_ASSERT(&pin != NULL);

		m_referencePosition = pin.GetPosition() - reference;
	}
}


void CPinCalibrationShape::SetLogDragPosition(const i2d::CVector2d& position)
{
    if (IsEditablePosition()){
		imod::IModel* modelPtr = GetModelPtr();
		if (modelPtr != NULL){
			i2d::CPosition2d& pin = *dynamic_cast<i2d::CPosition2d*>(modelPtr);
			I_ASSERT(&pin != NULL);

			BeginModelChanges();

			pin.SetPosition(m_referencePosition + position);

			EndModelChanges();
		}
	}
}


// reimplemented (iview::CShapeBase)

i2d::CRect CPinCalibrationShape::CalcBoundingBox() const
{
	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetModelPtr());
	if (IsDisplayConnected() && (pinPtr != NULL)){
		const IColorShema& colorShema = GetColorShema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::ITransformation2d& calib = GetIsomorphCalib();

		const i2d::CVector2d& logPos = pinPtr->GetPosition();
		i2d::CVector2d viewPos;
		calib.GetInvPositionAt(logPos, viewPos);
		istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

		IColorShema::TickerType tickerType;
		if (IsSelected()){
			if (IsEditablePosition()){
				tickerType = IColorShema::TT_MOVE;
			}
			else{
				tickerType = IColorShema::TT_SELECTED_INACTIVE;
			}
		}
		else{
			tickerType = IColorShema::TT_INACTIVE;
		}

		const i2d::CRect& tickerBox = colorShema.GetTickerBox(tickerType);

		return tickerBox.GetTranslated(screenPos);
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview


