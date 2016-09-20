#include <iview/CPinShape.h>


// ACF includes
#include <imod/IModel.h>

#include <i2d/CPosition2d.h>

#include <iview/IColorSchema.h>
#include <iview/CScreenTransform.h>


namespace iview
{


// constructors

CPinShape::CPinShape()
	:m_referencePosition(0.0, 0.0)
{
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CPinShape::IsTouched(istd::CIndex2d position) const
{
	i2d::CRect boundingBox = CPinShape::CalcBoundingBox();
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

bool CPinShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CPosition2d*>(modelPtr) != NULL);

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// reimplemented (iview::IVisualizable)

void CPinShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetObservedModel());
	if (pinPtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		istd::CIndex2d sp = GetScreenPosition(pinPtr->GetPosition()).ToIndex2d();

		if (IsSelected()){
			if (IsEditablePosition()){
				colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_MOVE);
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

bool CPinShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	BaseClass::OnMouseButton(position, buttonType, downFlag);

	if (!IsEditablePosition()){
		EndTickerDrag();
		return false;
	}

	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetObservedModel());
	if (IsDisplayConnected() && (pinPtr != NULL)){
		if (downFlag){
			const IColorSchema& colorSchema = GetColorSchema();
			const i2d::CVector2d& center = pinPtr->GetPosition();

			i2d::CVector2d screenCenter = GetScreenPosition(center);
			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_MOVE);
			if (tickerBox.IsInside(position - screenCenter.ToIndex2d())){
				m_referencePosition = center - GetLogPosition(position);

				BeginTickerDrag();

				return true;
			}
		}
	}

	EndTickerDrag();

	return false;
}


bool CPinShape::OnMouseMove(istd::CIndex2d position)
{
	if (!IsEditablePosition()){
		return false;
	}

	imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		i2d::CPosition2d& pin = *dynamic_cast<i2d::CPosition2d*>(modelPtr);
		Q_ASSERT(&pin != NULL);

		pin.SetPosition(m_referencePosition + GetLogPosition(position));

		UpdateModelChanges();

		return true;
	}

	return false;
}



// protected methods

// reimplemented (iview::CShapeBase)

i2d::CRect CPinShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetObservedModel());
	if (pinPtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		istd::CIndex2d sp = GetScreenPosition(pinPtr->GetPosition()).ToIndex2d();

		IColorSchema::TickerType tickerType;
		if (IsSelected()){
			if (IsEditablePosition()){
				tickerType = IColorSchema::TT_MOVE;
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


// reimplemented (iview::CInteractiveShapeBase)

void CPinShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	const imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		const i2d::CPosition2d& pin = *dynamic_cast<const i2d::CPosition2d*>(modelPtr);
		Q_ASSERT(&pin != NULL);

		m_referencePosition = pin.GetPosition() - reference;
	}
}


void CPinShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	if (IsEditablePosition()){
		imod::IModel* modelPtr = GetObservedModel();
		if (modelPtr != NULL){
			i2d::CPosition2d& pin = *dynamic_cast<i2d::CPosition2d*>(modelPtr);
			Q_ASSERT(&pin != NULL);

			pin.SetPosition(m_referencePosition + position);

			UpdateModelChanges();
		}
	}
}


} // namespace iview



