#ifndef imod_TSingleModelObserverBase_included
#define imod_TSingleModelObserverBase_included


#include "imod/CSingleModelObserverBase.h" 
#include "imod/IModel.h" 


namespace imod
{


/**
	\ingroup imod

	Basic implementation for a single model observer.
*/
template <class ObjectClass> 
class TSingleModelObserverBase: public CSingleModelObserverBase
{
public:
	typedef CSingleModelObserverBase BaseClass;

	TSingleModelObserverBase();

	ObjectClass* GetObjectPtr() const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

private:
	ObjectClass* m_objectPtr;
};


// public methods

template <class ObjectClass> 
TSingleModelObserverBase<ObjectClass>::TSingleModelObserverBase()
{
	m_objectPtr = NULL;
}


template <class ObjectClass> 
ObjectClass* TSingleModelObserverBase<ObjectClass>::GetObjectPtr() const
{
	return m_objectPtr;
}


// reimplemented (imod::IObserver)

template <class ObjectClass> 
bool TSingleModelObserverBase<ObjectClass>::OnAttached(imod::IModel* modelPtr)
{
	m_objectPtr = dynamic_cast<ObjectClass*>(modelPtr);
	if ((m_objectPtr != NULL) && BaseClass::OnAttached(modelPtr)){
		return true;
	}

	BaseClass::EnsureDetached();

	return false;
}


template <class ObjectClass> 
bool TSingleModelObserverBase<ObjectClass>::OnDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnDetached(modelPtr)){
		// If it was correct attached it cannot be NULL. OnDetach returns true if model was attached.
		I_ASSERT(m_objectPtr != NULL);

		m_objectPtr = NULL;

		return true;
	}

	return false;
}


} // namespace imod


#endif // !imod_TSingleModelObserverBase_included


