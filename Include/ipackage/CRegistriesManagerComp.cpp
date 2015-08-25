#include "ipackage/CRegistriesManagerComp.h"


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>

// ACF includes
#include "istd/CSystem.h"
#include "icomp/CRegistry.h"
#include "icomp/CXpcModel.h"
#include "ifile/CXmlFileReadArchive.h"


namespace ipackage
{


// reimplemented (icomp::IPackagesManager)

bool CRegistriesManagerComp::LoadPackages(const QString& configFilePath)
{
	bool retVal = true;

	m_registriesMap.clear();
	m_compositePackagesMap.clear();
	m_realPackagesMap.clear();

	if (!configFilePath.isEmpty()){
		SendVerboseMessage(QString("Configure component environment using ") + configFilePath);
	
		retVal = retVal && LoadConfigFile(configFilePath);
	}
	else{
		SendVerboseMessage("Configure component environment using default configuration");

		if (!LoadConfigFile("Default.awc")){
			QDir applicationDir = QCoreApplication::applicationDirPath();
			if (!LoadConfigFile(applicationDir.absoluteFilePath("Default.awc"))){
				RegisterPackagesDir(applicationDir.absolutePath());
			}
		}
	}

	return retVal;
}


int CRegistriesManagerComp::GetPackageType(const QByteArray& packageId) const
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


QString CRegistriesManagerComp::GetPackagePath(const QByteArray& packageId) const
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


QString CRegistriesManagerComp::GetRegistryPath(const icomp::CComponentAddress& address) const
{
	CompositePackagesMap::ConstIterator foundPackageIter = m_compositePackagesMap.constFind(address.GetPackageId());
	if (foundPackageIter != m_compositePackagesMap.constEnd()){
		const CompositePackageInfo& packageInfo = foundPackageIter.value();

		ComponentIdToRegistryFileMap::ConstIterator foundComponentIter = packageInfo.componentIdToRegistryFileMap.constFind(address.GetComponentId());
		if (foundComponentIter != packageInfo.componentIdToRegistryFileMap.constEnd()){
			return foundComponentIter.value();
		}
	}

	return QString();
}


// reimplemented (icomp::IExtRegistriesManager)

icomp::IExtPackagesManager::PathList CRegistriesManagerComp::GetConfigurationPathList(PathType pathType) const
{
	switch (pathType){
	case PT_CONFIG:
		return m_usedConfigFilesList;

	case PT_PACKAGES_DIR:
		return m_usedPackageDirsList;

	case PT_PACKAGE:
		return m_usedPackageFilesList;

	case PT_REGISTRY:
		return m_usedRegistryFilesList;

	default:
		break;
	}

	PathList retVal;
	retVal += m_usedConfigFilesList;
	retVal += m_usedPackageDirsList;
	retVal += m_usedPackageFilesList;
	retVal += m_usedRegistryFilesList;

	return retVal;
}


// reimplemented (icomp::IRegistriesManager)

const icomp::IRegistry* CRegistriesManagerComp::GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr) const
{
	if ((contextRegistryPtr != NULL) && address.GetPackageId().isEmpty()){
		return contextRegistryPtr->GetEmbeddedRegistry(address.GetComponentId());
	}

	CompositePackagesMap::ConstIterator foundPackageIter = m_compositePackagesMap.constFind(address.GetPackageId());
	if (foundPackageIter != m_compositePackagesMap.constEnd()){
		const CompositePackageInfo& packageInfo = foundPackageIter.value();

		ComponentIdToRegistryFileMap::ConstIterator foundComponentIter = packageInfo.componentIdToRegistryFileMap.constFind(address.GetComponentId());
		if (foundComponentIter != packageInfo.componentIdToRegistryFileMap.constEnd()){
			return GetRegistryFromFile(foundComponentIter.value());
		}
	}

	return NULL;
}


// reimplemented (icomp::IRegistryLoader)

const icomp::IRegistry* CRegistriesManagerComp::GetRegistryFromFile(const QString& path) const
{
	QFileInfo fileInfo(path);
	QString correctedPath = fileInfo.canonicalFilePath();

	RegistriesMap::ConstIterator iter = m_registriesMap.constFind(correctedPath);
	if (iter != m_registriesMap.constEnd()){
		return iter.value().GetPtr();
	}

	RegistryPtr& mapValue = m_registriesMap[correctedPath];
	if (m_registryLoaderCompPtr.IsValid()){
		istd::TDelPtr<icomp::IRegistry> newRegistryPtr(new icomp::CRegistry());
		if (m_registryLoaderCompPtr->LoadFromFile(*newRegistryPtr, correctedPath) == ifile::IFilePersistence::OS_OK){
			mapValue.TakeOver(newRegistryPtr);

			return mapValue.GetPtr();
		}
	}

	return NULL;
}


// protected methods

