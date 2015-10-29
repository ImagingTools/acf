#include "ifile/CCompressedXmlFileWriteArchive.h"


// Qt includes
#include <QtXml/QDomNodeList>
#include <QtCore/QTextStream>


namespace ifile
{


CCompressedXmlFileWriteArchive::CCompressedXmlFileWriteArchive(
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


CCompressedXmlFileWriteArchive::~CCompressedXmlFileWriteArchive()
{
	Flush();
}


bool CCompressedXmlFileWriteArchive::OpenFile(const QString& filePath)
{
	m_file.setFileName(filePath);

	return m_file.open(QIODevice::WriteOnly | QIODevice::Text);
}


bool CCompressedXmlFileWriteArchive::Flush()
{
	if (m_file.isOpen()){
		QByteArray data = qCompress(m_document.toByteArray());

		return (m_file.write(data) > 0);
	}

	return false;
}


} // namespace ifile


