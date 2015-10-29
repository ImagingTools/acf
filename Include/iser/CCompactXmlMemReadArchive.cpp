#include "iser/CCompactXmlMemReadArchive.h"


// include STL
#include <cstring>

// Qt includes
#include <QtCore/QFile>
#include <QtXml/QDomNodeList>


namespace iser
{


CCompactXmlMemReadArchive::CCompactXmlMemReadArchive(
			const QByteArray& inputString,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(rootTag)
{
	if (!m_document.setContent(inputString)){
		return;
	}

	m_currentParent = m_document.documentElement();

	if (!m_currentParent.isNull() && serializeHeader){
		SerializeAcfHeader();
	}
}


// protected methods

// reimplemented (istd::ILogger)

void CCompactXmlMemReadArchive::DecorateMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			int flags,
			QString& message,
			QString& messageSource) const
{
	BaseClass::DecorateMessage(category, id, flags, message, messageSource);

	int lineNumber = m_currentParent.lineNumber();
	if (lineNumber >= 0){
		message = QObject::tr("%1 (Line: %2)").arg(message).arg(lineNumber);
	}
}


} // namespace iser


