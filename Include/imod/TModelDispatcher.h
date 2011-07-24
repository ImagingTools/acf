#ifndef imod_TModelDispatcher_included
#define imod_TModelDispatcher_included


// STL includes
#include <map>


// ACF includes
#include "imod/CMultiModelObserverBase.h"


namespace imod
{


/**
	Generic implementation of a data model changes notifier.

	You can register several data models objects that you want to observe with RegisterModel method.
	You will get a notification event on each change in the observed data model via OnModelChanged method call of your notification consumer.  
	\note Notification consumer class must implement the method
	void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr) to support the notification callback.
*/
template <class NotifyReceiver>
class TModelDispatcher: protected imod::CMultiModelObserverBase
{
public:
	TModelDispatcher(NotifyReceiver& parent);
	~TModelDispatcher();

	/**
		Register the data model to be observed. If model registration was successfull, the function returns \c true.
		\param modelPtr	pointer to the data model object, which should be observed.
		\param modelId	logical model ID for possible event filtering in the notification callback.
	*/
	bool RegisterModel(imod::IModel* modelPtr, int modelId = 0);
	
	/**
		Unregister the data model object.
		\param modelId	object ID used at the object registration.
		\sa RegisterModel
	*/
	void UnregisterModel(int modelId = 0);

	/**
		Unregister all observed data objects.
	*/
	void UnregisterAllModels();

	/**
		Get concrete object instance of the obsered model.
	*/
	template <class Object>
	Object* GetObjectPtr(int modelId) const;

protected:
	// reimplemented (imod::CMultiModelObserverBase)
	virtual void OnUpdate(imod::IModel* modelPtr, int changeFlags, istd::IPolymorphic* updateParamsPtr);

private:
	typedef std::map<imod::IModel*, int> ModelMap;

	ModelMap m_modelMap;

	NotifyReceiver& m_parent;
};


template <class NotifyReceiver>
TModelDispatcher<NotifyReceiver>::TModelDispatcher(NotifyReceiver& parent)
	:m_parent(parent)
{
}


template <class NotifyReceiver>
TModelDispatcher<NotifyReceiver>::~TModelDispatcher()
{
	UnregisterAllModels();
}


template <class NotifyReceiver>
bool TModelDispatcher<NotifyReceiver>::RegisterModel(imod::IModel* modelPtr, int modelId)
{
	if (modelPtr->AttachObserver(this)){
		m_modelMap[modelPtr] = modelId;

		return true;
	}

	return false;
}


template <class NotifyReceiver>
void TModelDispatcher<NotifyReceiver>::UnregisterModel(int modelId)
{
	for (ModelMap::iterator index = m_modelMap.begin(); index != m_modelMap.end(); index++){
		if (modelId == index->second){
			imod::IModel* modelPtr = index->first;

			m_modelMap.erase(index);

			modelPtr->DetachObserver(this);
		}
	}
}


template <class NotifyReceiver>
void TModelDispatcher<NotifyReceiver>::UnregisterAllModels()
{
	for (ModelMap::iterator index = m_modelMap.begin(); index != m_modelMap.end(); index++){
		imod::IModel* modelPtr = index->first;

		modelPtr->DetachObserver(this);
	}

	m_modelMap.clear();
}


template <class NotifyReceiver>
template <class Object>
Object* TModelDispatcher<NotifyReceiver>::GetObjectPtr(int modelId) const
{
	for (ModelMap::const_iterator index = m_modelMap.begin(); index != m_modelMap.end(); index++){
		if (modelId == index->second){
			Object* objectPtr = dynamic_cast<Object*>(index->first);

			return objectPtr;
		}
	}

	return NULL;
}


// protected methods

// reimplemented (imod::CMultiModelObserverBase)

template <class NotifyReceiver>
void TModelDispatcher<NotifyReceiver>::OnUpdate(imod::IModel* modelPtr, int changeFlags, istd::IPolymorphic* updateParamsPtr)
{
	ModelMap::iterator foundModelIter = m_modelMap.find(modelPtr);
	if (foundModelIter != m_modelMap.end()){
		int modelId = foundModelIter->second;

		m_parent.OnModelChanged(modelId, changeFlags, updateParamsPtr);
	}
}


} // namespace imod


#endif // !imod_TModelDispatcher_included


