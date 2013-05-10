#ifndef ibase_TModelObserverCompWrap_included
#define ibase_TModelObserverCompWrap_included


#include "imod/IModel.h"
#include "imod/TSingleModelObserverBase.h"

#include "ilog/TLoggerCompWrap.h"


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

	I_BEGIN_BASE_COMPONENT(TModelObserverCompWrap);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN(m_defaultModelCompPtr, "DefaultModel", "Data model to be connected to the observer", false, "DefaultModel");
		I_TASSIGN_TO(m_defaultObjectCompPtr, m_defaultModelCompPtr, false);
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

	I_IF_DEBUG(
		if (m_defaultModelCompPtr.IsValid() && !m_defaultObjectCompPtr.IsValid()){
			const icomp::IComponentContext* contextPtr = BaseClass::GetComponentContext();
			QByteArray observerComponentId = "(unidentified)";
			if (contextPtr != NULL){
				const icomp::CComponentContext* extContextPtr = dynamic_cast<const icomp::CComponentContext*>(contextPtr);
				if (extContextPtr != NULL){
					observerComponentId = extContextPtr->GetCompleteContextId();
				}
				else{
					observerComponentId = contextPtr->GetContextId();
				}
			}

			QByteArray modelComponentId = "(unidentified)";

			icomp::IComponent* modelComponentPtr = dynamic_cast<icomp::IComponent*>(m_defaultModelCompPtr.GetPtr());
			if (modelComponentPtr != NULL){
				const icomp::IComponentContext* modelContextPtr = modelComponentPtr->GetComponentContext();
				if (modelContextPtr != NULL){
					const icomp::CComponentContext* extModelContextPtr = dynamic_cast<const icomp::CComponentContext*>(modelContextPtr);
					if (extModelContextPtr != NULL){
						modelComponentId = extModelContextPtr->GetCompleteContextId();
					}
					else{
						modelComponentId = modelContextPtr->GetContextId();
					}
				}
			}

			QString exptectedObjectInterface = typeid(typename ObserverComponent::ModelType).name();

			qDebug("Component %s: Component model %s doesn't implement observed interface %s",
						qPrintable(observerComponentId),
						qPrintable(modelComponentId),
						qPrintable(exptectedObjectInterface));
		}
	)

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


