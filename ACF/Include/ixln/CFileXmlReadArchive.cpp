#include "ixln/CFileXmlReadArchive.h"


#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>


namespace ixln
{


CFileXmlReadArchive::CFileXmlReadArchive(const std::string& fileName, bool serializeHeader)
{
	wchar_t* tmpData = xercesc::XMLString::transcode(fileName.c_str());
	m_fileName = tmpData;
	xercesc::XMLString::release(&tmpData);

	xercesc::LocalFileInputSource inputSource(m_fileName.c_str());
	Init(inputSource);

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


} // namespace ixln


