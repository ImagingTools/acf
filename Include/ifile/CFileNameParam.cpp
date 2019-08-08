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


// reimplemented (istd::IChangeable)

int CFileNameParam::GetSupportedOperations() const
{
	return SO_COPY | SO_COMPARE;
}


bool CFileNameParam::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const CFileNameParam* sourcePtr = dynamic_cast<const CFileNameParam*>(&object);
	if (sourcePtr != NULL){
		SetPath(sourcePtr->GetPath());

		return true;
	}

	return false;
}


bool CFileNameParam::IsEqual(const IChangeable& object) const
{
	const CFileNameParam* sourcePtr = dynamic_cast<const CFileNameParam*>(&object);
	if (sourcePtr != NULL){
		QString pathThis = GetPath();
		QString partSource = sourcePtr->GetPath();
		if (pathThis.isEmpty() && partSource.isEmpty()) {
			return true;
		}

		return QFileInfo(pathThis) == QFileInfo(partSource);
	}

	return false;
}


} // namespace ifile


