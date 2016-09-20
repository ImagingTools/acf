#ifndef iqtgui_CCommandsToolBarGuiComp_included
#define iqtgui_CCommandsToolBarGuiComp_included


// ACF includes
#include <ibase/ICommandsProvider.h>

#include <iqtgui/CToolBarGuiCompBase.h>
#include <iqtgui/CHierarchicalCommand.h>


namespace iqtgui
{


/**
	Component for construction of a tool bar for a given commands provider.
*/
class CCommandsToolBarGuiComp: public CToolBarGuiCompBase
{
public:
	typedef CToolBarGuiCompBase BaseClass; 
	
	I_BEGIN_COMPONENT(CCommandsToolBarGuiComp);
		I_REGISTER_INTERFACE(iqtgui::IMainWindowComponent);
		I_ASSIGN(m_commandsProviderCompPtr, "CommandsProvider", "Commands for the tool bar", true, "CommandsProvider");
	I_END_COMPONENT;

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(ibase::ICommandsProvider, m_commandsProviderCompPtr);

	iqtgui::CHierarchicalCommand m_toolBarCommands;
};


} // namespace iqtgui


#endif // !iqtgui_CCommandsToolBarGuiComp_included


