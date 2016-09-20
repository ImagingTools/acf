#include <icomp/CXpcModel.h>


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iser/IVersionInfo.h>


namespace icomp
{


// static constants
static const iser::CArchiveTag s_configFilesTag("ConfigFiles", "List of included config files", iser::CArchiveTag::TT_MULTIPLE);
static const iser::CArchiveTag s_configFilePathTag("FilePath", "Path of single configuration file", iser::CArchiveTag::TT_LEAF, &s_configFilesTag);
static const iser::CArchiveTag s_packageDirsTag("PackageDirs", "List of package directories", iser::CArchiveTag::TT_MULTIPLE);
static const iser::CArchiveTag s_dirPathTag("Dir", "List of package directories", iser::CArchiveTag::TT_LEAF, &s_packageDirsTag);
static const iser::CArchiveTag s_packageFilesTag("PackageFiles", "List of package files", iser::CArchiveTag::TT_MULTIPLE);
static const iser::CArchiveTag s_packageFilePathTag("FilePath", "Path of single package file", iser::CArchiveTag::TT_LEAF, &s_packageFilesTag);
static const iser::CArchiveTag s_registryFilesTag("RegistryFiles", "List of registry files", iser::CArchiveTag::TT_MULTIPLE);
static const iser::CArchiveTag s_registryFilePathTag("FilePath", "Path of single registry file", iser::CArchiveTag::TT_LEAF, &s_registryFilesTag);


bool CXpcModel::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	int configFilesCount = 0;
	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	if (!isStoring){
		m_confFiles.clear();
		m_packageDirs.clear();
		m_packages.clear();
		m_registryFiles.clear();
	}
	else{
		configFilesCount = m_confFiles.size();
	}
	retVal = retVal && archive.BeginMultiTag(s_configFilesTag, s_configFilePathTag, configFilesCount);

	if (!retVal){
		return false;
	}

	for (int i = 0; i < configFilesCount; ++i){
		retVal = retVal && archive.BeginTag(s_configFilePathTag);
		QString filePath;
		if (isStoring){
			filePath = m_confFiles[i];
		}
		retVal = retVal && archive.Process(filePath);
		if (retVal && !isStoring){
			m_confFiles.push_back(filePath);
		}

		retVal = retVal && archive.EndTag(s_configFilePathTag);
	}

	retVal = retVal && archive.EndTag(s_configFilesTag);

	int dirsCount = 0;
	if (isStoring){
		dirsCount = m_packageDirs.size();
	}

	retVal = retVal && archive.BeginMultiTag(s_packageDirsTag, s_dirPathTag, dirsCount);

	if (!retVal){
		return false;
	}

	for (int i = 0; i < dirsCount; ++i){
		retVal = retVal && archive.BeginTag(s_dirPathTag);
		QString dirPath;
		if (isStoring){
			dirPath = m_packageDirs[i];
		}
		retVal = retVal && archive.Process(dirPath);
		if (retVal && !isStoring){
			m_packageDirs.push_back(dirPath);
		}

		retVal = retVal && archive.EndTag(s_dirPathTag);
	}

	retVal = retVal && archive.EndTag(s_packageDirsTag);

	int filesCount = 0;
	if (isStoring){
		filesCount = m_packages.size();
	}

	retVal = retVal && archive.BeginMultiTag(s_packageFilesTag, s_packageFilePathTag, filesCount);

	if (!retVal){
		return false;
	}

	for (int i = 0; i < filesCount; ++i){
		retVal = retVal && archive.BeginTag(s_packageFilePathTag);
		QString filePath;
		if (isStoring){
			filePath = m_packages[i];
		}
		retVal = retVal && archive.Process(filePath);
		if (retVal){
			if (!isStoring){
				m_packages.push_back(filePath);
			}
		}

		retVal = retVal && archive.EndTag(s_packageFilePathTag);
	}

	retVal = retVal && archive.EndTag(s_packageFilesTag);

	int registryFilesCount = 0;
	if (isStoring){
		registryFilesCount = m_registryFiles.size();
	}

	retVal = retVal && archive.BeginMultiTag(s_registryFilesTag, s_registryFilePathTag, registryFilesCount);

	if (!retVal){
		return false;
	}

	for (int i = 0; i < registryFilesCount; ++i){
		retVal = retVal && archive.BeginTag(s_registryFilePathTag);
		QString filePath;
		if (isStoring){
			filePath = m_registryFiles[i];
		}
		retVal = retVal && archive.Process(filePath);
		if (retVal && !isStoring){
			m_registryFiles.push_back(filePath);
		}

		retVal = retVal && archive.EndTag(s_registryFilePathTag);
	}

	retVal = retVal && archive.EndTag(s_registryFilesTag);

	return retVal;
}


const QStringList& CXpcModel::GetConfFilesList() const
{
	return m_confFiles;
}


void CXpcModel::SetConfFilesList(const QStringList& list)
{
	if (list != m_confFiles){
		istd::CChangeNotifier notifier(this);

		m_confFiles = list;
	}
}


const QStringList& CXpcModel::GetPackageDirsList() const
{
	return m_packageDirs;
}


void CXpcModel::SetPackageDirsList(const QStringList& list)
{
	if (list != m_packageDirs){
		istd::CChangeNotifier notifier(this);

		m_packageDirs = list;
	}
}


const QStringList& CXpcModel::GetPackagesList() const
{
	return m_packages;
}


void CXpcModel::SetPackagesList(const QStringList& list)
{
	if (list != m_packages){
		istd::CChangeNotifier notifier(this);

		m_packages = list;
	}
}


const QStringList& CXpcModel::GetRegistryFilesList() const
{
	return m_registryFiles;
}


void CXpcModel::SetRegistryFilesList(const QStringList& list)
{
	if (list != m_registryFiles){
		istd::CChangeNotifier notifier(this);

		m_registryFiles = list;
	}
}


int CXpcModel::GetConfigFilesCount() const
{
	return m_confFiles.size();
}


QString CXpcModel::GetConfFile(int index) const
{
	if (index < 0 || index >= GetConfigFilesCount())
		return "";
	else
		return m_confFiles[index];
}


void CXpcModel::AddConfFile(const QString& path)
{
	istd::CChangeNotifier notifier(this);

	m_confFiles.push_back(path);
}


int CXpcModel::GetPackageDirsCount() const
{
	return m_packageDirs.size();
}


QString CXpcModel::GetPackageDir(int index) const
{
	if (index < 0 || index >= GetPackageDirsCount())
		return "";
	else
		return m_packageDirs[index];
}


void CXpcModel::AddPackageDir(const QString& path)
{
	istd::CChangeNotifier notifier(this);

	m_packageDirs.push_back(path);
}


int CXpcModel::GetPackagesCount() const
{
	return m_packages.size();
}


QString CXpcModel::GetPackage(int index) const
{
	if (index < 0 || index >= GetPackagesCount()){
		return "";
	}
	else{
		return m_packages[index];
	}
}


void CXpcModel::AddPackage(const QString& path)
{
	istd::CChangeNotifier notifier(this);

	m_packages.push_back(path);
}


int CXpcModel::GetRegistryFilesCount() const
{
	return m_registryFiles.size();
}


QString CXpcModel::GetRegistryFile(int index) const
{
	if (index < 0 || index >= GetRegistryFilesCount()){
		return "";
	}
	else{
		return m_registryFiles[index];
	}
}


void CXpcModel::AddRegistryFile(const QString& path)
{
	istd::CChangeNotifier notifier(this);

	m_registryFiles.push_back(path);
}


} // namespace icomp
