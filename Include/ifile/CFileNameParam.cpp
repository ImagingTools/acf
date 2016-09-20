#include <ifile/CFileNameParam.h>


// Qt includes
#include <QtCore/QFileInfo>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace ifile
{


// reimplemented (ifile::IFileNameParam)

int CFileNameParam::GetPathType() const
{
	int pathType = IFileNameParam::PT_UNKNOWN;

	QFileInfo info(m_path);

	if (info.isDir()){
		pathType = IFileNameParam::PT_DIRECTORY;
	} 
	else if (info.isFile()){
		pathType = IFileNameParam::PT_FILE;
	}

	return pathType;
}


const QString& CFileNameParam::GetPath() const
{
	return m_path;
}


void CFileNameParam::SetPath(const QString& path)
{
	if (path != m_path){
		istd::CChangeNotifier notifier(this);

		m_path = path;
	}
}


// reimplemented (iser::ISerializable)

bool CFileNameParam::Serialize(iser::IArchive& archive)
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


} // namespace ifile


