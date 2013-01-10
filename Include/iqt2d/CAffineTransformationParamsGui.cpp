#include "iqt2d/CAffineTransformationParamsGui.h"

namespace iqt2d
{

	
// public methods

// reimplemented (imod::IModelEditor)

void CAffineTransformationParamsGui::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CAffineTransformation2d* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	i2d::CVector2d translation(TranslationSpinX->value(), TranslationSpinY->value());

	istd::CChangeNotifier notifier(NULL);

	if(objectPtr->GetTransformation().GetTranslation() != translation){
		notifier.SetPtr(objectPtr);
		objectPtr->Reset(translation);		
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CAffineTransformationParamsGui::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	connect(TranslationSpinX, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	connect(TranslationSpinY, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));	
}


void CAffineTransformationParamsGui::OnGuiModelDetached()
{
	disconnect(TranslationSpinX, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	disconnect(TranslationSpinY, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));	

	BaseClass::OnGuiModelDetached();
}


void CAffineTransformationParamsGui::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	i2d::CAffineTransformation2d* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		i2d::CVector2d translation = objectPtr->GetTransformation().GetTranslation();

		TranslationSpinX->setValue(translation.GetX());
		TranslationSpinY->setValue(translation.GetY());		

		UpdateAllViews();
	}
}


// protected slots

void CAffineTransformationParamsGui::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}

}