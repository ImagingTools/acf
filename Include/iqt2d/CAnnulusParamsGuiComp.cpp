#include <iqt2d/CAnnulusParamsGuiComp.h>


// ACF includes
#include <istd/CChangeGroup.h>


namespace iqt2d
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CAnnulusParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CAnnulus* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeGroup changeGroup(objectPtr);
	Q_UNUSED(changeGroup);

	i2d::CVector2d postion(XSpin->value(), YSpin->value());
	objectPtr->SetPosition(postion);

	double innerRadius = InnerRadiusSpin->value();
	objectPtr->SetInnerRadius(innerRadius);

	double outerRadius = OuterRadiusSpin->value();
	objectPtr->SetOuterRadius(outerRadius);
}


void CAnnulusParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	i2d::CAnnulus* objectPtr = GetObservedObject();
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

	if (IsPositionFixed()){
		XSpin->setEnabled(false);
		YSpin->setEnabled(false);
	}
	else{
		QObject::connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
		QObject::connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	}

	QObject::connect(InnerRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(OuterRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
}


void CAnnulusParamsGuiComp::OnGuiDestroyed()
{
	if (!IsPositionFixed()){
		QObject::disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
		QObject::disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	}

	QObject::disconnect(InnerRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(OuterRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiDestroyed();
}


void CAnnulusParamsGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	QString unitName = GetUnitName();

	if (!unitName.isEmpty()){
		PositionUnitLabel->setText(unitName);
		RadiusUnitLabel->setText(unitName);
		PositionUnitLabel->setVisible(true);
		RadiusUnitLabel->setVisible(true);
	}
	else{
		PositionUnitLabel->setVisible(false);
		RadiusUnitLabel->setVisible(false);
	}
}


// protected slots

void CAnnulusParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


void CAnnulusParamsGuiComp::OnActionTriggered(QAction* /*actionPtr*/)
{
	// dummy implementation, to be reimplemented
}


} // namespace iqt2d


