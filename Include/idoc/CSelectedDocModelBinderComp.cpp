#include "idoc/CSelectedDocModelBinderComp.h"


namespace idoc
{


CSelectedDocModelBinderComp::CSelectedDocModelBinderComp()
:	m_isActive(false)
{
	static const istd::IChangeable::ChangeSet changeMask(
				IDocumentManager::CF_DOCUMENT_CREATED,
				IDocumentManager::CF_DOCUMENT_REMOVED,
				IDocumentManager::CF_VIEW_ACTIVATION_CHANGED);
	SetObservedIds(changeMask);
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
		qDebug("CSelectedDocModelBinderComp::TryConnectObservers: Observer component was not set");

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

			if ((observerPtr != NULL) && m_modelCompPtr->AttachObserver(observerPtr)){
				m_connectedMap[observerPtr] = m_modelCompPtr.GetPtr();
			}
		}
	}
}


void CSelectedDocModelBinderComp::TryDisconnectObservers()
{
	for (		ModelObserverMap::ConstIterator iter = m_connectedMap.constBegin();
				iter != m_connectedMap.constEnd();
				++iter){
		imod::IModel* modelPtr = iter.value();
		Q_ASSERT(modelPtr != NULL);

		imod::IObserver* observerPtr = iter.key();
		Q_ASSERT(observerPtr != NULL);

		if (modelPtr->IsAttached(observerPtr)){
			modelPtr->DetachObserver(observerPtr);
		}
	}

	m_connectedMap.clear();

	m_isActive = false;
}


// reimplemented (imod::IObserver)

void CSelectedDocModelBinderComp::BeforeUpdate(imod::IModel* /*modelPtr*/)
{
	TryDisconnectObservers();
}


void CSelectedDocModelBinderComp::AfterUpdate(imod::IModel* /*modelPtr*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	TryConnectObservers();
}


// reimplemented (icomp::CComponentBase)

void CSelectedDocModelBinderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (		m_documentManagerCompPtr.IsValid() &&
				m_observedObjectCompPtr.IsValid() &&
				m_modelCompPtr.IsValid()){
		m_documentManagerModelCompPtr->AttachObserver(this);
	}

	TryConnectObservers();
}


void CSelectedDocModelBinderComp::OnComponentDestroyed()
{
	TryDisconnectObservers();

	m_isActive = true;

	if (m_documentManagerModelCompPtr.IsValid() && m_documentManagerModelCompPtr->IsAttached(this)){
		m_documentManagerModelCompPtr->DetachObserver(this);
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace idoc


