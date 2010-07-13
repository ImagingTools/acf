#ifndef isys_CFileSystemBase_included
#define isys_CFileSystemBase_included


// ACF includes
#include "isys/IFileSystem.h"


namespace isys
{


/**
	Basic plattform independent implementation of the file system information.
*/
class CFileSystemBase: virtual public isys::IFileSystem
{
public:
	// reimplemented (isys::IFileSystem)
	virtual istd::CString GetNormalizedPath(const istd::CString& path) const;
	virtual istd::CString GetFileName(const istd::CString& filePath) const;
	virtual istd::CString GetBaseFileName(const istd::CString& filePath) const;
	virtual istd::CString GetDirPath(const istd::CString& filePath) const;
	virtual bool HasExtension(const istd::CString& filePath, const istd::CString& extension) const;
};


} // namespace isys


#endif // !isys_CFileSystemBase_included


