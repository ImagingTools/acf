#include "ibase/CMessage.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ibase
{		


CMessage::CMessage()
:	m_timeStamp(QDateTime::currentDateTime())
{
}


CMessage::CMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			const QString& text,
			const QString& source,
			int flags)
:	m_category(category),
	m_id(id),
	m_text(text),
	m_source(source),
	m_flags(flags),
	m_timeStamp(QDateTime::currentDateTime())
{
}


void CMessage::SetCategory(istd::IInformationProvider::InformationCategory category)
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
	static iser::CArchiveTag categoryTag("Category", "Message category");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	bool isStoring = archive.IsStoring();

	int category = m_category;

	bool retVal = archive.BeginTag(categoryTag);
	retVal = retVal && archive.Process(category);
	retVal = retVal && archive.EndTag(categoryTag);

	if (!isStoring){
		m_category = istd::IInformationProvider::InformationCategory(category);
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
	QString timeStampString = m_timeStamp.toString(Qt::ISODate);
	retVal = retVal && archive.Process(timeStampString);
	retVal = retVal && archive.EndTag(timeStampTag);

	if (!isStoring){
		m_timeStamp = QDateTime::fromString(timeStampString, Qt::ISODate);
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CMessage::CopyFrom(const istd::IChangeable& object)
{
	const CMessage* sourcePtr = dynamic_cast<const CMessage*>(&object);
	if (sourcePtr){
		m_category = sourcePtr->m_category;
		m_id = sourcePtr->m_id;
		m_text = sourcePtr->m_text;
		m_source = sourcePtr->m_source;
		m_flags = sourcePtr->m_flags;
		m_timeStamp = sourcePtr->m_timeStamp;
		
		return true;
	}

	return false;
}



} // namespace ibase

