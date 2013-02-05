#include "iqt2d/CRectangleParamsGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqt2d
{


// public methods

// reimplemented (imod::IModelEditor)

void CRectangleParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CRectangle* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeNotifier notifier(NULL);

	if (objectPtr->GetLeft() != LeftSpin->value()){
		notifier.SetPtr(objectPtr);
		objectPtr->SetLeft(LeftSpin->value());
	}

	if (objectPtr->GetRight() != RightSpin->value()){
		notifier.SetPtr(objectPtr);
		objectPtr->SetRight(RightSpin->value());
	}

	if (objectPtr->GetTop() != TopSpin->value()){
		notifier.SetPtr(objectPtr);
		objectPtr->SetTop(TopSpin->value());
	}

	if (objectPtr->GetBottom() != BottomSpin->value()){
		notifier.SetPtr(objectPtr);
		objectPtr->SetBottom(BottomSpin->value());
	}
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


void CRectangleParamsGuiComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	i2d::CRectangle* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		LeftSpin->setValue(objectPtr->GetLeft());
		RightSpin->setValue(objectPtr->GetRight());
		BottomSpin->setValue(objectPtr->GetBottom());
		TopSpin->setValue(objectPtr->GetTop());

		UpdateAllViews();
	}
}


// protected slots

void CRectangleParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


} // namespace iqt2d


