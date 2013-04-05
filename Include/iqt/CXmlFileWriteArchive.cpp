#include "iqt/CXmlFileWriteArchive.h"


// Qt includes
#include <QtXml/QDomNodeList>
#include <QtCore/QTextStream>


// ACF includes
#include "istd/CBase64.h"


namespace iqt
{


CXmlFileWriteArchive::CXmlFileWriteArchive(
			const QString& filePath,
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr),
	BaseClass2(filePath),
	m_serializeHeader(serializeHeader),
	m_rootTag(rootTag),
	m_isSeparatorNeeded(false)
{
	if (!filePath.isEmpty()){
		OpenFile(filePath);
	}
}


CXmlFileWriteArchive::~CXmlFileWriteArchive()
{
	Flush();
}


bool CXmlFileWriteArchive::Flush()
{
	if (m_file.isOpen()){
		QTextStream stream(&m_file);

		m_document.save(stream, 4);
		
		m_file.close();
		return true;
	}

	return false;
}


bool CXmlFileWriteArchive::OpenFile(const QString& filePath)
{
	bool retVal = true;

	m_file.setFileName(filePath);
	m_file.open(QIODevice::WriteOnly | QIODevice::Text);

	m_document.clear();

	m_currentParent = m_document.createElement(m_rootTag.GetId());

	m_document.appendChild(m_currentParent);

	if (m_serializeHeader){
		retVal = retVal && SerializeAcfHeader();
	}

	return retVal;
}


// reimplemented (iser::IArchive)

bool CXmlFileWriteArchive::IsTagSkippingSupported() const
{
	return true;
}


bool CXmlFileWriteArchive::BeginTag(const iser::CArchiveTag& tag)
{
	QDomElement newElement = m_document.createElement(tag.GetId());

	m_currentParent.appendChild(newElement);

	m_currentParent = newElement;

	m_isSeparatorNeeded = false;

	return true;
}


bool CXmlFileWriteArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& /*subTag*/, int& count)
{
	QDomElement newElement = m_document.createElement(tag.GetId());

	newElement.setAttribute("count", count);
	m_currentParent.appendChild(newElement);

	m_currentParent = newElement;
	m_isSeparatorNeeded = false;

	return true;
}


bool CXmlFileWriteArchive::EndTag(const iser::CArchiveTag& /*tag*/)
{
	m_currentParent = m_currentParent.parentNode().toElement();

	m_isSeparatorNeeded = false;

	return !m_currentParent.isNull();
}


bool CXmlFileWriteArchive::Process(bool& value)
{
	return PushTextNode(value? "true": "false");
}


bool CXmlFileWriteArchive::Process(char& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(quint8& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(qint8& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(quint16& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(qint16& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(quint32& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(qint32& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(quint64& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(qint64& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(float& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(double& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(QByteArray& value)
{
	return PushTextNode(value);
}


bool CXmlFileWriteArchive::Process(QString& value)
{
	return PushTextNode(value);
}


bool CXmlFileWriteArchive::ProcessData(void* dataPtr, int size)
{
	QByteArray encodedString = QByteArray((const char*)dataPtr, size).toBase64();

	return PushTextNode(encodedString);
}


// protected methods

bool CXmlFileWriteArchive::PushTextNode(const QString& text)
{
	if (m_isSeparatorNeeded){
		QDomElement separator = m_document.createElement(GetElementSeparator());

		m_currentParent.appendChild(separator);
	}

	QDomText newNode = m_document.createTextNode(text);
	m_currentParent.appendChild(newNode);
	m_isSeparatorNeeded = true;

	return true;
}


} // namespace iqt


