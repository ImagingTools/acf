#include "iqt/CXmlFileWriteArchive.h"


#include <sstream>

#include <QDomNodeList>
#include <QTextStream>


namespace iqt
{


CXmlFileWriteArchive::CXmlFileWriteArchive(
			const istd::CString& filePath,
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr),
	m_serializeHeader(serializeHeader),
	m_rootTag(rootTag),
	m_isSeparatorNeeded(false)
{
	if (!filePath.empty()){
		OpenDocument(filePath);
	}
}


CXmlFileWriteArchive::~CXmlFileWriteArchive()
{
	Flush();
}


bool CXmlFileWriteArchive::Flush()
{
	if (!m_file.isOpen()){
		QTextStream stream(&m_file);

		m_document.save(stream, 4);

		return true;
	}

	return false;
}


bool CXmlFileWriteArchive::OpenDocument(const istd::CString& filePath)
{
	bool retVal = true;

	m_file.setFileName(iqt::GetQString(filePath));
	m_file.open(QIODevice::WriteOnly);

	m_document.clear();

	m_currentParent = m_document.createElement(iqt::GetQString(m_rootTag.GetId()));

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
	QDomElement newElement = m_document.createElement(QString::fromStdString(tag.GetId()));

	m_currentParent.appendChild(newElement);

	m_isSeparatorNeeded = false;

	return true;
}


bool CXmlFileWriteArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& /*subTag*/, int& /*count*/)
{
	return BeginTag(tag);
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


bool CXmlFileWriteArchive::Process(I_BYTE& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(I_SBYTE& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(I_WORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(I_SWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(I_DWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(I_SDWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(I_QWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXmlFileWriteArchive::Process(I_SQWORD& value)
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


bool CXmlFileWriteArchive::Process(std::string& value)
{
	return PushTextNode(QString::fromStdString(value));
}


bool CXmlFileWriteArchive::Process(istd::CString& value)
{
	return PushTextNode(iqt::GetQString(value));
}


bool CXmlFileWriteArchive::ProcessData(void* dataPtr, int size)
{
	std::ostringstream stream;
	stream << std::hex;

	for (int i = 0; i < size; i++){
		stream << *((I_BYTE*)dataPtr + i);
		if (i < size -1){
			stream << " ";
		}
	}

	return PushTextNode(QString::fromStdString(stream.str()));
}


// protected methods

bool CXmlFileWriteArchive::PushTextNode(const QString& text)
{
	if (m_isSeparatorNeeded){
		QDomElement separator = m_document.createElement(GetQString(GetElementSeparator()));

		m_currentParent.appendChild(separator);
	}

	QDomText newNode = m_document.createTextNode(text);

	m_isSeparatorNeeded = true;

	return true;
}


} // namespace iqt


