#include "iqt/CXmlFileReadArchive.h"


// Qt includes
#include <QtCore/QFile>
#include <QtXml/QDomNodeList>

// ACF includes
#include "istd/CBase64.h"


namespace iqt
{


CXmlFileReadArchive::CXmlFileReadArchive(
			const QString& filePath,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	m_serializeHeader(serializeHeader),
	m_rootTag(rootTag)
{
	if (!filePath.isEmpty()){
		OpenDocument(filePath);
	}
}


bool CXmlFileReadArchive::OpenDocument(const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return false;
	}

	if (!m_document.setContent(&file)) {
		file.close();

		return false;
	}

	if (m_currentNode.nodeValue() != m_rootTag.GetId()){
		QDomElement mainElement = m_document.documentElement();

		m_currentNode = mainElement;
	}

	bool retVal = !m_currentNode.isNull();

	if (m_serializeHeader){
		retVal = retVal && SerializeAcfHeader();
	}

	return retVal;
}


// reimplemented (iser::IArchive)

bool CXmlFileReadArchive::IsTagSkippingSupported() const
{
	return true;
}


bool CXmlFileReadArchive::BeginTag(const iser::CArchiveTag& tag)
{
	QString tagId(tag.GetId());

	QDomElement element = m_currentNode.firstChildElement(tagId);
	if (!element.isNull()){
		m_currentNode = element;
	}
	return !element.isNull();
}


bool CXmlFileReadArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count)
{
	QString tagId(tag.GetId());

	QDomElement element = m_currentNode.firstChildElement(tagId);
	if (!element.isNull()){
		m_currentNode = element;
	}
	else{
		return false;
	}

	int tempCount = 0;
	QDomElement child = element.firstChildElement(QString(subTag.GetId()));
	while (!child.isNull()){
		tempCount++;
		child = child.nextSiblingElement(QString(subTag.GetId()));
	}
	count = tempCount;

	return !element.isNull();
}


bool CXmlFileReadArchive::EndTag(const iser::CArchiveTag& /*tag*/)
{
	QDomNode parent = m_currentNode.parentNode();
	
	parent.removeChild(m_currentNode);

	m_currentNode = parent;

	return !m_currentNode.isNull();
}


bool CXmlFileReadArchive::Process(bool& value)
{
	QString text = PullTextNode();

	if (text == "true"){
		value = true;

		return true;
	}

	if (text == "false"){
		value = false;

		return true;
	}

	return false;
}


bool CXmlFileReadArchive::Process(char& value)
{
	QString text = PullTextNode();

	if (!text.isEmpty()){
		value = text.at(0).toAscii();

		return true;
	}

	return false;
}


bool CXmlFileReadArchive::Process(quint8& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = quint8(text.toShort(&retVal));

	return retVal;
}


bool CXmlFileReadArchive::Process(qint8& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = qint8(text.toShort(&retVal));

	return retVal;
}


bool CXmlFileReadArchive::Process(quint16& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toUShort(&retVal);

	return retVal;
}


bool CXmlFileReadArchive::Process(qint16& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toShort(&retVal);

	return retVal;
}


bool CXmlFileReadArchive::Process(quint32& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toInt(&retVal);

	return retVal;
}


bool CXmlFileReadArchive::Process(qint32& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toUInt(&retVal);

	return retVal;
}


bool CXmlFileReadArchive::Process(quint64& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toULongLong(&retVal);

	return retVal;
}


bool CXmlFileReadArchive::Process(qint64& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toLongLong(&retVal);

	return retVal;
}


bool CXmlFileReadArchive::Process(float& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toFloat(&retVal);

	return retVal;
}


bool CXmlFileReadArchive::Process(double& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toDouble(&retVal);

	return retVal;
}


bool CXmlFileReadArchive::Process(QByteArray& value)
{
	QString text = PullTextNode();

	value = text.toLocal8Bit();

	return true;
}


bool CXmlFileReadArchive::Process(QString& value)
{
	QString text = PullTextNode();

	value = text;

	return !m_currentNode.isNull();
}


bool CXmlFileReadArchive::ProcessData(void* dataPtr, int size)
{
	QString text = PullTextNode();

	quint8* data = (quint8*)dataPtr;

	QVector<quint8> decodedData = istd::CBase64::ConvertFromBase64(text.toLocal8Bit());

	I_ASSERT(size == int(decodedData.size()));

	std::memcpy(data, &decodedData[0], size);

	return !m_currentNode.isNull();
}


// protected methods

QString CXmlFileReadArchive::PullTextNode()
{
	QString text;
	QDomNode node = m_currentNode.firstChild();
	//Kill separator tags (<br/>)
	while (node.nodeName() == "br"){
		QDomNode brNode = node;
		node = node.nextSibling();
		m_currentNode.removeChild(brNode);
	}
	
	text = node.nodeValue();

	m_currentNode.removeChild(node);

	return text;
}


} // namespace iqt


