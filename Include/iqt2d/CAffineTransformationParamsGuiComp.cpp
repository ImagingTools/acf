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


void CAffineTransformationParamsGuiComp::CreateShapes(int sceneId, Shapes& result)
{
	BaseClass::CreateShapes(sceneId, result);
	for (int i = 0; i < result.GetCount(); i++){
		iview::CAffineTransformation2dShape* shape = dynamic_cast<iview::CAffineTransformation2dShape*>(result.GetAt(i));
		if (shape != NULL){
			shape->SetEditablePosition();
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
	Q_ASSERT(IsGuiCreated());

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

