#ifndef iser_CXmlFileReadArchive_included
#define iser_CXmlFileReadArchive_included


#include <fstream>

#include "iser/TXmlStreamReadArchiveBase.h"


namespace iser
{


class CXmlFileReadArchive: public TXmlStreamReadArchiveBase<std::ifstream> 
{
public:
	typedef TXmlStreamReadArchiveBase<std::ifstream> BaseClass;

	CXmlFileReadArchive(const istd::CString& fileName, bool serializeHeader = true, const iser::CArchiveTag& rootTag = s_acfRootTag);
};


} // namespace iser


#endif // !iser_CXmlFileReadArchive_included


