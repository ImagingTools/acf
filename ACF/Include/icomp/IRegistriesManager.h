#ifndef icomp_IRegistriesManager_included
#define icomp_IRegistriesManager_included


#include <string>

#include "istd/IPolymorphic.h"

#include "icomp/icomp.h"


namespace icomp
{


class IRegistry;
class CComponentAddress;


class IRegistriesManager: virtual public istd::IPolymorphic
{
public:
	/**
		Get access to stored registry.
	*/
	virtual const IRegistry* GetRegistry(const icomp::CComponentAddress& address, const IRegistry* contextPtr) const = 0;
};


}//namespace icomp


#endif // !icomp_IRegistriesManager_included


