#include <iser/CJsonStringWriteArchive.h>


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>


namespace iser
{


// public methods

CJsonStringWriteArchive::CJsonStringWriteArchive(
			QByteArray &inputString,
			const IVersionInfo* versionInfoPtr,
			QJsonDocument::JsonFormat jsonFormat)
	:CTextWriteArchiveBase(versionInfoPtr),
	m_stream(&inputString),
	m_jsonFormat(jsonFormat)
{
	m_stream.setCodec("UTF-8");
	m_firstTag = true;
}


CJsonStringWriteArchive::~CJsonStringWriteArchive()
{
	m_stream.flush();
}


// reimplemented (iser::IArchive)

bool CJsonStringWriteArchive::IsTagSkippingSupported() const
{
	return true;
}


bool CJsonStringWriteArchive::BeginTag(const CArchiveTag& tag)
{
	m_isSeparatorNeeded = false;

	m_currentAttribute.clear();

	if (m_tagsStack.isEmpty() || (m_tagsStack.back() == NULL) || (m_tagsStack.back()->GetTagType() != iser::CArchiveTag::TT_MULTIPLE)){
		int tagType = tag.GetTagType();
		if (m_allowAttribute && (tagType == iser::CArchiveTag::TT_LEAF)){
			m_currentAttribute = tag.GetId();

			WriteTag(tag, "");

			m_tagsStack.push_back(NULL);

			return true;
		}
		else if (tagType == iser::CArchiveTag::TT_WEAK){
			m_tagsStack.push_back(NULL);

			WriteTag(tag, "{");

			return true;
		}
	}

	WriteTag(tag, "{");

	m_firstTag = true;

	m_tagsStack.push_back(&tag);

	m_allowAttribute = true;

	return true;
}


bool CJsonStringWriteArchive::BeginMultiTag(const CArchiveTag &tag, const CArchiveTag &subTag, int &count)
{
	WriteTag(tag,"[");
	m_firstTag = true;

	m_tagsStack.push_back(&tag);

	m_allowAttribute = true;

	m_isSeparatorNeeded = false;

	return true;
}

bool CJsonStringWriteArchive::EndTag(const CArchiveTag &tag)
{
	m_currentAttribute.clear();

	if (m_tagsStack.isEmpty()){
		return false;
	}

	const iser::CArchiveTag* lastTagPtr = m_tagsStack.back();
	m_tagsStack.pop_back();

	if (lastTagPtr == NULL){
		return true;
	}

	if (lastTagPtr->GetTagType() == iser::CArchiveTag::TT_MULTIPLE){
		m_stream << "]";
	}
	else if (lastTagPtr->GetTagType() == iser::CArchiveTag::TT_GROUP){
		m_stream << "}";
	}

	return true;
}


bool CJsonStringWriteArchive::Process(QString &value)
{
	return WriteTextNode("\"" + value.toUtf8() + "\"");
}


// protected methods

void CJsonStringWriteArchive::WriteTag(const CArchiveTag &tag, QString separator, bool isWriteTag)
{
	if (!m_firstTag){
		m_stream << ",";
	}

	if (isWriteTag && !tag.GetId().isEmpty()){
		m_stream << "\"" << tag.GetId() << "\":";
	}

	m_stream << separator;

	m_firstTag = false;
}


// reimplemented (iser::CTextWriteArchiveBase)

bool CJsonStringWriteArchive::WriteTextNode(const QByteArray &text)
{
	m_stream << text;

	return true;
}


} // namespace iser


