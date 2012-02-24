#ifndef iser_CFileArchiveInfo_included
#define iser_CFileArchiveInfo_included


#include "iser/IFileArchiveInfo.h"


namespace iser
{


/**
	Basic implementation of interface iser::IFileArchiveInfo.
*/
class CFileArchiveInfo: virtual public IFileArchiveInfo
{
public:
	CFileArchiveInfo(const QString& filePath);

	// reimplemented (iser::IFileArchiveInfo)
	virtual const QString& GetCurrentFilePath() const;

protected:
	QString m_filePath;
};


} // namespace iser


#endif // !iser_CFileArchiveInfo_included


