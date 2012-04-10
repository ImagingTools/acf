#include "imod/CMultiModelObserverBase.h"


namespace imod
{


// public methods

CMultiModelObserverBase::~CMultiModelObserverBase()
{
	CMultiModelObserverBase::EnsureModelsDetached();
}


IModel* CMultiModelObserverBase::GetModelPtr(int modelIndex) const
{
	I_ASSERT(modelIndex >= 0)
	I_ASSERT(modelIndex < GetModelCount());

	return m_models.at(modelIndex);
}


int CMultiModelObserverBase::GetModelCount() const
{
	return int(m_models.size());
}


// reimplemented (imod::IObserver)

bool CMultiModelObserverBase::IsModelAttached(const imod::IModel* modelPtr) const
{
	if (modelPtr == NULL){
		return !m_models.isEmpty();
	}

	Models::const_iterator foundIter = qFind(m_models.begin(), m_models.end(), modelPtr);

	return (foundIter != m_models.end());
}


bool CMultiModelObserverBase::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(modelPtr != NULL);

	if (IsModelAttached(modelPtr)){
		return false;
	}

	m_models.push_back(modelPtr);

	return true;
}


bool CMultiModelObserverBase::OnDetached(IModel* modelPtr)
{
	Models::iterator iter = qFind(m_models.begin(), m_models.end(), modelPtr);
	if (iter != m_models.end()){
		m_models.erase(iter);
		
		return true;
	}

	return false;
}


void CMultiModelObserverBase::BeforeUpdate(IModel* I_IF_DEBUG(modelPtr), int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	I_ASSERT(IsModelAttached(modelPtr));
}


void CMultiModelObserverBase::AfterUpdate(IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	I_ASSERT(IsModelAttached(modelPtr));

	if (IsModelAttached(modelPtr)){
		OnUpdate(modelPtr, updateFlags, updateParamsPtr);
	}
}


// protected methods

void CMultiModelObserverBase::EnsureModelsDetached()
{
	while (!m_models.isEmpty()){
		imod::IModel* modelPtr = m_models.front();
		I_ASSERT(modelPtr != NULL);

		modelPtr->DetachObserver(this);
	}
}


void CMultiModelObserverBase::OnUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
}


} // namespace imod

