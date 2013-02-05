#include "ibase/CMultiObserverBinderComp.h"


namespace ibase
{


CMultiObserverBinderComp::CMultiObserverBinderComp()
:	m_attachedModelPtr(NULL)
{
}


// public methods

bool CMultiObserverBinderComp::IsModelAttached(const imod::IModel* modelPtr) const
{
	return (modelPtr == m_attachedModelPtr);
}


bool CMultiObserverBinderComp::OnAttached(imod::IModel* modelPtr)
{
	Q_ASSERT(m_attachedModelPtr == NULL);

	bool retVal = true;

	int count = m_observersCompPtr.GetCount();
	for (int i = 0; i < count; ++i){
		imod::IObserver* observerPtr = m_observersCompPtr[i];
		if (observerPtr != NULL){
			retVal = modelPtr->AttachObserver(observerPtr) || retVal;
		}
	}

	if (retVal){
		m_attachedModelPtr = modelPtr;
	}

	return retVal;
}


bool CMultiObserverBinderComp::OnDetached(imod::IModel* modelPtr)
{
	Q_ASSERT(m_attachedModelPtr == modelPtr);

	int count = m_observersCompPtr.GetCount();
	for (int i = 0; i < count; ++i){
		imod::IObserver* observerPtr = m_observersCompPtr[i];
		if ((observerPtr != NULL) && observerPtr->IsModelAttached(modelPtr)){
			modelPtr->DetachObserver(observerPtr);
		}
	}

	m_attachedModelPtr = NULL;

	return true;
}


void CMultiObserverBinderComp::BeforeUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
}


void CMultiObserverBinderComp::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
}


void CMultiObserverBinderComp::UpdateEditor(int updateFlags)
{
	int count = m_modelEditorsCompPtr.GetCount();
	for (int i = 0; i < count; ++i){
		imod::IModelEditor* editorPtr = m_modelEditorsCompPtr[i];
		if (editorPtr != NULL){
			editorPtr->UpdateEditor(updateFlags);
		}
	}
}


void CMultiObserverBinderComp::UpdateModel() const
{
	int count = m_modelEditorsCompPtr.GetCount();
	for (int i = 0; i < count; ++i){
		imod::IModelEditor* editorPtr = m_modelEditorsCompPtr[i];
		if (editorPtr != NULL){
			editorPtr->UpdateModel();
		}
	}
}


bool CMultiObserverBinderComp::IsReadOnly() const
{
	int count = m_modelEditorsCompPtr.GetCount();
	for (int i = 0; i < count; ++i){
		imod::IModelEditor* editorPtr = m_modelEditorsCompPtr[i];
		if (editorPtr != NULL){
			if (!editorPtr->IsReadOnly()){
				return false;
			}
		}
	}

	return true;
}


void CMultiObserverBinderComp::SetReadOnly(bool state)
{
	int count = m_modelEditorsCompPtr.GetCount();
	for (int i = 0; i < count; ++i){
		imod::IModelEditor* editorPtr = m_modelEditorsCompPtr[i];
		if (editorPtr != NULL){
			editorPtr->SetReadOnly(state);
		}
	}
}


} // namespace ibase



