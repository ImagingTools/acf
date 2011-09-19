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
	virtual istd::CString GetConfigFilePath() const;

	// reimplemented (icomp::IPackagesManager)
	virtual bool LoadPackages(const istd::CString& configFilePath = istd::CString::GetEmpty());
	virtual int GetPackageType(const std::string& packageId) const;
	virtual istd::CString GetPackagePath(const std::string& packageId) const;

	// reimplemented (icomp::IRegistriesManager)
	virtual const IRegistry* GetRegistry(const CComponentAddress& address, const IRegistry* contextRegistryPtr = NULL) const;

	// reimplemented (icomp::IMetaInfoManager)
	virtual ComponentAddresses GetComponentAddresses(int typeFlag = CTF_ALL) const;
	virtual const IComponentStaticInfo* GetComponentMetaInfo(const CComponentAddress& address) const;
	virtual const IComponentStaticInfo* GetPackageMetaInfo(const std::string& packageId) const;
};


} // namespace icomp


#endif // !icomp_CEnvironmentManagerBase_included


