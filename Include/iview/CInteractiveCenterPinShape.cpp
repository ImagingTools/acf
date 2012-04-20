#include "iview/CInteractiveCenterPinShape.h"


// ACF includes
#include "imod/IModel.h"

#include "istd/TChangeNotifier.h"



#include "i2d/CPosition2d.h"
#include "iview/CScreenTransform.h"

#include "iview/iview.h"


namespace iview
{


CInteractiveCenterPinShape::CInteractiveCenterPinShape()
{
}


// reimplemented (iview::IVisualizable)

void CInteractiveCenterPinShape::Draw(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetModelPtr());
	if (pinPtr != NULL){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
        const IColorShema& colorShema = GetColorShema();

		istd::CIndex2d sp = transform.GetScreenPosition(pinPtr->GetPosition());

		if (IsSelected()){
			if (IsEditablePosition()){
				colorShema.DrawTicker(drawContext, sp, IColorShema::TT_MOVE_CENTER);
			}
			else{
				colorShema.DrawTicker(drawContext, sp, IColorShema::TT_SELECTED_INACTIVE);
			}
		}
		else{
			colorShema.DrawTicker(drawContext, sp, IColorShema::TT_INACTIVE);
		}
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractiveCenterPinShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
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

			const i2d::CVector2d& cp = pinPtr->GetPosition();
			istd::CIndex2d sp = transform.GetScreenPosition(cp);
			const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_MOVE_CENTER);
			if (tickerBox.IsInside(position - sp)){
				m_referencePosition = cp - transform.GetClientPosition(position);
				BeginModelChanges();
				return true;
			}
		}
	}

	EndModelChanges();
	return false;
}


bool CInteractiveCenterPinShape::OnMouseMove(istd::CIndex2d position)
{
	if (!IsEditablePosition()){
		return false;
	}

	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		i2d::CPosition2d& pin = *dynamic_cast<i2d::CPosition2d*>(modelPtr);
		I_ASSERT(&pin != NULL);

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		
		pin.SetPosition(m_referencePosition + transform.GetClientPosition(position));

		UpdateModelChanges();

		return true;
	}

	return false;
}


// protected methods

// reimplemented (iview::CInteractiveShapeBase)

i2d::CRect CInteractiveCenterPinShape::CalcBoundingBox() const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetModelPtr());
	if (pinPtr != NULL){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const IColorShema& colorShema = GetColorShema();

		istd::CIndex2d sp = transform.GetScreenPosition(pinPtr->GetPosition());

		IColorShema::TickerType tickerType;
		if (IsSelected()){
			if (IsEditablePosition()){
				tickerType = IColorShema::TT_MOVE_CENTER;
			}
			else{
				tickerType = IColorShema::TT_SELECTED_INACTIVE;
			}
		}
		else{
			tickerType = IColorShema::TT_INACTIVE;
		}

		const i2d::CRect& tickerBox = colorShema.GetTickerBox(tickerType);

		return tickerBox.GetTranslated(sp);
	}

	return i2d::CRect();
}


} // namespace iview



