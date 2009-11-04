#ifndef ibase_IFileConvertCopy_included
#define ibase_IFileConvertCopy_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "iprm/IParamsSet.h"


namespace ibase
{


/**
	Common interface for copy a file.
*/
class IFileConvertCopy: virtual public istd::IPolymorphic
{
public:
	/**
		Copy a file \c inputFilePath to destination \c outputFilePath. Optionally, you can use \c paramsPtr to control the copy process.
	*/
	virtual bool CopyFile(
				const istd::CString& inputFilePath,
				const istd::CString& outputFilePath,
				const iprm::IParamsSet* paramsPtr = NULL) const = 0;
};


} // namespace ibase


#endif // !ibase_IFileConvertCopy_included


