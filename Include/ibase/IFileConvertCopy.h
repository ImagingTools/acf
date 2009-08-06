#ifndef ibase_IFileConvertCopy_included
#define ibase_IFileConvertCopy_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace ibase
{


class IFileConvertCopy: virtual public istd::IPolymorphic
{
public:
	virtual bool CopyFile(const istd::CString& inputFilePath, const istd::CString& outputFilePath) const = 0;
};


} // namespace ibase


#endif // !ibase_IFileConvertCopy_included


