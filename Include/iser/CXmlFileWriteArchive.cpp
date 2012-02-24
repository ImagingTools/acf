#include "iser/CXmlFileWriteArchive.h"


namespace iser
{


CXmlFileWriteArchive::CXmlFileWriteArchive(
			const QString& filePath,
			const IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, rootTag),
	BaseClass2(filePath)
{
	m_stream.open(filePath.toStdString().c_str(), std::fstream::out);

	SerializeXmlHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


} // namespace iser


