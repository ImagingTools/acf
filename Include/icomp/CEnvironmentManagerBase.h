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
	// reimplemented (icomp::IComponentEnvironmentManager)
	virtual QString GetConfigFilePath() const;

	// reimplemented (icomp::IPackagesManager)
	virtual bool LoadPackages(const QString& configFilePath = QString());
	virtual int GetPackageType(const std::string& packageId) const;
	virtual QString GetPackagePath(const std::string& packageId) const;

	// reimplemented (icomp::IRegistriesManager)
	virtual const IRegistry* GetRegistry(const CComponentAddress& address, const IRegistry* contextRegistryPtr = NULL) const;

	// reimplemented (icomp::IMetaInfoManager)
	virtual ComponentAddresses GetComponentAddresses(int typeFlag = CTF_ALL) const;
	virtual const IComponentStaticInfo* GetComponentMetaInfo(const CComponentAddress& address) const;
	virtual const IComponentStaticInfo* GetPackageMetaInfo(const std::string& packageId) const;
};


} // namespace icomp


#endif // !icomp_CEnvironmentManagerBase_included


