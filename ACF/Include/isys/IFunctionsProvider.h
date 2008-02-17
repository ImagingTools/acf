#ifndef INCLUDED_isys_IFunctionsProvider_h
#define INCLUDED_isys_IFunctionsProvider_h


#include <string>

#include "istd/IPolymorphic.h"

#include "isys/isys.h"


namespace isys
{


/**
	Interface for set of functions accessed by its ID.
	Typically it is used to access dynamic loaded libraries.
 */
class IFunctionsProvider: public istd::IPolymorphic
{
public:
	/**
		Check if this object was correctly initialized and functions can be used.
	*/
	virtual bool IsValid() const = 0;
	/**
		Return function pointer using its ID.
	 *		\return	function pointer if success or NULL otherwise.
	 */
	virtual void* GetFunction(const ::std::string& id) const = 0;
};


} // namespace isys


#endif //!INCLUDED_isys_IFunctionsProvider_h



