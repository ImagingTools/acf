#include "ibase/CMessage.h"


#include "isys/IDateTime.h"

#include "iser/IArchive.h"


namespace ibase
{		


CMessage::CMessage()
{
	m_time.SetCurrentTime();
}


CMessage::CMessage(MessageCategory category, const istd::CString& text, const istd::CString& source)
	:m_category(category),
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

	return true;
}


} // namespace ibase

