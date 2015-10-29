#include "ifile/CCompactXmlFileWriteArchive.h"


// Qt includes
#include <QtXml/QDomNodeList>
#include <QtCore/QTextStream>


namespace ifile
{


CCompactXmlFileWriteArchive::CCompactXmlFileWriteArchive(
			const QString& filePath,
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, serializeHeader, rootTag),
	BaseClass2(filePath)
{
	if (!filePath.isEmpty()){
		OpenFile(filePath);
	}
}


CCompactXmlFileWriteArchive::~CCompactXmlFileWriteArchive()
{
	Flush();
}


bool CCompactXmlFileWriteArchive::OpenFile(const QString& filePath)
{
	m_file.setFileName(filePath);
	return m_file.open(QIODevice::WriteOnly | QIODevice::Text);
}


bool CCompactXmlFileWriteArchive::Flush()
{
	if (m_file.isOpen()){
		QTextStream stream(&m_file);

		m_document.save(stream, 4);
		
		m_file.close();

		return true;
	}

	return false;
}


} // namespace ifile


