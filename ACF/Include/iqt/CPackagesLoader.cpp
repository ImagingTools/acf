#include "iqt/CPackagesLoader.h"


#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>
#include <QObject>

#include "iser/CXmlFileReadArchive.h"

#include "icomp/export.h"


namespace iqt
{


bool CPackagesLoader::RegisterPackageFile(const istd::CString& file, const istd::CString& baseDir, bool beQuiet)
{
	CDllFunctionsProvider& provider = GetProviderRef(file, baseDir, beQuiet);
	if (provider.IsValid()){
		icomp::GetPackageInfoFunc getInfoPtr = (icomp::GetPackageInfoFunc)provider.GetFunction(I_PACKAGE_EXPORT_FUNCTION_NAME);
		if (getInfoPtr != NULL){
			icomp::IComponentStaticInfo* infoPtr = getInfoPtr();
			if (infoPtr != NULL){
				QFileInfo fileInfo(GetQString(file));
				return RegisterSubcomponentInfo(fileInfo.baseName().toStdString(), infoPtr);
			}
		}
	}

	return false;
}


bool CPackagesLoader::RegisterPackagesDir(const istd::CString& subDir, const istd::CString& baseDir, bool beQuiet)
{
	bool retVal = true;

	QDir packagesDir(GetQString(baseDir));
	packagesDir.cd(GetQString(subDir));

	QStringList filters;
	filters.append("*.arp");
	QStringList filesInfo = packagesDir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (		QStringList::iterator iter = filesInfo.begin();
				iter != filesInfo.end();
				++iter){
		istd::CString filePath = GetCString(*iter);

		retVal = RegisterPackageFile(filePath, GetCString(packagesDir.absolutePath()), beQuiet) && retVal;
	}

	return retVal;
}


bool CPackagesLoader::LoadConfigFile(const istd::CString& configFile, const istd::CString& baseDir)
{
	QDir packagesDir(GetQString(baseDir));
	QFileInfo fileInfo = packagesDir.absoluteFilePath(GetQString(configFile));
	istd::CString newBaseDir = GetCString(fileInfo.absoluteDir().absolutePath());

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
			RegisterPackagesDir(dirPath, newBaseDir, false);
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
			RegisterPackageFile(filePath, newBaseDir, false);
		}

		retVal = retVal && archive.EndTag(filePathTag);
	}

	retVal = retVal && archive.EndTag(packageFilesTag);

	return retVal;
}


// protected methods

CDllFunctionsProvider& CPackagesLoader::GetProviderRef(const istd::CString& file, const istd::CString& baseDir, bool beQuiet)
{
	QDir applicationDir(GetQString(baseDir));
	QString absolutePath = applicationDir.absoluteFilePath(GetQString(file));

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
					QObject::tr("Cannot register components from registry\n%1").arg(GetQString(file)));
	}

	return *providerPtr;
}


} // namespace iqt


