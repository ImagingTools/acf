#include "isys/CFileInfo.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace isys
{


CFileInfo::CFileInfo(
			const QString& filePath,
			int filePermissions,
			const isys::CSimpleDateTime& modificationTime)
	:m_filePath(filePath),
	m_filePermissions(filePermissions),
	m_modificationTime(modificationTime)
{
}


bool CFileInfo::operator== (const isys::CFileInfo& fileInfo) const
{
	return (m_filePath == fileInfo.m_filePath);
}

bool CFileInfo::operator!= (const isys::CFileInfo& fileInfo) const
{
	return (m_filePath != fileInfo.m_filePath);
}



void CFileInfo::SetPermissions(int filePermissions)
{
	if (m_filePermissions != filePermissions){
		istd::CChangeNotifier changePtr(this);
	
		m_filePermissions = filePermissions;
	}
}


void CFileInfo::SetModificationTime(const isys::IDateTime& modificationTime)
{
	if (m_modificationTime != modificationTime){
		istd::CChangeNotifier changePtr(this);
	
		m_modificationTime = modificationTime;
	}
}


// reimplemented (isys::IFileInfo)

QString CFileInfo::GetFilePath() const
{
	return m_filePath;
}


int CFileInfo::GetPermissions() const
{
	return m_filePermissions;
}


const isys::IDateTime& CFileInfo::GetModificationTime() const
{
	return m_modificationTime;
}


// reimplemented (iser::ISerializable)

bool CFileInfo::Serialize(iser::IArchive& archive)
{
	bool retVal = true;	

	static iser::CArchiveTag filePathTag("FilePath", "File path");
	retVal = retVal && archive.BeginTag(filePathTag);
	retVal = retVal && archive.Process(m_filePath);
	retVal = retVal && archive.EndTag(filePathTag);		

	static iser::CArchiveTag fileTimeStampTag("ModificationTime", "Time stamp of the last file modification");
	retVal = retVal && archive.BeginTag(fileTimeStampTag);
	retVal = retVal && m_modificationTime.Serialize(archive);
	retVal = retVal && archive.EndTag(fileTimeStampTag);		

	static iser::CArchiveTag filePermissionsTag("FilePermissions", "File permissions");
	retVal = retVal && archive.BeginTag(filePermissionsTag);
	retVal = retVal && archive.Process(m_filePermissions);
	retVal = retVal && archive.EndTag(filePermissionsTag);		

	return retVal;
}


} // namespace isys


