#include "ifile/CCompressedXmlFileReadArchive.h"


// include STL
#include <cstring>

// Qt includes
#include <QtCore/QFile>
#include <QtXml/QDomNodeList>


namespace ifile
{


CCompressedXmlFileReadArchive::CCompressedXmlFileReadArchive(
			const QString& filePath,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(rootTag),
	m_serializeHeader(serializeHeader)
{
	if (!filePath.isEmpty()){
		OpenFile(filePath);
	}
}


bool CCompressedXmlFileReadArchive::OpenFile(const QString& filePath)
{
	m_openFileName = "";

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return false;
	}

	m_openFileName = filePath;

	if (!m_document.setContent(qUncompress(file.readAll()))){
		file.close();

		return false;
	}

	m_currentParent = m_document.documentElement();

	bool retVal = !m_currentParent.isNull();

	if (m_serializeHeader){
		retVal = retVal && SerializeAcfHeader();
	}

	return retVal;
}


// protected methods

// reimplemented (istd::ILogger)

void CCompressedXmlFileReadArchive::DecorateMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			int flags,
			QString& message,
			QString& messageSource) const
{
	BaseClass::DecorateMessage(category, id, flags, message, messageSource);

	message = QObject::tr("%2 : %1").arg(message).arg(m_openFileName);
}


} // namespace ifile


