#include "iqt2d/CAffineTransformationParamsGuiComp.h"


namespace iqt2d
{

// public methods


void CAffineTransformationParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CAffineTransformation2d* objectPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetObjectPtr());
	if (objectPtr != NULL){
		i2d::CVector2d translation(TranslationSpinX->value(), TranslationSpinY->value());
		if (objectPtr->GetTransformation().GetTranslation() != translation){
			istd::CChangeNotifier notifier(objectPtr);
			objectPtr->Reset(translation);
		}
	}
}


// protected methods


void CAffineTransformationParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	connect(TranslationSpinX, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	connect(TranslationSpinY, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
}


void CAffineTransformationParamsGuiComp::OnGuiModelDetached()
{
	disconnect(TranslationSpinX, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	disconnect(TranslationSpinY, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiModelDetached();
}


void CAffineTransformationParamsGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	i2d::CAffineTransformation2d* objectPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetObjectPtr());
	if (objectPtr != NULL){
		i2d::CVector2d translation = objectPtr->GetTransformation().GetTranslation();

		TranslationSpinX->setValue(translation.GetX());
		TranslationSpinY->setValue(translation.GetY());

		UpdateAllViews();
	}
}


// protected slots


void CAffineTransformationParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


void CAffineTransformationParamsGuiComp::on_ResetButton_clicked(bool)
{
	i2d::CAffineTransformation2d* objectPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetObjectPtr());
	if (objectPtr != NULL){
		objectPtr->Reset();
	}
}


} // namespace iqt2d

