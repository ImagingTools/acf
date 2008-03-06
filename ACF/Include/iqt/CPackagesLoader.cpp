#include "iqt/CPackagesLoader.h"


#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QObject>

#include "iser/CXmlFileReadArchive.h"

#include "iqt/CDllFunctionsProvider.h"


namespace iqt
{


typedef icomp::IComponentStaticInfo* (*GetInfoFunc)();


bool CPackagesLoader::RegisterPackageFile(const istd::CString& file, bool beQuiet)
{
	CDllFunctionsProvider provider(file);
	if (provider.IsValid()){
		GetInfoFunc getInfoPtr = (GetInfoFunc)provider.GetFunction("GetInfo");
		if (getInfoPtr != NULL){
			icomp::IComponentStaticInfo* infoPtr = getInfoPtr();
			if (infoPtr != NULL){
				QFileInfo fileInfo(GetQString(file));
				return RegisterSubcomponentInfo(fileInfo.baseName().toStdString(), infoPtr);
			}
		}
	}

	if (!beQuiet){
		QMessageBox::warning(
					NULL,
					QObject::tr("Error"),
					QObject::tr("Cannot register components from registry\n%1").arg(GetQString(file)));
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

	return false;
}


} // namespace iqt


