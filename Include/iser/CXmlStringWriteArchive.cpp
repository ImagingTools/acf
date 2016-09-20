#include <iser/CXmlStringWriteArchive.h>


namespace iser
{


CXmlStringWriteArchive::CXmlStringWriteArchive(
			const IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, rootTag)
{
	if (m_buffer.open(QIODevice::WriteOnly | QIODevice::Text)){
		m_stream.setDevice(&m_buffer);

		WriteXmlHeader();

		if (serializeHeader){
			SerializeAcfHeader();
		}
	}
}


CXmlStringWriteArchive::~CXmlStringWriteArchive()
{
	EnsureArchiveClosed();
}


const QByteArray& CXmlStringWriteArchive::GetString() const
{
	const_cast<CXmlStringWriteArchive*>(this)->EnsureArchiveClosed();

	return m_buffer.data();
}


// protected methods

void CXmlStringWriteArchive::EnsureArchiveClosed()
{
	if (m_buffer.isOpen()){
		BaseClass::WriteXmlFooter();
		
		m_stream.setDevice(NULL);

		m_buffer.close();
	}
}


// reimplemented (iser::CXmlWriteArchiveBase)

bool CXmlStringWriteArchive::WriteString(const QByteArray& value)
{
	m_stream << value;

	return m_stream.status() == QTextStream::Ok;
}


} // namespace iser


