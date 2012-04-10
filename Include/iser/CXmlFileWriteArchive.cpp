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

		SerializeXmlHeader();

		if (serializeHeader){
			SerializeAcfHeader();
		}
	}
}


CXmlFileWriteArchive::~CXmlFileWriteArchive()
{
	m_stream.setDevice(NULL);
	m_file.close();
}


} // namespace iser


