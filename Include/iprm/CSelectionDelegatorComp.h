#ifndef iprm_CSelectionDelegatorComp_included
#define iprm_CSelectionDelegatorComp_included


#include "icomp/CComponentBase.h"

#include "imod/CSingleModelObserverBase.h"

#include "iprm/ISelectionParam.h"


namespace iprm
{


/**
	Component for synchronization of a list of selections.
	
*/
class CSelectionDelegatorComp:
			public icomp::CComponentBase,
			protected imod::CSingleModelObserverBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSelectionDelegatorComp);
		I_ASSIGN(m_sourceSelectionCompPtr, "SourceSelection", "Source selection", true, "SourceSelection");
		I_ASSIGN_TO(m_sourceSelectionModelCompPtr, m_sourceSelectionCompPtr, true);
		I_ASSIGN_MULTI_0(m_slaveSelectionsCompPtr, "SlaveSelections", "List of slave selection components", true);		
	I_END_COMPONENT;
	
protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iprm::ISelectionParam, m_sourceSelectionCompPtr);
	I_REF(imod::IModel, m_sourceSelectionModelCompPtr);
	I_MULTIREF(iprm::ISelectionParam, m_slaveSelectionsCompPtr);
};


} // namespace iprm


#endif // !iprm_CSelectionDelegatorComp_included


