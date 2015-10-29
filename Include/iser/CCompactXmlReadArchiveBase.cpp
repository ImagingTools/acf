#include "iser/CCompactXmlReadArchiveBase.h"


// include STL
#include <cstring>

// Qt includes
#include <QtCore/QFile>
#include <QtXml/QDomNodeList>


namespace iser
{


CCompactXmlReadArchiveBase::CCompactXmlReadArchiveBase(const iser::CArchiveTag& rootTag)
:	m_rootTag(rootTag)
{
}


// reimplemented (iser::IArchive)

bool CCompactXmlReadArchiveBase::IsTagSkippingSupported() const
{
	return true;
}


bool CCompactXmlReadArchiveBase::BeginTag(const iser::CArchiveTag& tag)
{
	Q_ASSERT(m_currentAttribute.isEmpty());

	QString tagId(tag.GetId());

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

	QDomElement element = m_currentParent.firstChildElement(tagId);
	if (!element.isNull()){
		m_currentParent = element;
	}
	else{
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Tag '%1' not found!").arg(QString(tagId)),
						"CompactXmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	m_tagsStack.push_back(&tag);

	return true;
}


bool CCompactXmlReadArchiveBase::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count)
{
	QString tagId(tag.GetId());

	QDomElement element = m_currentParent.firstChildElement(tagId);
	if (!element.isNull()){
		m_currentParent = element;
	}
	else{
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Tag '%1' not found!").arg(QString(tagId)),
						"CompactXmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	int tempCount = 0;
	QDomElement child = element.firstChildElement(QString(subTag.GetId()));
	while (!child.isNull()){
		tempCount++;
		child = child.nextSiblingElement(QString(subTag.GetId()));
	}
	count = tempCount;

	m_tagsStack.push_back(&tag);

	return !element.isNull();
}


bool CCompactXmlReadArchiveBase::EndTag(const iser::CArchiveTag& /*tag*/)
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

	QDomNode parent = m_currentParent.parentNode();
	
	parent.removeChild(m_currentParent);

	m_currentParent = parent.toElement();

	return !m_currentParent.isNull();
}


bool CCompactXmlReadArchiveBase::Process(QString& value)
{
	return ReadStringNode(value);
}


// protected methods

bool CCompactXmlReadArchiveBase::ReadStringNode(QString& text)
{
	if (m_currentAttribute.isEmpty()){
		QDomNode node = m_currentParent.firstChild();
		//Kill separator tags (<br/>)
		while (node.nodeName() == "br"){
			QDomNode brNode = node;
			node = node.nextSibling();
			m_currentParent.removeChild(brNode);
		}
	
		text = node.nodeValue();

		m_currentParent.removeChild(node);
	}
	else{
		if (m_currentParent.hasAttribute(m_currentAttribute)){
			text = m_currentParent.attribute(m_currentAttribute);
		}
		else{
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformationProvider::IC_ERROR,
							MI_TAG_ERROR,
							QString("No attribute '%1' found!").arg(QString(m_currentAttribute)),
							"CompactXmlReader",
							istd::IInformationProvider::ITF_SYSTEM);
			}

			return false;
		}
	}

	return !m_currentParent.isNull();
}


// reimplemented (iser::CTextReadArchiveBase)

bool CCompactXmlReadArchiveBase::ReadTextNode(QByteArray& text)
{
	QString stringText;

	if (ReadStringNode(stringText)){
		text = stringText.toLocal8Bit();

		return true;
	}

	return false;
}


// reimplemented (istd::ILogger)

void CCompactXmlReadArchiveBase::DecorateMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			int flags,
			QString& message,
			QString& messageSource) const
{
	BaseClass::DecorateMessage(category, id, flags, message, messageSource);

	QStringList nodesList;

	for (QDomNode el = m_currentParent; el.isElement(); el = el.parentNode()){
		nodesList.push_front(el.nodeName());
	}

	QString nodePath = nodesList.join("/");
	if (!m_currentAttribute.isEmpty()){
		nodePath += QString("@") + QString(m_currentAttribute);
	}

	message = QObject::tr("%1 (node: %2)").arg(message).arg(nodePath);
}


} // namespace iser


