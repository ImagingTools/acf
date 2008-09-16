#include "iqt/CPackagesLoaderComp.h"


// Qt includes
#include <QObject>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>
#include <QFileInfo>

#include "iser/CXmlFileReadArchive.h"

#include "icomp/export.h"
#include "icomp/CRegistry.h"


namespace iqt
{


bool CPackagesLoaderComp::RegisterPackageFile(const istd::CString& file, bool beQuiet)
{
	QFileInfo fileInfo(iqt::GetQString(file));

	if (fileInfo.isFile()){
		CDllFunctionsProvider& provider = GetProviderRef(fileInfo, beQuiet);
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
					return RegisterSubcomponentInfo(fileInfo.baseName().toStdString(), infoPtr);
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


bool CPackagesLoaderComp::RegisterPackagesDir(const istd::CString& path, bool beQuiet)
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

		retVal = RegisterPackageFile(filePath, beQuiet) && retVal;
	}

	return retVal;
}


bool CPackagesLoaderComp::LoadConfigFile(const istd::CString& configFile)
{
	QFileInfo fileInfo = GetQString(configFile);
	QDir baseDir = fileInfo.absoluteDir();

	iser::CXmlFileReadArchive archive(GetCString(fileInfo.absoluteFilePath()));

	bool retVal = true;

	iser::CArchiveTag packageDirsTag("PackageDirs", "List of package directories", true);
	iser::CArchiveTag dirPathTag("Dir", "List of package directories", true);
	int dirsCount = 0;
	retVal = retVal && archive.BeginMultiTag(packageDirsTag, dirPathTag, dirsCount);

	if (!retVal){
		return false;
	}

	for (int i = 0; i < dirsCount; ++i){
		retVal = retVal && archive.BeginTag(dirPathTag);
		istd::CString dirPath;
		retVal = retVal && archive.Process(dirPath);
		if (retVal){
			RegisterPackagesDir(iqt::GetCString(baseDir.absoluteFilePath(iqt::GetQString(dirPath))), false);
		}

		retVal = retVal && archive.EndTag(dirPathTag);
	}

	retVal = retVal && archive.EndTag(packageDirsTag);

	iser::CArchiveTag packageFilesTag("PackageFiles", "List of package files", true);
	iser::CArchiveTag filePathTag("FilePath", "Path of single file", true);
	int filesCount = 0;
	retVal = retVal && archive.BeginMultiTag(packageFilesTag, filePathTag, filesCount);

	if (!retVal){
		return false;
	}

	for (int i = 0; i < filesCount; ++i){
		retVal = retVal && archive.BeginTag(filePathTag);
		istd::CString filePath;
		retVal = retVal && archive.Process(filePath);
		if (retVal){
			RegisterPackageFile(iqt::GetCString(baseDir.absoluteFilePath(iqt::GetQString(filePath))), false);
		}

		retVal = retVal && archive.EndTag(filePathTag);
	}

	retVal = retVal && archive.EndTag(packageFilesTag);

	return retVal;
}


const icomp::IRegistry* CPackagesLoaderComp::GetRegistryFromFile(const istd::CString& path) const
{
	QFileInfo fileInfo(iqt::GetQString(path));
	istd::CString correctedPath = iqt::GetCString(fileInfo.absoluteFilePath());

	RegistriesMap::const_iterator iter = m_registriesMap.find(correctedPath);

	if (iter != m_registriesMap.end()){
		return iter->second.GetPtr();
	}

	RegistryPtr& mapValue = m_registriesMap[correctedPath];
	if (m_registryLoaderCompPtr.IsValid()){
		istd::TDelPtr<icomp::IRegistry> newRegistryPtr(new icomp::CRegistry(this));
		if (m_registryLoaderCompPtr->LoadFromFile(*newRegistryPtr, correctedPath) == iser::IFileLoader::StateOk){
			mapValue.TakeOver(newRegistryPtr);
			m_invRegistriesMap[mapValue.GetPtr()] = fileInfo;

			return mapValue.GetPtr();
		}
	}

	return NULL;
}


// reimplemented (icomp::IRegistriesManager)

const icomp::IRegistry* CPackagesLoaderComp::GetRegistry(const icomp::CComponentAddress& address) const
{
	CompositePackagesMap::const_iterator foundIter = m_compositePackagesMap.find(address.GetPackageId());
	if (foundIter != m_compositePackagesMap.end()){
		QString filePath = foundIter->second.directory.absoluteFilePath(QString(address.GetComponentId().c_str()) + ".arx");

		return GetRegistryFromFile(GetCString(filePath));
	}

	return NULL;
}


// protected methods

CDllFunctionsProvider& CPackagesLoaderComp::GetProviderRef(const QFileInfo& fileInfo, bool beQuiet)
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

	if (!providerPtr->IsValid() && !beQuiet){
		QMessageBox::warning(
					NULL,
					QObject::tr("Error"),
					QObject::tr("Cannot register components from registry\n%1").arg(fileInfo.fileName()));
	}

	return *providerPtr;
}


} // namespace iqt


