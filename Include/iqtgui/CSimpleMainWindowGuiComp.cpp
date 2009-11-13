#include "iqtgui/CSimpleMainWindowGuiComp.h"


// Qt includes
#include <QSettings>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IGuiObject)

void CSimpleMainWindowGuiComp::OnTryClose(bool* ignoredPtr)
{
	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->OnTryClose(ignoredPtr);

		return;
	}

	BaseClass::OnTryClose(ignoredPtr);
}


// protected methods

void CSimpleMainWindowGuiComp::AddMainComponent(iqtgui::IMainWindowComponent* componentPtr)
{
	QMainWindow* mainWindowPtr = GetQtWidget();
	if (mainWindowPtr != NULL){
		componentPtr->AddToMainWindow(*mainWindowPtr);
	}
}


void CSimpleMainWindowGuiComp::RemoveMainComponent(iqtgui::IMainWindowComponent* componentPtr)
{
	QMainWindow* mainWindowPtr = GetQtWidget();
	if (mainWindowPtr != NULL){
		componentPtr->RemoveFromMainWindow(*mainWindowPtr);
	}
}


// reimplemented (TRestorableGuiWrap)

void CSimpleMainWindowGuiComp::OnRestoreSettings(const QSettings& settings)
{
	I_ASSERT(IsGuiCreated());
	QMainWindow* mainWindowPtr = GetQtWidget();
	I_ASSERT(mainWindowPtr != NULL);

	QByteArray windowState = settings.value("MainWindow/State").toByteArray();
	QByteArray windowGeometry = settings.value("MainWindow/Geometry").toByteArray();

	mainWindowPtr->restoreState(windowState);
	mainWindowPtr->restoreGeometry(windowGeometry);
}


void CSimpleMainWindowGuiComp::OnSaveSettings(QSettings& settings) const
{
	I_ASSERT(IsGuiCreated());
	QMainWindow* mainWindowPtr = GetQtWidget();
	I_ASSERT(mainWindowPtr != NULL);

	QByteArray windowState = mainWindowPtr->saveState();
	QByteArray windowGeometry = mainWindowPtr->saveGeometry();

	settings.setValue("MainWindow/State", windowState);
	settings.setValue("MainWindow/Geometry", windowGeometry);
}


// reimplemented (iqtgui::TGuiComponentBase)

void CSimpleMainWindowGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QMainWindow* mainWindowPtr = GetQtWidget();
	if (mainWindowPtr == NULL){
		return;
	}

	if (m_iconSizeAttrPtr.IsValid() && m_iconSizeAttrPtr->GetValue() != 0){
		mainWindowPtr->setIconSize(QSize(m_iconSizeAttrPtr->GetValue(), m_iconSizeAttrPtr->GetValue()));
	}

	I_ASSERT(m_isNestingEnabledAttrPtr.IsValid());
	mainWindowPtr->setDockOptions(*m_isNestingEnabledAttrPtr? QMainWindow::AllowNestedDocks: QMainWindow::DockOption(0));

	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->CreateGui(NULL);
		QWidget* workspacePtr = m_workspaceCompPtr->GetWidget();
		if (workspacePtr != NULL){
			mainWindowPtr->setCentralWidget(workspacePtr);
		}
	}

	for (int componentIndex = 0; componentIndex < m_mainWindowComponentsPtr.GetCount(); componentIndex++){
		iqtgui::IMainWindowComponent* mainWindowComponentPtr =  m_mainWindowComponentsPtr[componentIndex];
		if (mainWindowComponentPtr != NULL){
			AddMainComponent(mainWindowComponentPtr);
		}
	}
}


void CSimpleMainWindowGuiComp::OnGuiDestroyed()
{
	if (m_workspaceCompPtr.IsValid()){
		m_workspaceCompPtr->DestroyGui();
	}

	for (int componentIndex = 0; componentIndex < m_mainWindowComponentsPtr.GetCount(); componentIndex++){
		iqtgui::IMainWindowComponent* mainWindowComponentPtr =  m_mainWindowComponentsPtr[componentIndex];
		if (mainWindowComponentPtr != NULL){
			RemoveMainComponent(mainWindowComponentPtr);
		}
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtgui


