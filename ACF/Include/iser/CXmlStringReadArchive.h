#ifndef iser_CXmlStringReadArchive_included
#define iser_CXmlStringReadArchive_included


#include <sstream>

#include "iser/TXmlStreamReadArchiveBase.h"


namespace iser
{


/**
	Simple implementation for reading of a XML from a string.
*/
class CXmlStringReadArchive: public TXmlStreamReadArchiveBase<std::istringstream> 
{
public:
	typedef TXmlStreamReadArchiveBase<std::istringstream> BaseClass;

	CXmlStringReadArchive(std::string& outputString, bool serializeHeader = true, const iser::CArchiveTag& rootTag = s_acfRootTag);
};


} // namespace iser


#endif // !iser_CXmlStringReadArchive_included

