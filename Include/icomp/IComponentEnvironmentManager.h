#ifndef icomp_IComponentEnvironmentManager_included
#define icomp_IComponentEnvironmentManager_included


#include "icomp/IRegistriesManager.h"
#include "icomp/IMetaInfoManager.h"


namespace icomp
{


class IComponentEnvironmentManager:
			virtual public IRegistriesManager,
			virtual public IMetaInfoManager
{
public:
	/**
		Configure environment for specified file path.
		If this file path is invalid or empty, standard configuration file will be used.
	*/
	virtual bool ConfigureEnvironment(const istd::CString& configFilePath = istd::CString()) = 0;

	/**
		Get file path of real used config file.
		If no config file was used, it returns empty string.
	*/
	virtual istd::CString GetConfigFilePath() const = 0;
};


} // namespace icomp


#endif // !icomp_IComponentEnvironmentManager_included


