#ifndef imod_CMultiModelDispatcherBase_included
#define imod_CMultiModelDispatcherBase_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "istd/TDelPtr.h"
#include "imod/IModel.h"
#include "imod/CSingleModelObserverBase.h"


namespace imod
{


/**
	Generic implementation of a data model changes notifier.

	You can register several data models objects that you want to observe with RegisterModel method.
	You will get a notification event on each change in the observed data model via OnModelChanged method call of your notification consumer.  
	\note Notification consumer class must implement the method
	void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr) to support the notification callback.
	\sa imod::CMultiModelBridgeBase.

	\ingroup ModelObserver
*/
class CMultiModelDispatcherBase: virtual public istd::IPolymorphic
{
public:
	/**
		Register the data model to be observed. If model registration was successfull, the function returns \c true.
		\param modelPtr	Pointer to the data model object, which should be observed.
		\param modelId	Logical model ID for possible event filtering in the notification callback.
		\param relefantFlags	The notification will only be triggered if the value of \c relevantFlags matches the model change flags.
	*/
	bool RegisterModel(IModel* modelPtr, int modelId = 0, int relevantFlags = 0);
	
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
	Object* GetObjectAt(int modelId) const;

protected:
	class ObserverProxy: public CSingleModelObserverBase
	{
	public:
		typedef CSingleModelObserverBase BaseClass;

		ObserverProxy(CMultiModelDispatcherBase* parentPtr, int modelId, int relevantFlags);

		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int changeFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CMultiModelDispatcherBase& m_parent;
		int m_modelId;
		int m_relevantFlags;
	};

	// abstract methods
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr) = 0;

private:
	typedef istd::TDelPtr<ObserverProxy> ObserverProxyPtr;
	typedef QMap<int, ObserverProxyPtr> ModelMap;

	ModelMap m_modelMap;
};


// public template methods

template <class Object>
Object* CMultiModelDispatcherBase::GetObjectAt(int modelId) const
{
	typename ModelMap::const_iterator foundIter = m_modelMap.find(modelId);
	if ((foundIter != m_modelMap.end()) && foundIter.value().IsValid()){
		return dynamic_cast<Object*>(foundIter.value()->GetModelPtr());
	}

	return NULL;
}


} // namespace imod


#endif // !imod_CMultiModelDispatcherBase_included


