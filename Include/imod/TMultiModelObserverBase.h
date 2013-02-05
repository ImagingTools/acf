#ifndef imod_TMultiModelObserverBase_included
#define imod_TMultiModelObserverBase_included


// ACF includes
#include "imod/CMultiModelObserverBase.h"

#include "imod/imod.h"


namespace imod
{


/**
	Basic implementation of a multiple model observer.

	\ingroup ModelObserver
*/
template<class ModelInterface>
class TMultiModelObserverBase: public CMultiModelObserverBase
{
public:
	typedef CMultiModelObserverBase BaseClass;
	typedef ModelInterface ModelType;

	ModelInterface* GetObjectPtr(int objectIndex) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
};


// public methods

template<class ModelInterface>
ModelInterface* TMultiModelObserverBase<ModelInterface>::GetObjectPtr(int objectIndex) const
{
	Q_ASSERT(objectIndex >= 0);
	Q_ASSERT(objectIndex < GetModelCount());

	return dynamic_cast<ModelInterface*>(GetModelPtr(objectIndex));
}
	

// reimplemented (imod::IObserver)

template<class ModelInterface>
bool TMultiModelObserverBase<ModelInterface>::OnAttached(imod::IModel* modelPtr)
{
	ModelInterface* objectPtr = dynamic_cast<ModelInterface*>(modelPtr);
	if (objectPtr == NULL){
		return false;
	}

	return BaseClass::OnAttached(modelPtr);
}


} // namespace imod


#endif // !imod_TMultiModelObserverBase_included