void CRegistriesManagerComp::RegisterPackageFile(const QString& file)
{
	QFileInfo fileInfo(file);

	QByteArray packageId(fileInfo.baseName().toLocal8Bit());

	if (fileInfo.isFile()){
		SendVerboseMessage(QString("Register real package %1 from file %2").arg(packageId.constData()).arg(file));

		RealPackagesMap::ConstIterator foundIter = m_realPackagesMap.constFind(packageId);
		if (foundIter == m_realPackagesMap.constEnd()){
			m_realPackagesMap[packageId] = fileInfo.canonicalFilePath();
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
		SendVerboseMessage(QString("Register composed package %1 from directory %2").arg(packageId.constData()).arg(file));

		CompositePackagesMap::ConstIterator foundIter = m_compositePackagesMap.constFind(packageId);
		if (foundIter == m_compositePackagesMap.constEnd()){
			CompositePackageInfo& packageInfo = m_compositePackagesMap[packageId];

			QStringList registryExtensions;
			m_registryLoaderCompPtr->GetFileExtensions(registryExtensions);

			QStringList filters;
			for (QStringList::ConstIterator extensionIter = registryExtensions.constBegin(); extensionIter != registryExtensions.constEnd(); ++extensionIter){
				filters.append("*." + *extensionIter);
			}

			QDir packageDir(fileInfo.canonicalFilePath());
			QStringList componentFiles = packageDir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);

			for (		QStringList::ConstIterator iter = componentFiles.constBegin();
						iter != componentFiles.constEnd();
						++iter){
				QFileInfo componentFileInfo(*iter);

				QByteArray componentId = componentFileInfo.baseName().toLocal8Bit();

				QString correctedPath;
				CheckAndMarkPath(m_usedRegistryFilesList, packageDir, *iter, correctedPath);

				packageInfo.componentIdToRegistryFileMap[componentId] = correctedPath;
			}

			packageInfo.directory = packageDir;
		}
		else if (foundIter.value().directory.canonicalPath() != fileInfo.canonicalFilePath()){
			SendWarningMessage(
						MI_CANNOT_REGISTER,
						QObject::tr("Second composed package definition was ignored %1 (previous: %2)")
									.arg(fileInfo.canonicalFilePath())
									.arg(foundIter.value().directory.canonicalPath()));
		}
	}
}


void CRegistriesManagerComp::RegisterPackagesDir(const QString& path)
{
	QDir packagesDir(path);

	SendVerboseMessage(QObject::tr("Register package directory: %1").arg(packagesDir.canonicalPath()));

	QStringList filters;
	filters.append("*.arp");
	QStringList filesInfo = packagesDir.entryList(filters, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
	for (		QStringList::iterator iter = filesInfo.begin();
				iter != filesInfo.end();
				++iter){
		QString filePath = packagesDir.absoluteFilePath(*iter);

		RegisterPackageFile(filePath);
	}
}


bool CRegistriesManagerComp::LoadConfigFile(const QString& configFile)
{
	QString correctedPath;
	if (!CheckAndMarkPath(m_usedConfigFilesList, QDir(), configFile, correctedPath)){
		return true;
	}

	QFileInfo fileInfo(correctedPath);

	QDir baseDir = fileInfo.absoluteDir();

	QString configFilePath = fileInfo.absoluteFilePath();

	SendVerboseMessage(QObject::tr("Load configuration file: %1").arg(configFilePath));

	ifile::CXmlFileReadArchive archive(configFilePath);

	icomp::CXpcModel configurationData;
	if (!configurationData.Serialize(archive)){
		SendErrorMessage(ifile::IFilePersistence::MI_CANNOT_LOAD, QObject::tr("Cannot open configuration file: %1").arg(configFilePath));

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
		if (CheckAndMarkPath(m_usedPackageDirsList, baseDir, configurationData.GetPackageDir(i), correctedPath)){
			RegisterPackagesDir(correctedPath);
		}
	}

	int packagesCount = configurationData.GetPackagesCount();
	for (int i = 0; i < packagesCount; ++i){
		QString correctedPath;
		if (CheckAndMarkPath(m_usedPackageFilesList, baseDir, configurationData.GetPackage(i), correctedPath)){
			RegisterPackageFile(correctedPath);
		}
	}

	return retVal;
}


bool CRegistriesManagerComp::CheckAndMarkPath(PathList& pathList, const QDir& directory, const QString& path, QString& resultPath) const
{
	QString enrolledPath = istd::CSystem::GetEnrolledPath(path);
	QString fullPath = QFileInfo(directory.absoluteFilePath(enrolledPath)).canonicalFilePath();

	if (pathList.find(fullPath) == pathList.end()){
		pathList.insert(fullPath);

		resultPath = fullPath;

		SendVerboseMessage(QString(">> new path: ") + resultPath + " (" + path + ")");

		return true;
	}

	return false;
}


// reimplemented (icomp::CComponentBase)

void CRegistriesManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_configFilePathCompPtr.IsValid()){
		QString path = m_configFilePathCompPtr->GetPath();

		if (!path.isEmpty()){
			QDir applicationDir = QCoreApplication::applicationDirPath();

			QString enrolledPath = istd::CSystem::GetEnrolledPath(path);

			LoadConfigFile(applicationDir.absoluteFilePath(enrolledPath));
		}
	}
}


} // namespace ipackage


