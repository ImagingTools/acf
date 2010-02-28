#ifndef iser_CXmlStringWriteArchive_included
#define iser_CXmlStringWriteArchive_included


// STL includes
#include <sstream>


// ACF includes
#include "iser/TXmlStreamWriteArchiveBase.h"


namespace iser
{


/**
	Simple implementation of archive for writing of XML given as a string.

	\ingroup Persistence
*/
class CXmlStringWriteArchive: public TXmlStreamWriteArchiveBase<std::ostringstream>
{
public:
	typedef TXmlStreamWriteArchiveBase<std::ostringstream> BaseClass;

	CXmlStringWriteArchive(
				const IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const CArchiveTag& rootTag = s_acfRootTag);


	virtual std::string GetString() const;
};


} // namespace iser


#endif // !iser_CXmlStringWriteArchive_included


