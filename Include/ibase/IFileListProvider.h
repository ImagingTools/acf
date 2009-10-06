#ifndef ibase_IFileListProvider_included
#define ibase_IFileListProvider_included


#include "istd/IChangeable.h"


namespace ibase
{


/**
	Provide access to some parameter list.
*/
class IFileListProvider: virtual public istd::IChangeable
{
public:
	/**
		Get list of files.
	*/
	virtual const istd::CStringList& GetFileList() const = 0;
};


} // namespace ibase


#endif //!ibase_IFileListProvider_included


