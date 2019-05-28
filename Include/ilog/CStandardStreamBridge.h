#ifndef ilog_CStandardStreamBridge_included
#define ilog_CStandardStreamBridge_included


// STL includes
#include <iostream>
#include <streambuf>
#include <string>

// Qt includes
#include <QtCore/QString>
#include <QtCore/QMutex>

// ACF includes
#include <ilog/IMessageConsumer.h>


namespace ilog
{


class CStandardStreamBridge: protected std::basic_streambuf<char>
{
public:
	typedef std::basic_streambuf<char> BaseClass;

	CStandardStreamBridge(
				std::ostream &stream,
				ilog::IMessageConsumer& messageConsumer,
				istd::IInformationProvider::InformationCategory informationSeverity);
	virtual ~CStandardStreamBridge();

protected:
	// reimplemented (std::basic_streambuf)
	virtual std::streamsize xsputn(const char* data, std::streamsize bufferSize);
	virtual int_type overflow(int_type v);

private:
	std::ostream& m_stream;
	std::streambuf* m_originalBufferPtr;

	ilog::IMessageConsumer& m_messageConsumer;
	istd::IInformationProvider::InformationCategory m_informationSeverity;

	QString m_textBuffer;
	mutable QMutex m_bufferMutex;
};


} // namespace ilog


#endif // !ilog_CStandardStreamBridge_included


