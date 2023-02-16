#include <ifile/CJsonFileWriteArchive.h>


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>


namespace ifile
{


// public methods

CJsonFileWriteArchive::CJsonFileWriteArchive(const QString& filePath, const iser::IVersionInfo* infoPtr)
		: CJsonWriteArchiveBase(infoPtr)
{
	bool serializeHeader = infoPtr != nullptr;
	OpenFile(filePath, serializeHeader);
}

CJsonFileWriteArchive::~CJsonFileWriteArchive()
{
	if (m_file.isOpen()){
		EndTag(m_rootTag);
		m_file.close();
	}
}


bool CJsonFileWriteArchive::OpenFile(const QString &filePath, bool serializeHeader)
{
	m_file.setFileName(filePath);

	if (m_file.open(QIODevice::WriteOnly)){
		InitArchive(&m_file, serializeHeader);
		return true;
	}

	return false;
}


} // namespace ifile


