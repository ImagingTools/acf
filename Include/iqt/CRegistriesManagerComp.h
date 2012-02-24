#ifndef iqt_CRegistriesManagerComp_included
#define iqt_CRegistriesManagerComp_included


// Qt includes
#include <QDir>

// STL includes
#include <map>

#include "istd/TDelPtr.h"

#include "iser/IFileLoader.h"

#include "icomp/IExtPackagesManager.h"
#include "icomp/IRegistriesManager.h"
#include "icomp/IRegistryLoader.h"
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
			virtual public icomp::IExtPackagesManager,
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

	I_BEGIN_COMPONENT(CRegistriesManagerComp);
		I_REGISTER_INTERFACE(icomp::IExtPackagesManager);
		I_REGISTER_INTERFACE(icomp::IPackagesManager);
		I_REGISTER_INTERFACE(icomp::IRegistriesManager);
		I_REGISTER_INTERFACE(icomp::IRegistryLoader);
		I_ASSIGN(m_registryLoaderCompPtr, "RegistryLoader", "Loader used to read registry", true, "RegistryLoader");
		I_ASSIGN(m_configFilePathCompPtr, "ConfigFilePath", "Path of packages configuration file will be loaded, if enabled", false, "ConfigFilePath");
	I_END_COMPONENT;

	// reimplemented (icomp::IPackagesManager)
	virtual bool LoadPackages(const QString& configFilePath = QString());
	virtual int GetPackageType(const std::string& packageId) const;
	virtual QString GetPackagePath(const std::string& packageId) const;

	// reimplemented (icomp::IExtRegistriesManager)
	virtual PathList GetConfigurationPathList(PathType pathType) const;

	// reimplemented (icomp::IRegistriesManager)
	virtual const icomp::IRegistry* GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr = NULL) const;

	// reimplemented (icomp::IRegistryLoader)
	virtual const icomp::IRegistry* GetRegistryFromFile(const QString& path) const;

protected:
	void RegisterPackageFile(const QString& file);
	void RegisterPackagesDir(const QString& subDir);
	bool LoadConfigFile(const QString& configFile);

	bool CheckAndMarkPath(PathList& pathList, const QDir& directory, const QString& path, QString& resultPath) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	/**
		Map package ID to package file path.
	*/
	typedef std::map<std::string, QString> RealPackagesMap;
	RealPackagesMap m_realPackagesMap;

	/**
		Map package ID to directory.
	*/
	typedef std::map<std::string, QDir> CompositePackagesMap;
	CompositePackagesMap m_compositePackagesMap;

	typedef istd::TDelPtr<icomp::IRegistry> RegistryPtr;
	typedef std::map<QString, RegistryPtr> RegistriesMap;

	mutable RegistriesMap m_registriesMap;

	mutable PathList m_usedConfigFilesList;
	mutable PathList m_usedPackageDirsList;
	mutable PathList m_usedPackageFilesList;
	mutable PathList m_usedRegistryFilesList;

	I_REF(iser::IFileLoader, m_registryLoaderCompPtr);
	I_REF(iprm::IFileNameParam, m_configFilePathCompPtr);
};


} // namespace iqt


#endif // !iqt_CRegistriesManagerComp_included


