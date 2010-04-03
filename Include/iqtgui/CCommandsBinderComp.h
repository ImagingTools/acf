#ifndef iqtgui_CCommandsBinderComp_included
#define iqtgui_CCommandsBinderComp_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "ibase/ICommandsProvider.h"

#include "iqtgui/CHierarchicalCommand.h"


namespace iqtgui
{


/**
	Component for binding of multiple command providers
*/
class CCommandsBinderComp: public icomp::CComponentBase, virtual public ibase::ICommandsProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCommandsBinderComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN_MULTI_0(m_commandProvidersCompPtr, "CommandProviders", "List of command providers", true)
	I_END_COMPONENT;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_MULTIREF(ibase::ICommandsProvider, m_commandProvidersCompPtr);

	mutable iqtgui::CHierarchicalCommand m_commands;
};


} // namespace iqtgui


#endif // !iqtgui_CCommandsBinderComp_included


