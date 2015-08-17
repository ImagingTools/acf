#include "ipackage/CPackagesLoaderComp.h"


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>

// ACF includes
#include "istd/CChangeNotifier.h"
#include "istd/CSystem.h"
#include "icomp/CXpcModel.h"
#include "ifile/CXmlFileReadArchive.h"


namespace ipackage
{


// reimplemented (icomp::IRegistryLoader)

const icomp::IRegistry* CPackagesLoaderComp::GetRegistryFromFile(const QString& path) const
{
	QFileInfo fileInfo(path);
	QString correctedPath = fileInfo.canonicalFilePath();

	RegistriesMap::ConstIterator iter = m_registriesMap.constFind(correctedPath);
	if (iter != m_registriesMap.constEnd()){
		return iter.value().GetPtr();
	}

	RegistryPtr& mapValue = m_registriesMap[correctedPath];
	if (m_registryLoaderCompPtr.IsValid()){
		istd::TDelPtr<icomp::IRegistry> newRegistryPtr(new LogingRegistry(const_cast<CPackagesLoaderComp*>(this)));
		if (m_registryLoaderCompPtr->LoadFromFile(*newRegistryPtr, correctedPath) == ifile::IFilePersistence::OS_OK){
			mapValue.TakeOver(newRegistryPtr);
			m_invRegistriesMap[mapValue.GetPtr()] = fileInfo;

			return mapValue.GetPtr();
		}
	}

	return NULL;
}


// reimplemented (icomp::IComponentEnvironmentManager)

QString CPackagesLoaderComp::GetConfigFilePath() const
{
	return m_configFilePath;
}


// reimplemented (icomp::IPackagesManager)

bool CPackagesLoaderComp::LoadPackages(const QString& configFilePath)
{
	istd::CChangeNotifier notifier(this);

	m_configFilePath = configFilePath;

	if (m_configFilePath.isEmpty() && m_configFilePathCompPtr.IsValid()){
		QString path = m_configFilePathCompPtr->GetPath();

		if (!path.isEmpty()){
			QDir applicationDir = QCoreApplication::applicationDirPath();

			QString enrolledPath = istd::CSystem::GetEnrolledPath(path);

			m_configFilePath = applicationDir.absoluteFilePath(enrolledPath);
		}

		SendVerboseMessage(QString("Configure component environment using ") + configFilePath);
	}

	if (m_configFilePath.isEmpty()){
		SendVerboseMessage("Configure component environment using default configuration");

		m_configFilePath = "Default.xpc";
	}

	BaseClass2::Reset();

	m_registriesMap.clear();
	m_invRegistriesMap.clear();
	m_usedFilesList.clear();
	m_compositePackagesMap.clear();
	m_realPackagesMap.clear();
	m_libraryToInfoFuncMap.clear();

	bool retVal = LoadConfigFile(m_configFilePath);

	return retVal;
}


int CPackagesLoaderComp::GetPackageType(const QByteArray& packageId) const
{
	RealPackagesMap::ConstIterator foundNormalIter = m_realPackagesMap.constFind(packageId);
	if (foundNormalIter != m_realPackagesMap.constEnd()){
		return PT_REAL;
	}

	CompositePackagesMap::ConstIterator foundCompositeIter = m_compositePackagesMap.constFind(packageId);
	if (foundCompositeIter != m_compositePackagesMap.constEnd()){
		return PT_COMPOSED;
	}

	return PT_UNKNOWN;
}


QString CPackagesLoaderComp::GetPackagePath(const QByteArray& packageId) const
{
	RealPackagesMap::ConstIterator foundNormalIter = m_realPackagesMap.constFind(packageId);
	if (foundNormalIter != m_realPackagesMap.constEnd()){
		return foundNormalIter.value();
	}

	CompositePackagesMap::ConstIterator foundCompositeIter = m_compositePackagesMap.constFind(packageId);
	if (foundCompositeIter != m_compositePackagesMap.constEnd()){
		return foundCompositeIter.value().directory.absolutePath();
	}

	return QString();
}


// reimplemented (icomp::IRegistriesManager)

const icomp::IRegistry* CPackagesLoaderComp::GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr) const
{
	CompositePackagesMap::ConstIterator foundPackageIter = m_compositePackagesMap.constFind(address.GetPackageId());
	if (foundPackageIter != m_compositePackagesMap.constEnd()){
		const CompositePackageInfo& packageInfo = foundPackageIter.value();

		ComponentIdToRegistryFileMap::ConstIterator foundComponentIter = packageInfo.componentIdToRegistryFileMap.constFind(address.GetComponentId());
		if (foundComponentIter != packageInfo.componentIdToRegistryFileMap.constEnd()){
			return GetRegistryFromFile(foundComponentIter.value());
		}
	}

	return BaseClass2::GetRegistry(address, contextRegistryPtr);
}


// reimplemented (icomp::IMetaInfoManager)

