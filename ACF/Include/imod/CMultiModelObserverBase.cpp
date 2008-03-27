#include "imod/CMultiModelObserverBase.h"


#include <algorithm>


namespace imod
{


// public methods

CMultiModelObserverBase::CMultiModelObserverBase()
{
}


CMultiModelObserverBase::~CMultiModelObserverBase()
{
	EnsureDetached();
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


// reimplemented (IObserver)

bool CMultiModelObserverBase::OnAttached(imod::IModel* modelPtr)
{
	// check of model is already model connected:
	for (Models::iterator index = m_models.begin(); index != m_models.end(); index++){
		if (modelPtr == *index){
			return false;
		}
	}

	m_models.push_back(modelPtr);

	return true;
}


bool CMultiModelObserverBase::OnDetached(IModel* modelPtr)
{
	Models::iterator iter = ::std::find(m_models.begin(), m_models.end(), modelPtr);
	if (iter != m_models.end()){
		m_models.erase(iter);
		
		return true;
	}

	return false;
}


void CMultiModelObserverBase::BeforeUpdate(IModel* I_IF_DEBUG(modelPtr), int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	I_ASSERT(IsModelObserved(modelPtr));
}


void CMultiModelObserverBase::AfterUpdate(IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	I_ASSERT(IsModelObserved(modelPtr));

	if (IsModelObserved(modelPtr)){
		OnUpdate(modelPtr, updateFlags, updateParamsPtr);
	}
}


// protected methods

void CMultiModelObserverBase::EnsureDetached()
{
	for (Models::iterator index = m_models.begin(); index != m_models.end(); index++){
		imod::IModel* modelPtr = *index;
		I_ASSERT(modelPtr != NULL);
		if (modelPtr != NULL){
			modelPtr->DetachObserver(this);
		}
	}

	m_models.clear();
}


void CMultiModelObserverBase::OnUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
}


// private methods:

bool CMultiModelObserverBase::IsModelObserved(const imod::IModel* modelPtr) const
{
	for (Models::const_iterator index = m_models.begin(); index != m_models.end(); index++){
		IModel* referenceModelPtr = *index;

		if (referenceModelPtr == modelPtr){
			return true;
		}
	}

	return false;
}


} // namespace imod

