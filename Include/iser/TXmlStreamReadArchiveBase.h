#ifndef iser_TXmlStreamReadArchiveBase_included
#define iser_TXmlStreamReadArchiveBase_included


#include "iser/CXmlReadArchiveBase.h"


namespace iser
{


template <class StreamClass>
class TXmlStreamReadArchiveBase: public CXmlReadArchiveBase
{
public:
	typedef CXmlReadArchiveBase BaseClass;

protected:
	TXmlStreamReadArchiveBase(const iser::CArchiveTag& rootTag = s_acfRootTag);

	// reimplemented (iser::CXmlReadArchiveBase)
	virtual bool ReadToDelimeter(
				const std::string& delimeters,
				std::string& result,
				bool skipDelimeter = true,
				char* foundDelimeterPtr = NULL);

protected:
	StreamClass m_stream;
	char m_lastReadChar;
	bool m_useLastReadChar;
};


// protected methods

template <class StreamClass>
TXmlStreamReadArchiveBase<StreamClass>::TXmlStreamReadArchiveBase(const iser::CArchiveTag& rootTag)
:	BaseClass(rootTag), m_useLastReadChar(false)
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
	}

	return false;
}


} // namespace iser


#endif // !iser_TXmlStreamReadArchiveBase_included


