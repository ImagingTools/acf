#ifndef iqt_CPackagesLoaderComp_included
#define iqt_CPackagesLoaderComp_included


// Qt includes
#include <QFileInfo>
#include <QDir>

// STL includes
#include <map>

#include "istd/TDelPtr.h"

#include "iser/IFileLoader.h"

#include "icomp/IRegistry.h"
#include "icomp/IRegistriesManager.h"
#include "icomp/CPackageStaticInfo.h"
#include "icomp/CComponentBase.h"
#include "icomp/CCompositePackageStaticInfo.h"

#include "iqt/CDllFunctionsProvider.h"


namespace iqt
{


/**
	Loads component packages from dynamic link libraries.
*/
class CPackagesLoaderComp: public icomp::CComponentBase, public icomp::CPackageStaticInfo, virtual public icomp::IRegistriesManager
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CPackagesLoaderComp)
		I_REGISTER_INTERFACE(icomp::IComponentStaticInfo)
		I_REGISTER_INTERFACE(icomp::IRegistriesManager)
		I_ASSIGN(m_registryLoaderCompPtr, "RegistryLoader", "Loader used to read registry", true, "RegistryLoader")
	I_END_COMPONENT

	bool RegisterPackageFile(const istd::CString& file, bool beQuiet = true);
	bool RegisterPackagesDir(const istd::CString& subDir, bool beQuiet = true);
	bool LoadConfigFile(const istd::CString& configFile);

	const icomp::IRegistry* GetRegistryFromFile(const istd::CString& path) const;

	// reimplemented (icomp::IRegistriesManager)
	virtual const icomp::IRegistry* GetRegistry(const icomp::CComponentAddress& address) const;

protected:
	CDllFunctionsProvider& GetProviderRef(const QFileInfo& fileInfo, bool beQuiet = true);

private:
	typedef istd::TDelPtr<CDllFunctionsProvider> FunctionsProviderPtr;
	typedef std::map<QString, FunctionsProviderPtr> DllCacheMap;

	DllCacheMap m_dllCacheMap;

	struct CompositePackageInfo
	{
		istd::TDelPtr<icomp::CCompositePackageStaticInfo> staticInfoPtr;
		QDir directory;
	};
	/**
		Map package ID to structure CompositePackageInfo.
	*/
	typedef std::map<std::string, CompositePackageInfo> CompositePackagesMap;

	CompositePackagesMap m_compositePackagesMap;

	typedef istd::TDelPtr<icomp::IRegistry> RegistryPtr;
	typedef std::map<istd::CString, RegistryPtr> RegistriesMap;
	typedef std::map<const icomp::IRegistry*, QFileInfo> InvRegistriesMap;

	mutable RegistriesMap m_registriesMap;
	mutable InvRegistriesMap m_invRegistriesMap;

	I_REF(iser::IFileLoader, m_registryLoaderCompPtr);
};


} // namespace iqt


#endif // !iqt_CPackagesLoaderComp_included


