#include "iqtgui/CCommandsBinderComp.h"

// ACF includes
#include "istd/CChangeNotifier.h"


namespace iqtgui
{


// public methods

CCommandsBinderComp::CCommandsBinderComp()
	:m_isUpdateLocked(false)
{
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CCommandsBinderComp::GetCommands() const
{
	return &m_commands;
}


// protected methods

// reimplemented (imod::CMultiModelDispatcherBase)

void CCommandsBinderComp::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& changeSet)
{
	if (m_isUpdateLocked){
		return;
	}

	UnregisterAllModels();

	istd::CChangeNotifier notifier(this, &changeSet);
	Q_UNUSED(notifier);

	m_commands.ResetChilds();

	CreateCommands();
}


// reimplemented (icomp::CComponentBase)

void CCommandsBinderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	CreateCommands();
}


// private methods

void CCommandsBinderComp::CreateCommands()
{
	m_isUpdateLocked = true;

	if (m_commandProvidersCompPtr.IsValid()){
		for (int index = 0; index < m_commandProvidersCompPtr.GetCount(); index++){
			ibase::ICommandsProvider* commandsProviderPtr = m_commandProvidersCompPtr[index];
			if (commandsProviderPtr != NULL){
				const ibase::IHierarchicalCommand* commandPtr = commandsProviderPtr->GetCommands();
				if (commandPtr != NULL){
					m_commands.JoinLinkFrom(commandPtr);
				}
			}

			imod::IModel* commandsProviderModelPtr = dynamic_cast<imod::IModel*>(commandsProviderPtr);
			if (commandsProviderModelPtr != NULL){
				static const istd::IChangeable::ChangeSet commandsChangeSet(ibase::ICommandsProvider::CF_COMMANDS);
				RegisterModel(commandsProviderModelPtr, index, commandsChangeSet);
			}
		}
	}

	m_isUpdateLocked = false;
}


} // namespace iqtgui


