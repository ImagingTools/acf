#include "iqt2d/CImageViewComp.h"


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

		consolePtr->SetMmPositionVisible(calibrationPtr != NULL);
	}
	else{
		consolePtr->SetMmPositionVisible(false);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CImageViewComp::UpdateGui(int /*updateFlags*/)
{
	iview::CConsoleGui* consolePtr = GetQtWidget();
	I_ASSERT(consolePtr != NULL);

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

	view.ConnectBackgroundShape(this);
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


