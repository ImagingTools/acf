#include "iqt/CRegistriesManagerComp.h"


// Qt includes
#include <QCoreApplication>
#include <QFileInfo>

// ACF includes
#include "iser/CXmlFileReadArchive.h"

#include "icomp/CRegistry.h"

#include "iqt/CFileSystem.h"


namespace iqt
{


// reimplemented (icomp::IPackagesManager)

bool CRegistriesManagerComp::LoadPackages(const istd::CString& configFilePath)
{
	bool retVal = true;

	if (!configFilePath.IsEmpty()){
		SendVerboseMessage(istd::CString("Configure component environment using ") + configFilePath);
	
		retVal = retVal && LoadConfigFile(configFilePath);
	}
	else{
		SendVerboseMessage("Configure component environment using default configuration");

		if (!LoadConfigFile(iqt::GetCString("Default.xpc"))){
			QDir applicationDir = QCoreApplication::applicationDirPath();
			if (!LoadConfigFile(iqt::GetCString(applicationDir.absoluteFilePath("Default.xpc")))){
				RegisterPackagesDir(iqt::GetCString(applicationDir.absolutePath()));
			}
		}
	}

	return retVal;
}


int CRegistriesManagerComp::GetPackageType(const std::string& packageId) const
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


istd::CString CRegistriesManagerComp::GetPackagePath(const std::string& packageId) const
{
	RealPackagesMap::const_iterator foundNormalIter = m_realPackagesMap.find(packageId);
	if (foundNormalIter != m_realPackagesMap.end()){
		return iqt::GetCString(foundNormalIter->second);
	}

	CompositePackagesMap::const_iterator foundCompositeIter = m_compositePackagesMap.find(packageId);
	if (foundCompositeIter != m_compositePackagesMap.end()){
		return iqt::GetCString(foundCompositeIter->second.absolutePath());
	}

	return istd::CString::GetEmpty();
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
	retVal.insert(m_usedConfigFilesList.begin(), m_usedConfigFilesList.end());
	retVal.insert(m_usedPackageDirsList.begin(), m_usedPackageDirsList.end());
	retVal.insert(m_usedPackageFilesList.begin(), m_usedPackageFilesList.end());
	retVal.insert(m_usedRegistryFilesList.begin(), m_usedRegistryFilesList.end());

	return retVal;
}


// reimplemented (icomp::IRegistriesManager)

const icomp::IRegistry* CRegistriesManagerComp::GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr) const
{
	if ((contextRegistryPtr != NULL) && address.GetPackageId().empty()){
		return contextRegistryPtr->GetEmbeddedRegistry(address.GetComponentId());
	}

	CompositePackagesMap::const_iterator foundCompositeIter = m_compositePackagesMap.find(address.GetPackageId());
	if (foundCompositeIter != m_compositePackagesMap.end()){
		QString filePath = foundCompositeIter->second.absoluteFilePath(QString(address.GetComponentId().c_str()) + ".arx");

		return GetRegistryFromFile(GetCString(filePath));
	}

	return NULL;
}


// reimplemented (icomp::IRegistryLoader)

