#include <iqt2d/CCircleParamsGuiComp.h>


namespace iqt2d
{


// protected methods

void CCircleParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	MoveToCenterButton->setVisible(m_centerScreenPointProviderCompPtr.IsValid());
}

// protected slots

void CCircleParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


void CCircleParamsGuiComp::OnActionTriggered(QAction* actionPtr)
{
	BaseClass::OnActionTriggered(actionPtr);
}

void CCircleParamsGuiComp::on_MoveToCenterButton_clicked(bool)
{
	i2d::CCircle* objectPtr = GetObservedObject();
	if (objectPtr != NULL && m_centerScreenPointProviderCompPtr.IsValid()) {
		i2d::CCircle screenCircle;
		screenCircle.CopyFrom(*objectPtr, istd::IChangeable::CompatibilityMode::CM_CONVERT);
		const i2d::CPosition2d* centerPtr = dynamic_cast<const i2d::CPosition2d*>(m_centerScreenPointProviderCompPtr->GetObject2d());
		if (centerPtr != NULL) {
			screenCircle.MoveCenterTo(centerPtr->GetCenter());
		}

		const i2d::ICalibration2d* calibrationPtr = objectPtr->GetCalibration();
		if (calibrationPtr != NULL) {
			i2d::CCircle centerCircle;
			screenCircle.GetInvTransformed(*calibrationPtr, centerCircle);
			objectPtr->SetPosition(centerCircle.GetPosition());
			objectPtr->SetRadius(centerCircle.GetRadius());
		}
		else {
			objectPtr->SetPosition(screenCircle.GetPosition());
			objectPtr->SetRadius(screenCircle.GetRadius());
		}
	}
}

} // namespace iqt2d


