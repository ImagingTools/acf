#include "iser/CXmlStringReadArchive.h"


namespace iser
{


CXmlStringReadArchive::CXmlStringReadArchive(
			const QByteArray& inputString,
			bool serializeHeader,
			const CArchiveTag& rootTag)
:	BaseClass(rootTag)
{
	m_buffer.setData(inputString);

	m_buffer.open(QIODevice::ReadOnly | QIODevice::Text);

	m_stream.setDevice(&m_buffer);

	SerializeXmlHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


} // namespace iser


