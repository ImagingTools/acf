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
	if (const_cast<CXmlStringWriteArchive*>(this)->Flush()){
		return m_stream.str();
	}
	else{
		return "";
	}
}



} // namespace iser


