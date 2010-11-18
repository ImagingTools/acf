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
	CFileArchiveInfo(const istd::CString& filePath);

	// reimplemented (iser::IFileArchiveInfo)
	virtual const istd::CString& GetCurrentFilePath() const;

protected:
	istd::CString m_filePath;
};


} // namespace iser


#endif // !iser_CFileArchiveInfo_included


