#include "iser/CXmlStringReadArchive.h"


namespace iser
{


CXmlStringReadArchive::CXmlStringReadArchive(std::string& outputString, bool serializeHeader, const iser::CArchiveTag& rootTag)
:	BaseClass(rootTag)
{
	m_stream.str(outputString);

	SerializeXmlHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


} // namespace iser


