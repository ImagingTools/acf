#ifndef ifile_CFileArchiveInfo_included
#define ifile_CFileArchiveInfo_included


// ACF includes
#include <ifile/IFileArchiveInfo.h>


namespace ifile
{


/**
	Basic implementation of interface ifile::IFileArchiveInfo.

	\ingroup Persistence
*/
class CFileArchiveInfo: virtual public IFileArchiveInfo
{
public:
	explicit CFileArchiveInfo(const QString& filePath);

	// reimplemented (ifile::IFileArchiveInfo)
	virtual const QString& GetCurrentFilePath() const;

protected:
	QString m_filePath;
};


} // namespace ifile


#endif // !ifile_CFileArchiveInfo_included


