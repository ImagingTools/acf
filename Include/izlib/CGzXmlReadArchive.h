#ifndef zlib_CGzXmlReadArchive_included
#define zlib_CGzXmlReadArchive_included


// ZLIB includes
#include "zlib.h"

// ACF includes
#include "istd/CString.h"
#include "iser/CXmlReadArchiveBase.h"


namespace izlib
{


/**
	ZLIB compressed implementation of simple archive reading from XML file.
	This imlementation doesn't use any external parser, it uses ZLIB library for file decompressing.
	It uses only single pass and is very fast, but it doesn't support of tags skipping and
	it needs \c counter attribute indicating number of subtags for each mutli tag node.
	Compressed XML file in this format can be created using \c izlib::CGzXmlWriteArchive.

	\ingroup Persistence
*/
class CGzXmlReadArchive: public iser::CXmlReadArchiveBase 
{
public:
	typedef iser::CXmlReadArchiveBase BaseClass;

	CGzXmlReadArchive(const istd::CString& filePath, bool serializeHeader = true, const iser::CArchiveTag& rootTag = s_acfRootTag);
	virtual ~CGzXmlReadArchive();

	// reimplemented (iser::CXmlReadArchiveBase)
	virtual bool ReadToDelimeter(
				const std::string& delimeters,
				std::string& result,
				bool skipDelimeter = true,
				char* foundDelimeterPtr = NULL);

private:
	gzFile m_file;

	int m_lastReadChar;
	bool m_useLastReadChar;
};


} // namespace izlib


#endif // !zlib_CGzXmlReadArchive_included


