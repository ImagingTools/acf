#include "iqtgui/CStatusBarWidgetComp.h"


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IMainWindowComponent)

bool CStatusBarWidgetComp::AddToMainWindow(QMainWindow& mainWindow)
{
	if (m_statusBarWidgetsCompPtr.IsValid()){
		QStatusBar* statusBar = mainWindow.statusBar();
		Q_ASSERT(statusBar != NULL);

		statusBar->setSizeGripEnabled(*m_isSizeGripEnabledAttrPtr);

		if (*m_suppressWidgetFrameAttrPtr){
			statusBar->setStyleSheet("QStatusBar::item { border: 0px solid transparent }");
		}

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


QByteArray CStatusBarWidgetComp::GetAssociatedDocumentTypeId() const
{
	static QByteArray emptyId;

	if (m_associatedDocumentTypeId.IsValid()){
		return *m_associatedDocumentTypeId;
	}

	return emptyId;
}


QString CStatusBarWidgetComp::GetTitle() const
{
	static QString emptyTitle;

	return emptyTitle;
}


int CStatusBarWidgetComp::GetFlags() const
{
	return WCF_NONE;
}


} // namespace iqtgui


