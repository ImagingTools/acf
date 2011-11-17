#include "iqt/CXslTransformationReadArchive.h"


// STL includes
#include <sstream>
#include <cstring>


// Qt includes
#include <QDomNodeList>
#include <QXmlQuery>
#include <QAbstractMessageHandler>
#include <QFile>


// ACF includes
#include "istd/CBase64.h"


namespace iqt
{


class ReadArchiveMessageHandler: public QAbstractMessageHandler
{
	I_DECLARE_TR_FUNCTION(ReadArchiveMessageHandler);
public:
	ReadArchiveMessageHandler(CXslTransformationReadArchive* logger);
protected:
	void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation);

	CXslTransformationReadArchive* m_loggerPtr;
};


ReadArchiveMessageHandler::ReadArchiveMessageHandler(CXslTransformationReadArchive* logger)
{
	m_loggerPtr = logger;
}


void ReadArchiveMessageHandler::handleMessage(
				QtMsgType /*type*/,
				const QString& description,
				const QUrl& /*identifier*/,
				const QSourceLocation& /*sourceLocation*/)
{
	m_loggerPtr->SendLogMessage(
					istd::ILogger::MC_WARNING,
					0,
					tr("Transformation message: ").append(iqt::GetCString(description)),
					"XslTransformationWriteArchive");
}


CXslTransformationReadArchive::CXslTransformationReadArchive(
			const istd::CString& filePath,
			const istd::CString& xslFilePath,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	m_serializeHeader(serializeHeader),
	m_rootTag(rootTag)
{
	if (!filePath.IsEmpty()){
		OpenDocument(filePath, xslFilePath);
	}
}


bool CXslTransformationReadArchive::OpenDocument(const istd::CString& filePath, const istd::CString& xslFilePath)
{
	QFile xmlFile(iqt::GetQString(filePath));
	if (!xmlFile.open(QIODevice::ReadOnly)){
		return false;
	}


	if (xslFilePath.IsEmpty()){
		if (!m_document.setContent(&xmlFile)) {
			xmlFile.close();

			return false;
		}
	}
	else{
		QFile xslfile(iqt::GetQString(xslFilePath));
		if (!xslfile.open(QIODevice::ReadOnly)){
			return false;
		}

		QString content;

		QXmlQuery query(QXmlQuery::XSLT20);
		ReadArchiveMessageHandler handler(this);
		query.setMessageHandler(&handler);
		query.setFocus(&xmlFile);
		query.setQuery(&xslfile);
		query.evaluateTo(&content); 

		if (!m_document.setContent(content)) {
			xmlFile.close();
			xslfile.close();

			return false;
		}
	}

	if (m_currentNode.nodeValue() != iqt::GetQString(m_rootTag.GetId())){
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

bool CXslTransformationReadArchive::IsTagSkippingSupported() const
{
	return true;
}


bool CXslTransformationReadArchive::BeginTag(const iser::CArchiveTag& tag)
{
	QString tagId(tag.GetId().c_str());

	QDomElement element = m_currentNode.firstChildElement(tagId);
	if (!element.isNull()){
		m_currentNode = element;
	}
	return !element.isNull();
}


bool CXslTransformationReadArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count)
{
	QString tagId(tag.GetId().c_str());

	QDomElement element = m_currentNode.firstChildElement(tagId);
	if (!element.isNull()){
		m_currentNode = element;
	}
	else{
		return false;
	}

	int tempCount = 0;
	QDomElement child = element.firstChildElement(QString(subTag.GetId().c_str()));
	while (!child.isNull()){
		tempCount++;
		child = child.nextSiblingElement(QString(subTag.GetId().c_str()));
	}
	count = tempCount;

	return !element.isNull();
}


bool CXslTransformationReadArchive::EndTag(const iser::CArchiveTag& /*tag*/)
{
	QDomNode parent = m_currentNode.parentNode();
	
	parent.removeChild(m_currentNode);

	m_currentNode = parent;

	return !m_currentNode.isNull();
}


bool CXslTransformationReadArchive::Process(bool& value)
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


bool CXslTransformationReadArchive::Process(char& value)
{
	QString text = PullTextNode();

	if (!text.isEmpty()){
		value = text.at(0).toAscii();

		return true;
	}

	return false;
}


bool CXslTransformationReadArchive::Process(I_BYTE& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = I_BYTE(text.toShort(&retVal));

	return retVal;
}


bool CXslTransformationReadArchive::Process(I_SBYTE& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = I_SBYTE(text.toShort(&retVal));

	return retVal;
}


bool CXslTransformationReadArchive::Process(I_WORD& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toUShort(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(I_SWORD& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toShort(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(I_DWORD& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toInt(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(I_SDWORD& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toUInt(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(I_QWORD& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toULongLong(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(I_SQWORD& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toLongLong(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(float& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toFloat(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(double& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toDouble(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(std::string& value)
{
	QString text = PullTextNode();

	value = text.toStdString();

	return true;
}


bool CXslTransformationReadArchive::Process(istd::CString& value)
{
	QString text = PullTextNode();

	value = iqt::GetCString(text);

	return !m_currentNode.isNull();
}


bool CXslTransformationReadArchive::ProcessData(void* dataPtr, int size)
{
	QString text = PullTextNode();

	I_BYTE* data = (I_BYTE*)dataPtr;

	std::vector<I_BYTE> decodedData = istd::CBase64::ConvertFromBase64(text.toStdString());

	I_ASSERT(size == int(decodedData.size()));

	std::memcpy(data, &decodedData[0], size);

	return !m_currentNode.isNull();
}


// protected methods

QString CXslTransformationReadArchive::PullTextNode()
{
	QString text;
	QDomNode node = m_currentNode.firstChild();
	//Kill separator tags (<br/>)
	while (!node.isText() && !node.isNull()){
		QDomNode brNode = node;
		node = node.nextSibling();
		m_currentNode.removeChild(brNode);
	}
	
	text = node.nodeValue();

	m_currentNode.removeChild(node);

	return text;
}


} // namespace iqt


