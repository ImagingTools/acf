#include "iview/CCalibrationShapeBase.h"


#include "i2d/ICalibrationProvider.h"


namespace iview
{


CCalibrationShapeBase::CCalibrationShapeBase()
{
	SetTransformMode(STM_SHAPE);
}


// reimplemented (iview::IInteractiveShape)

void CCalibrationShapeBase::BeginDrag(const i2d::CVector2d& reference)
{
	Q_ASSERT(IsDisplayConnected());
	Q_ASSERT(m_calibPtr != NULL);

	const i2d::ITransformation2d& isomorphCalib = GetIsomorphCalib();
	const i2d::CAffine2d& transform = GetLogToViewTransform();

	i2d::CVector2d logPos;
	isomorphCalib.GetPositionAt(transform.GetInvertedApply(reference), logPos);
	BeginLogDrag(logPos);
}


void CCalibrationShapeBase::SetDragPosition(const i2d::CVector2d& position)
{
	Q_ASSERT(IsDisplayConnected());
	Q_ASSERT(m_calibPtr != NULL);

	const i2d::ITransformation2d& isomorphCalib = GetIsomorphCalib();
	const i2d::CAffine2d& transform = GetLogToViewTransform();

	i2d::CVector2d logPos;
	isomorphCalib.GetPositionAt(transform.GetInvertedApply(position), logPos);

	SetLogDragPosition(logPos);
}


// reimplement (iview::IVisualizable)

bool CCalibrationShapeBase::IsDisplayAccepted(const iview::IDisplay& display) const
{
	return BaseClass::IsDisplayAccepted(display) && (FindCalibration(display) != NULL);
}


void CCalibrationShapeBase::OnConnectDisplay(iview::IDisplay* displayPtr)
{
	BaseClass::OnConnectDisplay(displayPtr);

	m_calibPtr = FindCalibration(*displayPtr);

	Q_ASSERT(m_calibPtr != NULL);
}


void CCalibrationShapeBase::OnDisconnectDisplay(iview::IDisplay* displayPtr)
{
	BaseClass::OnDisconnectDisplay(displayPtr);

	m_calibPtr = NULL;
}


// protected static methods

const i2d::ICalibration2d* CCalibrationShapeBase::FindCalibration(const iview::IDisplay& display)
{
	const iview::IDisplay* displayPtr = &display;
	while (displayPtr != NULL){
		const i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<const i2d::ICalibrationProvider*>(displayPtr);
		if (calibrationProviderPtr != NULL){
			const i2d::ICalibration2d* calibrationPtr = calibrationProviderPtr->GetCalibration();
			if (calibrationPtr != NULL){
				return calibrationPtr;
			}
		}

		displayPtr = displayPtr->GetParentDisplayPtr();
	}

	return NULL;
}


} // namespace iview


