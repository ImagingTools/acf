#ifndef iser_TXmlStreamReadArchiveBase_included
#define iser_TXmlStreamReadArchiveBase_included


// ACF includes
#include "iser/CXmlReadArchiveBase.h"


namespace iser
{


/**
	Base implementation of a reading archive for XML format using a stream implementation.
*/
template <class StreamClass>
class TXmlStreamReadArchiveBase: public CXmlReadArchiveBase
{
public:
	typedef CXmlReadArchiveBase BaseClass;

	int GetLastReadLine() const;

protected:
	TXmlStreamReadArchiveBase(const CArchiveTag& rootTag = s_acfRootTag);

	// reimplemented (iser::CXmlReadArchiveBase)
	virtual bool ReadToDelimeter(
				const std::string& delimeters,
				std::string& result,
				bool skipDelimeter = true,
				char* foundDelimeterPtr = NULL);

	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				istd::IInformation::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const;

protected:
	StreamClass m_stream;
	char m_lastReadChar;
	bool m_useLastReadChar;

	int m_lastReadLine;
};


template <class StreamClass>
int TXmlStreamReadArchiveBase<StreamClass>::GetLastReadLine() const
{
	return m_lastReadLine;
}


// protected methods

template <class StreamClass>
TXmlStreamReadArchiveBase<StreamClass>::TXmlStreamReadArchiveBase(const CArchiveTag& rootTag)
:	BaseClass(rootTag),
	m_useLastReadChar(false),
	m_lastReadLine(0)
{
}


// reimplemented (iser::CXmlReadArchiveBase)

template <class StreamClass>
bool TXmlStreamReadArchiveBase<StreamClass>::ReadToDelimeter(
			const std::string& delimeters,
			std::string& result,
			bool skipDelimeter,
			char* foundDelimeterPtr)
{
	int cutFromPos = -2;
	int cutToPos = -2;

	std::string readString;

	if (!m_useLastReadChar){
		m_stream.get(m_lastReadChar);

		if (m_lastReadChar == '\n'){
			++m_lastReadLine;
		}
	}

	while (!m_stream.fail()){
		std::string::size_type foundPosition = delimeters.find(m_lastReadChar);
		if (foundPosition != std::string::npos){
			m_useLastReadChar = !skipDelimeter;

			if (cutFromPos < 0){
				if (cutToPos < 0){
					result = "";

					return true;
				}

				cutFromPos = 0;
			}

			if (cutToPos < 0){
				cutToPos = int(readString.size());
			}

			result = readString.substr(cutFromPos, cutToPos - cutFromPos);

			if (foundDelimeterPtr != NULL){
				*foundDelimeterPtr = delimeters.at(foundPosition);
			}

			return true;
		}

		readString += m_lastReadChar;

		if (!isspace((unsigned char)m_lastReadChar) && !iscntrl((unsigned char)m_lastReadChar)){
			cutToPos = int(readString.size());

			if (cutFromPos < 0){
				cutFromPos = cutToPos - 1;
			}
		}

		m_stream.get(m_lastReadChar);

		if (m_lastReadChar == '\n'){
			++m_lastReadLine;
		}
	}

	return false;
}


// reimplemented (istd::ILogger)

template <class StreamClass>
void TXmlStreamReadArchiveBase<StreamClass>::DecorateMessage(
			istd::IInformation::InformationCategory category,
			int id,
			int flags,
			QString& message,
			QString& messageSource) const
{
	BaseClass::DecorateMessage(category, id, flags, message, messageSource);

	message += QString(" (Last parsed line: ") + QString("%1").arg(m_lastReadLine) + ")";
}


} // namespace iser


#endif // !iser_TXmlStreamReadArchiveBase_included


