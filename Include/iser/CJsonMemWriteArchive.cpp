#include <iser/CJsonMemWriteArchive.h>


namespace iser
{


// public methods

CJsonMemWriteArchive::CJsonMemWriteArchive(
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
	:BaseClass(versionInfoPtr, serializeHeader, rootTag)
{
	if (m_textBuffer.open(QIODevice::WriteOnly | QIODevice::Text)){
		InitArchive(&m_textBuffer);
	}
}


const QByteArray& CJsonMemWriteArchive::GetData() const
{
	const_cast<CJsonMemWriteArchive*>(this)->Flush();

	return m_textBuffer.buffer();
}


} // namespace iser


