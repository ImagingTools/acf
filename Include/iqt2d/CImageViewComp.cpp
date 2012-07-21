#include "iqt2d/CImageViewComp.h"


// ACF includes
#include "iimg/IRasterImage.h"


namespace iqt2d
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CImageViewComp::UpdateGui(int /*updateFlags*/)
{
	iview::CConsoleGui* consolePtr = GetQtWidget();
	I_ASSERT(consolePtr != NULL);

	const iimg::IRasterImage* imagePtr = dynamic_cast<const iimg::IRasterImage*>(GetModelPtr());
	if (imagePtr != NULL){
		i2d::CRectangle areaRect(imagePtr->GetImageSize());

		consolePtr->GetViewRef().SetFitArea(areaRect);
	}

	const i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<const i2d::ICalibrationProvider*>(GetModelPtr());
	const i2d::ITransformation2d* transformationPtr = NULL;
	if (calibrationProviderPtr != NULL){
		transformationPtr = calibrationProviderPtr->GetCalibration();
	}

	if (!m_calibrationProviderCompPtr.IsValid()){
		SetConsoleCalibration(transformationPtr);
	}

	consolePtr->UpdateView();
}


// reimplemented (iqtui::CComponentBase)

void CImageViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	iview::CConsoleGui* consolePtr = GetQtWidget();
	I_ASSERT(consolePtr != NULL);

	iview::CViewport& view = consolePtr->GetViewRef();
	view.SetViewDraggable();

	AssignToLayer(iview::IViewLayer::LT_BACKGROUND);

	view.ConnectShape(this);
}


} // namespace iview


