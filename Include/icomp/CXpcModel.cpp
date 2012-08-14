#include "icomp/CXpcModel.h"


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace icomp
{


bool CXpcModel::Serialize(iser::IArchive& archive)
{
    iser::CArchiveTag configFilesTag("ConfigFiles", "List of included config files", true);
    iser::CArchiveTag packageDirsTag("PackageDirs", "List of package directories", true);
    iser::CArchiveTag dirPathTag("Dir", "List of package directories", true);
    iser::CArchiveTag packageFilesTag("PackageFiles", "List of package files", true);
    iser::CArchiveTag filePathTag("FilePath", "Path of single file", true);

    bool retVal = true;

    int configFilesCount = 0;

    if (!archive.IsStoring()) {
	m_confFiles.clear();
	m_packageDirs.clear();
	m_packages.clear();
    } else {
	configFilesCount = m_confFiles.size();
    }
    retVal = retVal && archive.BeginMultiTag(configFilesTag, filePathTag, configFilesCount);

    if (!retVal) {
	return false;
    }

    for (int i = 0; i < configFilesCount; ++i) {
	retVal = retVal && archive.BeginTag(filePathTag);
	QString filePath;
	if (archive.IsStoring()) {
	    filePath = m_confFiles[i];
	}
	retVal = retVal && archive.Process(filePath);
	if (retVal && !archive.IsStoring()) {
	    m_confFiles.push_back(filePath);
	}

	retVal = retVal && archive.EndTag(filePathTag);
    }

    retVal = retVal && archive.EndTag(configFilesTag);

    int dirsCount = 0;
    if (archive.IsStoring()) {
	dirsCount = m_packageDirs.size();
    }

    retVal = retVal && archive.BeginMultiTag(packageDirsTag, dirPathTag, dirsCount);

    if (!retVal) {
	return false;
    }

    for (int i = 0; i < dirsCount; ++i) {
	retVal = retVal && archive.BeginTag(dirPathTag);
	QString dirPath;
	if (archive.IsStoring()) {
	    dirPath = m_packageDirs[i];
	}
	retVal = retVal && archive.Process(dirPath);
	if (retVal && !archive.IsStoring()) {
	    m_packageDirs.push_back(dirPath);
	}

	retVal = retVal && archive.EndTag(dirPathTag);
    }

    retVal = retVal && archive.EndTag(packageDirsTag);

    int filesCount = 0;
    if (!archive.IsStoring()) {
	filesCount = m_packages.size();
    }

    retVal = retVal && archive.BeginMultiTag(packageFilesTag, filePathTag, filesCount);

    if (!retVal) {
	return false;
    }

    for (int i = 0; i < filesCount; ++i) {
	retVal = retVal && archive.BeginTag(filePathTag);
	QString filePath;
	if (archive.IsStoring()) {
	    filePath = m_packages[i];
	}
	retVal = retVal && archive.Process(filePath);
	if (retVal) {
	    if (!archive.IsStoring()) {
		m_packages.push_back(filePath);
	    }
	}

	retVal = retVal && archive.EndTag(filePathTag);
    }

    retVal = retVal && archive.EndTag(packageFilesTag);

    return retVal;
}


const QStringList& CXpcModel::GetConfFilesList() const
{
    return m_confFiles;
}


void CXpcModel::SetConfFilesList(const QStringList& list)
{
    m_confFiles = list;
}


const QStringList& CXpcModel::GetPackageDirsList() const
{
    return m_packageDirs;
}


void CXpcModel::SetPackageDirsList(const QStringList& list)
{
    m_packageDirs = list;
}


const QStringList& CXpcModel::GetPackagesList() const
{
    return m_packages;
}


void CXpcModel::SetPackagesList(const QStringList& list)
{
    m_packages = list;
}


int CXpcModel::GetNumConfFiles() const
{
    return m_confFiles.size();
}


int CXpcModel::GetNumPackageDirs() const
{
    return m_packageDirs.size();
}


int CXpcModel::GetNumPackages() const
{
    return m_packages.size();
}


QString CXpcModel::GetConfFile(int index) const
{
    if (index < 0 || index >= GetNumConfFiles())
	return "";
    else
	return m_confFiles[index];
}


QString CXpcModel::GetPackageDir(int index) const
{
    if (index < 0 || index >= GetNumPackageDirs())
	return "";
    else
	return m_packageDirs[index];
}


QString CXpcModel::GetPackage(int index) const
{
    if (index < 0 || index >= GetNumPackages()) {
	return "";
    } else {
	return m_packages[index];
    }
}


void CXpcModel::AddConfFile(const QString& path)
{
    m_confFiles.push_back(path);
}


void CXpcModel::AddPackageDir(const QString& path)
{
    m_packageDirs.push_back(path);
}


void CXpcModel::AddPackage(const QString& path)
{
    m_packages.push_back(path);
}


} // namespace icomp
