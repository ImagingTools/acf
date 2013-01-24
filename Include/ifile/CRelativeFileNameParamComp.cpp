#include "ifile/CRelativeFileNameParamComp.h"


// Qt includes
#include <QtCore/QDir>


// ACF includes
#include "istd/TChangeNotifier.h"
#include "iser/IFileArchiveInfo.h"


namespace ifile
{


// reimplemented (ifile::IFileNameParam)

const QString& CRelativeFileNameParamComp::GetPath() const
{
	const QString& relativePath = BaseClass::GetPath();

	if (!relativePath.isEmpty() && m_relativeToCompPtr.IsValid()){
		const QString& basePath = m_relativeToCompPtr->GetPath();
		if (!basePath.isEmpty()){
			QDir baseDir(basePath);

			m_relativePath = QDir::cleanPath(baseDir.absoluteFilePath(relativePath));

			return m_relativePath;
		}
	}

	return relativePath;
}


void CRelativeFileNameParamComp::SetPath(const QString& path)
{
	if (!path.isEmpty() && m_relativeToCompPtr.IsValid()){
		const QString& basePath = m_relativeToCompPtr->GetPath();
		if (!basePath.isEmpty()){
			QDir baseDir(basePath);

			BaseClass::SetPath(baseDir.relativeFilePath(path));

			return;
		}
	}

	BaseClass::SetPath(path);
}


// reimplemented (iser::ISerializable)

bool CRelativeFileNameParamComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	QString documentPath;
	const iser::IFileArchiveInfo* fileArchiveInfoPtr = dynamic_cast<const iser::IFileArchiveInfo*>(&archive);
	if (fileArchiveInfoPtr != NULL){
		documentPath = fileArchiveInfoPtr->GetCurrentFilePath();
	}

	static iser::CArchiveTag pathTag("Path", "File path");

	if (archive.IsStoring()){
		QString filePath = GetPath();

		if (!documentPath.isEmpty()){
			QFileInfo docuInfo(documentPath);

			filePath = docuInfo.absoluteDir().relativeFilePath(filePath);
		}

		retVal = retVal && archive.BeginTag(pathTag);
		retVal = retVal && archive.Process(filePath);
		retVal = retVal && archive.EndTag(pathTag);
	}
	else{
		QString filePath;

		retVal = retVal && archive.BeginTag(pathTag);
		retVal = retVal && archive.Process(filePath);
		retVal = retVal && archive.EndTag(pathTag);

		if (!retVal){
			return false;
		}

		if (!documentPath.isEmpty()){
			QFileInfo docuInfo(documentPath);

			filePath = docuInfo.absoluteDir().absoluteFilePath(filePath);
		}

		if (filePath != GetPath()){
			istd::CChangeNotifier changePtr(this);

			SetPath(filePath);
		}
	}

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CRelativeFileNameParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_relativeToModelCompPtr.IsValid()){
		m_relativeToModelCompPtr->AttachObserver(this);
	}
}


void CRelativeFileNameParamComp::OnComponentDestroyed()
{
	EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace ifile


