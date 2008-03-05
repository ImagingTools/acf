#ifndef isys_IFileSystem_included
#define isys_IFileSystem_included


#include "isys/isys.h"


#include <string>

#include "istd/IPolymorphic.h"


namespace isys
{


class IFileSystem: public istd::IPolymorphic
{
public:
	/**
		Get normalized file path.
		For any path to the same file this function should return the same path.
	*/
	virtual istd::CString GetNormalizedPath(const istd::CString& path) const = 0;
};


} // namespace isys


#endif // !isys_IFileSystem_included


