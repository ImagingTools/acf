#include "iser/CXmlFileWriteArchive.h"


namespace iser
{


CXmlFileWriteArchive::CXmlFileWriteArchive(
			const QString& filePath,
			const IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, rootTag),
	BaseClass2(filePath),
	m_file(filePath)
{
	if (m_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
		m_stream.setDevice(&m_file);

		WriteXmlHeader();

		if (serializeHeader){
			SerializeAcfHeader();
		}
	}
}


CXmlFileWriteArchive::~CXmlFileWriteArchive()
{
	if (m_file.isOpen()){
		WriteXmlFooter();

		m_file.close();
	}
}


// protected methods

// reimplemented (iser::CXmlWriteArchiveBase)

bool CXmlFileWriteArchive::WriteString(const QByteArray& value)
{
	m_stream << value;

	return m_stream.status() == QTextStream::Ok;
}


} // namespace iser


