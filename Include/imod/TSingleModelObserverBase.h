#ifndef imod_TSingleModelObserverBase_included
#define imod_TSingleModelObserverBase_included


// STL includes
#include <typeinfo>

// Qt includes
#include <QtCore/QString>

// ACF includes
#include "imod/CSingleModelObserverBase.h"
#include "imod/IModel.h"


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

	ModelInterface* GetObjectPtr() const;

	/**
		Attach to model of object or set object pointer, if no connection is possible.
	*/
	bool AttachOrSetObject(ModelInterface* objectPtr);

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

protected:
	virtual ModelInterface* CastFromModel(imod::IModel* modelPtr) const;

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


template <class ModelInterface>
bool TSingleModelObserverBase<ModelInterface>::AttachOrSetObject(ModelInterface* objectPtr)
{
	EnsureModelDetached();

	imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(objectPtr);

	bool retVal = false;
	if ((modelPtr != NULL) && modelPtr->AttachObserver(this)){
		retVal = true;
	}

	m_objectPtr = objectPtr;

	return retVal;
}


// reimplemented (imod::IObserver)

template <class ModelInterface>
bool TSingleModelObserverBase<ModelInterface>::OnAttached(imod::IModel* modelPtr)
{
	m_objectPtr = CastFromModel(modelPtr);

	I_IF_DEBUG(
		if (m_objectPtr == NULL){
			QString exptectedObjectInterface = typeid(ModelInterface).name();

			qDebug("Data model interface is not supported by this observer. Expected interface is: %s", qPrintable(exptectedObjectInterface));
		}
	)

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
		Q_ASSERT(m_objectPtr != NULL);

		m_objectPtr = NULL;

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


