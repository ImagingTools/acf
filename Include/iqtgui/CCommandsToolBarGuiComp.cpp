#include "iqtgui/CCommandsToolBarGuiComp.h"


// Qt includes
#include <QtGui/QMainWindow>

// ACF includes
#include "iqtgui/CCommandTools.h"


namespace iqtgui
{


// protected methods

// reimplemented (CGuiComponentBase)

void CCommandsToolBarGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QToolBar* toolBarPtr = GetQtWidget();
	Q_ASSERT(toolBarPtr != NULL);
	if (toolBarPtr == NULL){
		return;
	}

	if (m_commandsProviderCompPtr.IsValid()){
		const ibase::IHierarchicalCommand* commandsPtr = m_commandsProviderCompPtr->GetCommands();
		if (commandsPtr != NULL){
			m_toolBarCommands.JoinLinkFrom(commandsPtr);
		}

		CCommandTools::SetupToolbar(m_toolBarCommands, *toolBarPtr);
	}
}


void CCommandsToolBarGuiComp::OnGuiDestroyed()
{
	m_toolBarCommands.ResetChilds();

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtgui


