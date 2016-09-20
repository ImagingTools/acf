#ifndef imod_TMultiModelObserverBase_included
#define imod_TMultiModelObserverBase_included


// ACF includes
#include <imod/CMultiModelObserverBase.h>

#include <imod/imod.h>


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

	/**
		Get instance of observed object.
		\param	objectIndex index of object
	*/
	ModelInterface* GetObservedObject(int objectIndex) const;
	/**
		Get instance of observed object.
		\deprecated use \c GetObservedObject instead.
	*/
	ModelInterface* GetObjectPtr(int objectIndex) const;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);
};


// public methods

template<class ModelInterface>
ModelInterface* TMultiModelObserverBase<ModelInterface>::GetObservedObject(int objectIndex) const
{
	Q_ASSERT(objectIndex >= 0);
	Q_ASSERT(objectIndex < GetModelCount());

	return dynamic_cast<ModelInterface*>(GetObservedModel(objectIndex));
}


template<class ModelInterface>
ModelInterface* TMultiModelObserverBase<ModelInterface>::GetObjectPtr(int objectIndex) const
{
	Q_ASSERT(objectIndex >= 0);
	Q_ASSERT(objectIndex < GetModelCount());

	return dynamic_cast<ModelInterface*>(GetObservedModel(objectIndex));
}


// reimplemented (imod::IObserver)

template<class ModelInterface>
bool TMultiModelObserverBase<ModelInterface>::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	ModelInterface* objectPtr = dynamic_cast<ModelInterface*>(modelPtr);
	if (objectPtr == NULL){
		return false;
	}

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


} // namespace imod


#endif // !imod_TMultiModelObserverBase_included

