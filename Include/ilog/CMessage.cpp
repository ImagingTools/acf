#include "ilog/CMessage.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CClassInfo.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iser/CPrimitiveTypesSerializer.h"


namespace ilog
{		


CMessage::CMessage()
:	m_category(IC_NONE),
	m_id(-1),
	m_flags(0),
	m_timeStamp(QDateTime::currentDateTime())
{
}


CMessage::CMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			const QString& text,
			const QString& source,
			int flags,
			const QDateTime* timeStampPtr)
:	m_category(category),
	m_id(id),
	m_text(text),
	m_source(source),
	m_flags(flags)
{
	if (timeStampPtr != NULL){
		m_timeStamp = *timeStampPtr;
	}
	else{
		m_timeStamp = QDateTime::currentDateTime();
	}
}


void CMessage::SetCategory(istd::IInformationProvider::InformationCategory category)
{
	if (category != m_category){
		istd::CChangeNotifier notifier(this);

		m_category = category;
	}
}


void CMessage::SetText(const QString& text)
{
	if (text != m_text){
		istd::CChangeNotifier notifier(this);

		m_text = text;
	}
}


void CMessage::SetSource(const QString& source)
{
	if (source != m_source){
		istd::CChangeNotifier notifier(this);

		m_source = source;
	}
}


// reimplemented (iser::IObject)

QByteArray CMessage::GetFactoryId() const
{
	return GetMessageTypeId();
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
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeDateTime(archive, m_timeStamp);
	retVal = retVal && archive.EndTag(timeStampTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CMessage::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
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

	const istd::IInformationProvider* informationProviderPtr = dynamic_cast<const istd::IInformationProvider*>(&object);
	if (informationProviderPtr != NULL){
		m_category = informationProviderPtr->GetInformationCategory();
		m_id = informationProviderPtr->GetInformationId();
		m_text = informationProviderPtr->GetInformationDescription();
		m_source = informationProviderPtr->GetInformationSource();
		m_flags = informationProviderPtr->GetInformationFlags();
		m_timeStamp = informationProviderPtr->GetInformationTimeStamp();

		return true;
	}

	return false;
}


istd::IChangeable* CMessage::CloneMe(CompatibilityMode /*mode*/) const
{
	return new CMessage(
			m_category,
			m_id,
			m_text,
			m_source,
			m_flags,
			&m_timeStamp);
}


// public static methods

QByteArray CMessage::GetMessageTypeId()
{
	return "DefaultMessage";
}


} // namespace ilog

