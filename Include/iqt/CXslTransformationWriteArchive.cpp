#include "iqt/CXslTransformationWriteArchive.h"


// STL includes
#include <sstream>


// Qt includes
#include <QDomNodeList>
#include <QTextStream>
#include <QXmlQuery>
#include <QAbstractMessageHandler>


// ACF includes
#include "istd/CBase64.h"


namespace iqt
{

class WriteArchiveMessageHandler: public QAbstractMessageHandler
{
	I_DECLARE_TR_FUNCTION(WriteArchiveMessageHandler);
public:
	WriteArchiveMessageHandler(CXslTransformationWriteArchive* logger);

protected:
	void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation);

	CXslTransformationWriteArchive* m_loggerPtr;
};


WriteArchiveMessageHandler::WriteArchiveMessageHandler(CXslTransformationWriteArchive* logger)
{
	m_loggerPtr = logger;
}


void WriteArchiveMessageHandler::handleMessage(
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


// public methods

CXslTransformationWriteArchive::CXslTransformationWriteArchive(
			const istd::CString& filePath,
			const istd::CString& xslFilePath,
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr),
	BaseClass2(filePath),
	m_serializeHeader(serializeHeader),
	m_rootTag(rootTag),
	m_isSeparatorNeeded(false)
{
	if (!filePath.empty()){
		OpenDocument(filePath, xslFilePath);
	}
}


CXslTransformationWriteArchive::~CXslTransformationWriteArchive()
{
	Flush();
}


bool CXslTransformationWriteArchive::Flush()
{
	if (m_file.isOpen()){
		if (!m_xslFilePath.IsEmpty()){
			QFile xslFile(iqt::GetQString(m_xslFilePath));
			if (!xslFile.open(QFile::ReadOnly)){
				return false;
			}
			QXmlQuery query(QXmlQuery::XSLT20);
			query.setMessageHandler(new WriteArchiveMessageHandler(this));
			QByteArray byteArray = m_document.toByteArray();
			QBuffer buffer(&byteArray);
			if (!buffer.open(QBuffer::WriteOnly)){
				return false;
			}
			query.setFocus(&buffer);
			query.setQuery(&xslFile);
			query.evaluateTo(&m_file); 
			xslFile.close();

			return true;
		}
		else{
			QTextStream stream(&m_file);

			m_document.save(stream, 4);

			m_file.close();
			return true;
		}
	}

	return false;
}


bool CXslTransformationWriteArchive::OpenDocument(const istd::CString& filePath, const istd::CString& xslFilePath)
{
	bool retVal = true;

	m_file.setFileName(iqt::GetQString(filePath));
	m_file.open(QIODevice::WriteOnly);

	m_xslFilePath = xslFilePath;

	m_document.clear();

	m_currentParent = m_document.createElement(iqt::GetQString(m_rootTag.GetId()));

	m_document.appendChild(m_currentParent);

	if (m_serializeHeader){
		retVal = retVal && SerializeAcfHeader();
	}

	return retVal;
}


// reimplemented (iser::IArchive)

bool CXslTransformationWriteArchive::IsTagSkippingSupported() const
{
	return true;
}


bool CXslTransformationWriteArchive::BeginTag(const iser::CArchiveTag& tag)
{
	QDomElement newElement = m_document.createElement(QString::fromStdString(tag.GetId()));

	m_currentParent.appendChild(newElement);

	m_currentParent = newElement;

	m_isSeparatorNeeded = false;

	return true;
}


bool CXslTransformationWriteArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& /*subTag*/, int& count)
{
	QDomElement newElement = m_document.createElement(QString::fromStdString(tag.GetId()));

	newElement.setAttribute("count", count);
	m_currentParent.appendChild(newElement);

	m_currentParent = newElement;
	m_isSeparatorNeeded = false;

	return true;
}


bool CXslTransformationWriteArchive::EndTag(const iser::CArchiveTag& /*tag*/)
{
	m_currentParent = m_currentParent.parentNode().toElement();

	m_isSeparatorNeeded = false;

	return !m_currentParent.isNull();
}


bool CXslTransformationWriteArchive::Process(bool& value)
{
	return PushTextNode(value? "true": "false");
}


bool CXslTransformationWriteArchive::Process(char& value)
{
	return PushTextNode(QString::number(value));
}


bool CXslTransformationWriteArchive::Process(I_BYTE& value)
{
	return PushTextNode(QString::number(value));
}


bool CXslTransformationWriteArchive::Process(I_SBYTE& value)
{
	return PushTextNode(QString::number(value));
}


bool CXslTransformationWriteArchive::Process(I_WORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXslTransformationWriteArchive::Process(I_SWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXslTransformationWriteArchive::Process(I_DWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXslTransformationWriteArchive::Process(I_SDWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXslTransformationWriteArchive::Process(I_QWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXslTransformationWriteArchive::Process(I_SQWORD& value)
{
	return PushTextNode(QString::number(value));
}


bool CXslTransformationWriteArchive::Process(float& value)
{
	return PushTextNode(QString::number(value));
}


bool CXslTransformationWriteArchive::Process(double& value)
{
	return PushTextNode(QString::number(value));
}


bool CXslTransformationWriteArchive::Process(std::string& value)
{
	return PushTextNode(QString::fromStdString(value));
}


bool CXslTransformationWriteArchive::Process(istd::CString& value)
{
	return PushTextNode(iqt::GetQString(value));
}


bool CXslTransformationWriteArchive::ProcessData(void* dataPtr, int size)
{
	std::string encodedString = istd::CBase64::ConvertToBase64(dataPtr, size);

	return PushTextNode(QString::fromStdString(encodedString));
}


// protected methods

bool CXslTransformationWriteArchive::PushTextNode(const QString& text)
{
	if (m_isSeparatorNeeded){
		QDomElement separator = m_document.createElement(GetQString(GetElementSeparator()));

		m_currentParent.appendChild(separator);
	}

	QDomText newNode = m_document.createTextNode(text);
	m_currentParent.appendChild(newNode);
	m_isSeparatorNeeded = true;

	return true;
}


} // namespace iqt


