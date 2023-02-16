#pragma once


// ACF includes
#include <iser/CJsonReadArchiveBase.h>


namespace ifile
{


class CJsonFileReadArchive: public iser::CJsonReadArchiveBase
{
public:
	typedef iser::CJsonReadArchiveBase BaseClass;

	CJsonFileReadArchive(const QString& filePath = "", bool serializeHeader = true);

	bool OpenFile(const QString& filePath, bool serializeHeader);
};


} // namespace ifile


