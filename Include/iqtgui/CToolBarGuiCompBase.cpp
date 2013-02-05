#include "iqtgui/CToolBarGuiCompBase.h"


// Qt includes
#include <QtGui/QHBoxLayout>
#include <QtGui/QComboBox>
#include <QtGui/QMainWindow>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IMainWindowComponent)

bool CToolBarGuiCompBase::AddToMainWindow(QMainWindow& mainWindow)
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
	Q_ASSERT(toolBarPtr != NULL); 
	if (toolBarPtr != NULL){
		toolBarPtr->setIconSize(mainWindow.iconSize());
		mainWindow.addToolBar(area, toolBarPtr);

		return true;
	}

	return false;
}


bool CToolBarGuiCompBase::RemoveFromMainWindow(QMainWindow& /*mainWindow*/)
{
	return DestroyGui();
}


// protected methods

// reimplemented (CGuiComponentBase)

void CToolBarGuiCompBase::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QToolBar* widgetPtr = GetQtWidget();
	Q_ASSERT(widgetPtr != NULL);
	if (widgetPtr == NULL){
		return;
	}

	if (m_titleAttrPtr.IsValid()){
		widgetPtr->setWindowTitle(m_titleAttrPtr->GetValue());
		widgetPtr->setObjectName(m_titleAttrPtr->GetValue());
	}

	if (m_iconSizeAttrPtr.IsValid()){
		widgetPtr->setIconSize(QSize(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr));
	}

//	widgetPtr->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}


} // namespace iqtgui


