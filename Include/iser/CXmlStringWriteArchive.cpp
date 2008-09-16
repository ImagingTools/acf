#include "iser/CXmlStringWriteArchive.h"


namespace iser
{


CXmlStringWriteArchive::CXmlStringWriteArchive(
			const IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, rootTag)
{
	SerializeXmlHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


std::string CXmlStringWriteArchive::GetString() const
{
	return m_stream.str();
}



} // namespace iser


