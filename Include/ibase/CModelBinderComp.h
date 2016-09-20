#ifndef ibase_CModelBinderComp_included
#define ibase_CModelBinderComp_included


// ACF includes
#include <imod/IModel.h>
#include <imod/IObserver.h>
#include <icomp/CComponentBase.h>


namespace ibase
{


/**
	Implementation of a simple model-observer binder component.

	This component is used for direct binding of a model component, defined by \c m_modelCompPtr reference,
	to a list of observers defined \c m_observersCompPtr multi-reference. 

	\note Please note, that this component doesn't provide any public interfaces. The data object will be attached to the observers during the initialization phase of the component.
	To enforce the instatiation of this component, you should activate 'Automatically create instance' flag in the Compositor.

	\ingroup ModelObserver
*/
class CModelBinderComp: public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CModelBinderComp);
		I_ASSIGN(m_modelCompPtr, "Model", "Model to be connected", true, "Model");
		I_ASSIGN_MULTI_0(m_observersCompPtr, "Observers", "Observers for the model", true);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	/**
		Reference to the model component.
	*/
	I_REF(imod::IModel, m_modelCompPtr);

	/**
		Reference to the observer components.
	*/
	I_MULTIREF(imod::IObserver, m_observersCompPtr);
};


} // namespace ibase


#endif // !ibase_CModelBinderComp_included


