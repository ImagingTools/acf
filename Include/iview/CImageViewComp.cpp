#include "iview/CImageViewComp.h"



namespace iview
{


// public methods

// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CImageViewComp::GetCommands() const
{
	if (IsGuiCreated()){
		iview::CConsoleGui* consolePtr = GetQtWidget();
	
		return consolePtr->GetCommands();
	}

	return NULL;
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CImageViewComp::UpdateGui(int /*updateFlags*/)
{
	iview::CConsoleGui* consolePtr = GetQtWidget();
	I_ASSERT(consolePtr != NULL);

	consolePtr->UpdateView();
}


void CImageViewComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iview::CConsoleGui* consolePtr = GetQtWidget();
	I_ASSERT(consolePtr != NULL);

	iview::CConsoleGui::ViewImpl& view = consolePtr->GetViewRef();
	view.SetViewDraggable();

	imod::IModel* modelPtr = GetModelPtr();
	I_ASSERT(modelPtr != NULL);
	if (modelPtr->IsAttached(this)){
		view.ConnectBackgroundShape(this);

		view.ConnectCalibrationShape(&m_calibrationShape);
		view.SetCalibrationPtr(&m_calibration);
	}
}


} // namespace iview


