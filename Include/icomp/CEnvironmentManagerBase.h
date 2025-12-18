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
	virtual QString GetConfigFilePath() const override;

	// reimplemented (icomp::IPackagesManager)
	virtual bool LoadPackages(const QString& configFilePath = QString()) override;
	virtual int GetPackageType(const QByteArray& packageId) const override;
	virtual QString GetPackagePath(const QByteArray& packageId) const override;
	virtual QString GetRegistryPath(const CComponentAddress& address) const override;

	// reimplemented (icomp::IRegistriesManager)
	virtual const IRegistry* GetRegistry(const CComponentAddress& address, const IRegistry* contextRegistryPtr = NULL) const override;

	// reimplemented (icomp::IComponentListProvider)
	virtual ComponentAddresses GetComponentAddresses(int typeFlag = CTF_ALL) const override;

	// reimplemented (icomp::IMetaInfoManager)
	virtual const IComponentStaticInfo* GetComponentMetaInfo(const CComponentAddress& address) const override;
	virtual const IComponentStaticInfo* GetPackageMetaInfo(const QByteArray& packageId) const override;
};


} // namespace icomp


#endif // !icomp_CEnvironmentManagerBase_included


