// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/CRelativeFileNameParamComp.h>


// Qt includes
#include <QtCore/QDir>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <ifile/IFileArchiveInfo.h>


namespace ifile
{


// public methods

CRelativeFileNameParamComp::CRelativeFileNameParamComp()
:	m_updateBridge(this)
{
}


// reimplemented (ifile::IRelativeFilePath)

QString CRelativeFileNameParamComp::GetBasePath() const
{
	if (m_relativeToCompPtr.IsValid()){
		return m_relativeToCompPtr->GetPath();
	}

	return QString();
}


// reimplemented (ifile::IFileNameParam)

const QString& CRelativeFileNameParamComp::GetPath() const
{
	const QString& relativePath = BaseClass::GetPath();

	if (m_relativeToCompPtr.IsValid()){
		QString basePath = m_relativeToCompPtr->GetPath();
		if (!basePath.isEmpty()){
			QString retVal = QDir::cleanPath(basePath + "/" + relativePath);

			if (m_relativePath != retVal){
				m_relativePath = retVal;
			}

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

	// When a ReferencePath is configured, serialize relative to it instead of
	// the document path. This avoids path corruption when the archive writes
	// to a temporary location (e.g. /tmp/) before moving to the final path.
	QString basePath;
	if (m_relativeToCompPtr.IsValid()){
		basePath = m_relativeToCompPtr->GetPath();
	}

	if (basePath.isEmpty()){
		const ifile::IFileArchiveInfo* fileArchiveInfoPtr = dynamic_cast<const ifile::IFileArchiveInfo*>(&archive);
		if (fileArchiveInfoPtr != NULL){
			QString documentPath = fileArchiveInfoPtr->GetCurrentFilePath();
			if (!documentPath.isEmpty()){
				QFileInfo docuInfo(documentPath);
				basePath = docuInfo.absoluteDir().absolutePath();
			}
		}
	}

	static iser::CArchiveTag pathTag("Path", "File path", iser::CArchiveTag::TT_LEAF);

	if (archive.IsStoring()){
		QString filePath = GetPath();

		if (!basePath.isEmpty()){
			QDir baseDir(basePath);

			filePath = baseDir.relativeFilePath(filePath);
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

		if (!basePath.isEmpty()){
			QDir baseDir(basePath);

			filePath = baseDir.absoluteFilePath(filePath);
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
		m_relativeToModelCompPtr->AttachObserver(&m_updateBridge);
	}
}


void CRelativeFileNameParamComp::OnComponentDestroyed()
{
	m_updateBridge.EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace ifile


