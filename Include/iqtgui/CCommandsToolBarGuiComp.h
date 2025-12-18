#ifndef iqtgui_CCommandsToolBarGuiComp_included
#define iqtgui_CCommandsToolBarGuiComp_included


// ACF includes
#include <imod/TSingleModelObserverBase.h>
#include <ibase/ICommandsProvider.h>
#include <ibase/ICommandsDisplayer.h>
#include <iqtgui/CToolBarGuiCompBase.h>
#include <iqtgui/CHierarchicalCommand.h>


namespace iqtgui
{


/**
	Component for construction of a tool bar for a given commands provider.
*/
class CCommandsToolBarGuiComp:
			public CToolBarGuiCompBase,
			virtual public ibase::ICommandsDisplayer,
			protected imod::TSingleModelObserverBase<ibase::ICommandsProvider>
{
public:
	typedef CToolBarGuiCompBase BaseClass; 
	
	I_BEGIN_COMPONENT(CCommandsToolBarGuiComp);
		I_REGISTER_INTERFACE(iqtgui::IMainWindowComponent);
		I_REGISTER_INTERFACE(ibase::ICommandsDisplayer);
		I_ASSIGN(m_slaveCommandsDisplayerCompPtr, "SlaveCommandsDisplayer", "Commands will be forwarded here as well", false, "");
		I_ASSIGN(m_commandsProviderCompPtr, "CommandsProvider", "Commands for the tool bar", false, "CommandsProvider");
		I_ASSIGN_TO(m_commandsProviderModelCompPtr, m_commandsProviderCompPtr, false);
	I_END_COMPONENT;

	// reimplemented (ibase::ICommandsDisplayer)
	virtual void ShowCommands(const ibase::ICommandsProvider* commandsProvider) override;

protected:
	// reimplemented (imod::TSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;

private:
	void UpdateCommands();

private:
	I_REF(ibase::ICommandsDisplayer, m_slaveCommandsDisplayerCompPtr);
	I_REF(ibase::ICommandsProvider, m_commandsProviderCompPtr);
	I_REF(imod::IModel, m_commandsProviderModelCompPtr);

	iqtgui::CHierarchicalCommand m_toolBarCommands;
};


} // namespace iqtgui


#endif // !iqtgui_CCommandsToolBarGuiComp_included


