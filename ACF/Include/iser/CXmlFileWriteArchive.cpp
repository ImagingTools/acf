#include "iser/CXmlFileWriteArchive.h"


namespace iser
{


CXmlFileWriteArchive::CXmlFileWriteArchive(
			const istd::CString& fileName,
			const IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, rootTag)
{
	m_stream.open(fileName.ToString().c_str(), std::fstream::out);

	SerializeXmlHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


} // namespace iser


