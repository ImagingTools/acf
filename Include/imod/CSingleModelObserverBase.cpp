#include "imod/CSingleModelObserverBase.h"


// ACF includes
#include "imod/IModel.h"
#include "imod/IUndoManager.h"


namespace imod
{


CSingleModelObserverBase::CSingleModelObserverBase()
{
	m_modelPtr = NULL;
}


CSingleModelObserverBase::~CSingleModelObserverBase()
{
	EnsureDetached();
}


// reimplemented (IObserver)

bool CSingleModelObserverBase::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(modelPtr != NULL);
	I_ASSERT(m_modelPtr == NULL);

	BeforeUpdate(NULL, IUndoManager::CF_NO_UNDO, NULL);

	m_modelPtr = modelPtr;

	AfterUpdate(m_modelPtr, IUndoManager::CF_NO_UNDO, NULL);

	return true;
}


bool CSingleModelObserverBase::OnDetached(imod::IModel* modelPtr)
{
	I_ASSERT(modelPtr != NULL);
	I_ASSERT(m_modelPtr == modelPtr);

	if (m_modelPtr == modelPtr){		
		m_modelPtr = NULL;

		return true;
	}

	return false;
}


void CSingleModelObserverBase::BeforeUpdate(
			imod::IModel* I_IF_DEBUG(modelPtr),
			int /*updateFlags*/,
			istd::IPolymorphic* /*updateParamsPtr*/)
{
	I_ASSERT(m_modelPtr == modelPtr);
}


void CSingleModelObserverBase::AfterUpdate(
			imod::IModel* I_IF_DEBUG(modelPtr),
			int updateFlags,
			istd::IPolymorphic* updateParamsPtr)
{
	I_ASSERT(m_modelPtr == modelPtr);

	OnUpdate(updateFlags, updateParamsPtr);
}


// protected methods

void CSingleModelObserverBase::EnsureDetached()
{
	if (m_modelPtr != NULL){
		m_modelPtr->DetachObserver(this);
	}

	m_modelPtr = NULL;
}


void CSingleModelObserverBase::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
}


} // namespace imod

