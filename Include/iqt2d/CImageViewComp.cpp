#include "iqt2d/CImageViewComp.h"


// ACF includes
#include "iimg/IRasterImage.h"


namespace iqt2d
{


// public methods

// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CImageViewComp::GetCalibration() const
{
	const i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<const i2d::ICalibrationProvider*>(GetModelPtr());
	if (calibrationProviderPtr != NULL){
		return calibrationProviderPtr->GetCalibration();
	}

	return NULL;
}


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

	if (*m_useBitmapCalibrationAttrPtr){
		const i2d::ICalibration2d* transformationPtr = NULL;
		const i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<const i2d::ICalibrationProvider*>(GetModelPtr());
		if (calibrationProviderPtr != NULL){
			transformationPtr = calibrationProviderPtr->GetCalibration();
		}

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


