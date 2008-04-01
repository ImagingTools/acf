#ifndef iser_CXmlFileWriteArchive_included
#define iser_CXmlFileWriteArchive_included


#include <fstream>

#include "iser/TXmlStreamWriteArchiveBase.h"


namespace iser
{


/**
	Simple implementation of archive for writing in XML format.
	This implementation doesn't use any parser.
*/
class CXmlFileWriteArchive: public TXmlStreamWriteArchiveBase<std::ofstream>
{
public:
	typedef TXmlStreamWriteArchiveBase<std::ofstream> BaseClass;

	CXmlFileWriteArchive(
				const istd::CString& fileName,
				const IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
};


} // namespace iser


#endif // !iser_CXmlFileWriteArchive_included


