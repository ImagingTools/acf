#include "iser/CFileArchiveInfo.h"


namespace iser
{


CFileArchiveInfo::CFileArchiveInfo(const istd::CString& filePath)
:	m_filePath(filePath)
{
}


// reimplemented (iser::IFileArchiveInfo)

const istd::CString& CFileArchiveInfo::GetCurrentFilePath() const
{
	return m_filePath;
}


} // namespace iser


