#ifndef icomp_IRegistriesManager_included
#define icomp_IRegistriesManager_included


#include <string>

#include "istd/IPolymorphic.h"

#include "icomp/icomp.h"


namespace icomp
{


class IRegistry;


class IRegistriesManager: virtual public istd::IPolymorphic
{
public:
	/**
		Get access to stored registry.
	*/
	virtual const IRegistry* GetRegistry(const std::string& packageId, const std::string& componentId, const IRegistry* contextPtr) const = 0;
};


}//namespace icomp


#endif // !icomp_IRegistriesManager_included


