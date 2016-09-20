#include <iser/CCompactXmlMemReadArchive.h>


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
:	BaseClass(serializeHeader, rootTag)
{
	m_buffer.setData(inputString);

	if (!BaseClass::SetContent(&m_buffer)){
		return;
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


