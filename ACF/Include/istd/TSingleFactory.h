#ifndef istd_TSingleFactory_included
#define istd_TSingleFactory_included


#include "istd/istd.h"


#include <string>


#include "istd/TIFactory.h"


namespace istd
{


/**
	Template based object factory interface.
*/
template <class Interface, class Implementation>
class TSingleFactory: virtual public TIFactory<Interface>
{
public:
	typedef Implementation ImplementationType;
	typedef TIFactory<InterfaceType> FactoryInterface;

	explicit TSingleFactory(const std::string& keyId);

	// reimplemented (istd::TIFactory)
	virtual KeyList GetFactoryKeys() const;
	virtual Interface* CreateInstance(const std::string& keyId = "") const;

private:
	std::string m_keyId;
};


// public methods

template <class Interface, class Implementation>
TSingleFactory<Interface, Implementation>::TSingleFactory(const std::string& keyId)
	:m_keyId(keyId)
{

}


// reimplemented (istd::TIFactory)

template <class Interface, class Implementation>
typename TSingleFactory<Interface, Implementation>::KeyList TSingleFactory<Interface, Implementation>::GetFactoryKeys() const
{
	KeyList retVal;

	retVal.push_back(m_keyId);

	return retVal;
}


template <class Interface, class Implementation>
Interface* TSingleFactory<Interface, Implementation>::CreateInstance(const std::string& keyId) const
{
	if (keyId.empty() || (keyId == m_keyId)){
		return new Implementation;
	}

	return NULL;
}


} // namespace istd


#endif // !istd_TSingleFactory_included


