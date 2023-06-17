#include <iser/CJsonMemWriteArchive.h>


// Qt includes
#include <QtCore/QJsonDocument>


namespace iser
{


// public methods

CJsonMemWriteArchive::CJsonMemWriteArchive(
			QByteArray &inputString,
			const IVersionInfo* versionInfoPtr,
			QJsonDocument::JsonFormat /*jsonFormat*/)
	:BaseClass(versionInfoPtr)
{
	bool serializeHeader = versionInfoPtr != nullptr;
	InitArchive(inputString, serializeHeader);
}


CJsonMemWriteArchive::~CJsonMemWriteArchive()
{
	if (m_buffer.isOpen()){
		if (m_rootTagEnabled){
			EndTag(m_rootTag);
		}
		m_buffer.close();
	}
}


} // namespace iser


