#pragma once


// Qt includes
#include <QtCore/QFileInfoList>

// ACF includes
#include <istd/IChangeable.h>


namespace ifile
{


/**
	Provide access to a list of files.
*/
class IFileListProvider: virtual public istd::IChangeable
{
public:
	/**
		Get list of files.
	*/
	virtual const QFileInfoList& GetFileList() const = 0;
};


} // namespace ifile


