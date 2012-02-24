#ifndef isys_IFileSystem_included
#define isys_IFileSystem_included


// Qt includes
#include <QString>


// ACF includes
#include "istd/IPolymorphic.h"

#include "isys/isys.h"


namespace isys
{


/**
	Defines some file system dependent operations.

	\ingroup System
*/
class IFileSystem: virtual public istd::IPolymorphic
{
public:
	/**
		Get normalized file path.
		For any path to the same file this function should return the same path.
		For example for "C:\Test\..\Test\blah.txt" it should return "C:\Test\blah.txt".
	*/
	virtual QString GetNormalizedPath(const QString& path) const = 0;

	/**
		Get file name of specified file path.
		For example for "C:\Test\blah.txt" it should return "blah.txt".
	*/
	virtual QString GetFileName(const QString& filePath) const = 0;

	/**
		Get base file name (also without extension) of specified file path.
		For example for "C:\Test\blah.txt.gz" it should return "blah".		
	*/
	virtual QString GetBaseFileName(const QString& filePath) const = 0;

	/**
		Get directory path of specified file path.
		For example for "C:\Test\blah.txt" it should return "C:\Test".
	*/
	virtual QString GetDirPath(const QString& filePath) const = 0;

	/**
		Check if file path has specified extension.
	*/
	virtual bool HasExtension(const QString& filePath, const QString& extension) const = 0;

	/**
		Check if some file is present.
	*/
	virtual bool IsPresent(const QString& filePath) const = 0;

	/**
		Copy any file to another. If the \c overwriteExisting is set on, the existing file will be overwritten.
	*/
	virtual bool CreateFileCopy(const QString& inputFile, const QString& outputFile, bool overwriteExisting = false) const = 0;

	/**
		Remove the file \c filePath from the file system.
	*/
	virtual bool RemoveFile(const QString& filePath) const = 0;
	
	/**
		Remove the directory \c directoryPath from the file system.
		If \c ignoreNonEmpty flag set, all files in the directory will be also deleted,
		otherwise operation will be canceled and the function returns \c false.
	*/
	virtual bool RemoveFolder(const QString& directoryPath, bool ignoreNonEmpty = false) const = 0;

	/**
		Create an empty directory.
	*/
	virtual bool CreateFolder(const QString& directoryPath) const = 0;
};


} // namespace isys


#endif // !isys_IFileSystem_included


