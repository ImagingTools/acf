#ifndef ibase_IFileConvertCopy_included
#define ibase_IFileConvertCopy_included


#include "istd/IPolymorphic.h"


class QString;


namespace iprm
{
	class IParamsSet;
}


namespace ibase
{


/**
	Common interface for copy a file.
*/
class IFileConvertCopy: virtual public istd::IPolymorphic
{
public:
	/**
		Copy a file or files from \c inputPath to destination \c outputPath.
		Optionally, you can use \c paramsPtr to control the copy process.
	*/
	virtual bool ConvertFiles(
				const QString& inputPath,
				const QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL) const = 0;
};


} // namespace ibase


#endif // !ibase_IFileConvertCopy_included


