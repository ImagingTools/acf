#ifndef icomp_CEnvironmentManagerBase_included
#define icomp_CEnvironmentManagerBase_included


// ACF includes
#include <icomp/CPackageStaticInfo.h>
#include <icomp/IComponentEnvironmentManager.h>


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
	virtual int GetPackageType(const QByteArray& packageId) const;
	virtual QString GetPackagePath(const QByteArray& packageId) const;
	virtual QString GetRegistryPath(const CComponentAddress& address) const;

	// reimplemented (icomp::IRegistriesManager)
	virtual const IRegistry* GetRegistry(const CComponentAddress& address, const IRegistry* contextRegistryPtr = NULL) const;

	// reimplemented (icomp::IMetaInfoManager)
	virtual ComponentAddresses GetComponentAddresses(int typeFlag = CTF_ALL) const;
	virtual const IComponentStaticInfo* GetComponentMetaInfo(const CComponentAddress& address) const;
	virtual const IComponentStaticInfo* GetPackageMetaInfo(const QByteArray& packageId) const;
};


} // namespace icomp


#endif // !icomp_CEnvironmentManagerBase_included


