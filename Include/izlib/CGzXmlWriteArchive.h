#ifndef zlib_CGzXmlWriteArchive_included
#define zlib_CGzXmlWriteArchive_included


// ZLIB includes
#include "zlib.h"

// STL includes
#include <fstream>

// ACF includes
#include "istd/CString.h"
#include "iser/CXmlWriteArchiveBase.h"


namespace izlib
{


/**
	ZLIB compressed implementation of archive for writing in XML format.
	This implementation doesn't use any external parser, it uses ZLIB library for file compressing.
	It creates \c counter attribute indicating number of subtags for each mutli tag node.
	Compressed XML file in this format are needed by \c izlib::CGzXmlReadArchive.

	\ingroup Persistence
*/
class CGzXmlWriteArchive: public iser::CXmlWriteArchiveBase 
{
public:
	typedef iser::CXmlWriteArchiveBase BaseClass;

	CGzXmlWriteArchive(
				const istd::CString& filePath,
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
	virtual ~CGzXmlWriteArchive();

protected:
	enum{
		CHUNK_SIZE = 16384
	};

	// reimplemented (iser::CXmlWriteArchiveBase)
	virtual bool WriteString(const std::string& value);

private:
	gzFile m_file;
};


} // namespace izlib


#endif // !zlib_CGzXmlWriteArchive_included


