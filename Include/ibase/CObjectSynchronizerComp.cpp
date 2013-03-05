#include "ibase/CObjectSynchronizerComp.h"


// ACF inlcudes
#include "imod/IModel.h"


namespace ibase
{


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CObjectSynchronizerComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	int slaveCounts = m_slaveObjectsCompPtr.GetCount();

	for (int slaveIndex = 0; slaveIndex < slaveCounts; slaveIndex++){
		istd::IChangeable* slaveObjectPtr = m_slaveObjectsCompPtr[slaveIndex];
		
		if ((slaveObjectPtr != NULL) && m_referenceObjectCompPtr.IsValid()){
			if (!slaveObjectPtr->IsEqual(*m_referenceObjectCompPtr)){
				slaveObjectPtr->CopyFrom(*m_referenceObjectCompPtr);
			}
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CObjectSynchronizerComp::OnComponentCreated()
{
	if (m_referenceObjectModelCompPtr.IsValid()){
		m_referenceObjectModelCompPtr->AttachObserver(this);
	}
	
	BaseClass::OnComponentCreated();
}


void CObjectSynchronizerComp::OnComponentDestroyed()
{
	EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace ibase


