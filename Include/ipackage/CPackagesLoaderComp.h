#ifndef ipackage_CPackagesLoaderComp_included
#define ipackage_CPackagesLoaderComp_included


// Qt includes
#include <QtCore/QMap>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QLibrary>

// ACF includes
#include "istd/TDelPtr.h"
#include "icomp/IRegistryLoader.h"
#include "icomp/CRegistry.h"
#include "icomp/CEnvironmentManagerBase.h"
#include "icomp/CComponentBase.h"
#include "icomp/CCompositePackageStaticInfo.h"
#include "icomp/export.h"
#include "ilog/TLoggerCompWrap.h"
#include "ifile/IFilePersistence.h"
#include "ifile/IFileNameParam.h"


namespace ipackage
{


/**
	Loads component packages from dynamic link libraries.
*/
class CPackagesLoaderComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			public icomp::CEnvironmentManagerBase,
			virtual public icomp::IRegistryLoader
{
	Q_OBJECT

public:
	typedef ilog::CLoggerComponentBase BaseClass;
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
	virtual int GetPackageType(const QByteArray& packageId) const;
	virtual QString GetPackagePath(const QByteArray& packageId) const;

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
					const QByteArray& elementId,
					const icomp::CComponentAddress& address,
					bool ensureElementCreated = true);

	private:
		CPackagesLoaderComp& m_parent;
	};

	virtual bool RegisterPackageFile(const QString& file);
	virtual bool RegisterPackagesDir(const QString& subDir);
	virtual bool LoadConfigFile(const QString& configFile);

	icomp::GetPackageInfoFunc GetPackageFunction(const QFileInfo& fileInfo);

	bool CheckAndMarkPath(const QDir& directory, const QString& path, QString& resultPath) const;

private:
	typedef QMap<QString, icomp::GetPackageInfoFunc> LibraryToInfoFuncMap;
	LibraryToInfoFuncMap m_libraryToInfoFuncMap;

	struct CompositePackageInfo
	{
		istd::TDelPtr<icomp::CCompositePackageStaticInfo> staticInfoPtr;
		QDir directory;
	};
	/**
		Map package ID to package file path.
	*/
	typedef QMap<QByteArray, QString> RealPackagesMap;
	RealPackagesMap m_realPackagesMap;

	/**
		Map package ID to structure CompositePackageInfo.
	*/
	typedef QMap<QByteArray, CompositePackageInfo> CompositePackagesMap;
	CompositePackagesMap m_compositePackagesMap;

	typedef istd::TDelPtr<icomp::IRegistry> RegistryPtr;
	typedef QMap<QString, RegistryPtr> RegistriesMap;
	typedef QMap<const icomp::IRegistry*, QFileInfo> InvRegistriesMap;

	mutable RegistriesMap m_registriesMap;
	mutable InvRegistriesMap m_invRegistriesMap;

	/**
		List of pathes.
	*/
	typedef QSet<QString> PathList;
	mutable PathList m_usedFilesList;

	QString m_configFilePath;

	I_REF(ifile::IFilePersistence, m_registryLoaderCompPtr);
	I_REF(ifile::IFileNameParam, m_configFilePathCompPtr);
};


} // namespace ipackage


#endif // !ipackage_CPackagesLoaderComp_included


