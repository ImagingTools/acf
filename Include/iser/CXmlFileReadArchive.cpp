#include "iser/CXmlFileReadArchive.h"


namespace iser
{


CXmlFileReadArchive::CXmlFileReadArchive(const QString& filePath, bool serializeHeader, const CArchiveTag& rootTag)
:	BaseClass(rootTag),
	BaseClass2(filePath)
{
	m_stream.open(filePath.toStdString().c_str(), std::fstream::in);

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
			QString& message,
			QString& /*messageSource*/) const
{
	message = m_filePath + "(" + QString("%1").arg(m_lastReadLine) + ") : " + message;
}


} // namespace iser


