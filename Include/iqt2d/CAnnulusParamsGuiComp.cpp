#include "iqt2d/CAnnulusParamsGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqt2d
{


// public methods

// reimplemented (imod::IModelEditor)

void CAnnulusParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CAnnulus* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeNotifier notifier(NULL);

	i2d::CVector2d postion(XSpin->value(), YSpin->value());
	if (objectPtr->GetCenter() != postion){
		notifier.SetPtr(objectPtr);
		objectPtr->SetPosition(postion);
	}

	double innerRadius = InnerRadiusSpin->value();
	if (objectPtr->GetInnerRadius() != innerRadius){
		notifier.SetPtr(objectPtr);
		objectPtr->SetInnerRadius(innerRadius);
	}

	double outerRadius = OuterRadiusSpin->value();
	if (objectPtr->GetOuterRadius() != outerRadius){
		notifier.SetPtr(objectPtr);
		objectPtr->SetOuterRadius(outerRadius);
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CAnnulusParamsGuiComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	i2d::CAnnulus* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const i2d::CVector2d& center = objectPtr->GetCenter();
		XSpin->setValue(center.GetX());
		YSpin->setValue(center.GetY());

		InnerRadiusSpin->setValue(objectPtr->GetInnerRadius());
		OuterRadiusSpin->setValue(objectPtr->GetOuterRadius());

		UpdateAllViews();
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CAnnulusParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QObject::connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(InnerRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(OuterRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	if (m_unitNameAttrPtr.IsValid()){
		PositionUnitLabel->setVisible(true);
		RadiusUnitLabel->setText(*m_unitNameAttrPtr);
	}
	else{
		PositionUnitLabel->setVisible(false);
		RadiusUnitLabel->setVisible(false);
	}
}


void CAnnulusParamsGuiComp::OnGuiDestroyed()
{
	QObject::disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(InnerRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(OuterRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiDestroyed();
}


// protected slots

void CAnnulusParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


} // namespace iqt2d


