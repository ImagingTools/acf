#include "iqt2d/CRectangleParamsGuiComp.h"


// ACF includes
#include "istd/CChangeGroup.h"


namespace iqt2d
{


// public methods

// reimplemented (imod::IModelEditor)

void CRectangleParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CRectangle* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeGroup changeGroup(objectPtr);
	Q_UNUSED(changeGroup);

	objectPtr->SetLeft(LeftSpin->value());
	objectPtr->SetRight(RightSpin->value());
	objectPtr->SetTop(TopSpin->value());
	objectPtr->SetBottom(BottomSpin->value());
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CRectangleParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(LeftSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(RightSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(BottomSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(TopSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
}


void CRectangleParamsGuiComp::OnGuiModelDetached()
{
	QObject::disconnect(LeftSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(RightSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(BottomSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(TopSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiModelDetached();
}


void CRectangleParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	i2d::CRectangle* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		LeftSpin->setValue(objectPtr->GetLeft());
		RightSpin->setValue(objectPtr->GetRight());
		BottomSpin->setValue(objectPtr->GetBottom());
		TopSpin->setValue(objectPtr->GetTop());

		UpdateAllViews();
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CRectangleParamsGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	QString unitName = GetUnitName();

	if (!unitName.isEmpty()){
		PositionUnitLabel->setText(unitName);
		PositionUnit2Label->setText(unitName);
		PositionUnitLabel->setVisible(true);
		PositionUnit2Label->setVisible(true);
	}
	else{
		PositionUnitLabel->setVisible(false);
		PositionUnit2Label->setVisible(false);
	}
}


// protected slots

void CRectangleParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


} // namespace iqt2d


