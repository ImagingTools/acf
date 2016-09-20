#ifndef imod_TSingleModelObserverBase_included
#define imod_TSingleModelObserverBase_included


// STL includes
#include <typeinfo>

// Qt includes
#include <QtCore/QString>

// ACF includes
#include <imod/CSingleModelObserverBase.h>
#include <imod/IModel.h>


namespace imod
{


/**
	Basic implementation for a single model observer with binding to concrete data object interface.

	\ingroup ModelObserver
*/
template <class ModelInterface>
class TSingleModelObserverBase: public CSingleModelObserverBase
{
public:
	typedef CSingleModelObserverBase BaseClass;
	typedef ModelInterface ModelType;

	TSingleModelObserverBase();

	/**
		Get instance of observed object.
	*/
	ModelInterface* GetObservedObject() const;
	/**
		Get instance of observed object.
		\deprecated use \c GetObservedObject instead.
	*/
	ModelInterface* GetObjectPtr() const;

	/**
		Attach to model of object or set object pointer, if no connection is possible.
	*/
	bool AttachOrSetObject(ModelInterface* objectPtr);

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);
	virtual bool OnModelDetached(imod::IModel* modelPtr);

protected:
	virtual ModelInterface* CastFromModel(imod::IModel* modelPtr) const;

private:
	ModelInterface* m_observedObjectPtr;
};


// public methods

template <class ModelInterface>
TSingleModelObserverBase<ModelInterface>::TSingleModelObserverBase()
{
	m_observedObjectPtr = NULL;
}


template <class ModelInterface>
ModelInterface* TSingleModelObserverBase<ModelInterface>::GetObservedObject() const
{
	return m_observedObjectPtr;
}


template <class ModelInterface>
ModelInterface* TSingleModelObserverBase<ModelInterface>::GetObjectPtr() const
{
	return m_observedObjectPtr;
}


template <class ModelInterface>
bool TSingleModelObserverBase<ModelInterface>::AttachOrSetObject(ModelInterface* objectPtr)
{
	EnsureModelDetached();

	imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(objectPtr);

	bool retVal = false;
	if ((modelPtr != NULL) && modelPtr->AttachObserver(this)){
		retVal = true;
	}

	m_observedObjectPtr = objectPtr;

	return retVal;
}


// reimplemented (imod::IObserver)

template <class ModelInterface>
bool TSingleModelObserverBase<ModelInterface>::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	m_observedObjectPtr = CastFromModel(modelPtr);

	I_IF_DEBUG(
		if (m_observedObjectPtr == NULL){
			QString exptectedObjectInterface = typeid(ModelInterface).name();

			qDebug("Data model interface is not supported by this observer. Expected interface is: %s", qPrintable(exptectedObjectInterface));
		}
	)

	if ((m_observedObjectPtr != NULL) && BaseClass::OnModelAttached(modelPtr, changeMask)){
		return true;
	}

	BaseClass::EnsureModelDetached();

	return false;
}


template <class ModelInterface>
bool TSingleModelObserverBase<ModelInterface>::OnModelDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnModelDetached(modelPtr)){
		// If model was correctly attached m_observedObjectPtr cannot be NULL. OnDetach returns true only if model was correctly attached.
		Q_ASSERT(m_observedObjectPtr != NULL);

		m_observedObjectPtr = NULL;

		return true;
	}

	return false;
}


// protected methods

template <class ModelInterface>
ModelInterface* TSingleModelObserverBase<ModelInterface>::CastFromModel(imod::IModel* modelPtr) const
{
	return dynamic_cast<ModelInterface*>(modelPtr);
}


} // namespace imod


#endif // !imod_TSingleModelObserverBase_included


