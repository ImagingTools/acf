#include <ifile/CFileArchiveInfo.h>


namespace ifile
{


CFileArchiveInfo::CFileArchiveInfo(const QString& filePath)
:	m_filePath(filePath)
{
}


// reimplemented (ifile::IFileArchiveInfo)

const QString& CFileArchiveInfo::GetCurrentFilePath() const
{
	return m_filePath;
}


} // namespace ifile


