#ifndef icomp_IRegistryLoader_included
#define icomp_IRegistryLoader_included


// Qt includes
#include <QString>


// ACF includes
#include "istd/IPolymorphic.h"


namespace icomp
{


class IRegistry;


/**
	Interface for a ACF registry loader.
*/
class IRegistryLoader: virtual public istd::IPolymorphic
{
public:
	/**
		Get an ACF registry from a given file.
	*/
	virtual const icomp::IRegistry* GetRegistryFromFile(const QString& path) const = 0;
};
	

} // namespace icomp


#endif // !icomp_IRegistryLoader_included


