#ifndef icomp_CEnvironmentManagerBase_included
#define icomp_CEnvironmentManagerBase_included


#include "icomp/CPackageStaticInfo.h"
#include "icomp/IComponentEnvironmentManager.h"


namespace icomp
{


class CEnvironmentManagerBase:
			public CPackageStaticInfo,
			virtual public IComponentEnvironmentManager
{
public:
	// reimplemented (icomp::IRegistryLoader)
	virtual const IRegistry* GetRegistryFromFile(const istd::CString& path) const;

	// reimplemented (icomp::IComponentEnvironmentManager)
	virtual bool ConfigureEnvironment(const istd::CString& configFilePath = istd::CString());
	virtual istd::CString GetConfigFilePath() const;

	// reimplemented (icomp::IRegistriesManager)
	virtual const IRegistry* GetRegistry(const CComponentAddress& address) const;
	virtual istd::CString GetPackageDirPath(const std::string& packageId) const;

	// reimplemented (icomp::IMetaInfoManager)
	virtual ComponentAddresses GetComponentAddresses(int typeFlag = CTF_ALL) const;
	virtual const IComponentStaticInfo* GetComponentStaticInfo(const CComponentAddress& address) const;
};


} // namespace icomp


#endif // !icomp_CEnvironmentManagerBase_included


