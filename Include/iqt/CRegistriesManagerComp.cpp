#include "iqt/CRegistriesManagerComp.h"


// Qt includes
#include <QCoreApplication>
#include <QFileInfo>


// ACF includes
#include "iser/CXmlFileReadArchive.h"

#include "iqt/CFileSystem.h"


namespace iqt
{


bool CRegistriesManagerComp::ConfigureEnvironment(const istd::CString& configFilePath)
{
	bool retVal = true;

	if (!configFilePath.IsEmpty()){
		retVal = retVal && LoadConfigFile(configFilePath);
	}
	else{
		if (!LoadConfigFile(iqt::GetCString("Default.xpc"))){
			QDir applicationDir = QCoreApplication::applicationDirPath();
			if (!LoadConfigFile(iqt::GetCString(applicationDir.absoluteFilePath("Default.xpc")))){
				RegisterPackagesDir(iqt::GetCString(applicationDir.absolutePath()));
			}
		}
	}

	return retVal;
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


// reimplemented (icomp::IRegistriesManager)

const icomp::IRegistry* CRegistriesManagerComp::GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr) const
{
	if ((contextRegistryPtr != NULL) && address.GetPackageId().empty()){
		return contextRegistryPtr->GetEmbeddedRegistry(address.GetComponentId());
	}

	PackagesMap::const_iterator foundCompositeIter = m_compositePackagesMap.find(address.GetPackageId());
	if (foundCompositeIter != m_compositePackagesMap.end()){
		QString filePath = foundCompositeIter->second.absoluteFilePath(QString(address.GetComponentId().c_str()) + ".arx");

		return GetRegistryFromFile(GetCString(filePath));
	}

	return NULL;
}


istd::CString CRegistriesManagerComp::GetPackageDirPath(const std::string& packageId) const
{
	PackagesMap::const_iterator foundCompositeIter = m_compositePackagesMap.find(packageId);
	if (foundCompositeIter != m_compositePackagesMap.end()){
		return iqt::GetCString(foundCompositeIter->second.absolutePath());
	}

	return istd::CString::GetEmpty();
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


// protected methods

void CRegistriesManagerComp::RegisterPackageFile(const istd::CString& file)
{
	QFileInfo fileInfo(iqt::GetQString(file));

	if (fileInfo.isDir()){
		std::string packageId = fileInfo.baseName().toStdString();
		PackagesMap::const_iterator foundIter = m_compositePackagesMap.find(packageId);
		if (foundIter == m_compositePackagesMap.end()){
			icomp::IComponentStaticInfo::Ids componentIds;

			QStringList filters;
			filters.append("*.arx");
			QDir packageDir(fileInfo.absoluteFilePath());
			QStringList componentFiles = packageDir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);
			for (		QStringList::iterator iter = componentFiles.begin();
						iter != componentFiles.end();
						++iter){
				QFileInfo componentFileInfo(*iter);
				componentIds.insert(componentFileInfo.baseName().toStdString());
			}

			m_compositePackagesMap[packageId] = packageDir;
		}
	}
}


void CRegistriesManagerComp::RegisterPackagesDir(const istd::CString& path)
{
	QDir packagesDir(GetQString(path));

	QStringList filters;
	filters.append("*.arp");
	QStringList filesInfo = packagesDir.entryList(filters, QDir::Dirs | QDir::NoDotAndDotDot);
	for (		QStringList::iterator iter = filesInfo.begin();
				iter != filesInfo.end();
				++iter){
		istd::CString filePath = GetCString(packagesDir.absoluteFilePath(*iter));

		RegisterPackageFile(filePath);
	}
}


bool CRegistriesManagerComp::LoadConfigFile(const istd::CString& configFile)
{
	QFileInfo fileInfo(iqt::GetQString(configFile));

	QDir baseDir = fileInfo.absoluteDir();

	istd::CString configFilePath = GetCString(fileInfo.absoluteFilePath());

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
		return false;
	}

	for (int i = 0; i < configFilesCount; ++i){
		retVal = retVal && archive.BeginTag(filePathTag);
		istd::CString filePath;
		retVal = retVal && archive.Process(filePath);
		istd::CString correctedPath;
		if (retVal && CheckAndMarkPath(baseDir, filePath, correctedPath)){
			LoadConfigFile(correctedPath);
		}

		retVal = retVal && archive.EndTag(filePathTag);
	}

	retVal = retVal && archive.EndTag(configFilesTag);


	int dirsCount = 0;
	retVal = retVal && archive.BeginMultiTag(packageDirsTag, dirPathTag, dirsCount);

	if (!retVal){
		return false;
	}

	for (int i = 0; i < dirsCount; ++i){
		retVal = retVal && archive.BeginTag(dirPathTag);
		istd::CString dirPath;
		retVal = retVal && archive.Process(dirPath);
		istd::CString correctedPath;
		if (retVal && CheckAndMarkPath(baseDir, dirPath, correctedPath)){
			RegisterPackagesDir(correctedPath);
		}

		retVal = retVal && archive.EndTag(dirPathTag);
	}

	retVal = retVal && archive.EndTag(packageDirsTag);

	int filesCount = 0;
	retVal = retVal && archive.BeginMultiTag(packageFilesTag, filePathTag, filesCount);

	if (!retVal){
		return false;
	}

	for (int i = 0; i < filesCount; ++i){
		retVal = retVal && archive.BeginTag(filePathTag);
		istd::CString filePath;
		retVal = retVal && archive.Process(filePath);
		istd::CString correctedPath;
		if (retVal && CheckAndMarkPath(baseDir, filePath, correctedPath)){
			RegisterPackageFile(correctedPath);
		}

		retVal = retVal && archive.EndTag(filePathTag);
	}

	retVal = retVal && archive.EndTag(packageFilesTag);

	return retVal;
}


bool CRegistriesManagerComp::CheckAndMarkPath(const QDir& directory, const istd::CString& path, istd::CString& resultPath) const
{
	istd::CString fullPath = iqt::GetCString(directory.absoluteFilePath(iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(path))));
	if (m_usedFilesList.find(fullPath) == m_usedFilesList.end()){
		m_usedFilesList.insert(fullPath);

		resultPath = fullPath;

		return true;
	}

	return false;
}


} // namespace iqt


