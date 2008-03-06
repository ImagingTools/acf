#include "iqt/CXmlFileReadArchive.h"


#include <sstream>

#include <QDomNodeList>
#include <QFile>


namespace iqt
{


CXmlFileReadArchive::CXmlFileReadArchive(
			const istd::CString& fileName,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	m_serializeHeader(serializeHeader),
	m_rootTag(rootTag)
{
	if (!fileName.IsEmpty()){
		OpenDocument(fileName);
	}
}


bool CXmlFileReadArchive::OpenDocument(const istd::CString& fileName)
{
	QFile file(iqt::GetQString(fileName));
	if (!file.open(QIODevice::ReadOnly)){
		return false;
	}

	if (!m_document.setContent(&file)) {
		file.close();

		return false;
	}

	if (m_currentNode.nodeValue() != iqt::GetQString(m_rootTag.GetId())){
		QDomElement mainElement = m_document.documentElement();

		m_currentNode = mainElement.firstChild();
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
	QString tagId(tag.GetId().c_str());

	QDomElement element = m_currentNode.toElement();
	if (		element.isNull() ||
				(element.nodeName() != tagId)){
		element = m_currentNode.nextSiblingElement(tagId);
	}

	if (element.isNull()){
		return false;
	}

	m_currentNode = element.firstChild();

	return !m_currentNode.isNull();
}


bool CXmlFileReadArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count)
{
	QString tagId(tag.GetId().c_str());

	QDomElement element = m_currentNode.toElement();
	if (		element.isNull() ||
				(element.nodeName() != tagId)){
		element = m_currentNode.nextSiblingElement(tagId);
	}

	if (element.isNull()){
		return false;
	}

	QDomNodeList elementsList = element.elementsByTagName(QString(subTag.GetId().c_str()));

	count = elementsList.size();

	m_currentNode = element.firstChild();

	return !m_currentNode.isNull();
}


bool CXmlFileReadArchive::EndTag(const iser::CArchiveTag& /*tag*/)
{
	m_currentNode = m_currentNode.parentNode();

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


bool CXmlFileReadArchive::Process(I_BYTE& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = I_BYTE(text.toShort(&retVal));

	return retVal;
}


bool CXmlFileReadArchive::Process(I_SBYTE& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = I_SBYTE(text.toShort(&retVal));

	return retVal;
}


bool CXmlFileReadArchive::Process(I_WORD& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toUShort(&retVal);

	return retVal;
}


bool CXmlFileReadArchive::Process(I_SWORD& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toShort(&retVal);

	return retVal;
}


bool CXmlFileReadArchive::Process(I_DWORD& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toInt(&retVal);

	return retVal;
}


bool CXmlFileReadArchive::Process(I_SDWORD& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toUInt(&retVal);

	return retVal;
}


bool CXmlFileReadArchive::Process(I_QWORD& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toULongLong(&retVal);

	return retVal;
}


bool CXmlFileReadArchive::Process(I_SQWORD& value)
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


bool CXmlFileReadArchive::Process(::std::string& value)
{
	QString text = PullTextNode();

	value = text.toStdString();

	return true;
}


bool CXmlFileReadArchive::Process(istd::CString& value)
{
	QString text = PullTextNode();

	value = iqt::GetCString(text);

	return !m_currentNode.isNull();
}


bool CXmlFileReadArchive::ProcessData(void* dataPtr, int size)
{
	QString text = PullTextNode();

	::std::istringstream stream(text.toStdString());
	stream >> ::std::hex;

	for (int i = 0; i < size; i++){
		I_WORD value;
		stream >> value;

		*((I_BYTE*)dataPtr + i) = value;
	}

	return !m_currentNode.isNull();
}


// protected methods

QString CXmlFileReadArchive::PullTextNode()
{
	while (		!m_currentNode.isNull() &&
				!m_currentNode.isText()){
		m_currentNode = m_currentNode.nextSibling();
	}

	QString retVal = m_currentNode.nodeValue();

	m_currentNode = m_currentNode.nextSibling();

	return retVal;
}


} // namespace iqt


