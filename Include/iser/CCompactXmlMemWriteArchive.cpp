#include <iser/CCompactXmlMemWriteArchive.h>


// Qt includes
#include <QtXml/QDomNodeList>
#include <QtCore/QTextStream>


namespace iser
{


CCompactXmlMemWriteArchive::CCompactXmlMemWriteArchive(
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, serializeHeader, rootTag)
{
	if (m_textBuffer.open(QIODevice::WriteOnly | QIODevice::Text)){
		InitArchive(&m_textBuffer);
	}
}


const QByteArray& CCompactXmlMemWriteArchive::GetString() const
{
	const_cast<CCompactXmlMemWriteArchive*>(this)->Flush();

	return m_textBuffer.buffer();
}


} // namespace iser


