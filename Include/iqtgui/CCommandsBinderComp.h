#ifndef iqtgui_CCommandsBinderComp_included
#define iqtgui_CCommandsBinderComp_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "imod/CMultiModelDispatcherBase.h"

#include "ibase/ICommandsProvider.h"
#include "ibase/TCommandsProviderCompWrap.h"

#include "iqtgui/CHierarchicalCommand.h"


namespace iqtgui
{


/**
	Component for binding of multiple command providers
*/
class CCommandsBinderComp:
			public ibase::TCommandsProviderCompWrap<icomp::CComponentBase>,
			protected imod::CMultiModelDispatcherBase
{
public:
	typedef ibase::TCommandsProviderCompWrap<icomp::CComponentBase> BaseClass;

	I_BEGIN_COMPONENT(CCommandsBinderComp);
		I_ASSIGN_MULTI_0(m_commandProvidersCompPtr, "CommandProviders", "List of command providers", true);
	I_END_COMPONENT;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	// reimplemented (imod::CMultiModelDispatcherBase)
	void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	void CreateCommands();
private:
	I_MULTIREF(ibase::ICommandsProvider, m_commandProvidersCompPtr);

	mutable iqtgui::CHierarchicalCommand m_commands;
};


} // namespace iqtgui


#endif // !iqtgui_CCommandsBinderComp_included


