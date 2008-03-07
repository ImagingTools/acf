#ifndef qstd_TIFactory_h_included
#define qstd_TIFactory_h_included


#include "istd/istd.h"


#include <string>
#include <vector>


#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Common interface for a factory implementation.	
*/
template <class InterfaceType>
class TIFactory: virtual public istd::IPolymorphic  
{
public:
	typedef ::std::set< ::std::string> KeyList;

	/**
		Returns all posible keys for this factory.
	*/
	virtual KeyList GetFactoryKeys() const = 0;

	/**
		Create an instance of the object, mapped to the keyId \c keyId.
		\param	keyId		
		\return			pointer to created object or NULL if it was not possible to create it or keyId does not exist.
	*/
	virtual InterfaceType* CreateInstance(const ::std::string& keyId = "") const = 0;
};


typedef TIFactory<istd::IPolymorphic> IFactory;


} // namespace istd


#endif // !qstd_TIFactory_h_included

