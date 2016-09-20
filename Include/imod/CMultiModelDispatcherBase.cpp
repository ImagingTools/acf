#include <imod/CMultiModelDispatcherBase.h>


// ACF includes
#include <imod/IModel.h>


namespace imod
{


bool CMultiModelDispatcherBase::RegisterModel(IModel* modelPtr, int modelId, const istd::IChangeable::ChangeSet& relevantFlags)
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

CMultiModelDispatcherBase::ObserverProxy::ObserverProxy(CMultiModelDispatcherBase* parentPtr, int modelId, const istd::IChangeable::ChangeSet& relevantFlags)
:	m_parent(*parentPtr),
	m_modelId(modelId)
{
	Q_ASSERT(parentPtr != NULL);

	SetObservedIds(relevantFlags);
}


// reimplemented (imod::CMultiModelObserverBase)

void CMultiModelDispatcherBase::ObserverProxy::OnUpdate(const istd::IChangeable::ChangeSet& changeSet)
{
	m_parent.OnModelChanged(m_modelId, changeSet);
}


} // namespace imod


