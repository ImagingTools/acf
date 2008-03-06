#ifndef iser_CXmlFileReadArchive_included
#define iser_CXmlFileReadArchive_included


#include <fstream>

#include "iser/CXmlReadArchiveBase.h"


namespace iser
{


class CXmlFileReadArchive: public CXmlReadArchiveBase
{
public:
	typedef CXmlReadArchiveBase BaseClass;

	CXmlFileReadArchive(const istd::CString& fileName, bool serializeHeader = true, const iser::CArchiveTag& rootTag = s_acfRootTag);

protected:
	// reimplemented (iser::CXmlReadArchiveBase)
	virtual bool ReadToDelimeter(
				const ::std::string& delimeters,
				::std::string& result,
				bool skipDelimeter = true,
				char* foundDelimeterPtr = NULL);

private:
	::std::ifstream m_stream;
	char m_lastReadChar;
	bool m_useLastReadChar;
};


} // namespace iser


#endif // !iser_CXmlFileReadArchive_included


