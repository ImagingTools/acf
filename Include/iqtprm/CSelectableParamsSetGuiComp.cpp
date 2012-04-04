#include "iqtprm/CSelectableParamsSetGuiComp.h"


// ACF includes
#include "iprm/ISelectionConstraints.h"
#include "iprm/IParamsSet.h"


namespace iqtprm
{


// public methods

CSelectableParamsSetGuiComp::CSelectableParamsSetGuiComp()
	:m_currentParamsSetIndex(-1)
{
}


// reimplemented (imod::CSingleModelObserverBase)

void CSelectableParamsSetGuiComp::BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if ((updateFlags & iprm::ISelectionParam::CF_SELECTION_CHANGED) != 0){
		EnsureDetachLastEditor();
	}

	BaseClass::BeforeUpdate(modelPtr, updateFlags, updateParamsPtr);
}

void CSelectableParamsSetGuiComp::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if ((updateFlags & iprm::ISelectionParam::CF_SELECTION_CHANGED) != 0){
		ConnectCurrentEditor();
	}

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


// protected methods

// reimplemented (iqtgui::TModelObserverWrap)

void CSelectableParamsSetGuiComp::OnGuiModelAttached()
{
	ConnectCurrentEditor();

	BaseClass::OnGuiModelAttached();
}


void CSelectableParamsSetGuiComp::OnGuiModelDetached()
{
	EnsureDetachLastEditor();

	BaseClass::OnGuiModelDetached();
}


// private methods

void CSelectableParamsSetGuiComp::EnsureDetachLastEditor()
{
	int observersCount = m_slaveObserversCompPtr.GetCount();

	int observerIndex = qMin(m_currentParamsSetIndex, observersCount - 1);

	iprm::IParamsSet* paramsSetPtr = dynamic_cast<iprm::IParamsSet*>(GetObjectPtr());
	if (paramsSetPtr != NULL && observerIndex >= 0){
		imod::IObserver* observerPtr = m_slaveObserversCompPtr[observerIndex];
		imod::IModel* paramsSetModelPtr = dynamic_cast<imod::IModel*>(paramsSetPtr);
		if (		(paramsSetModelPtr != NULL) &&
					(observerPtr != NULL) &&
					paramsSetModelPtr->IsAttached(observerPtr)){
			paramsSetModelPtr->DetachObserver(observerPtr);
		}
	}

	m_currentParamsSetIndex = -1;
}


void CSelectableParamsSetGuiComp::ConnectCurrentEditor()
{
	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	if (selectionPtr != NULL){
		int selectedIndex = selectionPtr->GetSelectedOptionIndex();

		if (selectedIndex != m_currentParamsSetIndex){
			EnsureDetachLastEditor();
	
			m_currentParamsSetIndex = selectedIndex;
			
			if (m_slaveObserversCompPtr.IsValid()){	
				int observersCount = m_slaveObserversCompPtr.GetCount();

				int observerIndex = qMin(m_currentParamsSetIndex, observersCount - 1);

				iprm::IParamsSet* paramsSetPtr = dynamic_cast<iprm::IParamsSet*>(selectionPtr);
				if (paramsSetPtr != NULL && observerIndex >= 0){
					imod::IObserver* observerPtr = m_slaveObserversCompPtr[observerIndex];
					imod::IModel* paramsSetModelPtr = dynamic_cast<imod::IModel*>(paramsSetPtr);
					if(paramsSetModelPtr != NULL && observerPtr != NULL){
						paramsSetModelPtr->AttachObserver(observerPtr);
					}
				}	
			}
		}
	}
}


} // namespace iqtprm


