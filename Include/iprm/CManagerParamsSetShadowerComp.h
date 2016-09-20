#ifndef iprm_CManagerParamsSetShadowerComp_included
#define iprm_CManagerParamsSetShadowerComp_included


#include <icomp/CComponentBase.h>

#include <imod/CSingleModelObserverBase.h>

#include <iprm/IOptionsList.h>
#include <iprm/IParamsSet.h>
#include <iprm/ISelectionParam.h>
#include <iprm/IParamsManager.h>


namespace iprm
{


/**
	Keep parameter set with selected one synchronized.
*/
class CManagerParamsSetShadowerComp:
			public icomp::CComponentBase,
			protected imod::CSingleModelObserverBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CManagerParamsSetShadowerComp);
		I_ASSIGN(m_paramsManagerCompPtr, "ParamsManager", "Manager of parameter set used to realize selection", true, "ParamsManager");
		I_ASSIGN_TO(m_paramsManagerModelCompPtr, m_paramsManagerCompPtr, true);
		I_ASSIGN(m_selectionParamCompPtr, "SelectionParam", "Selection parameter choising some object in parameter manager", true, "SelectionParam");
		I_ASSIGN(m_shadowParamsSetCompPtr, "ShadowParamsSet", "Shadow parameter set where current selected set will be copied", true, "ShadowParamsSet");
	I_END_COMPONENT;

	CManagerParamsSetShadowerComp();

protected:
	/**
		Do copy current selected parameter set to shadow object.
	*/
	bool CopyParamsSetToShadow();

	/**
		Set the bridge to the currently selected parameter set.
		Over this mechanism the changes in the slave parameter set will be reflected by this object.
		\sa CurrentParamsSetObserver
	*/
	void SetupCurrentParamsSetBridge();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

private:
	/**
		Observer for the selected parameter set.
		On changes in the observed model the change event will be delegated to the observers of this object.
	*/
	class CurrentParamsSetObserver: public imod::CSingleModelObserverBase
	{
	public:
		CurrentParamsSetObserver(CManagerParamsSetShadowerComp* parentPtr);

		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	private:
		CManagerParamsSetShadowerComp& m_parent;
	};

	CurrentParamsSetObserver m_currentParamsSetObserver;
	imod::IModel* m_currentParamsModelPtr;

	I_REF(IParamsManager, m_paramsManagerCompPtr);
	I_REF(imod::IModel, m_paramsManagerModelCompPtr);
	I_REF(ISelectionParam, m_selectionParamCompPtr);
	I_REF(IParamsSet, m_shadowParamsSetCompPtr);
};


} // namespace iprm


#endif // !iprm_CManagerParamsSetShadowerComp_included