const icomp::IRegistry* CRegistriesManagerComp::GetRegistryFromFile(const istd::CString& path) const
{
	QFileInfo fileInfo(iqt::GetQString(path));
	istd::CString correctedPath = iqt::GetCString(fileInfo.canonicalFilePath());

	RegistriesMap::const_iterator iter = m_registriesMap.find(correctedPath);

	if (iter != m_registriesMap.end()){
		return iter->second.GetPtr();
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

void CRegistriesManagerComp::RegisterPackageFile(const istd::CString& file)
{
	QFileInfo fileInfo(iqt::GetQString(file));

	std::string packageId(fileInfo.baseName().toStdString());

	SendVerboseMessage(istd::CString("Register package file: ") + file);

	if (fileInfo.isFile()){
		RealPackagesMap::const_iterator foundIter = m_realPackagesMap.find(packageId);
		if (foundIter == m_realPackagesMap.end()){
			m_realPackagesMap[packageId] = fileInfo.canonicalFilePath();
		}
		else if (foundIter->second != fileInfo.canonicalFilePath()){
			SendWarningMessage(
						MI_CANNOT_REGISTER,
						iqt::GetCString(QObject::tr("Second real package definition was ignored %1 (previous: %2)")
									.arg(fileInfo.canonicalFilePath())
									.arg(foundIter->second)));
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
				istd::CString correctedPath;

				CheckAndMarkPath(m_usedRegistryFilesList, packageDir, iqt::GetCString(*iter), correctedPath);
			}

			m_compositePackagesMap[packageId] = packageDir;
		}
		else if (foundIter->second != fileInfo.canonicalFilePath()){
			SendWarningMessage(
						MI_CANNOT_REGISTER,
						iqt::GetCString(QObject::tr("Second composed package definition was ignored %1 (previous: %2)")
									.arg(fileInfo.canonicalFilePath())
									.arg(foundIter->second.canonicalPath())));
		}
	}
}


void CRegistriesManagerComp::RegisterPackagesDir(const istd::CString& path)
{
	QDir packagesDir(GetQString(path));

	SendVerboseMessage(iqt::GetCString(QObject::tr("Register package directory: %1").arg(packagesDir.canonicalPath())));

	QStringList filters;
	filters.append("*.arp");
	QStringList filesInfo = packagesDir.entryList(filters, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
	for (		QStringList::iterator iter = filesInfo.begin();
				iter != filesInfo.end();
				++iter){
		istd::CString filePath = GetCString(packagesDir.absoluteFilePath(*iter));

		RegisterPackageFile(filePath);
	}
}


bool CRegistriesManagerComp::LoadConfigFile(const istd::CString& configFile)
{
	istd::CString correctedPath;
	if (!CheckAndMarkPath(m_usedConfigFilesList, QDir(), configFile, correctedPath)){
		return true;
	}

	QFileInfo fileInfo(iqt::GetQString(correctedPath));
	QDir baseDir = fileInfo.absoluteDir();
	QString configFilePath = fileInfo.absoluteFilePath();

	SendVerboseMessage(iqt::GetCString(QObject::tr("Load configuration file: %1").arg(configFilePath)));

	iser::CXmlFileReadArchive archive(GetCString(configFilePath));

	bool retVal = true;

	iser::CArchiveTag packageDirsTag("PackageDirs", "List of package directories", true);
	iser::CArchiveTag dirPathTag("Dir", "List of package directories", true);
	iser::CArchiveTag packageFilesTag("PackageFiles", "List of package files", true);
	iser::CArchiveTag filePathTag("FilePath", "Path of single file", true);

	iser::CArchiveTag configFilesTag("ConfigFiles", "List of included config files", true);

	int configFilesCount = 0;

	retVal = retVal && archive.BeginMultiTag(configFilesTag, filePathTag, configFilesCount);

	if (!retVal){
		SendVerboseMessage(iqt::GetCString(QObject::tr("Load of configuration file: %1 failed").arg(configFilePath)));
	
		return false;
	}

	for (int i = 0; i < configFilesCount; ++i){
		retVal = retVal && archive.BeginTag(filePathTag);
		istd::CString filePath;
		retVal = retVal && archive.Process(filePath);
		QString enrolledPath = iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(filePath));
		LoadConfigFile(iqt::GetCString(baseDir.absoluteFilePath(enrolledPath)));

		retVal = retVal && archive.EndTag(filePathTag);
	}

	retVal = retVal && archive.EndTag(configFilesTag);


	int dirsCount = 0;
	retVal = retVal && archive.BeginMultiTag(packageDirsTag, dirPathTag, dirsCount);

	if (!retVal){
		SendVerboseMessage(iqt::GetCString(QObject::tr("Load of configuration file: %1 failed").arg(configFilePath)));

		return false;
	}

	for (int i = 0; i < dirsCount; ++i){
		retVal = retVal && archive.BeginTag(dirPathTag);
		istd::CString dirPath;
		retVal = retVal && archive.Process(dirPath);
		istd::CString correctedPath;
		if (retVal && CheckAndMarkPath(m_usedPackageDirsList, baseDir, dirPath, correctedPath)){
			RegisterPackagesDir(correctedPath);
		}

		retVal = retVal && archive.EndTag(dirPathTag);
	}

	retVal = retVal && archive.EndTag(packageDirsTag);

	int filesCount = 0;
	retVal = retVal && archive.BeginMultiTag(packageFilesTag, filePathTag, filesCount);

	if (!retVal){
		SendVerboseMessage(iqt::GetCString(QObject::tr("Load of configuration file: %1 failed").arg(configFilePath)));

		return false;
	}

	for (int i = 0; i < filesCount; ++i){
		retVal = retVal && archive.BeginTag(filePathTag);
		istd::CString filePath;
		retVal = retVal && archive.Process(filePath);
		istd::CString correctedPath;
		if (retVal && CheckAndMarkPath(m_usedPackageFilesList, baseDir, filePath, correctedPath)){
			RegisterPackageFile(correctedPath);
		}

		retVal = retVal && archive.EndTag(filePathTag);
	}

	retVal = retVal && archive.EndTag(packageFilesTag);

	if (!retVal){
		SendVerboseMessage(iqt::GetCString(QObject::tr("Load of configuration file: %1 failed").arg(configFilePath)));

		return false;
	}

	return retVal;
}


bool CRegistriesManagerComp::CheckAndMarkPath(PathList& pathList, const QDir& directory, const istd::CString& path, istd::CString& resultPath) const
{
	QString enrolledPath = iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(path));
	istd::CString fullPath = iqt::GetCString(QFileInfo(directory.absoluteFilePath(enrolledPath)).canonicalFilePath());

	if (pathList.find(fullPath) == pathList.end()){
		pathList.insert(fullPath);

		resultPath = fullPath;

		SendVerboseMessage(istd::CString(">> new path: ") + resultPath + " (" + path + ")");

		return true;
	}

	return false;
}


// reimplemented (icomp::CComponentBase)

void CRegistriesManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_configFilePathCompPtr.IsValid()){
		istd::CString path = m_configFilePathCompPtr->GetPath();

		if (!path.IsEmpty()){
			QDir applicationDir = QCoreApplication::applicationDirPath();

			QString enrolledPath = iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(path));

			LoadConfigFile(iqt::GetCString(applicationDir.absoluteFilePath(enrolledPath)));
		}
	}
}


} // namespace iqt


