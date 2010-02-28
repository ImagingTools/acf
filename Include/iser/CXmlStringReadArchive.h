#ifndef iser_CXmlStringReadArchive_included
#define iser_CXmlStringReadArchive_included


// STL includes
#include <sstream>


// ACF includes
#include "iser/TXmlStreamReadArchiveBase.h"


namespace iser
{


/**
	Simple implementation for reading of a XML from a string.
	This imlementation uses only single pass and is very fast,
	but it needs \c counter attribute idicating number of subtags for each mutli tag node.

	\ingroup Persistence
*/
class CXmlStringReadArchive: public TXmlStreamReadArchiveBase<std::istringstream> 
{
public:
	typedef TXmlStreamReadArchiveBase<std::istringstream> BaseClass;

	CXmlStringReadArchive(const std::string& inputString, bool serializeHeader = true, const CArchiveTag& rootTag = s_acfRootTag);
};


} // namespace iser


#endif // !iser_CXmlStringReadArchive_included

