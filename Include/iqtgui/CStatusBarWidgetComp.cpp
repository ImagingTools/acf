#include "iqtgui/CStatusBarWidgetComp.h"


// Qt includes
#include <QtGui/QMainWindow>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IMainWindowComponent)

bool CStatusBarWidgetComp::AddToMainWindow(QMainWindow& mainWindow)
{
	if (m_statusBarWidgetsCompPtr.IsValid()){
		QStatusBar* statusBar = mainWindow.statusBar();
		Q_ASSERT(statusBar != NULL);

		int widgetsCount = m_statusBarWidgetsCompPtr.GetCount();

		for (int widgetIndex = 0; widgetIndex < widgetsCount; widgetIndex++){
			iqtgui::IGuiObject* guiPtr = m_statusBarWidgetsCompPtr[widgetIndex];
			if (guiPtr != NULL){
				if (!guiPtr->IsGuiCreated()){
					if (guiPtr->CreateGui(statusBar)){
						statusBar->addPermanentWidget(guiPtr->GetWidget());
					}
					else{
						return false;
					}
				}
				else{
					I_CRITICAL();

					return false;
				}
			}
			else{
				I_CRITICAL();

				return false;
			}
		}
	}

	return false;
}


bool CStatusBarWidgetComp::RemoveFromMainWindow(QMainWindow& /*mainWindow*/)
{
	int widgetsCount = m_statusBarWidgetsCompPtr.GetCount();

	bool retVal = true;

	if (m_statusBarWidgetsCompPtr.IsValid()){
		for (int widgetIndex = 0; widgetIndex < widgetsCount; widgetIndex++){
			iqtgui::IGuiObject* guiPtr = m_statusBarWidgetsCompPtr[widgetIndex];
			if (guiPtr != NULL && guiPtr->IsGuiCreated()){
				retVal = guiPtr->DestroyGui() && retVal;
			}
		}
	}
	else{
		return false;
	}

	return retVal;
}


} // namespace iqtgui


