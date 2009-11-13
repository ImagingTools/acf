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
#include "icomp/IRegistryLoader.h"
#include "icomp/CRegistry.h"
#include "icomp/CPackageStaticInfo.h"
#include "icomp/CComponentBase.h"
#include "icomp/CCompositePackageStaticInfo.h"

#include "ibase/TLoggerCompWrap.h"

#include "iprm/IFileNameParam.h"

#include "iqt/CDllFunctionsProvider.h"


namespace iqt
{


/**
	Loads component packages from dynamic link libraries.
*/
class CPackagesLoaderComp:
			public ibase::CLoggerComponentBase,
			public icomp::CPackageStaticInfo,
			virtual public icomp::IRegistriesManager,
			virtual public icomp::IRegistryLoader
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_CANNOT_REGISTER = 650,
		MI_CANNOT_CREATE_ELEMENT
	};

	I_BEGIN_COMPONENT(CPackagesLoaderComp)
		I_REGISTER_INTERFACE(icomp::IComponentStaticInfo)
		I_REGISTER_INTERFACE(icomp::IRegistriesManager)
		I_REGISTER_INTERFACE(icomp::IRegistryLoader)
		I_ASSIGN(m_registryLoaderCompPtr, "RegistryLoader", "Loader used to read registry", true, "RegistryLoader")
		I_ASSIGN(m_configFilePathCompPtr, "ConfigFilePath", "Path of packages configuration file will be loaded, if enabled", false, "ConfigFilePath")
	I_END_COMPONENT

	bool RegisterPackageFile(const istd::CString& file);
	bool RegisterPackagesDir(const istd::CString& subDir);
	bool LoadConfigFile(const istd::CString& configFile);

	// reimplemented (icomp::IRegistryLoader)
	virtual bool ConfigureEnvironment(const istd::CString& configFilePath = istd::CString());
	virtual istd::CString GetConfigFilePath() const;
	virtual const icomp::IRegistry* GetRegistryFromFile(const istd::CString& path) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

	// reimplemented (icomp::IRegistriesManager)
	virtual const icomp::IRegistry* GetRegistry(const icomp::CComponentAddress& address) const;
	virtual istd::CString GetPackageDirPath(const std::string& packageId) const;

protected:
	class LogingRegistry: public icomp::CRegistry
	{
	public:
		typedef icomp::CRegistry BaseClass;

		LogingRegistry(CPackagesLoaderComp* parentPtr);

		// reimplemented (icomp::IRegistry)
		virtual ElementInfo* InsertElementInfo(
					const std::string& elementId,
					const icomp::CComponentAddress& address,
					bool ensureElementCreated = true);

	private:
		CPackagesLoaderComp& m_parent;
	};

	CDllFunctionsProvider& GetProviderRef(const QFileInfo& fileInfo);

	bool CheckAndMarkPath(const QDir& directory, const istd::CString& path, istd::CString& resultPath) const;

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
		Map package ID to package file path.
	*/
	typedef std::map<std::string, istd::CString> NormalPackagesMap;
	/**
		Map package ID to structure CompositePackageInfo.
	*/
	typedef std::map<std::string, CompositePackageInfo> CompositePackagesMap;

	NormalPackagesMap m_normalPackagesMap;
	CompositePackagesMap m_compositePackagesMap;

	typedef istd::TDelPtr<icomp::IRegistry> RegistryPtr;
	typedef std::map<istd::CString, RegistryPtr> RegistriesMap;
	typedef std::map<const icomp::IRegistry*, QFileInfo> InvRegistriesMap;

	mutable RegistriesMap m_registriesMap;
	mutable InvRegistriesMap m_invRegistriesMap;

	typedef std::set<istd::CString> UsedFilesList;
	mutable UsedFilesList m_usedFilesList;

	istd::CString m_configFilePath;

	I_REF(iser::IFileLoader, m_registryLoaderCompPtr);
	I_REF(iprm::IFileNameParam, m_configFilePathCompPtr);
};


} // namespace iqt


#endif // !iqt_CPackagesLoaderComp_included


