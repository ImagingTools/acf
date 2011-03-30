#include "iqtgui/CToolBarWidgetGuiComp.h"


// Qt includes
#include <QHBoxLayout>
#include <QComboBox>
#include <QMainWindow>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IMainWindowComponent)

bool CToolBarWidgetGuiComp::AddToMainWindow(QMainWindow& mainWindow)
{
	if (IsGuiCreated()){
		return false;
	}
		
	if (!CreateGui(NULL)){
		return false;
	}

	Qt::ToolBarArea area = Qt::AllToolBarAreas;
	int dockArea = Qt::TopToolBarArea;

	if (m_dockAreaAttrPtr.IsValid()){
		dockArea = *m_dockAreaAttrPtr;
	}

	switch (dockArea){
		case 0:
			area = Qt::LeftToolBarArea;
			break;

		case 1:
			area = Qt::RightToolBarArea;
			break;
		case 2:
			area = Qt::TopToolBarArea;
			break;

		case 3:
			area = Qt::BottomToolBarArea;
			break;
	}

	QToolBar* toolBarPtr = GetQtWidget();
	I_ASSERT(toolBarPtr != NULL); 
	if (toolBarPtr != NULL){
		toolBarPtr->setIconSize(mainWindow.iconSize());
		mainWindow.addToolBar(area, toolBarPtr);

		return true;
	}

	return false;
}


bool CToolBarWidgetGuiComp::RemoveFromMainWindow(QMainWindow& /*mainWindow*/)
{
	return DestroyGui();
}


// protected methods

// reimplemented (CGuiComponentBase)

void CToolBarWidgetGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QToolBar* widgetPtr = GetQtWidget();
	I_ASSERT(widgetPtr != NULL);
	if (widgetPtr == NULL){
		return;
	}

	if (m_titleAttrPtr.IsValid()){
		widgetPtr->setWindowTitle(iqt::GetQString(m_titleAttrPtr->GetValue()));
		widgetPtr->setObjectName(iqt::GetQString(m_titleAttrPtr->GetValue()));
	}

	if (m_toolbarWidgetCompPtr.IsValid()){
		iqtgui::IGuiObject* guiPtr = m_toolbarWidgetCompPtr.GetPtr();
		if (guiPtr != NULL){
			I_ASSERT(!guiPtr->IsGuiCreated());

			if (guiPtr->CreateGui(NULL)){
				widgetPtr->addWidget(guiPtr->GetWidget());
			}
		}
	}
}


void CToolBarWidgetGuiComp::OnGuiDestroyed()
{
	if (m_toolbarWidgetCompPtr.IsValid()){
		iqtgui::IGuiObject* guiPtr = m_toolbarWidgetCompPtr.GetPtr();
		if (guiPtr != NULL){
			guiPtr->DestroyGui();
		}
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtgui


