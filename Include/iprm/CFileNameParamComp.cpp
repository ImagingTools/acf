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

const istd::CString& CFileNameParamComp::GetDirectory() const
{
	return m_directory;
}


void CFileNameParamComp::SetDirectory(const istd::CString& directory)
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


