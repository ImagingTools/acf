#include "iqtgui/CDockWidgetGuiComp.h"


#include <QMainWindow>
#include <QVBoxLayout>


namespace iqtgui
{


// public methods

bool CDockWidgetGuiComp::AddToMainWindow(QMainWindow& mainWindow)
{
	if (IsGuiCreated()){
		return false;
	}

	CreateGui(&mainWindow);

	Qt::DockWidgetArea area = Qt::LeftDockWidgetArea;
	Qt::Orientation orientation = Qt::Vertical;
	if (m_dockAreaAttrPtr.IsValid()){
		switch (m_dockAreaAttrPtr->GetValue()){
			case 0:
				area = Qt::LeftDockWidgetArea;
				orientation = Qt::Vertical;
				break;

			case 1:
				area = Qt::RightDockWidgetArea;
				orientation = Qt::Vertical;
				break;

			case 2:
				area = Qt::TopDockWidgetArea;
				orientation = Qt::Horizontal;
				break;

			case 3:
				area = Qt::BottomDockWidgetArea;
				orientation = Qt::Horizontal;
				break;
		}
	}

	QDockWidget* dockWidgetPtr = GetQtWidget();
	if (dockWidgetPtr != NULL){
		mainWindow.addDockWidget(area, dockWidgetPtr, orientation);

		return true;
	}

	return false;
}


bool CDockWidgetGuiComp::RemoveFromMainWindow(QMainWindow& /*mainWindow*/)
{
	return DestroyGui();
}


// protected methods

// reimplemented (CGuiComponentBase)

void CDockWidgetGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QDockWidget* dockWidgetPtr = GetQtWidget();
	I_ASSERT(dockWidgetPtr != NULL);

	if (m_dockTitleAttrPtr.IsValid()){
		dockWidgetPtr->setWindowTitle(iqt::GetQString(m_dockTitleAttrPtr->GetValue()));
		dockWidgetPtr->setObjectName(iqt::GetQString(m_dockTitleAttrPtr->GetValue()));
	}

	istd::TDelPtr<QWidget> containerWidgetPtr(new QWidget(NULL));
	QVBoxLayout* layoutPtr = new QVBoxLayout(containerWidgetPtr.GetPtr());
	layoutPtr->setMargin(3);
	containerWidgetPtr->setLayout(layoutPtr);

	if (m_slaveGuiCompPtr.IsValid() && m_slaveGuiCompPtr->CreateGui(containerWidgetPtr.GetPtr())){
		dockWidgetPtr->setWidget(containerWidgetPtr.PopPtr());
	}

	if (m_dockFeaturesAttrPtr.IsValid()){
		dockWidgetPtr->setFeatures(QDockWidget::DockWidgetFeature(*m_dockFeaturesAttrPtr));
	}
	else{
		dockWidgetPtr->setFeatures(	
					QDockWidget::DockWidgetMovable | 
					QDockWidget::DockWidgetFloatable | 
					QDockWidget::DockWidgetClosable);
	}

	if (m_allowedDockAreasAttrPtr.IsValid()){
		dockWidgetPtr->setAllowedAreas(Qt::DockWidgetAreas(*m_allowedDockAreasAttrPtr));
	}
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


