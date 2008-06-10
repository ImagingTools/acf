#ifndef iqt_CPackagesLoaderComp_included
#define iqt_CPackagesLoaderComp_included


// Qt includes
#include <QFileInfo>

#include "istd/TDelPtr.h"

#include "icomp/IRegistriesManager.h"
#include "icomp/CPackageStaticInfo.h"
#include "icomp/CComponentBase.h"

#include "iqt/CDllFunctionsProvider.h"
#include "iqt/CCompositePackageStaticInfo.h"


namespace iqt
{


/**
	Loads component packages from dynamic link libraries.
*/
class CPackagesLoaderComp: public icomp::CComponentBase, public icomp::CPackageStaticInfo
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CPackagesLoaderComp)
		I_REGISTER_INTERFACE(icomp::IComponentStaticInfo)
		I_REGISTER_INTERFACE(icomp::CComponentStaticInfoBase)
		I_ASSIGN(m_registriesManagerCompPtr, "RegistriesManager", "Manger of registries used to load composite components", false, "RegistriesManager");
	I_END_COMPONENT

	bool RegisterPackageFile(const istd::CString& file, bool beQuiet = true);
	bool RegisterPackagesDir(const istd::CString& subDir, bool beQuiet = true);
	bool LoadConfigFile(const istd::CString& configFile);

protected:
	CDllFunctionsProvider& GetProviderRef(const QFileInfo& fileInfo, bool beQuiet = true);

private:
	typedef istd::TDelPtr<CDllFunctionsProvider> FunctionsProviderPtr;
	typedef std::map<QString, FunctionsProviderPtr> DllCacheMap;

	DllCacheMap m_dllCacheMap;

	typedef istd::TDelPtr<CCompositePackageStaticInfo> CompositePackageInfoPtr;
	typedef std::map<istd::CString, CompositePackageInfoPtr> CompositePackagesMap;

	CompositePackagesMap m_compositePackagesMap;

	I_REF(icomp::IRegistriesManager, m_registriesManagerCompPtr);
};


} // namespace iqt


#endif // !iqt_CPackagesLoaderComp_included


