#include "iqtgui/CTabbedMultiViewGuiComp.h"


#include "imod/IModel.h"


namespace iqtgui
{


// public methods

// reimplemented (imod::IObserver)

bool CTabbedMultiViewGuiComp::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass2::OnAttached(modelPtr)){
		if (m_observersCompPtr.IsValid()){
			for (int index = 0; index < m_observersCompPtr.GetCount(); index++){
				modelPtr->AttachObserver(m_observersCompPtr[index]);
			}
		}

		return true;
	}

	return false;
}


bool CTabbedMultiViewGuiComp::OnDetached(imod::IModel* modelPtr)
{
	if (m_observersCompPtr.IsValid()){
		for (int index = 0; index < m_observersCompPtr.GetCount(); index++){
			if (modelPtr->IsAttached(m_observersCompPtr[index])){
				modelPtr->DetachObserver(m_observersCompPtr[index]);
			}
		}
	}

	return BaseClass2::OnDetached(modelPtr);
}


} // namespace iqtgui


