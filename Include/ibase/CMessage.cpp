#include "ibase/CMessage.h"


#include "isys/IDateTime.h"

#include "iser/IArchive.h"

#include "istd/CStaticServicesProvider.h"


namespace ibase
{		


CMessage::CMessage()
{
	InitializeMessageTime();
}


CMessage::CMessage(
			istd::ILogger::MessageCategory category,
			int id,
			const QString& text,
			const QString& source,
			int flags)
:	m_category(category),
	m_id(id),
	m_text(text),
	m_source(source),
	m_flags(flags)
{
	InitializeMessageTime();
}


void CMessage::SetCategory(istd::ILogger::MessageCategory category)
{
	m_category = category;
}


void CMessage::SetText(const QString& text)
{
	m_text = text;
}


void CMessage::SetSource(const QString& source)
{
	m_source = source;
}


// reimplemented (iser::ISerializable)

bool CMessage::Serialize(iser::IArchive& archive)
{
	int category = m_category;

	static iser::CArchiveTag categoryTag("Category", "Message category");
	bool retVal = archive.BeginTag(categoryTag);
	retVal = retVal && archive.Process(category);
	retVal = retVal && archive.EndTag(categoryTag);

	if (!archive.IsStoring()){
		m_category = istd::ILogger::MessageCategory(category);
	}

	static iser::CArchiveTag textTag("Text", "Message text");
	retVal = retVal && archive.BeginTag(textTag);
	retVal = retVal && archive.Process(m_text);
	retVal = retVal && archive.EndTag(textTag);

	static iser::CArchiveTag sourceTag("Source", "Message source");
	retVal = retVal && archive.BeginTag(sourceTag);
	retVal = retVal && archive.Process(m_source);
	retVal = retVal && archive.EndTag(sourceTag);

	I_ASSERT(m_timePtr.IsValid());
	static iser::CArchiveTag timeStampTag("Timestamp", "Message time stamp");
	retVal = retVal && archive.BeginTag(timeStampTag);
	retVal = retVal && m_timePtr->Serialize(archive);
	retVal = retVal && archive.EndTag(timeStampTag);


	return retVal;
}


// private methods

void CMessage::InitializeMessageTime()
{
	m_timePtr = istd::CreateService<isys::IDateTime>();
	if (!m_timePtr.IsValid()){
		m_timePtr.SetPtr(new isys::CSimpleDateTime);
	}

	m_timePtr->SetCurrentTime();
}


} // namespace ibase

