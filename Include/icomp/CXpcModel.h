#ifndef icomp_CXpcModel_included
#define icomp_CXpcModel_included


// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include "iser/ISerializable.h"


namespace icomp
{

    /** Data model for XPC files */
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

	// program interface (for loading XPC files)
	int GetNumConfFiles() const;
	int GetNumPackageDirs() const;
	int GetNumPackages() const;
	QString GetConfFile(int) const;
	QString GetPackageDir(int) const;
	QString GetPackage(int) const;
	void AddConfFile(const QString& path);
	void AddPackageDir(const QString& path);
	void AddPackage(const QString& path);

	// iser::ISerializable interface
	bool Serialize(iser::IArchive& archive);

    private:
	QStringList m_confFiles;
	QStringList m_packageDirs;
	QStringList m_packages;
    };


} // namespace icomp


#endif // !icomp_CXpcModel_included


