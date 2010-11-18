#include "iser/CXmlFileReadArchive.h"


namespace iser
{


CXmlFileReadArchive::CXmlFileReadArchive(const istd::CString& filePath, bool serializeHeader, const CArchiveTag& rootTag)
:	BaseClass(rootTag),
	BaseClass2(filePath)
{
	m_stream.open(filePath.ToString().c_str(), std::fstream::in);

	SerializeXmlHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


// protected methods

// reimplemented (istd::ILogger)

void CXmlFileReadArchive::DecorateMessage(
			MessageCategory /*category*/,
			int /*id*/,
			int /*flags*/,
			istd::CString& message,
			istd::CString& /*messageSource*/) const
{
	message = m_filePath + "(" + istd::CString::FromNumber(m_lastReadLine) + ") : " + message;
}


} // namespace iser


