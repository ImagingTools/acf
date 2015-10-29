#include "ifile/CCompactXmlFileReadArchive.h"


// include STL
#include <cstring>

// Qt includes
#include <QtCore/QFile>
#include <QtXml/QDomNodeList>


namespace ifile
{


CCompactXmlFileReadArchive::CCompactXmlFileReadArchive(
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


bool CCompactXmlFileReadArchive::OpenFile(const QString& filePath)
{
	m_openFileName = "";

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return false;
	}

	m_openFileName = filePath;

	if (!m_document.setContent(&file)){
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

void CCompactXmlFileReadArchive::DecorateMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			int flags,
			QString& message,
			QString& messageSource) const
{
	BaseClass::DecorateMessage(category, id, flags, message, messageSource);

	int lineNumber = m_currentParent.lineNumber();
	if (lineNumber >= 0){
		message = QObject::tr("%2(%3) : %1").arg(message).arg(m_openFileName).arg(lineNumber);
	}
	else{
		message = QObject::tr("%2 : %1").arg(message).arg(m_openFileName);
	}
}


} // namespace ifile


