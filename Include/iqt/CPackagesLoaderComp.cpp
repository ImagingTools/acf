#include "iqt/CPackagesLoaderComp.h"


// Qt includes
#include <QObject>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>
#include <QFileInfo>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/CXmlFileReadArchive.h"

#include "icomp/export.h"

#include "iqt/CFileSystem.h"


namespace iqt
{


// reimplemented (icomp::IRegistryLoader)

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


// reimplemented (icomp::IComponentEnvironmentManager)

istd::CString CPackagesLoaderComp::GetConfigFilePath() const
{
	return m_configFilePath;
}


// reimplemented (icomp::IPackagesManager)

bool CPackagesLoaderComp::LoadPackages(const istd::CString& configFilePath)
{
	istd::CChangeNotifier notifier(this);

	m_configFilePath = configFilePath;

	if (m_configFilePath.IsEmpty() && m_configFilePathCompPtr.IsValid()){
		istd::CString path = m_configFilePathCompPtr->GetPath();

		if (!path.IsEmpty()){
			QDir applicationDir = QCoreApplication::applicationDirPath();

			QString enrolledPath = iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(path));

			m_configFilePath = iqt::GetCString(applicationDir.absoluteFilePath(enrolledPath));
		}

		SendVerboseMessage(istd::CString("Configure component environment using ") + configFilePath);
	}

	if (m_configFilePath.IsEmpty()){
		SendVerboseMessage("Configure component environment using default configuration");

		m_configFilePath = "Default.xpc";
	}

	BaseClass2::Reset();

	m_registriesMap.clear();
	m_invRegistriesMap.clear();
	m_usedFilesList.clear();
	m_compositePackagesMap.clear();
	m_realPackagesMap.clear();
	m_dllCacheMap.clear();

	return LoadConfigFile(m_configFilePath);
}


int CPackagesLoaderComp::GetPackageType(const std::string& packageId) const
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


istd::CString CPackagesLoaderComp::GetPackagePath(const std::string& packageId) const
{
	RealPackagesMap::const_iterator foundNormalIter = m_realPackagesMap.find(packageId);
	if (foundNormalIter != m_realPackagesMap.end()){
		return foundNormalIter->second;
	}

	CompositePackagesMap::const_iterator foundCompositeIter = m_compositePackagesMap.find(packageId);
	if (foundCompositeIter != m_compositePackagesMap.end()){
		return iqt::GetCString(foundCompositeIter->second.directory.absolutePath());
	}

	return istd::CString::GetEmpty();
}


// reimplemented (icomp::IRegistriesManager)

const icomp::IRegistry* CPackagesLoaderComp::GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr) const
{
	CompositePackagesMap::const_iterator foundCompositeIter = m_compositePackagesMap.find(address.GetPackageId());
	if (foundCompositeIter != m_compositePackagesMap.end()){
		QString filePath = foundCompositeIter->second.directory.absoluteFilePath(QString(address.GetComponentId().c_str()) + ".arx");

		return GetRegistryFromFile(GetCString(filePath));
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
			const std::string packageId = packageIter->first;

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
			const std::string packageId = packageIter->first;

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

bool CPackagesLoaderComp::RegisterPackageFile(const istd::CString& file)
{
	QFileInfo fileInfo(iqt::GetQString(file));

	std::string packageId(fileInfo.baseName().toStdString());

	if (fileInfo.isFile()){
		RealPackagesMap::const_iterator foundIter = m_realPackagesMap.find(packageId);
		if (foundIter == m_realPackagesMap.end()){
			CDllFunctionsProvider& provider = GetProviderRef(fileInfo);
			if (provider.IsValid()){
				// register services:
				icomp::RegisterServicesFunc registerServicesInfoPtr = (icomp::RegisterServicesFunc)provider.GetFunction(I_EXPORT_SERVICES_FUNCTION_NAME);
				if (registerServicesInfoPtr != NULL){
					registerServicesInfoPtr(&istd::CStaticServicesProvider::GetProviderInstance());
				}

				icomp::GetPackageInfoFunc getInfoPtr = (icomp::GetPackageInfoFunc)provider.GetFunction(I_PACKAGE_EXPORT_FUNCTION_NAME);
				if (getInfoPtr != NULL){
					icomp::CPackageStaticInfo* infoPtr = getInfoPtr();
					if (infoPtr != NULL){
						m_realPackagesMap[packageId] = iqt::GetCString(fileInfo.absoluteFilePath());

						RegisterEmbeddedComponentInfo(packageId, infoPtr);

						return true;
					}
				}
			}
		}
		else{
			SendWarningMessage(
						MI_CANNOT_REGISTER,
						iqt::GetCString(QObject::tr("Second real package definition was ignored %1 (previous: %2)")
									.arg(fileInfo.absoluteFilePath())
									.arg(iqt::GetQString(foundIter->second))));
		}
	}
	else if (fileInfo.isDir()){
		CompositePackagesMap::const_iterator foundIter = m_compositePackagesMap.find(packageId);
		if (foundIter == m_compositePackagesMap.end()){
			CompositePackageInfo& packageInfo = m_compositePackagesMap[packageId];

			icomp::CCompositePackageStaticInfo* infoPtr = new icomp::CCompositePackageStaticInfo(
						packageId,
						this);
			if (infoPtr == NULL){
				return false;
			}

			QStringList filters;
			filters.append("*.arx");
			QDir packageDir(fileInfo.absoluteFilePath());
			QStringList componentFiles = packageDir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);
			for (		QStringList::iterator iter = componentFiles.begin();
						iter != componentFiles.end();
						++iter){
				QFileInfo componentFileInfo(*iter);
				infoPtr->RegisterEmbeddedComponent(componentFileInfo.baseName().toStdString());
			}

			packageInfo.staticInfoPtr.SetPtr(infoPtr);
			packageInfo.directory = packageDir;

			istd::CString metaInfoFile(iqt::GetCString(packageDir.absoluteFilePath("General.xml")));
			iser::CXmlFileReadArchive archive(metaInfoFile);
			if (!infoPtr->SerializeMeta(archive)){
				SendErrorMessage(
							iser::IFileLoader::MI_CANNOT_LOAD,
							iqt::GetCString(QObject::tr("Cannot load meta description for registry %1 (%2)")
										.arg(packageId.c_str())
										.arg(iqt::GetQString(metaInfoFile))));
			}

			RegisterEmbeddedComponentInfo(packageId, infoPtr);

			return true;
		}
		else{
			SendWarningMessage(
						MI_CANNOT_REGISTER,
						iqt::GetCString(QObject::tr("Second composed package definition was ignored %1 (previous: %2)")
									.arg(fileInfo.absoluteFilePath())
									.arg(foundIter->second.directory.absolutePath())));
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

	istd::CString configFilePath = GetCString(fileInfo.absoluteFilePath());

	SendVerboseMessage(istd::CString("Load configuration file: ") + configFilePath);

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


CDllFunctionsProvider& CPackagesLoaderComp::GetProviderRef(const QFileInfo& fileInfo)
{
	QString absolutePath = fileInfo.canonicalFilePath();

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
					iqt::GetCString(QObject::tr("Cannot register components from package %1").arg(fileInfo.fileName())));
	}

	return *providerPtr;
}


// public methods of embedded class LogingRegistry

CPackagesLoaderComp::LogingRegistry::LogingRegistry(CPackagesLoaderComp* parentPtr)
:	m_parent(*parentPtr)
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


