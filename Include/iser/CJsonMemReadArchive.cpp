#include <iser/CJsonMemReadArchive.h>


namespace iser
{


// public methods

CJsonMemReadArchive::CJsonMemReadArchive(const QByteArray& data, bool serializeHeader)
	:BaseClass(serializeHeader)
{
	m_buffer.setData(data);
	m_buffer.open(QIODevice::ReadOnly);

	SetContent(&m_buffer);
}


CJsonMemReadArchive::~CJsonMemReadArchive()
{
	m_buffer.close();
}


} // namespace iser


