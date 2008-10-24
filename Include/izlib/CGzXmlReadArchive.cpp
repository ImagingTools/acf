#include "CGzXmlReadArchive.h"


namespace izlib
{


CGzXmlReadArchive::CGzXmlReadArchive(const istd::CString& filePath, bool serializeHeader, const iser::CArchiveTag& rootTag)
:	BaseClass(rootTag),
	m_useLastReadChar(false)
{
	m_file = gzopen(filePath.ToString().c_str(), "rb");

	SerializeXmlHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


CGzXmlReadArchive::~CGzXmlReadArchive()
{
	if (m_file != NULL){
		gzclose(m_file);
	}
}


// reimplemented (iser::CXmlReadArchiveBase)

bool CGzXmlReadArchive::ReadToDelimeter(
			const std::string& delimeters,
			std::string& result,
			bool skipDelimeter,
			char* foundDelimeterPtr)
{
	if (m_file == NULL){
		return false;
	}

	int cutFromPos = -2;
	int cutToPos = -2;

	std::string readString;

	if (!m_useLastReadChar){
		m_lastReadChar = gzgetc(m_file);
	}

	while (m_lastReadChar >= 0){
		std::string::size_type foundPosition = delimeters.find(char(m_lastReadChar));
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

		readString += char(m_lastReadChar);

		if (!isspace((unsigned char)m_lastReadChar) && !iscntrl((unsigned char)m_lastReadChar)){
			cutToPos = int(readString.size());

			if (cutFromPos < 0){
				cutFromPos = cutToPos - 1;
			}
		}

		m_lastReadChar = gzgetc(m_file);
	}

	return false;
}


} // namespace izlib


