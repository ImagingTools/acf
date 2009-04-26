#ifndef iser_CXmlFileWriteArchive_included
#define iser_CXmlFileWriteArchive_included


// STL includes
#include <fstream>

#include "iser/TXmlStreamWriteArchiveBase.h"


namespace iser
{


/**
	Simple implementation of archive for writing in XML format.
	This implementation doesn't use any external parser.
	It creates \c counter attribute indicating number of subtags for each mutli tag node.
	XML file in this format are needed by \c iser::CXmlFileReadArchive.

	\ingroup Persistence
*/
class CXmlFileWriteArchive: public TXmlStreamWriteArchiveBase<std::ofstream>
{
public:
	typedef TXmlStreamWriteArchiveBase<std::ofstream> BaseClass;

	explicit CXmlFileWriteArchive(
				const istd::CString& filePath,
				const IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const CArchiveTag& rootTag = s_acfRootTag);
};


} // namespace iser


#endif // !iser_CXmlFileWriteArchive_included


