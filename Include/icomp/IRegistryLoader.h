#ifndef icomp_IRegistryLoader_included
#define icomp_IRegistryLoader_included


#include "icomp/IRegistry.h"


namespace icomp
{

/**
	Interface for a ACF registry loader.
*/
class IRegistryLoader: virtual public istd::IPolymorphic
{
public:
	virtual bool ConfigureEnvironment(const istd::CString& configFile = istd::CString()) = 0;

	/**
		Get an ACF registry from a given file.
	*/
	virtual const icomp::IRegistry* GetRegistryFromFile(const istd::CString& path) const = 0;
};
	

} // namespace icomp


#endif // !icomp_IRegistryLoader_included


