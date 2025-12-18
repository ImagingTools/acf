#include <iqtgui/CCommandsToolBarGuiComp.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif

// ACF includes
#include <iqtgui/CCommandTools.h>


namespace iqtgui
{


// reimplemented (ibase::ICommandsDisplayer)

void CCommandsToolBarGuiComp::ShowCommands(const ibase::ICommandsProvider* commandsProvider)
{
	if (m_slaveCommandsDisplayerCompPtr.IsValid()) {
		m_slaveCommandsDisplayerCompPtr->ShowCommands(commandsProvider);
	}

	EnsureModelDetached();

	m_toolBarCommands.ResetChilds();

	if (!IsGuiCreated()) {
		return;
	}

	QToolBar* toolBarPtr = GetQtWidget();
	Q_ASSERT(toolBarPtr != NULL);
	if (toolBarPtr == NULL) {
		return;
	}

	toolBarPtr->clear();

	if (!commandsProvider)
		return;

	const ibase::IHierarchicalCommand* commandsPtr = commandsProvider->GetCommands();
	if (commandsPtr != NULL) {
		m_toolBarCommands.JoinLinkFrom(commandsPtr);
	}

	CCommandTools::SetupToolbar(m_toolBarCommands, *toolBarPtr);
}


// protected methods

// reimplemented (imod::TSingleModelObserverBase)

void CCommandsToolBarGuiComp::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	UpdateCommands();
}


// reimplemented (CGuiComponentBase)

void CCommandsToolBarGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_commandsProviderCompPtr.IsValid() && m_commandsProviderModelCompPtr.IsValid()){
		m_commandsProviderModelCompPtr->AttachObserver(this);
	}

	UpdateCommands();
}


void CCommandsToolBarGuiComp::OnGuiDestroyed()
{
	if (m_commandsProviderModelCompPtr.IsValid() && m_commandsProviderModelCompPtr->IsAttached(this)){
		m_commandsProviderModelCompPtr->DetachObserver(this);
	}

	m_toolBarCommands.ResetChilds();

	BaseClass::OnGuiDestroyed();
}


// private methods

void CCommandsToolBarGuiComp::UpdateCommands()
{
	m_toolBarCommands.ResetChilds();

	if (!IsGuiCreated()){
		return;
	}

	QToolBar* toolBarPtr = GetQtWidget();
	Q_ASSERT(toolBarPtr != NULL);
	if (toolBarPtr == NULL){
		return;
	}

	toolBarPtr->clear();

	if (m_commandsProviderCompPtr.IsValid()){
		const ibase::IHierarchicalCommand* commandsPtr = m_commandsProviderCompPtr->GetCommands();
		if (commandsPtr != NULL){
			m_toolBarCommands.JoinLinkFrom(commandsPtr);
		}

		CCommandTools::SetupToolbar(m_toolBarCommands, *toolBarPtr);
	}
}


} // namespace iqtgui


