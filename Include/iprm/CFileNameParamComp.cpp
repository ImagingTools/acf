#include "iprm/CFileNameParamComp.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


// reimplemented (icomp::IComponent)

void CFileNameParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultDirAttrPtr.IsValid()){
		m_directory = *m_defaultDirAttrPtr;
	}
}


// reimplemented (iprm::IFileNameParam)

int CFileNameParamComp::GetPathType() const
{
	return (*m_isFileAttrPtr)? PT_FILE: (*m_isDirAttrPtr)? PT_DIRECTORY: PT_UNKNOWN;
}


const istd::CString& CFileNameParamComp::GetPath() const
{
	return m_directory;
}


void CFileNameParamComp::SetPath(const istd::CString& directory)
{
	m_directory = directory;
}


// reimplemented (iser::ISerializable)

bool CFileNameParamComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag directoryTag("Directory", "File directory of images");
	retVal = retVal && archive.BeginTag(directoryTag);
	retVal = retVal && archive.Process(m_directory);
	retVal = retVal && archive.EndTag(directoryTag);

	return retVal;
}


} // namespace iprm


