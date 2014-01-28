#include "ifile/CSystemLocationComp.h"


// Qt includes
#include <QtCore/QDir>
#if QT_VERSION < 0x050000
	#include <QtGui/QDesktopServices>
#else
	#include <QtCore/QStandardPaths>
#endif


namespace ifile
{


// public methods

// reimplemented (ifile::IFileNameParam)

int CSystemLocationComp::GetPathType() const
{
	return PT_DIRECTORY;
}


const QString& CSystemLocationComp::GetPath() const
{
	return m_storagePath;
}


void CSystemLocationComp::SetPath(const QString& /*path*/)
{
	I_CRITICAL();
}


// reimplemented (iser::ISerializable)

bool CSystemLocationComp::Serialize(iser::IArchive& /*archive*/)
{
	return true;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CSystemLocationComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	Q_ASSERT(m_locationTypeAttrPtr.IsValid());

#if QT_VERSION < 0x050000
	m_storagePath = QDesktopServices::storageLocation(QDesktopServices::StandardLocation(*m_locationTypeAttrPtr));
#else
	m_storagePath = QStandardPaths::writableLocation(QStandardPaths::StandardLocation(*m_locationTypeAttrPtr));
#endif

	switch (*m_locationTypeAttrPtr){
		case SL_WORKING_DIRECTORY:
			m_storagePath = QDir().absolutePath(); 
			break;

		case SL_EXECUTABLE_DIRECTORY:
			m_storagePath = QCoreApplication::applicationDirPath(); 
			break;

		case SL_EXECUTABLE_FILE:
			m_storagePath = QCoreApplication::applicationFilePath(); 
			break;
	}
}


} // namespace ifile


