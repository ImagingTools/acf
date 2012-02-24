#ifndef iqt_CFileSystem_included
#define iqt_CFileSystem_included


#include "isys/IFileSystem.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Implementation of file system information based on QT.
	This implementation is part of standard services if you use QT-based components.
*/
class CFileSystem: virtual public isys::IFileSystem
{
public:
	// reimplemented (isys::IFileSystem)
	virtual QString GetNormalizedPath(const QString& path) const;
	virtual QString GetFileName(const QString& filePath) const;
	virtual QString GetBaseFileName(const QString& filePath) const;
	virtual QString GetDirPath(const QString& filePath) const;
	virtual bool HasExtension(const QString& filePath, const QString& extension) const;
	virtual bool IsPresent(const QString& filePath) const;
	virtual bool CreateFileCopy(const QString& inputFile, const QString& outputFile, bool overwriteExisting = false) const;
	virtual bool RemoveFile(const QString& filePath) const;
	virtual bool RemoveFolder(const QString& directoryPath, bool ignoreNonEmpty = false) const;
	virtual bool CreateFolder(const QString& directoryPath) const;

	// static members
	static QString FindVariableValue(const QString& varName);
	static QString GetEnrolledPath(const QString& path);
};


} // namespace iqt


#endif // !iqt_CFileSystem_included


