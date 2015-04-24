#include "ifile/CFileNameParamComp.h"


// ACF includes
#include "istd/CChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ifile
{


// reimplemented (ifile::IFileNameParam)

int CFileNameParamComp::GetPathType() const
{
	Q_ASSERT(m_pathTypeAttrPtr.IsValid());

	return *m_pathTypeAttrPtr;
}


const QString& CFileNameParamComp::GetPath() const
{
	return m_path;
}


void CFileNameParamComp::SetPath(const QString& path)
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

	QString filePath = m_path;

	static iser::CArchiveTag pathTag("Path", "File path", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(pathTag);
	retVal = retVal && archive.Process(filePath);
	retVal = retVal && archive.EndTag(pathTag);

	if (!archive.IsStoring() && (filePath != m_path)){
		istd::CChangeNotifier notifier(this, &GetAllChanges());
		Q_UNUSED(notifier);

		m_path = filePath;
	}

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CFileNameParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultDirAttrPtr.IsValid()){
		m_path = *m_defaultDirAttrPtr;
	}
}


} // namespace ifile


