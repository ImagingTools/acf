#ifndef ixln_CFileXmlReadArchive_included
#define ixln_CFileXmlReadArchive_included


#include "ixln/CXmlReadArchiveBase.h"


namespace ixln
{


/**
	XERCES-based implementation of archive for writing in XML format.
	Please note that it doesn't create \c counter attribute needed by \c iser::CXmlFileReadArchive.

	\ingroup Persistence
*/
class CFileXmlReadArchive: public CXmlReadArchiveBase
{
public:
	typedef CXmlReadArchiveBase BaseClass;

	/**	Construct archive for specified file.
	 */
	explicit CFileXmlReadArchive(const istd::CString& filePath, bool serializeHeader = true, const iser::CArchiveTag& rootTag = s_acfRootTag);
};


} // namespace ixln


#endif // !ixln_CFileXmlReadArchive_included


