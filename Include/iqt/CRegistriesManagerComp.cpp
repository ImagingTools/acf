#include "iqt/CRegistriesManagerComp.h"


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>

// ACF includes
#include "iser/CXmlFileReadArchive.h"

#include "icomp/CRegistry.h"

#include "iqt/CSystem.h"


namespace iqt
{


// reimplemented (icomp::IPackagesManager)

bool CRegistriesManagerComp::LoadPackages(const QString& configFilePath)
{
	bool retVal = true;

	if (!configFilePath.isEmpty()){
		SendVerboseMessage(QString("Configure component environment using ") + configFilePath);
	
		retVal = retVal && LoadConfigFile(configFilePath);
	}
	else{
		SendVerboseMessage("Configure component environment using default configuration");

		if (!LoadConfigFile("Default.xpc")){
			QDir applicationDir = QCoreApplication::applicationDirPath();
			if (!LoadConfigFile(applicationDir.absoluteFilePath("Default.xpc"))){
				RegisterPackagesDir(applicationDir.absolutePath());
			}
		}
	}

	return retVal;
}


int CRegistriesManagerComp::GetPackageType(const QByteArray& packageId) const
{
	RealPackagesMap::const_iterator foundNormalIter = m_realPackagesMap.find(packageId);
	if (foundNormalIter != m_realPackagesMap.end()){
		return PT_REAL;
	}

	CompositePackagesMap::const_iterator foundCompositeIter = m_compositePackagesMap.find(packageId);
	if (foundCompositeIter != m_compositePackagesMap.end()){
		return PT_COMPOSED;
	}

	return PT_UNKNOWN;
}


QString CRegistriesManagerComp::GetPackagePath(const QByteArray& packageId) const
{
	RealPackagesMap::const_iterator foundNormalIter = m_realPackagesMap.find(packageId);
	if (foundNormalIter != m_realPackagesMap.end()){
		return foundNormalIter.value();
	}

	CompositePackagesMap::const_iterator foundCompositeIter = m_compositePackagesMap.find(packageId);
	if (foundCompositeIter != m_compositePackagesMap.end()){
		return foundCompositeIter.value().absolutePath();
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

	CompositePackagesMap::const_iterator foundCompositeIter = m_compositePackagesMap.find(address.GetPackageId());
	if (foundCompositeIter != m_compositePackagesMap.end()){
		QString filePath = foundCompositeIter.value().absoluteFilePath(QString(address.GetComponentId()) + ".arx");

		return GetRegistryFromFile(filePath);
	}

	return NULL;
}


// reimplemented (icomp::IRegistryLoader)

const icomp::IRegistry* CRegistriesManagerComp::GetRegistryFromFile(const QString& path) const
{
	QFileInfo fileInfo(path);
	QString correctedPath = fileInfo.canonicalFilePath();

	RegistriesMap::const_iterator iter = m_registriesMap.find(correctedPath);

	if (iter != m_registriesMap.end()){
		return iter.value().GetPtr();
	}

	RegistryPtr& mapValue = m_registriesMap[correctedPath];
	if (m_registryLoaderCompPtr.IsValid()){
		istd::TDelPtr<icomp::IRegistry> newRegistryPtr(new icomp::CRegistry());
		if (m_registryLoaderCompPtr->LoadFromFile(*newRegistryPtr, correctedPath) == iser::IFileLoader::StateOk){
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

	SendVerboseMessage(QString("Register package file: ") + file);

	if (fileInfo.isFile()){
		RealPackagesMap::const_iterator foundIter = m_realPackagesMap.find(packageId);
		if (foundIter == m_realPackagesMap.end()){
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
	else if (fileInfo.isDir()){
		CompositePackagesMap::const_iterator foundIter = m_compositePackagesMap.find(packageId);
		if (foundIter == m_compositePackagesMap.end()){
			QStringList filters;
			filters.append("*.arx");
			QDir packageDir(fileInfo.canonicalFilePath());
			QStringList componentFiles = packageDir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);
			for (		QStringList::iterator iter = componentFiles.begin();
						iter != componentFiles.end();
						++iter){
				QString correctedPath;

				CheckAndMarkPath(m_usedRegistryFilesList, packageDir, *iter, correctedPath);
			}

			m_compositePackagesMap[packageId] = packageDir;
		}
		else if (foundIter.value() != fileInfo.canonicalFilePath()){
			SendWarningMessage(
						MI_CANNOT_REGISTER,
						QObject::tr("Second composed package definition was ignored %1 (previous: %2)")
									.arg(fileInfo.canonicalFilePath())
									.arg(foundIter.value().canonicalPath()));
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

	iser::CXmlFileReadArchive archive(configFilePath);

	bool retVal = true;

	iser::CArchiveTag packageDirsTag("PackageDirs", "List of package directories", true);
	iser::CArchiveTag dirPathTag("Dir", "List of package directories", true);
	iser::CArchiveTag packageFilesTag("PackageFiles", "List of package files", true);
	iser::CArchiveTag filePathTag("FilePath", "Path of single file", true);

	iser::CArchiveTag configFilesTag("ConfigFiles", "List of included config files", true);

	int configFilesCount = 0;

	retVal = retVal && archive.BeginMultiTag(configFilesTag, filePathTag, configFilesCount);

	if (!retVal){
		SendVerboseMessage(QObject::tr("Load of configuration file: %1 failed").arg(configFilePath));
	
		return false;
	}

	for (int i = 0; i < configFilesCount; ++i){
		retVal = retVal && archive.BeginTag(filePathTag);
		QString filePath;
		retVal = retVal && archive.Process(filePath);
		QString enrolledPath = iqt::CSystem::GetEnrolledPath(filePath);
		LoadConfigFile(baseDir.absoluteFilePath(enrolledPath));

		retVal = retVal && archive.EndTag(filePathTag);
	}

	retVal = retVal && archive.EndTag(configFilesTag);


	int dirsCount = 0;
	retVal = retVal && archive.BeginMultiTag(packageDirsTag, dirPathTag, dirsCount);

	if (!retVal){
		SendVerboseMessage(QObject::tr("Load of configuration file: %1 failed").arg(configFilePath));

		return false;
	}

	for (int i = 0; i < dirsCount; ++i){
		retVal = retVal && archive.BeginTag(dirPathTag);
		QString dirPath;
		retVal = retVal && archive.Process(dirPath);
		QString correctedPath;
		if (retVal && CheckAndMarkPath(m_usedPackageDirsList, baseDir, dirPath, correctedPath)){
			RegisterPackagesDir(correctedPath);
		}

		retVal = retVal && archive.EndTag(dirPathTag);
	}

	retVal = retVal && archive.EndTag(packageDirsTag);

	int filesCount = 0;
	retVal = retVal && archive.BeginMultiTag(packageFilesTag, filePathTag, filesCount);

	if (!retVal){
		SendVerboseMessage(QObject::tr("Load of configuration file: %1 failed").arg(configFilePath));

		return false;
	}

	for (int i = 0; i < filesCount; ++i){
		retVal = retVal && archive.BeginTag(filePathTag);
		QString filePath;
		retVal = retVal && archive.Process(filePath);
		QString correctedPath;
		if (retVal && CheckAndMarkPath(m_usedPackageFilesList, baseDir, filePath, correctedPath)){
			RegisterPackageFile(correctedPath);
		}

		retVal = retVal && archive.EndTag(filePathTag);
	}

	retVal = retVal && archive.EndTag(packageFilesTag);

	if (!retVal){
		SendVerboseMessage(QObject::tr("Load of configuration file: %1 failed").arg(configFilePath));

		return false;
	}

	return retVal;
}


bool CRegistriesManagerComp::CheckAndMarkPath(PathList& pathList, const QDir& directory, const QString& path, QString& resultPath) const
{
	QString enrolledPath = iqt::CSystem::GetEnrolledPath(path);
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

			QString enrolledPath = iqt::CSystem::GetEnrolledPath(path);

			LoadConfigFile(applicationDir.absoluteFilePath(enrolledPath));
		}
	}
}


} // namespace iqt


