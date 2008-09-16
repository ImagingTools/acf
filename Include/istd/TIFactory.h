#ifndef istd_TIFactory_included
#define istd_TIFactory_included


#include "istd/istd.h"


#include <string>
#include <vector>


#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Common interface for a factory implementation.	
*/
template <class Interface>
class TIFactory: virtual public istd::IPolymorphic  
{
public:
	typedef Interface InterfaceType;
	typedef std::vector<std::string> KeyList;

	/**
		Returns all posible keys for this factory.
	*/
	virtual KeyList GetFactoryKeys() const = 0;

	/**
		Create an instance of the object, mapped to the keyId \c keyId.
		\param	keyId		
		\return			pointer to created object or NULL if it was not possible to create it or keyId does not exist.
	*/
	virtual Interface* CreateInstance(const std::string& keyId = "") const = 0;
};


typedef TIFactory<istd::IPolymorphic> IFactory;


} // namespace istd


#endif // !istd_TIFactory_included

