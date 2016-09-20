#include <ifile/CGeneratedFileNameParamComp.h>


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QUuid>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

// ACF includes
#include <istd/CSystem.h>


namespace ifile
{


// reimplemented (ifile::IFileNameParam)

void CGeneratedFileNameParamComp::SetPath(const QString& /*path*/)
{
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CGeneratedFileNameParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	QString generatedPath = QUuid::createUuid().toString();

	QStringList extensions;
	if (		m_fileTypeInfoCompPtr.IsValid() &&
				(GetPathType() != PT_DIRECTORY) && 
				m_fileTypeInfoCompPtr->GetFileExtensions(extensions)){
		if (!extensions.empty() && !extensions[0].isEmpty()){
			generatedPath += QString(".") + extensions[0];
		}
	}

	BaseClass::SetPath(generatedPath);

	QString fullPath = GetPath();

	if (m_ensureDirectoryCreatedAttrPtr.IsValid() && *m_ensureDirectoryCreatedAttrPtr){		
		QFileInfo pathInfo(fullPath);

		QString targetPath = GetPathType() == PT_DIRECTORY ? pathInfo.absoluteFilePath() : pathInfo.absolutePath();
		
		QDir directoryPath(pathInfo.absoluteDir());
		if (!directoryPath.mkpath(targetPath)){
			SendErrorMessage(0, QObject::tr("Directory %1 could not be created").arg(targetPath));			
		}
	}
}


void CGeneratedFileNameParamComp::OnComponentDestroyed()
{
	if (m_autoDeleteAttrPtr.IsValid() && *m_autoDeleteAttrPtr){
		QString path = GetPath();

		QFileInfo fileInfo(path);
		if (fileInfo.isDir()){
			istd::CSystem::RemoveDirectory(path);
		}
		else{
			QFile::remove(path);
		}
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace ifile


