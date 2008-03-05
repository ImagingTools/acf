#ifndef istd_TSingleFactory_included
#define istd_TSingleFactory_included


#include "istd/istd.h"


#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Template based object factory interface.
*/
template <class ObjectClass, class Implementation>
class TSingleFactory: virtual public TIFactory<InterfaceType>
{
public:
	explicit TSingleFactory(const ::std::string& keyId);

	// reimplemented (istd::TIFactory)
	virtual KeyList GetKeys() const;
	virtual InterfaceType* CreateInstance(const ::std::string& keyId = "") const;

private:
	::std::string m_keyId;
};


// public methods

// reimplemented (istd::IFactory)

template <class InterfaceType, class Implementation>
typename TIFactory<InterfaceType, Implementation>::KeyList TComposedFactory<InterfaceType, Implementation>::GetFactoryKeys() const
{
	KeyList retVal;

	retVal.push_back(m_keyId);

	return retVal;
}


template <class InterfaceType, class Implementation>
istd::IPolymorphic* TComposedFactory<InterfaceType, Implementation>::CreateInstance(const ::std::string& keyId)
{
	if (keyId.empty() || (keyId == m_keyId)){
		return new Implementation;
	}

	return NULL;
}


} // namespace istd


#endif // !istd_TSingleFactory_included


