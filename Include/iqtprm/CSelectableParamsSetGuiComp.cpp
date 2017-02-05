#include <iqtprm/CSelectableParamsSetGuiComp.h>


// ACF includes
#include <iprm/IParamsManager.h>


namespace iqtprm
{


// public methods

CSelectableParamsSetGuiComp::CSelectableParamsSetGuiComp()
	:m_currentParamsSetIndex(-1)
{
	static const istd::IChangeable::ChangeSet changeMask(iprm::ISelectionParam::CF_SELECTION_CHANGED);
	SetObservedIds(changeMask);
}


// reimplemented (imod::CSingleModelObserverBase)

void CSelectableParamsSetGuiComp::BeforeUpdate(imod::IModel* modelPtr)
{
	EnsureDetachLastEditor();

	BaseClass::BeforeUpdate(modelPtr);
}


void CSelectableParamsSetGuiComp::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::AfterUpdate(modelPtr, changeSet);

	ConnectCurrentEditor();
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

	iprm::IParamsSet* paramsSetPtr = ExtractParamsSetPtr(GetObservedModel());

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
	iprm::ISelectionParam* selectionPtr = GetObservedObject();
	if (selectionPtr != NULL){
		int selectedIndex = selectionPtr->GetSelectedOptionIndex();

		if (selectedIndex != m_currentParamsSetIndex){
			EnsureDetachLastEditor();
	
			m_currentParamsSetIndex = selectedIndex;
			
			if (m_slaveObserversCompPtr.IsValid()){	
				int observersCount = m_slaveObserversCompPtr.GetCount();

				int observerIndex = qMin(m_currentParamsSetIndex, observersCount - 1);

				iprm::IParamsSet* paramsSetPtr = ExtractParamsSetPtr(GetObservedModel());
				if (paramsSetPtr != NULL && observerIndex >= 0){
					imod::IObserver* observerPtr = m_slaveObserversCompPtr[observerIndex];
					imod::IModel* paramsSetModelPtr = dynamic_cast<imod::IModel*>(paramsSetPtr);
					if (paramsSetModelPtr != NULL && observerPtr != NULL){
						paramsSetModelPtr->AttachObserver(observerPtr);
					}
				}
			}
		}
	}
}


// private methods

iprm::IParamsSet* CSelectableParamsSetGuiComp::ExtractParamsSetPtr(imod::IModel* modelPtr) const
{
	iprm::IParamsManager* paramsManagerPtr = dynamic_cast<iprm::IParamsManager*>(modelPtr);
	if ((paramsManagerPtr != NULL) && (m_currentParamsSetIndex >= 0)){
		return paramsManagerPtr->GetParamsSet(m_currentParamsSetIndex);
	}

	return NULL;
}


} // namespace iqtprm


