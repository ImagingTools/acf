#include <ilog/CMessage.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CClassInfo.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>


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


void CMessage::SetMessageValues(
			istd::IInformationProvider::InformationCategory category,
			int id,
			const QString& text, 
			const QString& source,
			int flags,
			const QDateTime* timeStampPtr)
{
	istd::CChangeNotifier changeNotifier(this);
	Q_UNUSED(changeNotifier);

	m_category = category;
	m_id = id;
	m_text = text;
	m_source = source;
	m_flags = flags;

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
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

		m_category = category;
	}
}


void CMessage::SetText(const QString& text)
{
	if (text != m_text){
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

		m_text = text;
	}
}


void CMessage::SetSource(const QString& source)
{
	if (source != m_source){
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

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
	static iser::CArchiveTag categoryTag("Category", "Message category", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag textTag("Text", "Message text", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag sourceTag("Source", "Message source", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag timeStampTag("Timestamp", "Message time stamp", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag idTag("ID", "ID of the message", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool isStoring = archive.IsStoring();

	int category = m_category;

	bool retVal = archive.BeginTag(categoryTag);
	retVal = retVal && archive.Process(category);
	retVal = retVal && archive.EndTag(categoryTag);

	if (!isStoring){
		m_category = istd::IInformationProvider::InformationCategory(category);
	}

	retVal = retVal && archive.BeginTag(textTag);
	retVal = retVal && archive.Process(m_text);
	retVal = retVal && archive.EndTag(textTag);

	retVal = retVal && archive.BeginTag(sourceTag);
	retVal = retVal && archive.Process(m_source);
	retVal = retVal && archive.EndTag(sourceTag);

	retVal = retVal && archive.BeginTag(timeStampTag);
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeDateTime(archive, m_timeStamp);
	retVal = retVal && archive.EndTag(timeStampTag);

	quint32 version = 0;
	if (		!archive.GetVersionInfo().GetVersionNumber(iser::IVersionInfo::AcfVersionId, version) ||
				(version > 3264)){
		retVal = retVal && archive.BeginTag(idTag);
		retVal = retVal && archive.Process(m_id);
		retVal = retVal && archive.EndTag(idTag);
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CMessage::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const CMessage* sourcePtr = dynamic_cast<const CMessage*>(&object);
	if (sourcePtr){
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

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
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

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

