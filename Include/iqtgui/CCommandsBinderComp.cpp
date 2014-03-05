#include "iqtgui/CCommandsBinderComp.h"

// ACF includes
#include "istd/TChangeNotifier.h"


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

void CCommandsBinderComp::OnModelChanged(int /*modelId*/, int changeFlags, istd::IPolymorphic* updateParamsPtr)
{
	if (m_isUpdateLocked){
		return;
	}

	UnregisterAllModels();

	istd::CChangeNotifier changePtr(this, changeFlags, updateParamsPtr);

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
				RegisterModel(commandsProviderModelPtr, index, ibase::ICommandsProvider::CF_COMMANDS);
			}
		}
	}

	m_isUpdateLocked = false;
}


} // namespace iqtgui


