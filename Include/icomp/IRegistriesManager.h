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
		\param	address		adress of registry treated as composed component.
	*/
	virtual const IRegistry* GetRegistry(const icomp::CComponentAddress& address) const = 0;
};


}//namespace icomp


#endif // !icomp_IRegistriesManager_included


