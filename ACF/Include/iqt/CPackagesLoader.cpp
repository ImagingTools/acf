#include "iqt/CPackagesLoader.h"


#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QObject>

#include "iser/CXmlFileReadArchive.h"

#include "icomp/export.h"


namespace iqt
{


bool CPackagesLoader::RegisterPackageFile(const istd::CString& file, bool beQuiet)
{
	CDllFunctionsProvider& provider = GetProviderRef(file, beQuiet);
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


bool CPackagesLoader::RegisterPackagesDir(const istd::CString& directory, bool beQuiet)
{
	bool retVal = true;

	QDir packagesDir(GetQString(directory));
	QStringList filters;
	filters.append("*.arp");
	QStringList filesInfo = packagesDir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (		QStringList::iterator iter = filesInfo.begin();
				iter != filesInfo.end();
				++iter){
		istd::CString filePath = GetCString(packagesDir.absoluteFilePath(*iter));

		retVal = RegisterPackageFile(filePath, beQuiet) && retVal;
	}

	return retVal;
}


bool CPackagesLoader::LoadConfigFile(const istd::CString& configFile)
{
	iser::CXmlFileReadArchive archive(configFile);

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
			RegisterPackagesDir(dirPath, false);
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
			RegisterPackageFile(filePath, false);
		}

		retVal = retVal && archive.EndTag(filePathTag);
	}

	retVal = retVal && archive.EndTag(packageFilesTag);

	return retVal;
}


// protected methods

CDllFunctionsProvider& CPackagesLoader::GetProviderRef(const istd::CString& file, bool beQuiet)
{
	QFileInfo fileInfo(GetQString(file));
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
					QObject::tr("Cannot register components from registry\n%1").arg(GetQString(file)));
	}

	return *providerPtr;
}


} // namespace iqt


