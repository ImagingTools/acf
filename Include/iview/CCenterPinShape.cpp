#include "iview/CCenterPinShape.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"

#include "i2d/CPosition2d.h"

#include "iview/IColorSchema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CCenterPinShape::CCenterPinShape()
{
}


// reimplemented (iview::IVisualizable)

void CCenterPinShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetModelPtr());
	if (pinPtr != NULL){
		const i2d::ICalibration2d* calibrationPtr = pinPtr->GetCalibration();

		const IColorSchema& colorSchema = GetColorSchema();

		istd::CIndex2d sp = GetScreenPosition(pinPtr->GetPosition(), calibrationPtr).ToIndex2d();

		if (IsSelected()){
			if (IsEditablePosition()){
				colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_MOVE_CENTER);
			}
			else{
				colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_SELECTED_INACTIVE);
			}
		}
		else{
			colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_INACTIVE);
		}
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CCenterPinShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	if (!IsEditablePosition()){
		EndModelChanges();

		return false;
	}

	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetModelPtr());
	if (IsDisplayConnected() && (pinPtr != NULL)){
		if (downFlag){
			const i2d::ICalibration2d* calibrationPtr = pinPtr->GetCalibration();

			const IColorSchema& colorSchema = GetColorSchema();

			const i2d::CVector2d& cp = pinPtr->GetPosition();
			istd::CIndex2d sp = GetScreenPosition(cp, calibrationPtr).ToIndex2d();
			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_MOVE_CENTER);
			if (tickerBox.IsInside(position - sp)){
				m_referencePosition = cp - GetLogPosition(position, calibrationPtr);

				BeginModelChanges();

				return true;
			}
		}
	}

	EndModelChanges();

	return false;
}


bool CCenterPinShape::OnMouseMove(istd::CIndex2d position)
{
	if (!IsEditablePosition()){
		return false;
	}

	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		i2d::CPosition2d& pin = *dynamic_cast<i2d::CPosition2d*>(modelPtr);
		Q_ASSERT(&pin != NULL);

		const i2d::ICalibration2d* calibrationPtr = pin.GetCalibration();

		pin.SetPosition(m_referencePosition + GetLogPosition(position, calibrationPtr));

		UpdateModelChanges();

		return true;
	}

	return false;
}


// protected methods

// reimplemented (iview::CShapeBase)

i2d::CRect CCenterPinShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetModelPtr());
	if (pinPtr != NULL){
		const i2d::ICalibration2d* calibrationPtr = pinPtr->GetCalibration();

		const IColorSchema& colorSchema = GetColorSchema();

		istd::CIndex2d sp = GetScreenPosition(pinPtr->GetPosition(), calibrationPtr).ToIndex2d();

		IColorSchema::TickerType tickerType;
		if (IsSelected()){
			if (IsEditablePosition()){
				tickerType = IColorSchema::TT_MOVE_CENTER;
			}
			else{
				tickerType = IColorSchema::TT_SELECTED_INACTIVE;
			}
		}
		else{
			tickerType = IColorSchema::TT_INACTIVE;
		}

		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(tickerType);

		return tickerBox.GetTranslated(sp);
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview



