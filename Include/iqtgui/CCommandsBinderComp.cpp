#include "iqtgui/CCommandsBinderComp.h"


namespace iqtgui
{


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CCommandsBinderComp::GetCommands() const
{
	return &m_commands;
}


// reimplemented (icomp::IComponent)

void CCommandsBinderComp::OnComponentCreated()
{
	if (m_commandProvidersCompPtr.IsValid()){
		for (int index = 0; index < m_commandProvidersCompPtr.GetCount(); index++){
			ibase::ICommandsProvider* commandsProviderPtr = m_commandProvidersCompPtr[index];
			if (commandsProviderPtr != NULL){
				const ibase::IHierarchicalCommand* commandPtr = commandsProviderPtr->GetCommands();
				if (commandPtr != NULL){
					m_commands.JoinLinkFrom(commandPtr);
				}
			}
		}
	}
}


} // namespace iqtgui


