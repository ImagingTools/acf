#pragma once


// ACF includes
#include <ifile/IFileNameParam.h>


namespace ifile
{


class IRelativeFilePath: virtual public IFileNameParam
{
public:
	virtual QString GetBasePath() const = 0;
};


} // namespace ifile


