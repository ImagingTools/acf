#include "ibase/CMessage.h"


#include "isys/IDateTime.h"

#include "iser/IArchive.h"


namespace ibase
{		


CMessage::CMessage()
{
	m_time.SetCurrentTime();
}


CMessage::CMessage(MessageCategory category, int id, const istd::CString& text, const istd::CString& source)
:	m_category(category),
	m_id(id),
	m_text(text),
	m_source(source)
{
	m_time.SetCurrentTime();
}


void CMessage::SetCategory(MessageCategory category)
{
	m_category = category;
}


void CMessage::SetText(const istd::CString& text)
{
	m_text = text;
}


void CMessage::SetSource(const istd::CString& source)
{
	m_source = source;
}


// reimplemented (ibase::IMessage)

const isys::IDateTime& CMessage::GetTimeStamp() const
{
	return m_time;
}


CMessage::MessageCategory CMessage::GetCategory() const
{
	return m_category;
}


int CMessage::GetId() const
{
	return m_id;
}


istd::CString CMessage::GetText() const
{
	return m_text;
}


istd::CString CMessage::GetSource() const
{
	return m_source;
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
		m_category = ibase::IMessage::MessageCategory(category);
	}

	static iser::CArchiveTag textTag("Text", "Message text");
	retVal = retVal && archive.BeginTag(textTag);
	retVal = retVal && archive.Process(m_text);
	retVal = retVal && archive.EndTag(textTag);

	static iser::CArchiveTag sourceTag("Source", "Message source");
	retVal = retVal && archive.BeginTag(sourceTag);
	retVal = retVal && archive.Process(m_source);
	retVal = retVal && archive.EndTag(sourceTag);

	static iser::CArchiveTag timeStampTag("Timestamp", "Message time stamp");
	retVal = retVal && archive.BeginTag(timeStampTag);
	retVal = retVal && m_time.Serialize(archive);
	retVal = retVal && archive.EndTag(timeStampTag);

	return retVal;
}


} // namespace ibase

