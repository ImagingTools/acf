#include "iqtgui/CTabbedMultiViewGuiComp.h"


#include "imod/IModel.h"


namespace iqtgui
{


// public methods

// reimplemented (imod::IObserver)

bool CTabbedMultiViewGuiComp::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass2::OnAttached(modelPtr)){
		if (m_observersCompPtr.IsValid()){
			for (int index = 0; index < m_observersCompPtr.GetCount(); index++){
				modelPtr->AttachObserver(m_observersCompPtr[index]);
			}
		}

		return true;
	}

	return false;
}


bool CTabbedMultiViewGuiComp::OnDetached(imod::IModel* modelPtr)
{
	if (m_observersCompPtr.IsValid()){
		for (int index = 0; index < m_observersCompPtr.GetCount(); index++){
			if (modelPtr->IsAttached(m_observersCompPtr[index])){
				modelPtr->DetachObserver(m_observersCompPtr[index]);
			}
		}
	}

	return BaseClass2::OnDetached(modelPtr);
}


// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CTabbedMultiViewGuiComp::GetCommands() const
{
	if (m_observersCompPtr.IsValid()){
		int observerCount = m_observersCompPtr.GetCount();
		for (int observerIndex = 0; observerIndex < observerCount; observerIndex++){
			idoc::ICommandsProvider* commandsProviderPtr = CompCastPtr<idoc::ICommandsProvider>(m_observersCompPtr[observerIndex]);
			if (commandsProviderPtr != NULL){
				const idoc::IHierarchicalCommand* commandPtr = commandsProviderPtr->GetCommands();
				if (commandPtr != NULL){
					m_commands.JoinLinkFrom(commandPtr);
				}
			}
		}
	}

	return &m_commands;
}


} // namespace iqtgui


