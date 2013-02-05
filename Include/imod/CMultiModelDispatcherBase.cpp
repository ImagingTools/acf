#include "imod/CMultiModelDispatcherBase.h"


// ACF includes
#include "imod/IModel.h"


namespace imod
{


bool CMultiModelDispatcherBase::RegisterModel(IModel* modelPtr, int modelId, int relevantFlags)
{
	ObserverProxyPtr& proxyPtr = m_modelMap[modelId];
	if (!proxyPtr.IsValid()){
		proxyPtr.SetPtr(new ObserverProxy(this, modelId, relevantFlags));
	}

	if (!modelPtr->AttachObserver(proxyPtr.GetPtr())){
		m_modelMap.remove(modelId);

		return false;
	}

	return true;
}


void CMultiModelDispatcherBase::UnregisterModel(int modelId)
{
	m_modelMap.remove(modelId);
}


void CMultiModelDispatcherBase::UnregisterAllModels()
{
	m_modelMap.clear();
}


// protected methods of the embedded class ObserverProxy

CMultiModelDispatcherBase::ObserverProxy::ObserverProxy(CMultiModelDispatcherBase* parentPtr, int modelId, int relevantFlags)
:	m_parent(*parentPtr),
	m_modelId(modelId),
	m_relevantFlags(relevantFlags)
{
	Q_ASSERT(parentPtr != NULL);
}


// reimplemented (imod::CMultiModelObserverBase)

void CMultiModelDispatcherBase::ObserverProxy::OnUpdate(int changeFlags, istd::IPolymorphic* updateParamsPtr)
{
	if ((m_relevantFlags != 0) && (m_relevantFlags & changeFlags) == 0){
		return;
	}
		
	m_parent.OnModelChanged(m_modelId, changeFlags, updateParamsPtr);
}


} // namespace imod


