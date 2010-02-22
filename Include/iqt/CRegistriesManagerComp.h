#ifndef iqt_CRegistriesManagerComp_included
#define iqt_CRegistriesManagerComp_included


// Qt includes
#include <QDir>

// STL includes
#include <map>

#include "istd/TDelPtr.h"

#include "iser/IFileLoader.h"

#include "icomp/IRegistryLoader.h"
#include "icomp/CRegistry.h"
#include "icomp/CEnvironmentManagerBase.h"
#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "iprm/IFileNameParam.h"


namespace iqt
{


/**
	Loads component packages from dynamic link libraries.
*/
class CRegistriesManagerComp:
			public ibase::CLoggerComponentBase,
			public icomp::IRegistriesManager,
			virtual public icomp::IRegistryLoader
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_CANNOT_REGISTER = 650,
		MI_CANNOT_CREATE_ELEMENT
	};

	I_BEGIN_COMPONENT(CRegistriesManagerComp);
		I_REGISTER_INTERFACE(icomp::IRegistryLoader);
		I_REGISTER_INTERFACE(icomp::IRegistriesManager);
		I_ASSIGN(m_registryLoaderCompPtr, "RegistryLoader", "Loader used to read registry", true, "RegistryLoader");
		I_ASSIGN(m_configFilePathCompPtr, "ConfigFilePath", "Path of packages configuration file will be loaded, if enabled", false, "ConfigFilePath");
	I_END_COMPONENT;

	bool ConfigureEnvironment(const istd::CString& configFilePath = istd::CString());

	// reimplemented (icomp::IRegistryLoader)
	virtual const icomp::IRegistry* GetRegistryFromFile(const istd::CString& path) const;

	// reimplemented (icomp::IRegistriesManager)
	virtual const icomp::IRegistry* GetRegistry(const icomp::CComponentAddress& address) const;
	virtual istd::CString GetPackageDirPath(const std::string& packageId) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

protected:
	void RegisterPackageFile(const istd::CString& file);
	void RegisterPackagesDir(const istd::CString& subDir);
	bool LoadConfigFile(const istd::CString& configFile);

	bool CheckAndMarkPath(const QDir& directory, const istd::CString& path, istd::CString& resultPath) const;

private:
	/**
		Map package ID to directory.
	*/
	typedef std::map<std::string, QDir> PackagesMap;

	PackagesMap m_compositePackagesMap;

	typedef istd::TDelPtr<icomp::IRegistry> RegistryPtr;
	typedef std::map<istd::CString, RegistryPtr> RegistriesMap;

	mutable RegistriesMap m_registriesMap;

	typedef std::set<istd::CString> UsedFilesList;
	mutable UsedFilesList m_usedFilesList;

	I_REF(iser::IFileLoader, m_registryLoaderCompPtr);
	I_REF(iprm::IFileNameParam, m_configFilePathCompPtr);
};


} // namespace iqt


#endif // !iqt_CRegistriesManagerComp_included


