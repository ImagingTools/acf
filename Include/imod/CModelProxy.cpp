#include "imod/CModelProxy.h"


namespace imod
{


// public methods

CModelProxy::CModelProxy()
	:m_modelObserver(*this),
	m_modelPtr(NULL)
{
}


void CModelProxy::SetModelPtr(imod::IModel* modelPtr)
{
	if (m_modelPtr == modelPtr){
		return;
	}

	if (m_modelPtr != NULL){
		DetachProxyObservers();

		Q_ASSERT(m_modelPtr->IsAttached(&m_modelObserver));
		m_modelPtr->DetachObserver(&m_modelObserver);
	}

	if (modelPtr != NULL && modelPtr->AttachObserver(&m_modelObserver)){
		m_modelPtr = modelPtr;

		AttachProxyObservers();
	}
}


void CModelProxy::ResetModel()
{
	m_modelPtr = NULL;
}


// reimplemented (imod::IModel)

bool CModelProxy::AttachObserver(IObserver* observerPtr)
{
	Q_ASSERT(observerPtr != NULL);
	if (observerPtr == NULL){
		return false;
	}

	// get the current model:
	if (m_modelPtr != NULL){
		// try to connect the observer to the model. 
		// If successfull, we put the observer into the list of connected observers for this model.
		bool retVal = m_modelPtr->AttachObserver(observerPtr);
		if (retVal){
			m_proxyObservers.push_back(Observer(observerPtr, false));
		}

		return retVal;
	}
	else{
		// The model is not present, so we put the observer into the list and mark it to be pending.
		m_proxyObservers.push_back(Observer(observerPtr, true));
	}

	return true;
}


void CModelProxy::DetachObserver(IObserver* observerPtr)
{
	if (m_modelPtr != NULL){
		m_modelPtr->DetachObserver(observerPtr);
	}

	m_proxyObservers.erase(qFind(m_proxyObservers.begin(), m_proxyObservers.end(), observerPtr));
}


void CModelProxy::DetachAllObservers()
{
	DetachProxyObservers();

	m_proxyObservers.clear();
}


bool CModelProxy::IsAttached(const IObserver* observerPtr) const
{
	if (m_modelPtr != NULL){
		return m_modelPtr->IsAttached(observerPtr);
	}

	Observers::const_iterator waitingIter = qFind(m_proxyObservers.begin(), m_proxyObservers.end(), observerPtr);
	if (waitingIter != m_proxyObservers.end()){
		return true;
	}

	return false;
}


// private methods

void CModelProxy::AttachProxyObservers()
{
	if (m_modelPtr == NULL){
		return;
	}

	for (		Observers::iterator index = m_proxyObservers.begin();
				index != m_proxyObservers.end();
				index++){
		imod::IObserver* observerPtr = (*index).m_observerPtr;
		if (!m_modelPtr->IsAttached(observerPtr)){
			if (m_modelPtr->AttachObserver(observerPtr)){
				(*index).m_isPending = false;
			}
		}
	}
}


void CModelProxy::DetachProxyObservers()
{
	imod::IModel* modelPtr = m_modelObserver.GetModelPtr();
	if (modelPtr == NULL){
		return;
	}

	for (		Observers::iterator index = m_proxyObservers.begin();
				index != m_proxyObservers.end();
				index++){
		imod::IObserver* observerPtr = (*index).m_observerPtr;

		if (modelPtr->IsAttached(observerPtr)){
			modelPtr->DetachObserver(observerPtr);
		}

		(*index).m_isPending = true;
	}
}


CModelProxy::ModelObserver::ModelObserver(CModelProxy& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)

bool CModelProxy::ModelObserver::OnDetached(imod::IModel* modelPtr)
{
	Q_ASSERT(modelPtr == m_parent.m_modelPtr);

	if (modelPtr == m_parent.m_modelPtr){
		m_parent.ResetModel();
	}

	return BaseClass::OnDetached(modelPtr);
}


} // namespace imod


