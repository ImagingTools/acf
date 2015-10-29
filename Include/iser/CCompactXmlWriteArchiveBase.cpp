#include "iser/CCompactXmlWriteArchiveBase.h"


// Qt includes
#include <QtXml/QDomNodeList>
#include <QtCore/QTextStream>


namespace iser
{


QByteArray CCompactXmlWriteArchiveBase::GetString() const
{
	return m_document.toByteArray(4);
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
		if (tagType == iser::CArchiveTag::TT_LEAF){
			m_currentAttribute = tag.GetId();

			m_tagsStack.push_back(NULL);

			return true;
		}
		else if (tagType == iser::CArchiveTag::TT_WEAK){
			m_tagsStack.push_back(NULL);

			return true;
		}
	}

	QDomElement newElement = m_document.createElement(tag.GetId());

	m_currentParent.appendChild(newElement);

	m_currentParent = newElement;

	m_tagsStack.push_back(&tag);

	return true;
}


bool CCompactXmlWriteArchiveBase::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& /*subTag*/, int& /*count*/)
{
	QDomElement newElement = m_document.createElement(tag.GetId());

	m_currentParent.appendChild(newElement);

	m_currentParent = newElement;

	m_tagsStack.push_back(&tag);

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

	m_currentParent = m_currentParent.parentNode().toElement();

	return !m_currentParent.isNull();
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
	m_isSeparatorNeeded(false)
{
	m_document.clear();

	m_currentParent = m_document.createElement(m_rootTag.GetId());

	m_document.appendChild(m_currentParent);

	if (m_serializeHeader){
		SerializeAcfHeader();
	}
}


bool CCompactXmlWriteArchiveBase::WriteStringNode(const QString& text)
{
	if (m_currentAttribute.isEmpty()){
		if (m_isSeparatorNeeded){
			QDomElement separator = m_document.createElement(GetElementSeparator());

			m_currentParent.appendChild(separator);
		}

		QDomText newNode = m_document.createTextNode(text);
		m_currentParent.appendChild(newNode);
	}
	else{
		m_currentParent.setAttribute(m_currentAttribute, text);
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


