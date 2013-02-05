#include "iview/CShapeControl.h"


namespace iview
{


CShapeControl::CShapeControl()
{
	m_shapeTransformMode = STM_COMBINE;
	m_shapeTransform.Reset();
}


CShapeControl::CShapeControl(const CShapeControl& shapeControl)
{
	m_shapeTransform = shapeControl.m_shapeTransform;
	m_shapeTransformMode = shapeControl.m_shapeTransformMode;
}


void CShapeControl::SetTransformMode(ShapeTransformMode mode)
{
	if (m_shapeTransformMode != mode){
		m_shapeTransformMode = mode;

		Invalidate(CS_CONSOLE);
	}
}


// protected methods

void CShapeControl::CalcTransforms() const
{
	const IDisplay* displayPtr = GetDisplayPtr();
	Q_ASSERT(displayPtr != NULL);

	if (m_shapeTransformMode == STM_SHAPE){
		displayPtr->GetTransform().GetApply(m_shapeTransform, m_logToScreenTransform);
		m_logToViewTransform = m_shapeTransform;
		m_areTransformsValid = true;

		return;
	}

	do{
		const ILogicalCoords* logicalPtr = dynamic_cast<const ILogicalCoords*>(displayPtr);

		if (logicalPtr != NULL){
			switch (m_shapeTransformMode){
			case STM_VIEW:
				m_logToScreenTransform = logicalPtr->GetLogToScreenTransform();
				m_logToViewTransform = logicalPtr->GetLogToViewTransform();
				break;

			case STM_COMBINE:
				logicalPtr->GetLogToScreenTransform().GetApply(m_shapeTransform, m_logToScreenTransform);
				logicalPtr->GetLogToViewTransform().GetApply(m_shapeTransform, m_logToViewTransform);
				break;

			default:
				I_CRITICAL();	// Other modes not implemented.
			}
			m_areTransformsValid = true;
			return;
		}
		displayPtr = displayPtr->GetParentDisplayPtr();
	} while (displayPtr != NULL);

	switch (m_shapeTransformMode){
	case STM_VIEW:
		m_logToScreenTransform = displayPtr->GetTransform();
		m_logToViewTransform = i2d::CAffine2d::GetIdentity();
		break;

	case STM_SHAPE:
	case STM_COMBINE:
		displayPtr->GetTransform().GetApply(m_shapeTransform, m_logToScreenTransform);
		m_logToViewTransform = m_shapeTransform;
		break;

	default:
		I_CRITICAL();	// Other modes not implemented.
	}

	m_areTransformsValid = true;
}


} // namespace iview



