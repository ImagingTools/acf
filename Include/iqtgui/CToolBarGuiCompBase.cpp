#include "iqtgui/CToolBarGuiCompBase.h"


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QAction>
#else
#include <QtGui/QHBoxLayout>
#include <QtGui/QComboBox>
#include <QtGui/QMainWindow>
#include <QtGui/QAction>
#endif


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

	Qt::ToolBarArea toolBarArea = Qt::TopToolBarArea;
	if (m_toolBarAreaAttrPtr.IsValid()){
		switch (*m_toolBarAreaAttrPtr){
			case 0:
				toolBarArea = Qt::LeftToolBarArea;
				break;

			case 1:
				toolBarArea = Qt::RightToolBarArea;
				break;
			case 2:
				toolBarArea = Qt::TopToolBarArea;
				break;

			case 3:
				toolBarArea = Qt::BottomToolBarArea;
				break;
			default:
				break;
		}
	}

	QToolBar* toolBarPtr = GetQtWidget();
	Q_ASSERT(toolBarPtr != NULL);
	if (toolBarPtr != NULL){
		toolBarPtr->setIconSize(mainWindow.iconSize());

		toolBarPtr->setOrientation(*m_useVerticalOrientationAttrPtr ? Qt::Vertical : Qt::Horizontal);
		toolBarPtr->setFloatable(false);
		toolBarPtr->setMovable(false);

		switch (*m_dockFeaturesAttrPtr){
			case DF_MOVEABLE:
				toolBarPtr->setFloatable(true);
				break;
			case DF_FLOATABLE:
				toolBarPtr->setMovable(true);
				break;
			default:
				break;
		}

		if (m_allowedDockAreasAttrPtr.IsValid()){
			toolBarPtr->setAllowedAreas(Qt::ToolBarArea(*m_allowedDockAreasAttrPtr));
		}

		mainWindow.addToolBar(toolBarArea, toolBarPtr);

		return true;
	}

	return false;
}


bool CToolBarGuiCompBase::RemoveFromMainWindow(QMainWindow& /*mainWindow*/)
{
	return DestroyGui();
}


QByteArray CToolBarGuiCompBase::GetAssociatedDocumentTypeId() const
{
	static QByteArray emptyId;

	if (m_associatedDocumentTypeId.IsValid()){
		return *m_associatedDocumentTypeId;
	}

	return emptyId;
}


QString CToolBarGuiCompBase::GetTitle() const
{
	static QString emptyTitle;

	if (m_titleAttrPtr.IsValid()){
		return *m_titleAttrPtr;
	}

	return emptyTitle;
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

	QAction* toggleViewAction = widgetPtr->toggleViewAction();
	if (toggleViewAction != NULL){
		toggleViewAction->setVisible(false);
	}

	if (m_titleAttrPtr.IsValid()){
		widgetPtr->setWindowTitle(m_titleAttrPtr->GetValue());
		widgetPtr->setObjectName(m_titleAttrPtr->GetValue());
	}

	if (m_iconSizeAttrPtr.IsValid()){
		widgetPtr->setIconSize(QSize(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr));
	}

	widgetPtr->setToolButtonStyle(Qt::ToolButtonStyle(*m_buttonStyleAttrPtr));
}


} // namespace iqtgui


