#include "iqt2d/CCircleParamsGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqt2d
{


// public methods

// reimplemented (imod::IModelEditor)

void CCircleParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CCircle* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeNotifier notifier(NULL);

	i2d::CVector2d postion(XSpin->value(), YSpin->value());
	if (objectPtr->GetCenter() != postion){
		notifier.SetPtr(objectPtr);
		objectPtr->SetPosition(postion);
	}

	double radius = RadiusSpin->value();
	if (objectPtr->GetRadius() != radius){
		notifier.SetPtr(objectPtr);
		objectPtr->SetRadius(radius);
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CCircleParamsGuiComp::OnGuiModelAttached()
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


void CCircleParamsGuiComp::OnGuiModelDetached()
{
	QObject::disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(RadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiModelDetached();
}


void CCircleParamsGuiComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	i2d::CCircle* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const i2d::CVector2d& center = objectPtr->GetCenter();

		XSpin->setValue(center.GetX());
		YSpin->setValue(center.GetY());

		RadiusSpin->setValue(objectPtr->GetRadius());

		UpdateAllViews();
	}
}


// protected slots

void CCircleParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


} // namespace iqt2d


