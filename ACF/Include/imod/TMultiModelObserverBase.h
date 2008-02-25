#ifndef imod_TMultiModelObserverBase_included
#define imod_TMultiModelObserverBase_included


#include "imod/imod.h"


#include <vector>

#include "imod/IModel.h"
#include "imod/IObserver.h"


namespace imod
{


/**
	\ingroup imod

	Basic implementation of an multiple model observer.
/*/
template<class ObjectClass>
class TMultiModelObserverBase: virtual public IObserver
{
public:
	TMultiModelObserverBase();
	~TMultiModelObserverBase();

	ObjectClass* GetObjectPtr(int objectIndex) const;
	IModel* GetModelPtr(int modelIndex) const;
	int GetModelCount() const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

protected:
	virtual void OnUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL) = 0;

	typedef std::vector<imod::IModel*> Models;

	Models m_models;
};


// public methods

template<class ObjectClass>
TMultiModelObserverBase<ObjectClass>::TMultiModelObserverBase()
{

}


template<class ObjectClass>
TMultiModelObserverBase<ObjectClass>::~TMultiModelObserverBase()
{
	for (int objectIndex = 0; objectIndex < (int)m_models.size(); objectIndex++){
		imod::IModel* modelPtr = m_models.at(objectIndex);
		if (modelPtr != NULL){
			modelPtr->DetachObserver(const_cast<TMultiModelObserverBase*>(this));
		}
	}

	m_models.clear();
}


template<class ObjectClass>
ObjectClass* TMultiModelObserverBase<ObjectClass>::GetObjectPtr(int objectIndex) const
{
	I_ASSERT(objectIndex >= 0)
	I_ASSERT(objectIndex < GetModelCount());

	return dynamic_cast<ObjectClass*>(m_models.at(objectIndex));
}
	

template<class ObjectClass>
IModel* TMultiModelObserverBase<ObjectClass>::GetModelPtr(int modelIndex) const
{
	I_ASSERT(modelIndex >= 0)
	I_ASSERT(modelIndex < GetModelCount());

	return dynamic_cast<ObjectClass*>(m_models.at(objectIndex));
}



template<class ObjectClass>
int TMultiModelObserverBase<ObjectClass>::GetModelCount() const
{
	return int(m_models.size());
}


// reimplemented (IObserver)

template<class ObjectClass>
bool TMultiModelObserverBase<ObjectClass>::OnAttached(imod::IModel* modelPtr)
{
	ObjectClass* objectPtr = dynamic_cast<ObjectClass*>(modelPtr);
	if (objectPtr == NULL){
		return false;
	}

	for (int modelIndex = 0; modelIndex < (int)m_models.size(); modelIndex++){
		if (modelPtr == m_models.at(modelIndex)){
			return false;
		}
	}

	m_models.push_back(modelPtr);

	return true;
}


template<class ObjectClass>
bool TMultiModelObserverBase<ObjectClass>::OnDetached(IModel* modelPtr)
{
	Models::iterator begIter =  m_models.begin();
	while (begIter != m_models.end()){
		IModel* modelPtr = *begIter;
		if (modelPtr == modelPtr){
			m_models.erase(begIter);
			
			return true;
		}

		++begIter;
	}

	return false;
}


template<class ObjectClass>
void TMultiModelObserverBase<ObjectClass>::BeforeUpdate(IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{

}


template<class ObjectClass>
void TMultiModelObserverBase<ObjectClass>::AfterUpdate(IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	OnUpdate(modelPtr, updateFlags, updateParamsPtr);
}


} // namespace imod


#endif // !MultiModelObserver_h_included