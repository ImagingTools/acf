#include <ifile/CJsonFileWriteArchive.h>


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>


namespace ifile
{


// public methods

CJsonFileWriteArchive::CJsonFileWriteArchive(const QString& filePath,
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
	:BaseClass(versionInfoPtr, serializeHeader, rootTag)
{
	if (!filePath.isEmpty()) {
		OpenFile(filePath);
	}
}

CJsonFileWriteArchive::~CJsonFileWriteArchive()
{
	if (m_file.isOpen()){
		Flush();

		m_file.close();
	}
}


bool CJsonFileWriteArchive::OpenFile(const QString &filePath)
{
	m_file.setFileName(filePath);

	if (m_file.open(QIODevice::WriteOnly)){
		InitArchive(&m_file);

		return true;
	}

	return false;
}


} // namespace ifile


