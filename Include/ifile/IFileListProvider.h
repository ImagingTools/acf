#ifndef ifile_IFileListProvider_included
#define ifile_IFileListProvider_included


#include "istd/IChangeable.h"

#include <QtCore/QString>


namespace ifile
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


} // namespace ifile


#endif //!ifile_IFileListProvider_included


