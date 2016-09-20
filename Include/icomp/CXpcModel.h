#ifndef icomp_CXpcModel_included
#define icomp_CXpcModel_included


// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include <iser/ISerializable.h>


namespace icomp
{

/**
	Data model for registry configuration files (XPC).
*/
class CXpcModel : public iser::ISerializable
{
public:
	// gui interface (for editing)
	const QStringList& GetConfFilesList() const;
	void SetConfFilesList(const QStringList& list);
	const QStringList& GetPackageDirsList() const;
	void SetPackageDirsList(const QStringList& list);
	const QStringList& GetPackagesList() const;
	void SetPackagesList(const QStringList& list);
	const QStringList& GetRegistryFilesList() const;
	void SetRegistryFilesList(const QStringList& list);

	// program interface (for loading XPC files)
	int GetConfigFilesCount() const;
	QString GetConfFile(int) const;
	void AddConfFile(const QString& path);
	int GetPackageDirsCount() const;
	QString GetPackageDir(int) const;
	void AddPackageDir(const QString& path);
	int GetPackagesCount() const;
	QString GetPackage(int) const;
	void AddPackage(const QString& path);
	int GetRegistryFilesCount() const;
	QString GetRegistryFile(int) const;
	void AddRegistryFile(const QString& path);

	// iser::ISerializable interface
	bool Serialize(iser::IArchive& archive);

private:
	QStringList m_confFiles;
	QStringList m_packageDirs;
	QStringList m_packages;
	QStringList m_registryFiles;
};


} // namespace icomp


#endif // !icomp_CXpcModel_included


