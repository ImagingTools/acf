#pragma once


// Qt includes
#include <QtCore/QDebug>

// ACF includes
#include <imod/IModel.h>
#include <imod/TSingleModelObserverBase.h>
#include <ilog/TLoggerCompWrap.h>


namespace ibase
{


/**
	Generic basic implementation of component wrapper for model observer classes.

	The model can be connected on component level during the initialization phase using component reference \c m_defaultModelCompPtr

	\ingroup DataModel
*/
template <class ObserverComponent>
class TModelObserverCompBaseWrap: public ObserverComponent
{
public:
	typedef ObserverComponent BaseClass;

	I_BEGIN_BASE_COMPONENT(TModelObserverCompBaseWrap);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN(m_defaultModelCompPtr, "DefaultModel", "Data model to be connected to the observer", false, "DefaultModel");
		I_TASSIGN_TO(m_defaultObjectCompPtr, m_defaultModelCompPtr, false);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	I_REF(imod::IModel, m_defaultModelCompPtr);
	I_TREF(typename ObserverComponent::ModelType, m_defaultObjectCompPtr);
};


// protected methods

// reimplemented (icomp::CComponentBase)

template <class ObserverComponent>
void TModelObserverCompBaseWrap<ObserverComponent>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_IF_DEBUG(
		if (m_defaultModelCompPtr.IsValid() && !m_defaultObjectCompPtr.IsValid()){
			const icomp::IComponentContextSharedPtr contextPtr = BaseClass::GetComponentContext();
			QByteArray observerComponentId = "(unidentified)";
			if (contextPtr != nullptr){
				const icomp::CComponentContext* extContextPtr = dynamic_cast<const icomp::CComponentContext*>(contextPtr.get());
				if (extContextPtr != nullptr){
					observerComponentId = extContextPtr->GetCompleteContextId();
				}
				else{
					observerComponentId = contextPtr->GetContextId();
				}
			}

			QByteArray modelComponentId = "(unidentified)";

			icomp::IComponent* modelComponentPtr = dynamic_cast<icomp::IComponent*>(m_defaultModelCompPtr.GetPtr());
			if (modelComponentPtr != nullptr){
				const icomp::IComponentContextSharedPtr modelContextPtr = modelComponentPtr->GetComponentContext();
				if (modelContextPtr != nullptr){
					const icomp::CComponentContext* extModelContextPtr = dynamic_cast<const icomp::CComponentContext*>(modelContextPtr.get());
					if (extModelContextPtr != nullptr){
						modelComponentId = extModelContextPtr->GetCompleteContextId();
					}
					else{
						modelComponentId = modelContextPtr->GetContextId();
					}
				}
			}

			QString exptectedObjectInterface = typeid(typename ObserverComponent::ModelType).name();

			qDebug() << "Component " << observerComponentId << " Component model " << modelComponentId << " doesn't implement observed interface " << exptectedObjectInterface;
		}
	)

	if (m_defaultModelCompPtr.IsValid() && m_defaultObjectCompPtr.IsValid()){
		m_defaultModelCompPtr->AttachObserver(this);
	}
}


template <class ObserverComponent>
void TModelObserverCompBaseWrap<ObserverComponent>::OnComponentDestroyed()
{
	if (m_defaultModelCompPtr.IsValid() && m_defaultModelCompPtr->IsAttached(this)){
		m_defaultModelCompPtr->DetachObserver(this);
	}

	BaseClass::OnComponentDestroyed();
}


/**
	Final implementation of the component-based observer wrapper.
	In contrast to the TModelObserverCompBaseWrap it can be used also as outer-most-wrapper (e.g. by component registration in the package).
*/
template <class BaseComponent>
class TModelObserverCompWrap : public TModelObserverCompBaseWrap<BaseComponent>
{
public:
	typedef TModelObserverCompBaseWrap<BaseComponent> BaseClass;

	I_BEGIN_COMPONENT(TModelObserverCompWrap);
	I_END_COMPONENT;
};


} // namespace ibase

