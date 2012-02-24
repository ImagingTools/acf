#ifndef ibase_IFileListProvider_included
#define ibase_IFileListProvider_included


#include "istd/IChangeable.h"

#include <QString>


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
	virtual QStringList GetFileList() const = 0;
};


} // namespace ibase


#endif //!ibase_IFileListProvider_included


