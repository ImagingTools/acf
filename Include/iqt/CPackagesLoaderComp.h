#ifndef iqt_CPackagesLoaderComp_included
#define iqt_CPackagesLoaderComp_included


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QLibrary>

// STL includes
#include <map>

// ACF includes
#include "istd/TDelPtr.h"

#include "iser/IFileLoader.h"

#include "icomp/IRegistryLoader.h"
#include "icomp/CRegistry.h"
#include "icomp/CEnvironmentManagerBase.h"
#include "icomp/CComponentBase.h"
#include "icomp/CCompositePackageStaticInfo.h"

#include "ibase/TLoggerCompWrap.h"

#include "iprm/IFileNameParam.h"


namespace iqt
{


/**
	Loads component packages from dynamic link libraries.
*/
class CPackagesLoaderComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			public icomp::CEnvironmentManagerBase,
			virtual public icomp::IRegistryLoader
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef icomp::CEnvironmentManagerBase BaseClass2;

	enum MessageId
	{
		MI_CANNOT_REGISTER = 650,
		MI_CANNOT_CREATE_ELEMENT
	};

	I_BEGIN_COMPONENT(CPackagesLoaderComp);
		I_REGISTER_INTERFACE(icomp::IRegistryLoader);
		I_REGISTER_INTERFACE(icomp::IComponentEnvironmentManager);
		I_REGISTER_INTERFACE(icomp::IComponentStaticInfo);
		I_REGISTER_INTERFACE(icomp::IPackagesManager);
		I_REGISTER_INTERFACE(icomp::IRegistriesManager);
		I_REGISTER_INTERFACE(icomp::IMetaInfoManager);
		I_ASSIGN(m_registryLoaderCompPtr, "RegistryLoader", "Loader used to read registry", true, "RegistryLoader");
		I_ASSIGN(m_configFilePathCompPtr, "ConfigFilePath", "Path of packages configuration file will be loaded, if enabled", false, "ConfigFilePath");
	I_END_COMPONENT;

	// reimplemented (icomp::IRegistryLoader)
	virtual const icomp::IRegistry* GetRegistryFromFile(const QString& path) const;

	// reimplemented (icomp::IComponentEnvironmentManager)
	virtual QString GetConfigFilePath() const;

	// reimplemented (icomp::IPackagesManager)
	virtual bool LoadPackages(const QString& configFilePath = QString());
	virtual int GetPackageType(const std::string& packageId) const;
	virtual QString GetPackagePath(const std::string& packageId) const;

	// reimplemented (icomp::IRegistriesManager)
	virtual const icomp::IRegistry* GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr = NULL) const;

	// reimplemented (icomp::IMetaInfoManager)
	virtual ComponentAddresses GetComponentAddresses(int typeFlag = CTF_ALL) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

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

	virtual bool RegisterPackageFile(const QString& file);
	virtual bool RegisterPackagesDir(const QString& subDir);
	virtual bool LoadConfigFile(const QString& configFile);

	QLibrary& GetLibrary(const QFileInfo& fileInfo);

	bool CheckAndMarkPath(const QDir& directory, const QString& path, QString& resultPath) const;

private:
	typedef istd::TDelPtr<QLibrary> LibraryPtr;
	typedef std::map<QString, LibraryPtr> DllCacheMap;

	DllCacheMap m_dllCacheMap;

	struct CompositePackageInfo
	{
		istd::TDelPtr<icomp::CCompositePackageStaticInfo> staticInfoPtr;
		QDir directory;
	};
	/**
		Map package ID to package file path.
	*/
	typedef std::map<std::string, QString> RealPackagesMap;
	RealPackagesMap m_realPackagesMap;

	/**
		Map package ID to structure CompositePackageInfo.
	*/
	typedef std::map<std::string, CompositePackageInfo> CompositePackagesMap;
	CompositePackagesMap m_compositePackagesMap;

	typedef istd::TDelPtr<icomp::IRegistry> RegistryPtr;
	typedef std::map<QString, RegistryPtr> RegistriesMap;
	typedef std::map<const icomp::IRegistry*, QFileInfo> InvRegistriesMap;

	mutable RegistriesMap m_registriesMap;
	mutable InvRegistriesMap m_invRegistriesMap;

	/**
		List of pathes.
	*/
	typedef std::set<QString> PathList;
	mutable PathList m_usedFilesList;

	QString m_configFilePath;

	I_REF(iser::IFileLoader, m_registryLoaderCompPtr);
	I_REF(iprm::IFileNameParam, m_configFilePathCompPtr);
};


} // namespace iqt


#endif // !iqt_CPackagesLoaderComp_included


