#ifndef icomp_IComponentEnvironmentManager_included
#define icomp_IComponentEnvironmentManager_included


#include "icomp/IPackagesManager.h"
#include "icomp/IRegistriesManager.h"
#include "icomp/IMetaInfoManager.h"


namespace icomp
{


class IComponentEnvironmentManager:
			virtual public IPackagesManager,
			virtual public IRegistriesManager,
			virtual public IMetaInfoManager
{
public:
	/**
		Get file path of real used config file.
		If no config file was used, it returns empty string.
	*/
	virtual QString GetConfigFilePath() const = 0;
};


} // namespace icomp


#endif // !icomp_IComponentEnvironmentManager_included


