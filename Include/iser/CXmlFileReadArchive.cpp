#include "iser/CXmlFileReadArchive.h"


namespace iser
{


CXmlFileReadArchive::CXmlFileReadArchive(const QString& filePath, bool serializeHeader, const CArchiveTag& rootTag)
:	BaseClass(rootTag),
	BaseClass2(filePath),
	m_file(filePath)
{
	if (m_file.open(QIODevice::ReadOnly | QIODevice::Text)){
		m_stream.setDevice(&m_file);

		ReadXmlHeader();

		if (serializeHeader){
			SerializeAcfHeader();
		}
	}
}


CXmlFileReadArchive::~CXmlFileReadArchive()
{
	if (m_file.isOpen()){
		ReadXmlFooter();

		m_stream.setDevice(NULL);

		m_file.close();
	}
}


// protected methods

// reimplemented (istd::ILogger)

void CXmlFileReadArchive::DecorateMessage(
			istd::IInformation::InformationCategory /*category*/,
			int /*id*/,
			int /*flags*/,
			QString& message,
			QString& /*messageSource*/) const
{
	message = m_filePath + "(" + QString("%1").arg(GetLastReadLine()) + ") : " + message;
}


} // namespace iser


