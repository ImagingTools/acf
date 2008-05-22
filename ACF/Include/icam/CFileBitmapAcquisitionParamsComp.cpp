#include "icam/CFileBitmapAcquisitionParamsComp.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace icam
{


// reimplemented (icomp::IComponent)

void CFileBitmapAcquisitionParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultDirAttrPtr.IsValid()){
		m_directory = *m_defaultDirAttrPtr;
	}
}


// reimplemented (icam::IFileBitmapAcquisitionParams)

const istd::CString& CFileBitmapAcquisitionParamsComp::GetDirectory() const
{
	return m_directory;
}


void CFileBitmapAcquisitionParamsComp::SetDirectory(const istd::CString& directory)
{
	m_directory = directory;
}


// reimplemented (iser::ISerializable)

bool CFileBitmapAcquisitionParamsComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag directoryTag("Directory", "File directory of images");
	retVal = retVal && archive.BeginTag(directoryTag);
	retVal = retVal && archive.Process(m_directory);
	retVal = retVal && archive.EndTag(directoryTag);

	return retVal;
}


} // namespace icam


