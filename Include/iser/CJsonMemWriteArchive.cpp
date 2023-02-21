#include <iser/CJsonMemWriteArchive.h>


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>


namespace iser
{


// public methods


CJsonMemWriteArchive::CJsonMemWriteArchive(
			QByteArray &inputString,
			const IVersionInfo* versionInfoPtr,
			QJsonDocument::JsonFormat jsonFormat)
	:BaseClass(versionInfoPtr)
{
	InitArchive(inputString);	
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


