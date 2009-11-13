#include "iqt/CPackagesLoaderComp.h"


// Qt includes
#include <QObject>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>
#include <QFileInfo>

#include "iser/CXmlFileReadArchive.h"

#include "icomp/export.h"

#include "iqt/CFileSystem.h"


namespace iqt
{


bool CPackagesLoaderComp::RegisterPackageFile(const istd::CString& file)
{
	QFileInfo fileInfo(iqt::GetQString(file));

	if (fileInfo.isFile()){
		CDllFunctionsProvider& provider = GetProviderRef(fileInfo);
		if (provider.IsValid()){
			// register services:
			icomp::RegisterServicesFunc registerServicesInfoPtr = (icomp::RegisterServicesFunc)provider.GetFunction(I_EXPORT_SERVICES_FUNCTION_NAME);
			if (registerServicesInfoPtr != NULL){
				registerServicesInfoPtr(&istd::CStaticServicesProvider::GetProviderInstance());
			}

			icomp::GetPackageInfoFunc getInfoPtr = (icomp::GetPackageInfoFunc)provider.GetFunction(I_PACKAGE_EXPORT_FUNCTION_NAME);
			if (getInfoPtr != NULL){
				icomp::IComponentStaticInfo* infoPtr = getInfoPtr();
				if (infoPtr != NULL){
					std::string packageName(fileInfo.baseName().toStdString());

					m_normalPackagesMap[packageName] = iqt::GetCString(fileInfo.absoluteFilePath());

					return RegisterSubcomponentInfo(packageName, infoPtr);
				}
			}
		}
	}
	else if (fileInfo.isDir()){
		std::string packageId = fileInfo.baseName().toStdString();
		CompositePackagesMap::const_iterator foundIter = m_compositePackagesMap.find(packageId);
		if (foundIter == m_compositePackagesMap.end()){
			CompositePackageInfo& packageInfo = m_compositePackagesMap[packageId];

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

			icomp::CCompositePackageStaticInfo* infoPtr = new icomp::CCompositePackageStaticInfo(
						packageId,
						componentIds,
						this);
			packageInfo.staticInfoPtr.SetPtr(infoPtr);
			packageInfo.directory = packageDir;

			return RegisterSubcomponentInfo(packageId, infoPtr);
		}
	}

	return false;
}


bool CPackagesLoaderComp::RegisterPackagesDir(const istd::CString& path)
{
	bool retVal = true;

	QDir packagesDir(GetQString(path));

	QStringList filters;
	filters.append("*.arp");
	QStringList filesInfo = packagesDir.entryList(filters, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
	for (		QStringList::iterator iter = filesInfo.begin();
				iter != filesInfo.end();
				++iter){
		istd::CString filePath = GetCString(packagesDir.absoluteFilePath(*iter));

		retVal = RegisterPackageFile(filePath) && retVal;
	}

	return retVal;
}


bool CPackagesLoaderComp::LoadConfigFile(const istd::CString& configFile)
{
	QFileInfo fileInfo(iqt::GetQString(configFile));

	QDir baseDir = fileInfo.absoluteDir();

	m_configFilePath = GetCString(fileInfo.absoluteFilePath());

	iser::CXmlFileReadArchive archive(m_configFilePath);

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


// reimplemented (icomp::IRegistryLoader)

bool CPackagesLoaderComp::ConfigureEnvironment(const istd::CString& configFilePath)
{
	bool useDefaultRegistries = true;
	bool retVal = true;

	if (!configFilePath.IsEmpty()){
		retVal = retVal && LoadConfigFile(configFilePath);

		useDefaultRegistries = false;
	}

	// register default package path
	if (useDefaultRegistries){
		if (!LoadConfigFile(iqt::GetCString("Default.xpc"))){
			QDir applicationDir = QCoreApplication::applicationDirPath();
			if (!LoadConfigFile(iqt::GetCString(applicationDir.absoluteFilePath("Default.xpc")))){
				retVal = retVal && RegisterPackagesDir(iqt::GetCString(applicationDir.absolutePath()));
			}
		}
	}

	return retVal;
}


istd::CString CPackagesLoaderComp::GetConfigFilePath() const
{
	return m_configFilePath;
}


const icomp::IRegistry* CPackagesLoaderComp::GetRegistryFromFile(const istd::CString& path) const
{
	QFileInfo fileInfo(iqt::GetQString(path));
	istd::CString correctedPath = iqt::GetCString(fileInfo.canonicalFilePath());

	RegistriesMap::const_iterator iter = m_registriesMap.find(correctedPath);

	if (iter != m_registriesMap.end()){
		return iter->second.GetPtr();
	}

	RegistryPtr& mapValue = m_registriesMap[correctedPath];
	if (m_registryLoaderCompPtr.IsValid()){
		istd::TDelPtr<icomp::IRegistry> newRegistryPtr(new LogingRegistry(const_cast<CPackagesLoaderComp*>(this)));
		if (m_registryLoaderCompPtr->LoadFromFile(*newRegistryPtr, correctedPath) == iser::IFileLoader::StateOk){
			mapValue.TakeOver(newRegistryPtr);
			m_invRegistriesMap[mapValue.GetPtr()] = fileInfo;

			return mapValue.GetPtr();
		}
	}

	return NULL;
}


// reimplemented (icomp::CComponentBase)

void CPackagesLoaderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_configFilePathCompPtr.IsValid()){
		istd::CString path = m_configFilePathCompPtr->GetPath();

		if (!path.IsEmpty()){
			QDir applicationDir = QCoreApplication::applicationDirPath();

			LoadConfigFile(iqt::GetCString(applicationDir.absoluteFilePath(iqt::GetQString(path))));
		}
	}
}


// reimplemented (icomp::IRegistriesManager)

const icomp::IRegistry* CPackagesLoaderComp::GetRegistry(const icomp::CComponentAddress& address) const
{
	CompositePackagesMap::const_iterator foundCompositeIter = m_compositePackagesMap.find(address.GetPackageId());
	if (foundCompositeIter != m_compositePackagesMap.end()){
		QString filePath = foundCompositeIter->second.directory.absoluteFilePath(QString(address.GetComponentId().c_str()) + ".arx");

		return GetRegistryFromFile(GetCString(filePath));
	}

	return NULL;
}


istd::CString CPackagesLoaderComp::GetPackageDirPath(const std::string& packageId) const
{
	NormalPackagesMap::const_iterator foundNormalIter = m_normalPackagesMap.find(packageId);
	if (foundNormalIter != m_normalPackagesMap.end()){
		return foundNormalIter->second;
	}

	CompositePackagesMap::const_iterator foundCompositeIter = m_compositePackagesMap.find(packageId);
	if (foundCompositeIter != m_compositePackagesMap.end()){
		return iqt::GetCString(foundCompositeIter->second.directory.absolutePath());
	}

	return istd::CString::GetEmpty();
}


// protected methods

CDllFunctionsProvider& CPackagesLoaderComp::GetProviderRef(const QFileInfo& fileInfo)
{
	QString absolutePath = fileInfo.absoluteFilePath();

	DllCacheMap::iterator iter = m_dllCacheMap.find(absolutePath);
	if (iter != m_dllCacheMap.end()){
		I_ASSERT(iter->second.IsValid());

		return *iter->second;
	}

	FunctionsProviderPtr& providerPtr = m_dllCacheMap[absolutePath];
	providerPtr.SetPtr(new CDllFunctionsProvider(GetCString(absolutePath)));
	I_ASSERT(providerPtr.IsValid());

	if (!providerPtr->IsValid()){
		SendErrorMessage(
					MI_CANNOT_REGISTER,
					iqt::GetCString(QObject::tr("Cannot register components from registry %1").arg(fileInfo.fileName())));
	}

	return *providerPtr;
}


// public methods of embedded class LogingRegistry

CPackagesLoaderComp::LogingRegistry::LogingRegistry(CPackagesLoaderComp* parentPtr)
:	BaseClass(parentPtr), m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// reimplemented (icomp::IRegistry)

CPackagesLoaderComp::LogingRegistry::ElementInfo* CPackagesLoaderComp::LogingRegistry::InsertElementInfo(
			const std::string& elementId,
			const icomp::CComponentAddress& address,
			bool ensureElementCreated)
{
	ElementInfo* retVal = BaseClass::InsertElementInfo(elementId, address, ensureElementCreated);

	if (retVal == NULL){
		m_parent.SendErrorMessage(
					MI_CANNOT_CREATE_ELEMENT,
					iqt::GetCString(QObject::tr("Cannot create %1 (%2: %3)").
								arg(elementId.c_str()).
								arg(address.GetPackageId().c_str()).
								arg(address.GetComponentId().c_str())));
	}

	return retVal;
}


bool CPackagesLoaderComp::CheckAndMarkPath(const QDir& directory, const istd::CString& path, istd::CString& resultPath) const
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


