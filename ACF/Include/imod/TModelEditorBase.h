#ifndef imod_TModelEditorBase_included
#define imod_TModelEditorBase_included


#include "imod/imod.h"


#include "imod/IModelEditor.h"
#include "imod/TSingleModelObserverBase.h"


namespace imod
{


/**
	\ingroup imod

	Basic implementation for an single model editor
*/
template <class InterfaceClass>
class TModelEditorBase:	virtual public imod::IModelEditor, 
						public imod::TSingleModelObserverBase<InterfaceClass>
{
public:
	typedef imod::TSingleModelObserverBase<InterfaceClass> BaseClass;

	TModelEditorBase();

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

	// reimplemented (imod::IModelEditor)
	virtual void ResetEditor();
	virtual bool IsModelChangeable() const;

protected:
	// reimplemented (TSingleModelObserverBase<InterfaceClass>)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	bool m_ignoreUpdates;
};


template <class InterfaceClass>
TModelEditorBase<InterfaceClass>::TModelEditorBase()
{
	m_ignoreUpdates = false;
}


// reimplemented (imod::IObserver)

template <class InterfaceClass>
bool TModelEditorBase<InterfaceClass>::OnAttached(imod::IModel* modelPtr)
{
	ResetEditor();

	return BaseClass::OnAttached(modelPtr);
}


template <class InterfaceClass>
bool TModelEditorBase<InterfaceClass>::OnDetached(imod::IModel* modelPtr)
{
	I_ASSERT(modelPtr != NULL);

	if (IsModelChangeable()){
		UpdateModel();
	}
	
	ResetEditor();
	
	return BaseClass::OnDetached(modelPtr);
}


// reimplemented (imod::IModelEditor)

template <class InterfaceClass>
void TModelEditorBase<InterfaceClass>::ResetEditor()
{
}

template <class InterfaceClass>
bool TModelEditorBase<InterfaceClass>::IsModelChangeable() const
{
	return true;
}


// protected methods

// reimplemented (TSingleModelObserverBase<InterfaceClass>)

template <class InterfaceClass>
void TModelEditorBase<InterfaceClass>::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		if (!m_ignoreUpdates){
			m_ignoreUpdates = true;
			
			UpdateEditor();
		
			m_ignoreUpdates = false;
		}
	}
}


} // namespace imod


#endif // !imod_TModelEditorBase_included

