#include "iqtgui/CStatusBarWidgetComp.h"


// Qt includes
#include <QMainWindow>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IMainWindowComponent)

bool CStatusBarWidgetComp::AddToMainWindow(QMainWindow& mainWindow)
{
	if (m_statusBarWidgetCompPtr.IsValid()){
		QStatusBar* statusBar = mainWindow.statusBar();
		I_ASSERT(statusBar != NULL);

		if (!m_statusBarWidgetCompPtr->IsGuiCreated()){
			if (m_statusBarWidgetCompPtr->CreateGui(statusBar)){
				statusBar->addPermanentWidget(m_statusBarWidgetCompPtr->GetWidget());
			}

			return true;
		}
	}

	return false;
}


bool CStatusBarWidgetComp::RemoveFromMainWindow(QMainWindow& /*mainWindow*/)
{
	if (m_statusBarWidgetCompPtr.IsValid()){
		if (m_statusBarWidgetCompPtr->IsGuiCreated()){
			return m_statusBarWidgetCompPtr->DestroyGui();
		}
	}

	return true;
}


} // namespace iqtgui


