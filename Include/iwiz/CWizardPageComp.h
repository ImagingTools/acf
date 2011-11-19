#ifndef iwiz_CWizardPageComp_included
#define iwiz_CWizardPageComp_included


#include "imod/CMultiModelDispatcherBase.h"

#include "iprm/CComposedParamsSetComp.h"

#include "iproc/IStateController.h"
#include "iproc/IProcessor.h"


namespace iwiz
{


/**
	Component implementation of a wizard page.
*/
class CWizardPageComp:
			public iprm::CComposedParamsSetComp,
			protected imod::CMultiModelDispatcherBase,
			virtual public iproc::IStateController
{
public:
	typedef iprm::CComposedParamsSetComp BaseClass;
	typedef imod::CMultiModelDispatcherBase BaseClass2;

	I_BEGIN_COMPONENT(CWizardPageComp);
		I_REGISTER_INTERFACE(iproc::IStateController);
		I_ASSIGN_MULTI_0(m_slaveControllersCompPtr, "SlaveControllers", "State controllers used to calculation general page state", true);
		I_ASSIGN_TO(m_slaveControllerModelsCompPtr, m_slaveControllersCompPtr, true);
	I_END_COMPONENT;

	CWizardPageComp();

	// reimplemented (iproc::IStateController)
	virtual bool IsStateEnabled() const;
	virtual bool IsStateActive() const;
	virtual bool IsEnterAllowed(bool isActionAllowed = true, const IStateController* prevStatePtr = NULL) const;
	virtual bool IsLeaveAllowed(bool isActionAllowed = true, const IStateController* nextStatePtr = NULL) const;
	virtual bool TryEnterState(bool isActionAllowed = true, const IStateController* prevStatePtr = NULL);
	virtual bool TryLeaveState(bool isActionAllowed = true, const IStateController* prevStatePtr = NULL);

protected:
	void UpdateAllMembers();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	bool m_isStateEnabled;
	bool m_isStateActive;

	I_MULTIREF(iproc::IStateController, m_slaveControllersCompPtr);
	I_MULTIREF(imod::IModel, m_slaveControllerModelsCompPtr);
	I_REF(iproc::IProcessor, m_processOnEnterCompPtr);
	I_REF(iproc::IProcessor, m_processOnLeaveCompPtr);
};


} // namespace iwiz


#endif // !iwiz_CWizardPageComp_included


