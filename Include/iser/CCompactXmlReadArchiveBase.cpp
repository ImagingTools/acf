#include <iser/CCompactXmlReadArchiveBase.h>


// include STL
#include <cstring>

// Qt includes
#include <QtCore/QFile>
#include <QtXml/QDomNodeList>


namespace iser
{


CCompactXmlReadArchiveBase::CCompactXmlReadArchiveBase(
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	m_serializeHeader(serializeHeader),
	m_rootTag(rootTag),
	m_isNewFormat(true),
	m_allowAttribute(false)
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

	m_allowAttribute = true;

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
	for (		QDomElement child = element.firstChildElement(QString(subTag.GetId()));
				!child.isNull();
				child = child.nextSiblingElement(QString(subTag.GetId()))){
		tempCount++;
	}
	count = tempCount;

	m_tagsStack.push_back(&tag);

	m_allowAttribute = true;

	return !element.isNull();
}


bool CCompactXmlReadArchiveBase::EndTag(const iser::CArchiveTag& tag)
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

	if (&tag != lastTagPtr){
		SendLogMessage(
					istd::IInformationProvider::IC_ERROR,
					MI_TAG_ERROR,
					QString("Closing tag '%1' was not opened!").arg(QString(tag.GetId())),
					"CompactXmlReader",
					istd::IInformationProvider::ITF_SYSTEM);

		return false;
	}

	Q_ASSERT (lastTagPtr->GetId() == m_currentParent.tagName().toLatin1());

	QDomNode parent = m_currentParent.parentNode();
	
	parent.removeChild(m_currentParent);

	m_currentParent = parent.toElement();

	if (m_isNewFormat){
		m_allowAttribute = false;
	}

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
		// check if the node is separator
		QDomNode node = m_currentParent.firstChild();
		while (!node.isNull() && ((node.nodeName() == GetElementSeparator()) || (node.nodeType() != QDomNode::TextNode))){
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


bool CCompactXmlReadArchiveBase::SetContent(QIODevice* devicePtr)
{
	bool retVal = false;

	if (m_document.setContent(devicePtr)){
		m_currentParent = m_document.documentElement();

		retVal = !m_currentParent.isNull();

		m_allowAttribute = true;

		if (m_serializeHeader){
			retVal = retVal && SerializeAcfHeader();
		}

		quint32 frameworkVersion = quint32(-1);
		GetVersionInfo().GetVersionNumber(iser::IVersionInfo::AcfVersionId, frameworkVersion);

		m_isNewFormat = (frameworkVersion >= 4052);
	}

	return retVal;
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