CPackagesLoaderComp::ComponentAddresses CPackagesLoaderComp::GetComponentAddresses(int typeFlag) const
{
	ComponentAddresses retVal;

	if ((typeFlag & CTF_REAL) != 0){
		for (		RealPackagesMap::const_iterator packageIter = m_realPackagesMap.begin();
					packageIter != m_realPackagesMap.end();
					++packageIter){
			const QByteArray packageId = packageIter.key();

			const IComponentStaticInfo* packageInfoPtr = GetEmbeddedComponentInfo(packageId);
			if (packageInfoPtr != NULL){
				Ids componentIds = packageInfoPtr->GetMetaIds(MGI_EMBEDDED_COMPONENTS);

				icomp::CComponentAddress address;
				address.SetPackageId(packageId);

				for (		Ids::const_iterator componentIter = componentIds.begin();
							componentIter != componentIds.end();
							++componentIter){
					address.SetComponentId(*componentIter);

					retVal.insert(address);
				}
			}
		}
	}

	if ((typeFlag & CTF_COMPOSITE) != 0){
		for (		CompositePackagesMap::const_iterator packageIter = m_compositePackagesMap.begin();
					packageIter != m_compositePackagesMap.end();
					++packageIter){
			const QByteArray packageId = packageIter.key();

			const IComponentStaticInfo* packageInfoPtr = GetEmbeddedComponentInfo(packageId);
			if (packageInfoPtr != NULL){
				Ids componentIds = packageInfoPtr->GetMetaIds(MGI_EMBEDDED_COMPONENTS);

				icomp::CComponentAddress address;
				address.SetPackageId(packageId);

				for (		Ids::const_iterator componentIter = componentIds.begin();
							componentIter != componentIds.end();
							++componentIter){
					address.SetComponentId(*componentIter);

					retVal.insert(address);
				}
			}
		}
	}

	return retVal;
}


// reimplemented (icomp::CComponentBase)

void CPackagesLoaderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	LoadPackages();
}


// protected methods

bool CPackagesLoaderComp::RegisterPackageFile(const QString& file)
{
	QFileInfo fileInfo(file);

	QByteArray packageId(fileInfo.baseName().toLocal8Bit());

	if (fileInfo.isFile()){
		RealPackagesMap::ConstIterator foundIter = m_realPackagesMap.constFind(packageId);
		if (foundIter == m_realPackagesMap.constEnd()){
			icomp::GetPackageInfoFunc getInfoPtr = GetPackageFunction(fileInfo);
			if (getInfoPtr != NULL){
				icomp::CPackageStaticInfo* infoPtr = getInfoPtr();
				if (infoPtr != NULL){
					m_realPackagesMap[packageId] = fileInfo.canonicalFilePath();

					RegisterEmbeddedComponentInfo(packageId, infoPtr);

					return true;
				}
			}
		}
		else if (foundIter.value() != fileInfo.canonicalFilePath()){
			SendWarningMessage(
						MI_CANNOT_REGISTER,
						QObject::tr("Second real package definition was ignored %1 (previous: %2)")
									.arg(fileInfo.canonicalFilePath())
									.arg(foundIter.value()));
		}
	}
	else if (m_registryLoaderCompPtr.IsValid() && fileInfo.isDir()){
		CompositePackagesMap::ConstIterator foundIter = m_compositePackagesMap.constFind(packageId);
		if (foundIter == m_compositePackagesMap.constEnd()){
			CompositePackageInfo& packageInfo = m_compositePackagesMap[packageId];

			icomp::CCompositePackageStaticInfo* infoPtr = new icomp::CCompositePackageStaticInfo(
						packageId,
						this);
			if (infoPtr == NULL){
				return false;
			}

			QStringList registryExtensions;
			m_registryLoaderCompPtr->GetFileExtensions(registryExtensions);

			QStringList filters;
			for (QStringList::ConstIterator extensionIter = registryExtensions.constBegin(); extensionIter != registryExtensions.constEnd(); ++extensionIter){
				filters.append("*." + *extensionIter);
			}

			QDir packageDir(fileInfo.absoluteFilePath());
			QStringList componentFiles = packageDir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);

			for (		QStringList::ConstIterator iter = componentFiles.constBegin();
						iter != componentFiles.constEnd();
						++iter){
				QString fullPath = QFileInfo(packageDir.absoluteFilePath(istd::CSystem::GetEnrolledPath(*iter))).canonicalFilePath();

				QByteArray componentId = QFileInfo(fullPath).baseName().toLocal8Bit();

				infoPtr->RegisterEmbeddedComponent(componentId);

				packageInfo.componentIdToRegistryFileMap[componentId] = fullPath;
			}

			packageInfo.staticInfoPtr.SetPtr(infoPtr);
			packageInfo.directory = packageDir;

			QString metaInfoFile(packageDir.absoluteFilePath("General.xml"));
			ifile::CXmlFileReadArchive archive(metaInfoFile);
			if (!infoPtr->SerializeMeta(archive)){
				SendWarningMessage(
							ifile::IFilePersistence::MI_CANNOT_LOAD,
							QObject::tr("Cannot load meta description for registry %1 (%2)")
										.arg(QString(packageId))
										.arg(metaInfoFile));
			}

			RegisterEmbeddedComponentInfo(packageId, infoPtr);

			return true;
		}
		else if (foundIter.value().directory.canonicalPath() != fileInfo.canonicalFilePath()){
			SendWarningMessage(
						MI_CANNOT_REGISTER,
						QObject::tr("Second composed package definition was ignored %1 (previous: %2)")
									.arg(fileInfo.canonicalFilePath())
									.arg(foundIter.value().directory.canonicalPath()));
		}
	}

	return false;
}


