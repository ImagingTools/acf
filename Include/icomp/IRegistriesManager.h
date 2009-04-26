#ifndef icomp_IRegistriesManager_included
#define icomp_IRegistriesManager_included


#include <string>

#include "istd/IPolymorphic.h"
#include "istd/CString.h"

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
		\return				registry object or NULL if specified composite component is not registered.
	*/
	virtual const IRegistry* GetRegistry(const CComponentAddress& address) const = 0;
	/**
		Get directory path of specified component package.
		\param	packageId	ID of component package.
		\return				package dir path or empty string if specified package is not registered.
	*/
	virtual istd::CString GetPackageDirPath(const std::string& packageId) const = 0;
};


}//namespace icomp


#endif // !icomp_IRegistriesManager_included


