#include <ifile/CCompressedXmlFileReadArchive.h>


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
:	BaseClass(serializeHeader, rootTag)
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

	m_buffer.setData(qUncompress(file.readAll()));

	if (!BaseClass::SetContent(&m_buffer)){
		file.close();

		return false;
	}

	return true;
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


