#include "iqt/CXmlWriteArchive.h"


#include <sstream>

#include <QDomNodeList>
#include <QTextStream>


namespace iqt
{


CXmlWriteArchive::CXmlWriteArchive(const iser::IVersionInfo* versionInfoPtr)
:	BaseClass(versionInfoPtr), m_isSeparatorNeeded(false)
{
}


CXmlWriteArchive::CXmlWriteArchive(
			const istd::CString& fileName,
			const iser::IVersionInfo* versionInfoPtr,
			const istd::CString& xmlRootName)
:	BaseClass(versionInfoPtr), m_isSeparatorNeeded(false)
{
	OpenDocument(fileName, xmlRootName);
}


CXmlWriteArchive::~CXmlWriteArchive()
{
	Flush();
}


bool CXmlWriteArchive::Flush()
{
	if (!m_file.isOpen()){
		QTextStream stream(&m_file);

		m_document.save(stream, 4);

		return true;
	}

	return false;
}


bool CXmlWriteArchive::OpenDocument(const istd::CString& fileName, const istd::CString& xmlRootName)
{
	m_file.setFileName(iqt::GetQString(fileName));
	m_file.open(QIODevice::WriteOnly);

	m_document.clear();

	m_currentParent = m_document.createElement(iqt::GetQString(xmlRootName));

	m_document.appendChild(m_currentParent);

	return true;
}


// reimplemented (iser::IArchive)

bool CXmlWriteArchive::IsTagSkippingSupported() const
{
	return true;
}


bool CXmlWriteArchive::BeginTag(const iser::CArchiveTag& tag)
{
	QDomElement newElement = m_document.createElement(QString::fromStdString(tag.GetId()));

	m_currentParent.appendChild(newElement);

	m_isSeparatorNeeded = false;

	return true;
}


bool CXmlWriteArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& /*subTag*/, int& /*count*/)
{
	return BeginTag(tag);
}


bool CXmlWriteArchive::EndTag(const iser::CArchiveTag& tag)
{
	m_currentParent = m_currentParent.parentNode().toElement();

	m_isSeparatorNeeded = false;

	return !m_currentParent.isNull();
}


bool CXmlWriteArchive::Process(bool& value)
{
	return PushTextNode(value? "true": "false");
}


bool CXmlWriteArchive::Process(char& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlWriteArchive::Process(I_BYTE& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlWriteArchive::Process(I_SBYTE& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlWriteArchive::Process(I_WORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlWriteArchive::Process(I_SWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlWriteArchive::Process(I_DWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlWriteArchive::Process(I_SDWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlWriteArchive::Process(I_QWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlWriteArchive::Process(I_SQWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlWriteArchive::Process(float& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlWriteArchive::Process(double& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlWriteArchive::Process(std::string& value)
{
	return PushTextNode(QString::fromStdString(value));
}


bool CXmlWriteArchive::Process(istd::CString& value)
{
	return PushTextNode(iqt::GetQString(value));
}


bool CXmlWriteArchive::ProcessData(void* dataPtr, int size)
{
	::std::ostringstream stream;
	stream << ::std::hex;

	for (int i = 0; i < size; i++){
		stream << *((I_BYTE*)dataPtr + i);
		if (i < size -1){
			stream << " ";
		}
	}

	return PushTextNode(QString::fromStdString(stream.str()));
}


// protected methods

bool CXmlWriteArchive::PushTextNode(const QString& text)
{
	if (m_isSeparatorNeeded){
		QDomElement separator = m_document.createElement("br");

		m_currentParent.appendChild(separator);
	}

	QDomText newNode = m_document.createTextNode(text);

	m_isSeparatorNeeded = true;

	return true;
}


} // namespace iqt


