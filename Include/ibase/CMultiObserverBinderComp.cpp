#include <ibase/CMultiObserverBinderComp.h>


namespace ibase
{


CMultiObserverBinderComp::CMultiObserverBinderComp()
	:m_attachedModelPtr(NULL)
{
}



// public methods

bool CMultiObserverBinderComp::IsModelAttached(const imod::IModel* modelPtr) const
{
	return (modelPtr == m_attachedModelPtr);
}


bool CMultiObserverBinderComp::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(m_attachedModelPtr == NULL);

	bool retVal = false;

	int count = m_observersCompPtr.GetCount();
	for (int i = 0; i < count; ++i){
		imod::IObserver* observerPtr = m_observersCompPtr[i];
		if (observerPtr != NULL){
			if (modelPtr->AttachObserver(observerPtr)){
				retVal = true;
			}
		}
	}

	if (retVal){
		changeMask = istd::IChangeable::GetAllChanges();
		m_attachedModelPtr = modelPtr;
	}

	return retVal;
}


bool CMultiObserverBinderComp::OnModelDetached(imod::IModel* modelPtr)
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


void CMultiObserverBinderComp::BeforeUpdate(imod::IModel* /*modelPtr*/)
{
}


void CMultiObserverBinderComp::AfterUpdate(imod::IModel* /*modelPtr*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
}


void CMultiObserverBinderComp::UpdateEditor(const istd::IChangeable::ChangeSet& changeSet)
{
	int count = m_modelEditorsCompPtr.GetCount();
	for (int i = 0; i < count; ++i){
		imod::IModelEditor* editorPtr = m_modelEditorsCompPtr[i];
		if (editorPtr != NULL){
			editorPtr->UpdateEditor(changeSet);
		}
	}
}


void CMultiObserverBinderComp::UpdateModelFromEditor() const
{
	int count = m_modelEditorsCompPtr.GetCount();
	for (int i = 0; i < count; ++i){
		imod::IModelEditor* editorPtr = m_modelEditorsCompPtr[i];
		if (editorPtr != NULL){
			editorPtr->UpdateModelFromEditor();
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


// protected methods

// reimplemented (icomp::CComponentBase)

void CMultiObserverBinderComp::OnComponentDestroyed()
{
	EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// private methods

void CMultiObserverBinderComp::EnsureModelDetached()
{
	if ((m_attachedModelPtr != NULL) && m_attachedModelPtr->IsAttached(this)){
		m_attachedModelPtr->DetachObserver(this);
	}
}


} // namespace ibase



