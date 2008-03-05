#ifndef istd_TComposedFactory_h_included
#define istd_TComposedFactory_h_included


#include <map>

#include "istd/TIFactory.h"


namespace istd
{


/**
	Standard implementation of a factory.
*/
template <class InterfaceType>
class TComposedFactory: virtual public TIFactory<InterfaceType>
{
public:
	typedef TIFactory<InterfaceType> FactoryClass;

	/**
		Registers object factory \c factoryPtr that will be mapped to the \c keyId.
		\param	factoryPtr	pointer to slave factory object.
							It cannot be NULL.
		\param	releaseFlag	if it is true, factory object will be automatically removed from memory.
	*/
	bool RegisterFactory(FactoryClass* factoryPtr, bool releaseFlag = false);

	// reimplemented (istd::TIFactory)
	virtual KeyList GetFactoryKeys() const;
	virtual InterfaceType* CreateInstance(const ::std::string& keyId = "") const;

protected:
	typedef istd::ToptDelPtr<FactoryClass> FactoryPtr;
	typedef ::std::list<::std::string, FactoryPtr> FactoryList;

	FactoryList m_factoryList;
};


// public methods

template <class InterfaceType>
bool TComposedFactory<InterfaceType>::RegisterFactory(FactoryClass* factoryPtr, bool releaseFlag)
{
	I_ASSERT(factoryPtr != NULL);

	if ((factoryPtr == NULL) || factoryPtr->GetFactoryKeys().empty()){
		return false;
	}

	m_factoryList.push_back(FactoryPtr());
	m_factoryList.back().SetPtr(factoryPtr, releaseFlag);

	return true;
}

	
// reimplemented (istd::IFactory)

template <class InterfaceType>
typename TComposedFactory<InterfaceType>::KeyList TComposedFactory<InterfaceType>::GetFactoryKeys() const
{
	KeyList retVal;
	for (		FactoryList::const_iterator iter = m_factoryList.begin();
				iter != m_factoryList.end();
				++iter){
		const FactoryPtr& factoryPtr = *iter;

		KeyList factoryKeys = factoryPtr->GetFactoryKeys();

		retVal.insert(factoryKeys.begin(), factoryKeys.end());
	}

	return retVal;
}


template <class InterfaceType>
istd::IPolymorphic* TComposedFactory<InterfaceType>::CreateInstance(const ::std::string& keyId)
{
	for (		FactoryList::const_iterator iter = m_factoryList.begin();
				iter != m_factoryList.end();
				++iter){
		const FactoryPtr& factoryPtr = *iter;

		InterfaceType* createdPtr = factoryPtr->CreateInstance(keyId);

		if (createdPtr != NULL){
			return createdPtr;
		}
	}

	return NULL;
}


} // namespace istd


#endif // !istd_TComposedFactory_h_included


