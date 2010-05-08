#include "iprm/CFileNameParamComp.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


// reimplemented (icomp::IComponent)

void CFileNameParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultDirAttrPtr.IsValid()){
		m_path = *m_defaultDirAttrPtr;
	}
}


// reimplemented (iprm::IFileNameParam)

int CFileNameParamComp::GetPathType() const
{
	I_ASSERT(m_pathTypeAttrPtr.IsValid());

	return *m_pathTypeAttrPtr;
}


const istd::CString& CFileNameParamComp::GetPath() const
{
	return m_path;
}


void CFileNameParamComp::SetPath(const istd::CString& path)
{
	if (path != m_path){
		istd::CChangeNotifier notifier(this);

		m_path = path;
	}
}


// reimplemented (iser::ISerializable)

bool CFileNameParamComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag pathTag("Path", "File path");
	retVal = retVal && archive.BeginTag(pathTag);
	retVal = retVal && archive.Process(m_path);
	retVal = retVal && archive.EndTag(pathTag);

	return retVal;
}


} // namespace iprm


