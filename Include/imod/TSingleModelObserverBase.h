#ifndef imod_TSingleModelObserverBase_included
#define imod_TSingleModelObserverBase_included


#include "imod/CSingleModelObserverBase.h" 
#include "imod/IModel.h" 


namespace imod
{


/**
	Basic implementation for a single model observer.

	\ingroup ModelObserver
*/
template <class ModelInterface> 
class TSingleModelObserverBase: public CSingleModelObserverBase
{
public:
	typedef CSingleModelObserverBase BaseClass;
	typedef ModelInterface ModelType;

	TSingleModelObserverBase();

	ModelInterface* GetObjectPtr() const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

private:
	ModelInterface* m_objectPtr;
};


// public methods

template <class ModelInterface> 
TSingleModelObserverBase<ModelInterface>::TSingleModelObserverBase()
{
	m_objectPtr = NULL;
}


template <class ModelInterface> 
ModelInterface* TSingleModelObserverBase<ModelInterface>::GetObjectPtr() const
{
	return m_objectPtr;
}


// reimplemented (imod::IObserver)

template <class ModelInterface> 
bool TSingleModelObserverBase<ModelInterface>::OnAttached(imod::IModel* modelPtr)
{
	m_objectPtr = dynamic_cast<ModelInterface*>(modelPtr);
	if ((m_objectPtr != NULL) && BaseClass::OnAttached(modelPtr)){
		return true;
	}

	BaseClass::EnsureModelDetached();

	return false;
}


template <class ModelInterface> 
bool TSingleModelObserverBase<ModelInterface>::OnDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnDetached(modelPtr)){
		// If model was correctly attached m_objectPtr cannot be NULL. OnDetach returns true only if model was correctly attached.
		I_ASSERT(m_objectPtr != NULL);

		m_objectPtr = NULL;

		return true;
	}

	return false;
}


} // namespace imod


#endif // !imod_TSingleModelObserverBase_included


