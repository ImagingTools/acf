#ifndef istd_TFactory_h_included
#define istd_TFactory_h_included


#include "istd/istd.h"


#include <map>
#include <algorithm>


#include "istd/IFactory.h"
#include "istd/TIObjectFactory.h"


namespace istd
{


/**
	Standard implementation of a factory.
*/
template <class ObjectClass>
class TFactory: virtual public istd::IFactory
{
public:
	typedef istd::TIObjectFactory<ObjectClass> FactoryClass;

	void RegisterFactory(FactoryClass* factoryPtr, const std::string& key);

	// reimplemented (istd::IFactory)
	virtual KeyList GetKeys() const;
	virtual istd::PolymorphicInterface* CreateInstance(const std::string& key);

protected:
	typedef std::pair<FactoryClass*, int> FactoryItem;

	typedef std::map<std::string, FactoryItem> FactoryMap;

	FactoryMap m_factoryMap;
};



template <class ObjectClass>
void TFactory<ObjectClass>::RegisterFactory(FactoryClass* factoryPtr, const std::string& key)
{

	if (key.empty()){
		return;
	}

	if (factoryPtr == NULL){
		return;
	}

	m_factoryMap[key] = std::make_pair(factoryPtr, int(m_factoryMap.size()));
}

	
// reimplemented (istd::IFactory)

template <class ObjectClass>
typename TFactory<ObjectClass>::KeyList TFactory<ObjectClass>::GetKeys() const
{
	KeyList keyList;
	keyList.resize(m_factoryMap.size());

	for (FactoryMap::const_iterator index = m_factoryMap.begin(); index != m_factoryMap.end(); index++){
		keyList[index->second.second] = index->first;
	}

	return keyList;
}


template <class ObjectClass>
istd::PolymorphicInterface* TFactory<ObjectClass>::CreateInstance(const std::string& key)
{
	FactoryMap::iterator foundIter = m_factoryMap.find(key);
	if (foundIter != m_factoryMap.end()){
		return foundIter->second.first->createObject();
	}

	return NULL;
}


} // namespace istd


#endif // !istd_TFactory_h_included

