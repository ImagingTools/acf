#include <iser/CJsonWriteArchiveBase.h>


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>


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


// reimplemented (iser::IArchive)

bool CJsonWriteArchiveBase::IsTagSkippingSupported() const
{
	return true;
}


bool CJsonWriteArchiveBase::BeginTag(const CArchiveTag& tag)
{
	bool retVal = true;

	int tagType = tag.GetTagType();
	if (tagType == iser::CArchiveTag::TT_LEAF){
		if (m_tagsStack.isEmpty() || m_tagsStack.back()->GetTagType() == iser::CArchiveTag::TT_MULTIPLE){

			return false;
		}

		retVal = WriteTag(tag, "");
		m_tagsStack.push_back(&tag);

		return retVal;
	}
	else if (tagType == iser::CArchiveTag::TT_GROUP){
		retVal = WriteTag(tag, "{");
		m_firstTag = true;
		m_tagsStack.push_back(&tag);

		return retVal;
	}

	return false;
}


bool CJsonWriteArchiveBase::BeginMultiTag(const CArchiveTag &tag, const CArchiveTag &subTag, int &count)
{
	bool retVal = WriteTag(tag,"[");
	m_firstTag = true;

	m_tagsStack.push_back(&tag);

	m_allowAttribute = true;

	return retVal;
}


bool CJsonWriteArchiveBase::EndTag(const CArchiveTag &tag)
{
	if (m_tagsStack.isEmpty()){
		return false;
	}

	const iser::CArchiveTag* lastTagPtr = m_tagsStack.back();
	m_tagsStack.pop_back();

	if (lastTagPtr == NULL){
		return false;
	}

	if (lastTagPtr->GetTagType() == iser::CArchiveTag::TT_MULTIPLE){
		m_stream << "]";
	}
	else if (lastTagPtr->GetTagType() == iser::CArchiveTag::TT_GROUP){
		m_stream << "}";
	}
	m_firstTag = false;

	return true;
}


bool CJsonWriteArchiveBase::Process(QString &value)
{
	return WriteTextNode("\"" + value.toUtf8() + "\"");
}


bool CJsonWriteArchiveBase::Process(QByteArray &value)
{
	return WriteTextNode("\"" + value + "\"");
}


// protected methods

bool CJsonWriteArchiveBase::InitStream()
{
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
	m_stream.setCodec("UTF-8");
#endif
	m_firstTag = true;
	BeginTag(m_rootTag);

	return true;
}


bool CJsonWriteArchiveBase::InitArchive(QIODevice *devicePtr)
{
	m_stream.setDevice(devicePtr);
	
	return InitStream();
}


bool CJsonWriteArchiveBase::InitArchive(QByteArray &inputString)
{
	m_buffer.setBuffer(&inputString);
	if (m_buffer.open(QIODevice::WriteOnly | QIODevice::Text)){
		m_stream.setDevice(&m_buffer);
	}
	
	return InitStream();
}


bool CJsonWriteArchiveBase::WriteTag(const CArchiveTag &tag, QString separator)
{
	if (!m_firstTag){
		m_stream << ",";
	}

	bool isWritePrefix = true;

	if (tag.GetTagType() == iser::CArchiveTag::TT_GROUP && !m_tagsStack.isEmpty()){
		const iser::CArchiveTag* lastTagPtr = m_tagsStack.last();
		if (lastTagPtr->GetTagType() == iser::CArchiveTag::TT_MULTIPLE){
			isWritePrefix = false;
		}
	}

	if (!tag.GetId().isEmpty() && isWritePrefix){
		m_stream << "\"" << tag.GetId() << "\":";
	}

	m_stream << separator;

	m_firstTag = false;

	return true;
}

// reimplemented (iser::CTextWriteArchiveBase)

bool CJsonWriteArchiveBase::WriteTextNode(const QByteArray &text)
{
	m_stream << text;

	return true;
}


} // namespace iser


