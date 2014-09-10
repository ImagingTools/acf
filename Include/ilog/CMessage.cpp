#include "ilog/CMessage.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "istd/CClassInfo.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iser/CPrimitiveTypesSerializer.h"


namespace ilog
{


// local variables
static istd::IChangeable::ChangeSet allDataChangeIds(istd::IChangeable::CF_ALL_DATA);


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
	BeginChanges(allDataChangeIds);

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

	EndChanges(allDataChangeIds);
}


void CMessage::SetCategory(istd::IInformationProvider::InformationCategory category)
{
	if (category != m_category){
		BeginChanges(allDataChangeIds);

		m_category = category;

		EndChanges(allDataChangeIds);
	}
}


void CMessage::SetText(const QString& text)
{
	if (text != m_text){
		BeginChanges(allDataChangeIds);

		m_text = text;

		EndChanges(allDataChangeIds);
	}
}


void CMessage::SetSource(const QString& source)
{
	if (source != m_source){
		BeginChanges(allDataChangeIds);

		m_source = source;

		EndChanges(allDataChangeIds);
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

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, GetAllChanges());
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
		BeginChanges(allDataChangeIds);

		m_category = sourcePtr->m_category;
		m_id = sourcePtr->m_id;
		m_text = sourcePtr->m_text;
		m_source = sourcePtr->m_source;
		m_flags = sourcePtr->m_flags;
		m_timeStamp = sourcePtr->m_timeStamp;

		EndChanges(allDataChangeIds);

		return true;
	}

	const istd::IInformationProvider* informationProviderPtr = dynamic_cast<const istd::IInformationProvider*>(&object);
	if (informationProviderPtr != NULL){
		BeginChanges(allDataChangeIds);

		m_category = informationProviderPtr->GetInformationCategory();
		m_id = informationProviderPtr->GetInformationId();
		m_text = informationProviderPtr->GetInformationDescription();
		m_source = informationProviderPtr->GetInformationSource();
		m_flags = informationProviderPtr->GetInformationFlags();
		m_timeStamp = informationProviderPtr->GetInformationTimeStamp();

		EndChanges(allDataChangeIds);

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

