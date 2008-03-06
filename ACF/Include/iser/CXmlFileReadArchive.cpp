#include "iser/CXmlFileReadArchive.h"


namespace iser
{


CXmlFileReadArchive::CXmlFileReadArchive(const istd::CString& fileName, bool serializeHeader, const iser::CArchiveTag& rootTag)
:	BaseClass(rootTag), m_useLastReadChar(false)
{
	m_stream.open(fileName.ToString().c_str(), ::std::fstream::in);

	SerializeXmlHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


// protected methods

// reimplemented (iser::CXmlReadArchiveBase)

bool CXmlFileReadArchive::ReadToDelimeter(
			const ::std::string& delimeters,
			::std::string& result,
			bool skipDelimeter,
			char* foundDelimeterPtr)
{
	int cutFromPos = -1;
	int cutToPos = -1;

	::std::string readString;

	if (!m_useLastReadChar){
		m_stream.get(m_lastReadChar);
	}

	while (!m_stream.fail()){
		::std::string::size_type foundPosition = delimeters.find(m_lastReadChar);
		if (foundPosition != ::std::string::npos){
			if (cutFromPos < 0){
				cutFromPos = 0;
			}

			if (cutToPos < 0){
				cutToPos = int(readString.size());
			}

			result = readString.substr(cutFromPos, cutToPos - cutFromPos);

			m_useLastReadChar = !skipDelimeter;

			if (foundDelimeterPtr != NULL){
				*foundDelimeterPtr = delimeters.at(foundPosition);
			}

			return true;
		}

		readString += m_lastReadChar;

		if (!isspace(m_lastReadChar) && !iscntrl(m_lastReadChar)){
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


