#ifndef isys_CFileInfo_included
#define isys_CFileInfo_included


// ACF includes
#include "isys/IFileInfo.h"

#include "isys/CSimpleDateTime.h"


namespace isys
{


/**
	Simple implementation of the file info

	\ingroup File System
*/
class CFileInfo: virtual public isys::IFileInfo
{
public:
	CFileInfo(
				const istd::CString& filePath = istd::CString(),
				int filePermissions = 0,
				const isys::CSimpleDateTime& modificationTime = isys::CSimpleDateTime::GetCurrent());

	bool operator== (const CFileInfo& fileInfo) const;
	bool operator!= (const CFileInfo& fileInfo) const;

	virtual void SetPermissions(int filePermissions);
	virtual void SetModificationTime(const isys::IDateTime& modificationTime);

	// reimplemented (isys::IFileInfo)
	virtual istd::CString GetFilePath() const;
	virtual int GetPermissions() const;
	virtual const isys::IDateTime& GetModificationTime() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_filePath;

	int m_filePermissions;

	isys::CSimpleDateTime m_modificationTime;
};


} // namespace isys


#endif // !isys_CFileInfo_included


