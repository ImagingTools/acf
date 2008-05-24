#ifndef iqt_CPackagesLoader_included
#define iqt_CPackagesLoader_included


#include "istd/TDelPtr.h"

#include "icomp/CPackageStaticInfo.h"

#include "iqt/CDllFunctionsProvider.h"


namespace iqt
{


/**
	Loads component packages from dynamic link libraries.
*/
class CPackagesLoader: public icomp::CPackageStaticInfo
{
public:
	bool RegisterPackageFile(const istd::CString& file, const istd::CString& baseDir = "", bool beQuiet = true);
	bool RegisterPackagesDir(const istd::CString& subDir, const istd::CString& baseDir = "", bool beQuiet = true);
	bool LoadConfigFile(const istd::CString& configFile, const istd::CString& baseDir = "");

protected:
	CDllFunctionsProvider& GetProviderRef(const istd::CString& file, const istd::CString& baseDir, bool beQuiet = true);

private:
	typedef istd::TDelPtr<CDllFunctionsProvider> FunctionsProviderPtr;
	typedef std::map<QString, FunctionsProviderPtr> DllCacheMap;

	DllCacheMap m_dllCacheMap;
};


} // namespace iqt


#endif // !iqt_CPackagesLoader_included


