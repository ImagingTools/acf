#ifndef ibase_TModelObserverCompWrap_included
#define ibase_TModelObserverCompWrap_included


#include "imod/IModel.h"
#include "imod/TSingleModelObserverBase.h"

#include "ibase/TLoggerCompWrap.h"


namespace ibase
{


/**
	Basic implementation for a single model observer component.
*/
template <class ObjectInterface, class ObserverImpl>
class TModelObserverCompWrap: public ObserverImpl
{
public:
	typedef ObserverImpl BaseClass;
	typedef ObjectInterface ModelInterface;

	I_BEGIN_COMPONENT(TModelObserverCompWrap);
		I_ASSIGN(m_modelCompPtr, "DefaultModel", "Data model to be connected to the observer", false, "DefaultModel");
		I_ASSIGN_TO(m_objectCompPtr, m_modelCompPtr, false);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(imod::IModel, m_modelCompPtr);
	I_TREF(ModelInterface, m_objectCompPtr);
};


// protected methods

// reimplemented (icomp::CComponentBase)

template <class ObjectInterface, class ObserverImpl>
void TModelObserverCompWrap<ObjectInterface, ObserverImpl>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_modelCompPtr.IsValid() && m_objectCompPtr.IsValid()){
		m_modelCompPtr->AttachObserver(this);
	}
}


template <class ObjectInterface, class ObserverImpl>
void TModelObserverCompWrap<ObjectInterface, ObserverImpl>::OnComponentDestroyed()
{
	if (m_modelCompPtr.IsValid()){
		if (m_modelCompPtr->IsAttached(this)){
			m_modelCompPtr->DetachObserver(this);
		}
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace ibase


#endif // !ibase_TModelObserverCompWrap_included


