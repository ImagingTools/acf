#include "iqt2d/COrientedCircleEditorComp.h"

// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqt2d
{


void COrientedCircleEditorComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::COrientedCircle* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeNotifier notifier(NULL);

	i2d::CVector2d position(XSpin->value(), YSpin->value());

	if (objectPtr->GetCenter() != position){
		notifier.SetPtr(objectPtr);
		objectPtr->SetPosition(position);
	}

	double radius = RadiusSpin->value();
	if (objectPtr->GetRadius() != radius){
		notifier.SetPtr(objectPtr);
		objectPtr->SetRadius(radius);
	}

	objectPtr->SetOrientedOutside(AxisOrientationCheckBox->checkState() == Qt::Checked);
}


void COrientedCircleEditorComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(RadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	if (m_unitNameAttrPtr.IsValid()){
		PositionUnitLabel->setVisible(true);
		RadiusUnitLabel->setText(*m_unitNameAttrPtr);
	}
	else{
		PositionUnitLabel->setVisible(false);
		RadiusUnitLabel->setVisible(false);
	}
}


void COrientedCircleEditorComp::OnGuiModelDetached()
{
	QObject::disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(RadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiModelDetached();
}


void COrientedCircleEditorComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	i2d::COrientedCircle* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const i2d::CVector2d& center = objectPtr->GetCenter();

		XSpin->setValue(center.GetX());
		YSpin->setValue(center.GetY());

		RadiusSpin->setValue(objectPtr->GetRadius());

		AxisOrientationCheckBox->setCheckState(objectPtr->IsOrientedOutside() ? Qt::Checked : Qt::Unchecked);

		UpdateAllViews();
	}
}


void COrientedCircleEditorComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


void COrientedCircleEditorComp::on_AxisOrientationCheckBox_stateChanged(int /*state*/)
{
	DoUpdateModel();
}


} // namespace iqt2d


