#include "iqt2d/CLine2dParamsGuiComp.h"


// ACF includes
#include "istd/CChangeGroup.h"


namespace iqt2d
{


// public methods

// reimplemented (imod::IModelEditor)

void CLine2dParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CLine2d* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeGroup changeGroup(objectPtr);
	Q_UNUSED(changeGroup);

	i2d::CVector2d point1(Point1XSB->value(), Point1YSB->value());
	objectPtr->SetPoint1(point1);

	i2d::CVector2d point2(Point2XSB->value(), Point2YSB->value());
	objectPtr->SetPoint2(point2);
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CLine2dParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	i2d::CLine2d* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const i2d::CVector2d& point1 = objectPtr->GetPoint1();

		Point1XSB->setValue(point1.GetX());
		Point1YSB->setValue(point1.GetY());

		const i2d::CVector2d& point2 = objectPtr->GetPoint2();

		Point2XSB->setValue(point2.GetX());
		Point2YSB->setValue(point2.GetY());

		UpdateAllViews();
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CLine2dParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QObject::connect(Point1XSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(Point1YSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(Point2XSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(Point2YSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
}


void CLine2dParamsGuiComp::OnGuiDestroyed()
{
	QObject::disconnect(Point1XSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(Point1YSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(Point2XSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(Point2YSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiDestroyed();
}


void CLine2dParamsGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	QString unitName = GetUnitName();

	if (!unitName.isEmpty()){
		Point1UnitLabel->setVisible(true);
		Point1UnitLabel->setText(unitName);
		Point2UnitLabel->setVisible(true);
		Point2UnitLabel->setText(unitName);
	}
	else{
		Point1UnitLabel->setVisible(false);
		Point2UnitLabel->setVisible(false);
	}
}


// protected slots

void CLine2dParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


} // namespace iqt2d


