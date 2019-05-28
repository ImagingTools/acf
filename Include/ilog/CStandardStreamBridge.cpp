#include <ilog/CStandardStreamBridge.h>


// ACF includes
#include <ilog/CMessage.h>


namespace ilog
{


// public methods

CStandardStreamBridge::CStandardStreamBridge(
			std::ostream &stream,
			ilog::IMessageConsumer& messageConsumer,
			istd::IInformationProvider::InformationCategory informationSeverity)
	:m_stream(stream),
	m_messageConsumer(messageConsumer),
	m_informationSeverity(informationSeverity)
{
	// Store original buffer:
	m_originalBufferPtr = stream.rdbuf();

	// Insert hook:
	stream.rdbuf(this);
}


CStandardStreamBridge::~CStandardStreamBridge()
{
	QMutexLocker lock(&m_bufferMutex);

	// Restore the original buffer:
	m_stream.rdbuf(m_originalBufferPtr);
}


// protected methods

// reimplemented (std::basic_streambuf)

std::streamsize CStandardStreamBridge::xsputn(const char* data, std::streamsize bufferSize)
{
	QMutexLocker lock(&m_bufferMutex);

	m_textBuffer += data;

	return bufferSize;
}


std::basic_streambuf<char>::int_type CStandardStreamBridge::overflow(int_type v)
{
	QMutexLocker lock(&m_bufferMutex);

	if (v == '\n'){
		m_messageConsumer.AddMessage(istd::TSmartPtr<const istd::IInformationProvider>(new ilog::CMessage(m_informationSeverity, 0, m_textBuffer, "")));

		m_textBuffer.clear();
	}
	else{
		m_textBuffer += v;
	}

	return v;
}


} // namespace ilog


