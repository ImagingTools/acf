#include "ibase/CModelBinderComp.h"


namespace ibase
{


// protected methods

// reimplemented (icomp::IComponent)

void CModelBinderComp::OnComponentCreated()
{
	if (m_modelCompPtr.IsValid() && m_observersCompPtr.IsValid()){
		int observersCount = m_observersCompPtr.GetCount();
		for (int observerIndex = 0; observerIndex < observersCount; observerIndex++){
			I_ASSERT(m_observersCompPtr[observerIndex] != NULL);

			if (!m_modelCompPtr->IsAttached(m_observersCompPtr[observerIndex])){
				m_modelCompPtr->AttachObserver(m_observersCompPtr[observerIndex]);
			}
		}
	}
}


void CModelBinderComp::OnComponentDestroyed()
{
	if (m_modelCompPtr.IsValid() && m_observersCompPtr.IsValid()){
		int observersCount = m_observersCompPtr.GetCount();
		for (int observerIndex = 0; observerIndex < observersCount; observerIndex++){
			I_ASSERT(m_observersCompPtr[observerIndex] != NULL);

			if (m_modelCompPtr->IsAttached(m_observersCompPtr[observerIndex])){
				m_modelCompPtr->DetachObserver(m_observersCompPtr[observerIndex]);
			}
		}
	}
}


} // namespace ibase


