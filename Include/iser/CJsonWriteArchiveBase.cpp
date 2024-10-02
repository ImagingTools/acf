#include <iser/CJsonWriteArchiveBase.h>


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

// ACF includes
#include <iser/CArchiveHeaderInfo.h>


namespace iser
{


// public methods

CJsonWriteArchiveBase::CJsonWriteArchiveBase(
			const IVersionInfo *versionInfoPtr,
			QJsonDocument::JsonFormat jsonFormat)
			:  CTextWriteArchiveBase(versionInfoPtr),
			m_jsonFormat(jsonFormat),
			m_rootTag("", "", iser::CArchiveTag::TT_GROUP)
{
}


CJsonWriteArchiveBase::~CJsonWriteArchiveBase()
{
}


// reimplemented (iser::IArchive)

bool CJsonWriteArchiveBase::IsTagSkippingSupported() const
{
	return true;
}


bool CJsonWriteArchiveBase::BeginTag(const CArchiveTag& tag)
{
	bool retVal = true;
	QString tagId(tag.GetId());
	int tagType = tag.GetTagType();

	if (tagType == iser::CArchiveTag::TT_LEAF || tagType == iser::CArchiveTag::TT_WEAK){
		retVal = retVal && WriteTag(tag, "");
		m_tagsStack.push_back({ &tag, false });

		return retVal;
	}
	else if (tagType == iser::CArchiveTag::TT_GROUP || tagType == iser::CArchiveTag::TT_UNKNOWN){
		retVal = WriteTag(tag, "{");
		m_firstTag = true;
		m_tagsStack.push_back({ &tag, false });

		return retVal;
	}

	return false;
}


bool CJsonWriteArchiveBase::BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& /*subTag*/, int&/*count*/)
{
	bool retVal = WriteTag(tag,"[");
	m_firstTag = true;

	m_tagsStack.push_back({ &tag, true });

	m_allowAttribute = true;

	return retVal;
}


bool CJsonWriteArchiveBase::EndTag(const CArchiveTag& /*tag*/)
{
	if (m_tagsStack.isEmpty()){
		return false;
	}

	TagsStackItem lastItem = m_tagsStack.last();
	m_tagsStack.pop_back();

	if (lastItem.m_tagPtr == NULL){
		return false;
	}

	int tagType = lastItem.m_tagPtr->GetTagType();

	if (lastItem.m_isMultiTag){
		m_stream << "]";
	}
	else if (tagType == iser::CArchiveTag::TT_GROUP || tagType == iser::CArchiveTag::TT_UNKNOWN){
		m_stream << "}";
	}
	m_firstTag = false;

	return true;
}


bool CJsonWriteArchiveBase::Process(QString &value)
{
	QByteArray valueData = value.toUtf8();

	return Process(valueData);
}


bool CJsonWriteArchiveBase::Process(QByteArray &value)
{
	value.replace('\\', "\\\\");
	value.replace('\"', "\\\"");
	value.replace('\n', "\\n");
	value.replace('\r', "\\r");
	value.replace('\t', "\\t");


	m_quotationMarksRequired = true;

	return WriteTextNode(value);
}


bool CJsonWriteArchiveBase::ProcessData(void* dataPtr, int size)
{
	m_quotationMarksRequired = true;

	return BaseClass::ProcessData(dataPtr, size);
}


// protected methods

bool CJsonWriteArchiveBase::InitStream(bool serializeHeader)
{
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
	m_stream.setCodec("UTF-8");
#endif
	m_firstTag = true;

	WriteJsonHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}

	return true;
}


bool CJsonWriteArchiveBase::InitArchive(QIODevice *devicePtr, bool serializeHeader)
{
	m_stream.setDevice(devicePtr);
	
	return InitStream(serializeHeader);
}


bool CJsonWriteArchiveBase::InitArchive(QByteArray &inputString, bool serializeHeader)
{
	m_buffer.setBuffer(&inputString);
	if (m_buffer.open(QIODevice::WriteOnly | QIODevice::Text)){
		m_stream.setDevice(&m_buffer);
	}
	
	return InitStream(serializeHeader);
}


bool CJsonWriteArchiveBase::WriteTag(const CArchiveTag &tag, QString separator)
{
	if (!m_firstTag){
		m_stream << ",";
	}

	bool isWritePrefix = true;
	int tagType = tag.GetTagType();

	if ((tagType == iser::CArchiveTag::TT_UNKNOWN || tagType == iser::CArchiveTag::TT_GROUP || tagType == iser::CArchiveTag::TT_LEAF || tagType == iser::CArchiveTag::TT_WEAK) && !m_tagsStack.isEmpty()){
		if (m_tagsStack.last().m_isMultiTag){
			isWritePrefix = false;
		}
	}

	if (m_tagsStack.isEmpty()){
		isWritePrefix = false;
	}

	if (!tag.GetId().isEmpty() && isWritePrefix){
		m_stream << "\"" << tag.GetId() << "\":";
	}

	m_stream << separator;

	m_firstTag = false;

	return true;
}


bool CJsonWriteArchiveBase::WriteJsonHeader()
{
	return BeginTag(m_rootTag);
}


bool CJsonWriteArchiveBase::Flush()
{
	QIODevice* devicePtr = m_stream.device();
	if (devicePtr != nullptr){
		if (devicePtr->isOpen()){
			bool retVal = EndTag(m_rootTag);

			devicePtr->close();

			return retVal;
		}
	}

	return false;
}


// reimplemented (iser::CTextWriteArchiveBase)

bool CJsonWriteArchiveBase::WriteTextNode(const QByteArray &text)
{
	int tagType = m_tagsStack.last().m_tagPtr->GetTagType();

	bool createFakeTag = tagType == iser::CArchiveTag::TT_GROUP || tagType == iser::CArchiveTag::TT_UNKNOWN;

	if (createFakeTag){
		m_stream << "\"" << m_tagsStack.last().m_tagPtr->GetId() << "\": ";
	}

	if (m_quotationMarksRequired){
		m_stream << "\"";
	}

	m_stream << text;
	
	if (m_quotationMarksRequired){
		m_stream << "\"";
	}

	m_quotationMarksRequired = false;

	return true;
}


} // namespace iser


