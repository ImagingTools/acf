#include "iqt2d/CCircleParamsGuiComp.h"


// ACF includes
#include "istd/CChangeGroup.h"


namespace iqt2d
{


// public methods

// reimplemented (imod::IModelEditor)

void CCircleParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CCircle* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeGroup changeGroup(objectPtr);
	Q_UNUSED(changeGroup);

	i2d::CVector2d postion(XSpin->value(), YSpin->value());
	objectPtr->SetPosition(postion);

	double radius = RadiusSpin->value();
	objectPtr->SetRadius(radius);
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CCircleParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	i2d::CCircle* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		const i2d::CVector2d& center = objectPtr->GetCenter();

		XSpin->setValue(center.GetX());
		YSpin->setValue(center.GetY());

		RadiusSpin->setValue(objectPtr->GetRadius());

		UpdateAllViews();
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CCircleParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (IsPositionFixed()){
		XSpin->setEnabled(false);
		YSpin->setEnabled(false);
	}
	else{
		QObject::connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
		QObject::connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	}

	QObject::connect(RadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
}


void CCircleParamsGuiComp::OnGuiDestroyed()
{
	if (!IsPositionFixed()){
		QObject::disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
		QObject::disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	}

	QObject::disconnect(RadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiDestroyed();
}


void CCircleParamsGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	QString unitName = GetUnitName();

	if (!unitName.isEmpty()){
		PositionUnitLabel->setVisible(true);
		RadiusUnitLabel->setText(unitName);
	}
	else{
		PositionUnitLabel->setVisible(false);
		RadiusUnitLabel->setVisible(false);
	}
}


// protected slots

void CCircleParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


} // namespace iqt2d


