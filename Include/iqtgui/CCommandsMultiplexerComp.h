#ifndef iqtgui_CCommandsMultiplexerComp_included
#define iqtgui_CCommandsMultiplexerComp_included


// ACF includes
#include <imod/CModelUpdateBridge.h>
#include <icomp/CComponentBase.h>
#include <ibase/ICommandsProvider.h>
#include <iprm/ISelectionParam.h>


namespace iqtgui
{


/**
	Command multipling component.
*/
class CCommandsMultiplexerComp:
			public QObject,
			public icomp::CComponentBase,
			protected imod::CModelUpdateBridge,
			virtual public ibase::ICommandsProvider
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::CModelUpdateBridge BaseClass2;
	
	I_BEGIN_COMPONENT(CCommandsMultiplexerComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_commandSelectionCompPtr, "CommandSelector", "Commands selector", true, "CommandSelector");
		I_ASSIGN_TO(m_commandSelectionModelCompPtr, m_commandSelectionCompPtr, true);
		I_ASSIGN_MULTI_0(m_commandProvidersCompPtr, "CommandProviders", "List if underlaying command providers", true);
		I_ASSIGN_TO(m_commandProviderModelsCompPtr, m_commandProvidersCompPtr, false);
	I_END_COMPONENT;

	CCommandsMultiplexerComp();

	// reimpemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

	// reimpemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iprm::ISelectionParam, m_commandSelectionCompPtr);
	I_REF(imod::IModel, m_commandSelectionModelCompPtr);
	I_MULTIREF(ibase::ICommandsProvider, m_commandProvidersCompPtr);
	I_MULTIREF(imod::IModel, m_commandProviderModelsCompPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CCommandsMultiplexerComp_included


