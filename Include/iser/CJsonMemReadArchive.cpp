#include <iser/CJsonMemReadArchive.h>


namespace iser
{


// public methods

CJsonMemReadArchive::CJsonMemReadArchive(const QByteArray& data, bool serializeHeader)
{
	InitArchive(data, serializeHeader);
}


} // namespace iser


