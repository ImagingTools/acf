#ifndef isys_IFileSystem_included
#define isys_IFileSystem_included


#include "isys/isys.h"


#include <string>

#include "istd/IPolymorphic.h"


namespace isys
{


class IFileSystem: virtual public istd::IPolymorphic
{
public:
	/**
		Get normalized file path.
		For any path to the same file this function should return the same path.
		For example for "C:\Test\..\Test\blah.txt" it should return "C:\Test\blah.txt".
	*/
	virtual istd::CString GetNormalizedPath(const istd::CString& path) const = 0;
	/**
		Get file name of specified file path.
		For example for "C:\Test\blah.txt" it should return "blah.txt".
	*/
	virtual istd::CString GetFileName(const istd::CString& filePath) const = 0;
	/**
		Get extension of specified file path.
		For example for "C:\Test\blah.txt" it should return "txt".
	*/
	virtual istd::CString GetExtension(const istd::CString& filePath) const = 0;
};


} // namespace isys


#endif // !isys_IFileSystem_included


