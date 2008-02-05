#ifndef qstd_IFactory_h_included
#define qstd_IFactory_h_included


#include "istd/istd.h"


#include <string>
#include <vector>


#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Common interface for a factory implentation.	
*/
class IFactory: virtual public istd::IPolymorphic  
{
public:
	typedef std::vector<std::string> KeyList;

	/**
	* Returns all posible keys for this factory
	*/
	virtual KeyList GetKeys() const = 0;

	/**
	* Create an instance of the object, mapped to the key \c key
	*/
	virtual istd::IPolymorphic* CreateInstance(const std::string& key) = 0;
};


} // namespace istd


#endif // !qstd_IFactory_h_included

