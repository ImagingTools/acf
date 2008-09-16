#ifndef istd_IServicesProvider_included
#define istd_IServicesProvider_included


#include "istd/istd.h"


#include "istd/IPolymorphic.h"


namespace istd
{


/**
Allow access to service implementation.
 */
class IServicesProvider: virtual public istd::IPolymorphic
{
public:
	/**
	Get pointer to service implementation.
	If implementation for specified ID is not available, it returns NULL.
	*/
	virtual void* GetService(const type_info& serviceId) const = 0;
};


} // namespace istd


#endif // !istd_IServicesProvider_included


