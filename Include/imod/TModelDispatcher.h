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
	typedef imod::CMultiModelObserverBase BaseClass;

	TModelDispatcher(NotifyReceiver& parent);
	~TModelDispatcher();

	/**
		Register the data model to be observed. If model registration was successfull, the function returns \c true.
		\param modelPtr	Pointer to the data model object, which should be observed.
		\param modelId	Logical model ID for possible event filtering in the notification callback.
		\param relefantFlags	The notification will only be triggered if the value of \c relevantFlags matches the model change flags.
	*/
	bool RegisterModel(imod::IModel* modelPtr, int modelId = 0, int relevantFlags = 0);
	
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
	// reimplemented (imod::IObserver)
	virtual bool OnDetached(imod::IModel* modelPtr);

	// reimplemented (imod::CMultiModelObserverBase)
	virtual void OnUpdate(imod::IModel* modelPtr, int changeFlags, istd::IPolymorphic* updateParamsPtr);

private:
	struct ModelInfo
	{
		int modelId;
		int relevantFlags;
	};

	typedef std::map<imod::IModel*, ModelInfo> ModelMap;

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
bool TModelDispatcher<NotifyReceiver>::RegisterModel(imod::IModel* modelPtr, int modelId, int relevantFlags)
{
	if (modelPtr->AttachObserver(this)){
		ModelInfo modelInfo;
		modelInfo.modelId = modelId;
		modelInfo.relevantFlags = relevantFlags;

		m_modelMap[modelPtr] = modelInfo;

		return true;
	}

	return false;
}


template <class NotifyReceiver>
void TModelDispatcher<NotifyReceiver>::UnregisterModel(int modelId)
{
	for (ModelMap::iterator index = m_modelMap.begin(); index != m_modelMap.end(); index++){
		if (modelId == index->second.modelId){
			imod::IModel* modelPtr = index->first;

			m_modelMap.erase(index);

			modelPtr->DetachObserver(this);

			break;
		}
	}
}


template <class NotifyReceiver>
void TModelDispatcher<NotifyReceiver>::UnregisterAllModels()
{
	while (!m_modelMap.empty()){
		ModelMap::iterator currentIter = m_modelMap.begin();

		imod::IModel* modelPtr = currentIter->first;

		m_modelMap.erase(currentIter);

		modelPtr->DetachObserver(this);
	}

	m_modelMap.clear();
}


template <class NotifyReceiver>
template <class Object>
Object* TModelDispatcher<NotifyReceiver>::GetObjectPtr(int modelId) const
{
	for (ModelMap::const_iterator index = m_modelMap.begin(); index != m_modelMap.end(); index++){
		if (modelId == index->second.modelId){
			Object* objectPtr = dynamic_cast<Object*>(index->first);

			return objectPtr;
		}
	}

	return NULL;
}


// protected methods

// reimplemented (imod::IObserver)

template <class NotifyReceiver>
bool TModelDispatcher<NotifyReceiver>::OnDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnDetached(modelPtr)){
		for (ModelMap::iterator index = m_modelMap.begin(); index != m_modelMap.end(); index++){
			if (index->first == modelPtr){
				m_modelMap.erase(index);

				break;
			}
		}

		return true;
	}

	return false;
}


// reimplemented (imod::CMultiModelObserverBase)

template <class NotifyReceiver>
void TModelDispatcher<NotifyReceiver>::OnUpdate(imod::IModel* modelPtr, int changeFlags, istd::IPolymorphic* updateParamsPtr)
{
	ModelMap::iterator foundModelIter = m_modelMap.find(modelPtr);
	if (foundModelIter != m_modelMap.end()){
		int relevantFlags = foundModelIter->second.relevantFlags;
		if (relevantFlags != 0 && (relevantFlags & changeFlags) == 0){
			return;
		}
			
		int modelId = foundModelIter->second.modelId;

		m_parent.OnModelChanged(modelId, changeFlags, updateParamsPtr);
	}
}


} // namespace imod


#endif // !imod_TModelDispatcher_included


