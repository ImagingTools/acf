#include "iqt2d/CImageViewComp.h"


namespace iqt2d
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


// reimplemented (iqtui::CComponentBase)

void CImageViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	iview::CConsoleGui* consolePtr = GetQtWidget();
	I_ASSERT(consolePtr != NULL);

	iview::CConsoleGui::ViewImpl& view = consolePtr->GetViewRef();
	view.SetViewDraggable();

	view.ConnectBackgroundShape(this);
	view.ConnectCalibrationShape(&m_calibrationShape);
	view.SetCalibrationPtr(&m_calibration);
}


} // namespace iview


