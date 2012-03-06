#ifndef istd_TComposedFactory_included
#define istd_TComposedFactory_included


// STL includes
#include <list>

// ACF includes
#include "istd/TIFactory.h"
#include "istd/TOptDelPtr.h"


namespace istd
{


/**
	Standard generic implementation of the composed factory.
*/
template <class InterfaceType>
class TComposedFactory: virtual public TIFactory<InterfaceType>
{
public:
	typedef TIFactory<InterfaceType> FactoryInterface;

	/**
		Registers object factory \c factoryPtr that will be mapped to the \c keyId.
		\param	factoryPtr	pointer to slave factory object.
							It cannot be NULL.
		\param	releaseFlag	if it is true, factory object will be automatically removed from memory.
	*/
	bool RegisterFactory(FactoryInterface* factoryPtr, bool releaseFlag = false);

	// reimplemented (istd::IFactoryInfo)
	virtual IFactoryInfo::KeyList GetFactoryKeys() const;

	// reimplemented (istd::TIFactory)
	virtual InterfaceType* CreateInstance(const std::string& keyId = "") const;

protected:
	typedef istd::TOptDelPtr<FactoryInterface> FactoryPtr;
	typedef std::list<FactoryPtr> FactoryList;

	FactoryList m_factoryList;
};


// public methods

template <class InterfaceType>
bool TComposedFactory<InterfaceType>::RegisterFactory(FactoryInterface* factoryPtr, bool releaseFlag)
{
	I_ASSERT(factoryPtr != NULL);

	if ((factoryPtr == NULL) || factoryPtr->GetFactoryKeys().empty()){
		return false;
	}

	m_factoryList.push_back(FactoryPtr());
	m_factoryList.back().SetPtr(factoryPtr, releaseFlag);

	return true;
}

	
// reimplemented (istd::IFactoryInfo)

template <class InterfaceType>
IFactoryInfo::KeyList TComposedFactory<InterfaceType>::GetFactoryKeys() const
{
	IFactoryInfo::KeyList retVal;
	for (		typename FactoryList::const_iterator iter = m_factoryList.begin();
				iter != m_factoryList.end();
				++iter){
		const FactoryPtr& factoryPtr = *iter;

		IFactoryInfo::KeyList factoryKeys = factoryPtr->GetFactoryKeys();

		retVal.insert(retVal.end(), factoryKeys.begin(), factoryKeys.end());
	}

	return retVal;
}


// reimplemented (istd::TIFactory)

template <class InterfaceType>
InterfaceType* TComposedFactory<InterfaceType>::CreateInstance(const std::string& keyId) const
{
	for (		typename FactoryList::const_iterator iter = m_factoryList.begin();
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


#endif // !istd_TComposedFactory_included


