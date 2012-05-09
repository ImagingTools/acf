#include "iview/CInteractivePinShape.h"


// ACF includes
#include "imod/IModel.h"

#include "i2d/CPosition2d.h"

#include "iview/IColorShema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


// constructors

CInteractivePinShape::CInteractivePinShape()
{
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CInteractivePinShape::IsTouched(istd::CIndex2d position) const
{
	i2d::CRect boundingBox = CInteractivePinShape::CalcBoundingBox();
	if (boundingBox.IsInside(position)){
		if (IsEditablePosition()){
			return TS_TICKER;
		}
		else{
			return TS_INACTIVE;
		}
	}

	return TS_NONE;
}


// reimplemented (imod::IObserver)

bool CInteractivePinShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CPosition2d*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::IVisualizable)

void CInteractivePinShape::Draw(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetModelPtr());
	if (pinPtr != NULL){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
        const IColorShema& colorShema = GetColorShema();

		istd::CIndex2d sp = transform.GetScreenPosition(pinPtr->GetPosition());

		if (IsSelected()){
			if (IsEditablePosition()){
				colorShema.DrawTicker(drawContext, sp, IColorShema::TT_MOVE);
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

bool CInteractivePinShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
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
			const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_MOVE);
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


bool CInteractivePinShape::OnMouseMove(istd::CIndex2d position)
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

// reimplemented (iview::CShapeBase)

i2d::CRect CInteractivePinShape::CalcBoundingBox() const
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

		return tickerBox.GetTranslated(sp);
	}

	return i2d::CRect::GetEmpty();
}


// reimplemented (iview::CInteractiveShapeBase)

void CInteractivePinShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CPosition2d& pin = *dynamic_cast<const i2d::CPosition2d*>(modelPtr);
		I_ASSERT(&pin != NULL);

		m_referencePosition = pin.GetPosition() - reference;
	}
}


void CInteractivePinShape::SetLogDragPosition(const i2d::CVector2d& position)
{
    if (IsEditablePosition()){
		imod::IModel* modelPtr = GetModelPtr();
		if (modelPtr != NULL){
			BeginModelChanges();

			i2d::CPosition2d& pin = *dynamic_cast<i2d::CPosition2d*>(modelPtr);
			I_ASSERT(&pin != NULL);

			pin.SetPosition(m_referencePosition + position);

			EndModelChanges();
		}
	}
}


} // namespace iview



