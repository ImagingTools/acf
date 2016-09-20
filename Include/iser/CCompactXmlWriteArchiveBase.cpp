#include <iser/CCompactXmlWriteArchiveBase.h>


// Qt includes
#include <QtXml/QDomNodeList>
#include <QtCore/QTextStream>


namespace iser
{


CCompactXmlWriteArchiveBase::~CCompactXmlWriteArchiveBase()
{
	Flush();
}


// reimplemented (iser::IArchive)

bool CCompactXmlWriteArchiveBase::IsTagSkippingSupported() const
{
	return true;
}


bool CCompactXmlWriteArchiveBase::BeginTag(const iser::CArchiveTag& tag)
{
	Q_ASSERT(m_currentAttribute.isEmpty());

	m_isSeparatorNeeded = false;
	m_currentAttribute.clear();

	if (m_tagsStack.isEmpty() || (m_tagsStack.back() == NULL) || (m_tagsStack.back()->GetTagType() != iser::CArchiveTag::TT_MULTIPLE)){
		int tagType = tag.GetTagType();
		if (m_allowAttribute && (tagType == iser::CArchiveTag::TT_LEAF)){
			m_currentAttribute = tag.GetId();

			m_tagsStack.push_back(NULL);

			return true;
		}
		else if (tagType == iser::CArchiveTag::TT_WEAK){
			m_tagsStack.push_back(NULL);

			return true;
		}
	}

	m_xmlWriter.writeStartElement(tag.GetId());

	m_tagsStack.push_back(&tag);

	m_allowAttribute = true;

	return true;
}


bool CCompactXmlWriteArchiveBase::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& /*subTag*/, int& /*count*/)
{
	m_xmlWriter.writeStartElement(tag.GetId());

	m_tagsStack.push_back(&tag);

	m_allowAttribute = true;

	m_isSeparatorNeeded = false;

	return true;
}


bool CCompactXmlWriteArchiveBase::EndTag(const iser::CArchiveTag& /*tag*/)
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

	m_allowAttribute = false;

	m_xmlWriter.writeEndElement();

	return true;
}


bool CCompactXmlWriteArchiveBase::Process(QString& value)
{
	return WriteStringNode(value);
}


// protected methods

CCompactXmlWriteArchiveBase::CCompactXmlWriteArchiveBase(
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr),
	m_serializeHeader(serializeHeader),
	m_rootTag(rootTag),
	m_isSeparatorNeeded(false),
	m_allowAttribute(false)
{
	m_xmlWriter.setAutoFormatting(true);
}


bool CCompactXmlWriteArchiveBase::InitArchive(QIODevice* devicePtr)
{
	if (devicePtr != NULL){
		m_xmlWriter.setDevice(devicePtr);

		m_xmlWriter.writeStartDocument();
		m_xmlWriter.writeStartElement(m_rootTag.GetId());

		m_allowAttribute = true;

		if (m_serializeHeader){
			SerializeAcfHeader();
		}

		return true;
	}

	return false;
}


bool CCompactXmlWriteArchiveBase::Flush()
{
	if (m_xmlWriter.device() != NULL){
		m_xmlWriter.writeEndElement();
		m_xmlWriter.writeEndDocument();

		m_allowAttribute = false;

		m_xmlWriter.setDevice(NULL);

		return true;
	}

	return false;
}


bool CCompactXmlWriteArchiveBase::WriteStringNode(const QString& text)
{
	if (m_currentAttribute.isEmpty()){
		if (m_isSeparatorNeeded){
			m_xmlWriter.writeEmptyElement(GetElementSeparator());
		}

		m_xmlWriter.writeCharacters(text);
	}
	else{
		m_xmlWriter.writeAttribute(m_currentAttribute, text);
	}

	m_isSeparatorNeeded = true;

	return true;
}


// reimplemented (iser::CTextWriteArchiveBase)

bool CCompactXmlWriteArchiveBase::WriteTextNode(const QByteArray& text)
{
	return WriteStringNode(text);
}


} // namespace iser


