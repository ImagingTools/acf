#include "ixln/CFileXmlWriteArchive.h"


#include <xercesc/framework/LocalFileFormatTarget.hpp>


namespace ixln
{


CFileXmlWriteArchive::CFileXmlWriteArchive(
					const std::string& fileName,
					const iser::IVersionInfo* versionInfoPtr,
					bool serializeHeader)
:	BaseClass(versionInfoPtr)
{
	wchar_t* tmpData = xercesc::XMLString::transcode(fileName.c_str());
	m_fileName = tmpData;
	xercesc::XMLString::release(&tmpData);

	Init(new xercesc::LocalFileFormatTarget(m_fileName.c_str()));

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


} // namespace ixln


