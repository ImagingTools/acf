#ifndef iqt_CPackagesLoader_included
#define iqt_CPackagesLoader_included


#include "istd/TDelPtr.h"

#include "icomp/CPackageStaticInfo.h"

#include "iqt/CDllFunctionsProvider.h"


namespace iqt
{


/**
	Loads packages from dynamic link libraries.
*/
class CPackagesLoader: public icomp::CPackageStaticInfo
{
public:
	bool RegisterPackageFile(const istd::CString& file, bool beQuiet = true);
	bool RegisterPackagesDir(const istd::CString& directory, bool beQuiet = true);
	bool LoadConfigFile(const istd::CString& configFile);

protected:
	CDllFunctionsProvider& GetProviderRef(const istd::CString& file, bool beQuiet = true);

private:
	typedef istd::TDelPtr<CDllFunctionsProvider> FunctionsProviderPtr;
	typedef ::std::map<QString, FunctionsProviderPtr> DllCacheMap;

	DllCacheMap m_dllCacheMap;
};


} // namespace iqt


#endif // !iqt_CPackagesLoader_included


