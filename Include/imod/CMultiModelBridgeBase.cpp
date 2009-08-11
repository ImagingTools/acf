#include "imod/CMultiModelBridgeBase.h"


// STL includes
#include <algorithm>


namespace imod
{


// public methods

CMultiModelBridgeBase::~CMultiModelBridgeBase()
{
	CMultiModelBridgeBase::EnsureDetached();
}


IModel* CMultiModelBridgeBase::GetModelPtr(int modelIndex) const
{
	I_ASSERT(modelIndex >= 0)
	I_ASSERT(modelIndex < GetModelCount());

	return m_models.at(modelIndex);
}


int CMultiModelBridgeBase::GetModelCount() const
{
	return int(m_models.size());
}


// reimplemented (imod::IObserver)

bool CMultiModelBridgeBase::IsModelAttached(const imod::IModel* modelPtr) const
{
	if (modelPtr == NULL){
		return !m_models.empty();
	}

	Models::const_iterator foundIter = std::find(m_models.begin(), m_models.end(), modelPtr);

	return (foundIter != m_models.end());
}


bool CMultiModelBridgeBase::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(modelPtr != NULL);

	if (IsModelAttached(modelPtr)){
		return false;
	}

	m_models.push_back(modelPtr);

	return true;
}


bool CMultiModelBridgeBase::OnDetached(IModel* modelPtr)
{
	Models::iterator iter = std::find(m_models.begin(), m_models.end(), modelPtr);
	if (iter != m_models.end()){
		m_models.erase(iter);
		
		return true;
	}

	return false;
}


void CMultiModelBridgeBase::BeforeUpdate(IModel* I_IF_DEBUG(modelPtr), int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	I_ASSERT(IsModelAttached(modelPtr));

	BeginChanges(updateFlags, updateParamsPtr);
}


void CMultiModelBridgeBase::AfterUpdate(IModel* I_IF_DEBUG(modelPtr), int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	I_ASSERT(IsModelAttached(modelPtr));

	EndChanges(updateFlags, updateParamsPtr);
}


// protected methods

void CMultiModelBridgeBase::EnsureDetached()
{
	while (!m_models.empty()){
		imod::IModel* modelPtr = m_models.front();
		I_ASSERT(modelPtr != NULL);

		modelPtr->DetachObserver(this);
	}
}


} // namespace imod

