#include "iqt/CPackagesLoaderComp.h"


// Qt includes
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>
#include <QObject>

#include "iser/CXmlFileReadArchive.h"

#include "icomp/export.h"


namespace iqt
{


bool CPackagesLoaderComp::RegisterPackageFile(const istd::CString& file, bool beQuiet)
{
	QFileInfo fileInfo(iqt::GetQString(file));

	if (fileInfo.isFile()){
		CDllFunctionsProvider& provider = GetProviderRef(fileInfo, beQuiet);
		if (provider.IsValid()){
			icomp::GetPackageInfoFunc getInfoPtr = (icomp::GetPackageInfoFunc)provider.GetFunction(I_PACKAGE_EXPORT_FUNCTION_NAME);
			if (getInfoPtr != NULL){
				icomp::IComponentStaticInfo* infoPtr = getInfoPtr();
				if (infoPtr != NULL){
					return RegisterSubcomponentInfo(fileInfo.baseName().toStdString(), infoPtr);
				}
			}
		}
	}
	else if (m_registriesManagerCompPtr.IsValid()){
		CompositePackagesMap::const_iterator foundIter = m_compositePackagesMap.find(GetCString(fileInfo.absoluteFilePath()));
		if (foundIter == m_compositePackagesMap.end()){
			CompositePackageInfoPtr& packageInfoPtr = m_compositePackagesMap[GetCString(fileInfo.absoluteFilePath())];
			CCompositePackageStaticInfo* infoPtr = new CCompositePackageStaticInfo(
						fileInfo.absoluteDir(),
						m_registriesManagerCompPtr.GetPtr());
			packageInfoPtr.SetPtr(infoPtr);

			return RegisterSubcomponentInfo(fileInfo.baseName().toStdString(), infoPtr);
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


