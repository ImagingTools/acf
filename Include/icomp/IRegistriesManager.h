#ifndef icomp_IRegistriesManager_included
#define icomp_IRegistriesManager_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include "istd/IChangeable.h"


namespace icomp
{


class IRegistry;
class CComponentAddress;


/**
	Allow access to registries defined by composed components.
*/
class IRegistriesManager: virtual public istd::IChangeable
{
public:
	/**
		Get access to stored registry.
		\param	address		adress of registry treated as composed component.
		\return				registry object or NULL if specified composite component is not registered.
	*/
	virtual const IRegistry* GetRegistry(const CComponentAddress& address, const IRegistry* contextRegistryPtr = NULL) const = 0;
};


} // namespace icomp


#endif // !icomp_IRegistriesManager_included


