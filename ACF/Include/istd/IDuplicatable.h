#ifndef istd_IDuplicatable_included
#define istd_IDuplicatable_included


#include "istd/istd.h"


#include "istd/IPolymorphic.h"


namespace istd
{


/**
	\ingroup ACF

	Common interface for all objects that can copies their data from another object.
*/
class IDuplicatable: virtual public istd::IPolymorphic
{
public:
	/**
		Copies data from \c otherObject to this object.
		The function returns \c true if the operation was successfull.
	*/
	virtual bool CopyFrom(istd::IDuplicatable& otherObject);
};


} // namespace istd


#endif // !istd_IDuplicatable_included

