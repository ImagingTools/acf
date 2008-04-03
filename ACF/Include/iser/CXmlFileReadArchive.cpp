#include "iser/CXmlFileReadArchive.h"


namespace iser
{


CXmlFileReadArchive::CXmlFileReadArchive(const istd::CString& fileName, bool serializeHeader, const iser::CArchiveTag& rootTag)
:	BaseClass(rootTag)
{
	m_stream.open(fileName.ToString().c_str(), std::fstream::in);

	SerializeXmlHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


} // namespace iser


