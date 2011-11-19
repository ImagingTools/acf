#ifndef ibase_TModelObserverCompWrap_included
#define ibase_TModelObserverCompWrap_included


#include "imod/IModel.h"
#include "imod/TSingleModelObserverBase.h"

#include "ibase/TLoggerCompWrap.h"


namespace ibase
{


/**
	Implementation of component wrapper for model observer classes.
*/
template <class ObserverComponent>
class TModelObserverCompWrap: public ObserverComponent
{
public:
	typedef ObserverComponent BaseClass;

	I_BEGIN_COMPONENT(TModelObserverCompWrap);
		I_ASSIGN(m_defaultModelCompPtr, "DefaultModel", "Data model to be connected to the observer", false, "DefaultModel");
		I_ASSIGN_TO(m_defaultObjectCompPtr, m_defaultModelCompPtr, false);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(imod::IModel, m_defaultModelCompPtr);
	I_TREF(typename ObserverComponent::ModelType, m_defaultObjectCompPtr);
};


// protected methods

// reimplemented (icomp::CComponentBase)

template <class ObserverComponent>
void TModelObserverCompWrap<ObserverComponent>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultModelCompPtr.IsValid() && m_defaultObjectCompPtr.IsValid()){
		m_defaultModelCompPtr->AttachObserver(this);
	}
}


template <class ObserverComponent>
void TModelObserverCompWrap<ObserverComponent>::OnComponentDestroyed()
{
	if (m_defaultModelCompPtr.IsValid() && m_defaultModelCompPtr->IsAttached(this)){
		m_defaultModelCompPtr->DetachObserver(this);
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace ibase


#endif // !ibase_TModelObserverCompWrap_included


