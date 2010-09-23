#include "idoc/CSelectedDocModelBinderComp.h"


namespace idoc
{


CSelectedDocModelBinderComp::CSelectedDocModelBinderComp()
:	m_isActive(false)
{
}


// protected methods

void CSelectedDocModelBinderComp::TryConnectObservers()
{
	if (m_isActive){
		return;
	}

	if (!m_documentManagerCompPtr.IsValid()){
		return;
	}

	const icomp::IComponent* observedObjectPtr = m_observedObjectCompPtr.GetPtr();
	if (observedObjectPtr == NULL){
		return;
	}

	const istd::IPolymorphic* viewPtr = m_documentManagerCompPtr->GetActiveView();
	if (viewPtr == NULL){
		return;
	}

	const icomp::IComponent* viewCompPtr = dynamic_cast<const icomp::IComponent*>(viewPtr);
	const icomp::IComponent* documentCompPtr = dynamic_cast<const icomp::IComponent*>(m_documentManagerCompPtr->GetDocumentFromView(*viewPtr));
	m_isActive =
				((viewCompPtr != NULL) && (observedObjectPtr == viewCompPtr)) ||
				((documentCompPtr != NULL) && (observedObjectPtr == documentCompPtr));

	if (m_isActive){
		int observersCount = m_observersCompPtr.GetCount();
		for (int i = 0; i < observersCount; ++i){
			imod::IObserver* observerPtr = m_observersCompPtr[i];

			if (observerPtr != NULL){
				m_modelCompPtr->AttachObserver(observerPtr);
			}
		}
	}
}


void CSelectedDocModelBinderComp::TryDisconnectObservers()
{
	if (m_isActive){
		int observersCount = m_observersCompPtr.GetCount();
		for (int i = 0; i < observersCount; ++i){
			imod::IObserver* observerPtr = m_observersCompPtr[i];

			if ((observerPtr != NULL) && m_modelCompPtr->IsAttached(observerPtr)){
				m_modelCompPtr->DetachObserver(observerPtr);
			}
		}

		m_isActive = false;
	}
}


// reimplemented (icomp::IComponent)

void CSelectedDocModelBinderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_documentManagerModelCompPtr.IsValid()){
		m_documentManagerModelCompPtr->AttachObserver(this);
	}

	TryConnectObservers();
}


void CSelectedDocModelBinderComp::OnComponentDestroyed()
{
	if (m_documentManagerModelCompPtr.IsValid() && m_documentManagerModelCompPtr->IsAttached(this)){
		m_documentManagerModelCompPtr->DetachObserver(this);
	}

	TryDisconnectObservers();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::IObserver)

void CSelectedDocModelBinderComp::BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	BaseClass2::BeforeUpdate(modelPtr, updateFlags, updateParamsPtr);

	if (		m_documentManagerCompPtr.IsValid() &&
				m_observedObjectCompPtr.IsValid() &&
				m_modelCompPtr.IsValid() &&
				((updateFlags & RELEVANT_FLAGS) != 0)){
		TryDisconnectObservers();
	}
}


void CSelectedDocModelBinderComp::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if (		m_documentManagerCompPtr.IsValid() &&
				m_observedObjectCompPtr.IsValid() &&
				m_modelCompPtr.IsValid() &&
				((updateFlags & RELEVANT_FLAGS) != 0)){
		TryConnectObservers();
	}

	BaseClass2::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


} // namespace idoc