bool CPackagesLoaderComp::RegisterPackagesDir(const QString& path)
{
	bool retVal = true;

	QDir packagesDir(path);

	QStringList filters;
	filters.append("*.arp");
	QStringList filesInfo = packagesDir.entryList(filters, QDir::NoSymLinks | QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
	for (		QStringList::iterator iter = filesInfo.begin();
				iter != filesInfo.end();
				++iter){
		QString filePath = packagesDir.absoluteFilePath(*iter);

		retVal = RegisterPackageFile(filePath) && retVal;
	}

	return retVal;
}


bool CPackagesLoaderComp::LoadConfigFile(const QString& configFile)
{
	QString correctedPath;
	if (!CheckAndMarkPath(QDir(), configFile, correctedPath)){
		return true;
	}

	QFileInfo fileInfo(correctedPath);

	QDir baseDir = fileInfo.absoluteDir();

	QString configFilePath = fileInfo.absoluteFilePath();

	SendVerboseMessage(tr("Load configuration file: %1").arg(configFilePath));

	ifile::CXmlFileReadArchive archive(configFilePath);

	icomp::CXpcModel configurationData;
	if (!configurationData.Serialize(archive)){
		SendErrorMessage(ifile::IFilePersistence::MI_CANNOT_LOAD, tr("Cannot open configuration file: %1").arg(configFilePath));

		return false;
	}

	bool retVal = true;

	int configFilesCount = configurationData.GetConfigFilesCount();
	for (int i = 0; i < configFilesCount; ++i){
		QString configFilePath = istd::CSystem::GetEnrolledPath(configurationData.GetConfFile(i));
		retVal = LoadConfigFile(baseDir.absoluteFilePath(configFilePath)) && retVal;
	}

	int packageDirsCount = configurationData.GetPackageDirsCount();
	for (int i = 0; i < packageDirsCount; ++i){
		QString correctedPath;
		if (CheckAndMarkPath(baseDir, configurationData.GetPackageDir(i), correctedPath)){
			RegisterPackagesDir(correctedPath);
		}
	}

	int packagesCount = configurationData.GetPackagesCount();
	for (int i = 0; i < packagesCount; ++i){
		QString correctedPath;
		if (CheckAndMarkPath(baseDir, configurationData.GetPackage(i), correctedPath)){
			RegisterPackageFile(correctedPath);
		}
	}

	return retVal;
}


icomp::GetPackageInfoFunc CPackagesLoaderComp::GetPackageFunction(const QFileInfo& fileInfo)
{
	QString absolutePath = fileInfo.canonicalFilePath();

	LibraryToInfoFuncMap::ConstIterator iter = m_libraryToInfoFuncMap.constFind(absolutePath);
	if (iter != m_libraryToInfoFuncMap.constEnd()){
		return iter.value();
	}

	QLibrary library(absolutePath);
	icomp::GetPackageInfoFunc getInfoPtr = (icomp::GetPackageInfoFunc)library.resolve(I_PACKAGE_EXPORT_FUNCTION_NAME);
	if (getInfoPtr == NULL){
		SendErrorMessage(
					MI_CANNOT_REGISTER,
					QObject::tr("Cannot register package %1 (%2)").arg(fileInfo.fileName()).arg(library.errorString()));
	}

	m_libraryToInfoFuncMap[absolutePath] = getInfoPtr;

	return getInfoPtr;
}


// public methods of embedded class LogingRegistry

CPackagesLoaderComp::LogingRegistry::LogingRegistry(CPackagesLoaderComp* parentPtr)
:	m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// reimplemented (icomp::IRegistry)

CPackagesLoaderComp::LogingRegistry::ElementInfo* CPackagesLoaderComp::LogingRegistry::InsertElementInfo(
			const QByteArray& elementId,
			const icomp::CComponentAddress& address,
			bool ensureElementCreated)
{
	ElementInfo* retVal = BaseClass::InsertElementInfo(elementId, address, ensureElementCreated);

	if (retVal == NULL){
		m_parent.SendErrorMessage(
					MI_CANNOT_CREATE_ELEMENT,
					QObject::tr("Cannot create %1 (%2)").
								arg(QString(elementId)).
								arg(address.ToString()));
	}

	return retVal;
}


bool CPackagesLoaderComp::CheckAndMarkPath(const QDir& directory, const QString& path, QString& resultPath) const
{
	QString fullPath = QFileInfo(directory.filePath(istd::CSystem::GetEnrolledPath(path))).canonicalFilePath();
	if (!fullPath.isEmpty() && (m_usedFilesList.find(fullPath) == m_usedFilesList.end())){
		m_usedFilesList.insert(fullPath);

		resultPath = fullPath;

		return true;
	}

	return false;
}


} // namespace ipackage


