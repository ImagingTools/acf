#include "iser/CFileArchiveInfo.h"


namespace iser
{


CFileArchiveInfo::CFileArchiveInfo(const QString& filePath)
:	m_filePath(filePath)
{
}


// reimplemented (iser::IFileArchiveInfo)

const QString& CFileArchiveInfo::GetCurrentFilePath() const
{
	return m_filePath;
}


} // namespace iser


