#include "iser/CXmlStringReadArchive.h"


namespace iser
{


CXmlStringReadArchive::CXmlStringReadArchive(
			const std::string& inputString,
			bool serializeHeader,
			const CArchiveTag& rootTag)
:	BaseClass(rootTag)
{
	m_stream.str(inputString);

	SerializeXmlHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


} // namespace iser


