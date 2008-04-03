#ifndef ixln_CFileXmlReadArchive_included
#define ixln_CFileXmlReadArchive_included


#include "ixln/CXmlReadArchiveBase.h"


namespace ixln
{


class CFileXmlReadArchive: public CXmlReadArchiveBase
{
public:
	typedef CXmlReadArchiveBase BaseClass;

	/**	Construct archive for specified file.
	 */
	explicit CFileXmlReadArchive(const std::string& fileName, bool serializeHeader = true);

private:
	std::wstring m_fileName;
};


} // namespace ixln


#endif // !ixln_CFileXmlReadArchive_included


