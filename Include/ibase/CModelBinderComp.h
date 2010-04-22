#ifndef ibase_CModelBinderComp_included
#define ibase_CModelBinderComp_included


#include "imod/IModel.h"
#include "imod/IObserver.h"

#include "icomp/CComponentBase.h"


namespace ibase
{


/**
	Implementation of a simple model-observer binder component.
*/
class CModelBinderComp: public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CModelBinderComp)
		I_ASSIGN(m_modelCompPtr, "Model", "Model to be connected", true, "Model")
		I_ASSIGN_MULTI_0(m_observersCompPtr, "Observers", "Observers for the model", true)
	I_END_COMPONENT

protected:
	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(imod::IModel, m_modelCompPtr);
	I_MULTIREF(imod::IObserver, m_observersCompPtr);
};


} // namespace ibase


#endif // !ibase_CModelBinderComp_included


