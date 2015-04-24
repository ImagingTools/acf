#include "iqt2d/CAnnulusSegmentParamsGuiComp.h"


// ACF includes
#include "istd/CChangeGroup.h"


namespace iqt2d
{


// public methods

// reimplemented (imod::IModelEditor)

void CAnnulusSegmentParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CAnnulusSegment* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeGroup changeGroup(objectPtr);
	Q_UNUSED(changeGroup);

	i2d::CVector2d postion(XSpin->value(), YSpin->value());
	objectPtr->SetPosition(postion);

	double innerRadius = InnerRadiusSpin->value();
	objectPtr->SetInnerRadius(innerRadius);

	double outerRadius = OuterRadiusSpin->value();
	objectPtr->SetOuterRadius(outerRadius);

	double beginAngle = imath::GetRadianFromDegree(BeginAngleSB->value());
	double prec = qPow(0.1, BeginAngleSB->decimals() + 1);
	if (beginAngle - objectPtr->GetBeginAngle() > prec){
		objectPtr->SetBeginAngle(beginAngle);
	}

	double endAngle = imath::GetRadianFromDegree(EndAngleSB->value());
	prec = qPow(0.1, EndAngleSB->decimals() + 1);
	if (endAngle - objectPtr->GetEndAngle() > prec){
		objectPtr->SetEndAngle(endAngle);
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CAnnulusSegmentParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	i2d::CAnnulusSegment* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const i2d::CVector2d& center = objectPtr->GetCenter();
		XSpin->setValue(center.GetX());
		YSpin->setValue(center.GetY());

		InnerRadiusSpin->setValue(objectPtr->GetInnerRadius());
		OuterRadiusSpin->setValue(objectPtr->GetOuterRadius());

		BeginAngleSB->setValue(imath::GetDegreeFromRadian(objectPtr->GetBeginAngle()));
		EndAngleSB->setValue(imath::GetDegreeFromRadian(objectPtr->GetEndAngle()));

		UpdateAllViews();
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CAnnulusSegmentParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QObject::connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(InnerRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(OuterRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(BeginAngleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(EndAngleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
}


void CAnnulusSegmentParamsGuiComp::OnGuiDestroyed()
{
	QObject::disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(InnerRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(OuterRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(BeginAngleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(EndAngleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiDestroyed();
}


void CAnnulusSegmentParamsGuiComp::OnGuiRetranslate()
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

void CAnnulusSegmentParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


} // namespace iqt2d


