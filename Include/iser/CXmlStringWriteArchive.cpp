#include "iser/CXmlStringWriteArchive.h"


namespace iser
{


CXmlStringWriteArchive::CXmlStringWriteArchive(
			const IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, rootTag)
{
	m_buffer.open(QIODevice::WriteOnly | QIODevice::Text);

	m_stream.setDevice(&m_buffer);

	SerializeXmlHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


CXmlStringWriteArchive::~CXmlStringWriteArchive()
{
	m_stream.setDevice(NULL);
}


const QByteArray& CXmlStringWriteArchive::GetString() const
{
	return m_buffer.data();
}



} // namespace iser


