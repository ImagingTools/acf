#include <iser/CJsonMemReadArchive.h>


namespace iser
{


// public methods

CJsonMemReadArchive::CJsonMemReadArchive(const QByteArray& data, bool serializeHeader)
	:BaseClass(serializeHeader)
{
	m_buffer.setData(data);

	SetContent(&m_buffer);
}


} // namespace iser


