#ifndef ipackage_CRegistriesManagerComp_included
#define ipackage_CRegistriesManagerComp_included


// Qt includes
#include <QtCore/QMap>
#include <QtCore/QDir>

// ACF includes
#include <istd/TDelPtr.h>
#include <icomp/IExtPackagesManager.h>
#include <icomp/IRegistriesManager.h>
#include <icomp/IRegistryLoader.h>
#include <icomp/CComponentBase.h>
#include <ilog/TLoggerCompWrap.h>
#include <ifile/IFilePersistence.h>
#include <ifile/IFileNameParam.h>


namespace ipackage
{


/**
	Manages registries and composed component.
	It allows simple management of packages without loading of real packages.
*/
class CRegistriesManagerComp:
			public ilog::CLoggerComponentBase,
			virtual public icomp::IExtPackagesManager,
			virtual public icomp::IRegistriesManager,
			virtual public icomp::IRegistryLoader
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

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
		I_ASSIGN(m_ignoreRealPackagesAttrPtr, "AreRealPackagesIgnored", "If set to true, all real packages will be ignored", true, false);
	I_END_COMPONENT;

	// reimplemented (icomp::IExtPackagesManager)
	virtual PathList GetConfigurationPathList(PathType pathType) const override;
	virtual int GetPackageDefinitionLevel(const QByteArray& packageId) const override;

	// reimplemented (icomp::IPackagesManager)
	virtual bool LoadPackages(const QString& configFilePath = QString()) override;
	virtual int GetPackageType(const QByteArray& packageId) const override;
	virtual QString GetPackagePath(const QByteArray& packageId) const override;
	virtual QString GetRegistryPath(const icomp::CComponentAddress& address) const override;

	// reimplemented (icomp::IRegistriesManager)
	virtual const icomp::IRegistry* GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr = NULL) const override;

	// reimplemented (icomp::IComponentListProvider)
	virtual ComponentAddresses GetComponentAddresses(int typeFlag = CTF_ALL) const override;

	// reimplemented (icomp::IRegistryLoader)
	virtual const icomp::IRegistry* GetRegistryFromFile(const QString& path) const override;

protected:
	void RegisterPackageFile(const QString& file, int definitionLevel);
	void RegisterPackagesDir(const QString& subDir, int definitionLevel);
	bool LoadConfigFile(const QString& configFile, int definitionLevel);

	bool CheckAndMarkPath(PathList& pathList, const QDir& directory, const QString& path, QString& resultPath) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	struct RealPackageInfo
	{
		QString filePath;
		int definitionLevel;
	};
	/**
		Map package ID to package file path.
	*/
	typedef QMap<QByteArray, RealPackageInfo> RealPackagesMap;
	RealPackagesMap m_realPackagesMap;

	typedef QMap<QByteArray, QString> ComponentIdToRegistryFileMap;
	struct CompositePackageInfo
	{
		QDir directory;
		ComponentIdToRegistryFileMap componentIdToRegistryFileMap;
		int definitionLevel;
	};
	/**
		Map package ID to directory.
	*/
	typedef QMap<QByteArray, CompositePackageInfo> CompositePackagesMap;
	CompositePackagesMap m_compositePackagesMap;

	typedef istd::TDelPtr<icomp::IRegistry> RegistryPtr;
	typedef QMap<QString, RegistryPtr> RegistriesMap;

	mutable RegistriesMap m_registriesMap;

	mutable PathList m_usedConfigFilesList;
	mutable PathList m_usedPackageDirsList;
	mutable PathList m_usedPackageFilesList;
	mutable PathList m_usedRegistryFilesList;

	I_REF(ifile::IFilePersistence, m_registryLoaderCompPtr);
	I_REF(ifile::IFileNameParam, m_configFilePathCompPtr);
	I_ATTR(bool, m_ignoreRealPackagesAttrPtr);
};


} // namespace ipackage


#endif // !ipackage_CRegistriesManagerComp_included


