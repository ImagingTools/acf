#ifndef imod_TMultiModelObserverBase_included
#define imod_TMultiModelObserverBase_included


// ACF inlcudes
#include "imod/CMultiModelObserverBase.h"

#include "imod/imod.h"


namespace imod
{


/**
	Basic implementation of an multiple model observer.

	\ingroup ModelObserver
*/
template<class ObjectClass>
class TMultiModelObserverBase: public CMultiModelObserverBase
{
public:
	typedef CMultiModelObserverBase BaseClass;

	ObjectClass* GetObjectPtr(int objectIndex) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
};


// public methods

template<class ObjectClass>
ObjectClass* TMultiModelObserverBase<ObjectClass>::GetObjectPtr(int objectIndex) const
{
	I_ASSERT(objectIndex >= 0)
	I_ASSERT(objectIndex < GetModelCount());

	return dynamic_cast<ObjectClass*>(GetModelPtr(objectIndex));
}
	

// reimplemented (imod::IObserver)

template<class ObjectClass>
bool TMultiModelObserverBase<ObjectClass>::OnAttached(imod::IModel* modelPtr)
{
	ObjectClass* objectPtr = dynamic_cast<ObjectClass*>(modelPtr);
	if (objectPtr == NULL){
		return false;
	}

	return BaseClass::OnAttached(modelPtr);
}


} // namespace imod


#endif // !imod_TMultiModelObserverBase_included

