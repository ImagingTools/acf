#ifndef imod_TSingleModelObserverBase_included
#define imod_TSingleModelObserverBase_included


#include "imod/imod.h"


#include <vector>

#include "imod/IObserver.h" 
#include "imod/IModel.h" 


namespace imod
{


/**
	\ingroup imod

	Basic implementation for a single model observer.
*/
template <class ObjectClass> 
class TSingleModelObserverBase: virtual public IObserver 
{
public:
	TSingleModelObserverBase();
	~TSingleModelObserverBase();

	imod::IModel* GetModelPtr() const;
	ObjectClass* GetObjectPtr() const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

protected:
	virtual void OnUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL) = 0;

	ObjectClass* m_objectPtr;
	imod::IModel* m_modelPtr;
};


// public methods

template <class ObjectClass> 
TSingleModelObserverBase<ObjectClass>::TSingleModelObserverBase()
{
	m_objectPtr = NULL;
	m_modelPtr = NULL;
}


template <class ObjectClass> 
TSingleModelObserverBase<ObjectClass>::~TSingleModelObserverBase()
{
	if (m_modelPtr != NULL){
		m_modelPtr->DetachObserver(this);
	}
}


template <class ObjectClass> 
imod::IModel* TSingleModelObserverBase<ObjectClass>::GetModelPtr() const
{
	return m_modelPtr;
}
	

template <class ObjectClass> 
ObjectClass* TSingleModelObserverBase<ObjectClass>::GetObjectPtr() const
{
	return m_objectPtr;
}


// pseudo-reimplemented (IObserver)

template <class ObjectClass> 
bool TSingleModelObserverBase<ObjectClass>::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(modelPtr != NULL);
	I_ASSERT(m_modelPtr == NULL);

	if (modelPtr == NULL){
		return false;
	}

	if (m_modelPtr != NULL){
		return false;
	}

	m_objectPtr = dynamic_cast<ObjectClass*>(modelPtr);
	if (m_objectPtr == NULL){
		m_modelPtr = NULL;
		return false;
	}

	BeforeUpdate(NULL);

	m_modelPtr = modelPtr;

	AfterUpdate(m_modelPtr);

	return true;
}


template <class ObjectClass> 
bool TSingleModelObserverBase<ObjectClass>::OnDetached(imod::IModel* modelPtr)
{
	I_ASSERT(modelPtr != NULL);
	I_ASSERT(m_modelPtr == modelPtr);

	if (m_modelPtr == modelPtr){		
		m_modelPtr = NULL;
		m_objectPtr = NULL;

		return true;
	}

	return false;
}


template <class ObjectClass> 
void TSingleModelObserverBase<ObjectClass>::BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{

}


template <class ObjectClass> 
void TSingleModelObserverBase<ObjectClass>::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	OnUpdate(modelPtr, updateFlags, updateParamsPtr);
}


} // namespace imod


#endif // !imod_TSingleModelObserverBase_included