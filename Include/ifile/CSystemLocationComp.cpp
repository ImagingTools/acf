#include "ifile/CSystemLocationComp.h"


// Qt includes
#include <QtGui/QDesktopServices>


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

	m_storagePath = QDesktopServices::storageLocation(QDesktopServices::StandardLocation(*m_locationTypeAttrPtr));
}


} // namespace ifile


