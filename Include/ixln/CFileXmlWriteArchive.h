#ifndef ixln_CFileXmlWriteArchive_included
#define ixln_CFileXmlWriteArchive_included


#include "ixln/CXmlWriteArchiveBase.h"


namespace ixln
{


/**
	XERCES-based implementation of archive for writing in XML format.
	Please note that it doesn't create \c counter attribute needed by \c iser::CXmlFileReadArchive.

	\ingroup Persistence
*/
class CFileXmlWriteArchive: public CXmlWriteArchiveBase
{
public:
	typedef CXmlWriteArchiveBase BaseClass;

	explicit CFileXmlWriteArchive(
				const istd::CString& filePath,
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
};


} // namespace ixln


#endif // !ixln_CFileXmlWriteArchive_included


