#include "iprm/CSelectionDelegatorComp.h"


// ACF inlcudes
#include "imod/IModel.h"


namespace iprm
{


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CSelectionDelegatorComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	int slaveCounts = m_slaveSelectionsCompPtr.GetCount();

	for (int slaveIndex = 0; slaveIndex < slaveCounts; slaveIndex++){
		iprm::ISelectionParam* selectionPtr = m_slaveSelectionsCompPtr[slaveIndex];
		if (selectionPtr != NULL){
			selectionPtr->SetSelectedOptionIndex(m_sourceSelectionCompPtr->GetSelectedOptionIndex());
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CSelectionDelegatorComp::OnComponentCreated()
{
	if (m_sourceSelectionModelCompPtr.IsValid()){
		m_sourceSelectionModelCompPtr->AttachObserver(this);
	}
	
	BaseClass::OnComponentCreated();
}


void CSelectionDelegatorComp::OnComponentDestroyed()
{
	EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace iprm


