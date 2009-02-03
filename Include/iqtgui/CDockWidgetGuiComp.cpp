#include "iqtgui/CDockWidgetGuiComp.h"


#include <QMainWindow>


namespace iqtgui
{


// public methods

bool CDockWidgetGuiComp::AddToMainWindow(QMainWindow& mainWindow)
{
	Qt::DockWidgetArea area = Qt::LeftDockWidgetArea;
	if (m_dockAreaAttrPtr.IsValid()){
		switch (m_dockAreaAttrPtr->GetValue()){
			case 0: area = Qt::LeftDockWidgetArea;
				break;

			case 1: area = Qt::TopDockWidgetArea;
				break;

			case 2: area = Qt::BottomDockWidgetArea;
				break;

			case 3: area = Qt::RightDockWidgetArea;
				break;
		}
	}

	QDockWidget* dockWidgetPtr = GetQtWidget();
	if (dockWidgetPtr != NULL){
		mainWindow.addDockWidget(area, dockWidgetPtr);

		return true;
	}

	return false;
}


// reimplemented (CGuiComponentBase)

void CDockWidgetGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QDockWidget* dockWidgetPtr = GetQtWidget();
	I_ASSERT(dockWidgetPtr != NULL);

	if (m_dockTitleAttrPtr.IsValid()){
		dockWidgetPtr->setWindowTitle(iqt::GetQString(m_dockTitleAttrPtr->GetValue()));
	}

	if (m_slaveGuiCompPtr.IsValid() && m_slaveGuiCompPtr->CreateGui(NULL)){
		QWidget* slaveWidgetPtr = m_slaveGuiCompPtr->GetWidget();
		if (slaveWidgetPtr != NULL){
			dockWidgetPtr->setWidget(slaveWidgetPtr);
		}
	}

	dockWidgetPtr->setFeatures(	
				QDockWidget::DockWidgetMovable | 
				QDockWidget::DockWidgetFloatable | 
				QDockWidget::DockWidgetClosable);
}


void CDockWidgetGuiComp::OnGuiDestroyed()
{
	if (m_slaveGuiCompPtr.IsValid() && m_slaveGuiCompPtr->IsGuiCreated()){
		m_slaveGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


void CDockWidgetGuiComp::OnRetranslate()
{
	QDockWidget* dockWidgetPtr = GetQtWidget();
	I_ASSERT(dockWidgetPtr != NULL);

	if (m_dockTitleAttrPtr.IsValid()){
		QString title = iqt::GetQString(m_dockTitleAttrPtr->GetValue());
		dockWidgetPtr->setWindowTitle(tr(title.toAscii()));
	}
}


} // namespace iqtgui


