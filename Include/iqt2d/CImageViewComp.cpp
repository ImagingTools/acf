#include "iqt2d/CImageViewComp.h"


// ACF includes
#include "iimg/IRasterImage.h"


namespace iqt2d
{


// protected methods

// reimplemented (imod::CMultiModelDispatcherBase)

void CImageViewComp::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (!IsGuiCreated()){
		return;
	}

	iview::CConsoleGui* consolePtr = GetQtWidget();
	I_ASSERT(consolePtr != NULL);

	iview::CViewport& view = consolePtr->GetViewRef();

	if (m_calibrationProviderCompPtr.IsValid()){
		const i2d::ITransformation2d* calibrationPtr = m_calibrationProviderCompPtr->GetCalibration();

		view.SetCalibrationPtr(calibrationPtr);
	}
}


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
	view.ConnectCalibrationShape(&m_calibrationShape);
}


// reimplemented (icomp::CComponentBase)

void CImageViewComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_calibrationProviderModelCompPtr.IsValid()){
		RegisterModel(m_calibrationProviderModelCompPtr.GetPtr());
	}
}


void CImageViewComp::OnComponentDestroyed()
{
	UnregisterAllModels();

	BaseClass::OnComponentDestroyed();
}


} // namespace iview


