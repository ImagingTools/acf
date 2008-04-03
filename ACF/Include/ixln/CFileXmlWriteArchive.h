#ifndef ixln_CFileXmlWriteArchive_included
#define ixln_CFileXmlWriteArchive_included


#include "ixln/CXmlWriteArchiveBase.h"


namespace ixln
{


class CFileXmlWriteArchive: public CXmlWriteArchiveBase
{
public:
	typedef CXmlWriteArchiveBase BaseClass;

	explicit CFileXmlWriteArchive(
					const std::string& fileName,
					const iser::IVersionInfo* versionInfoPtr = NULL,
					bool serializeHeader = true);
private:
	std::wstring m_fileName;
};


} // namespace ixln


#endif // !ixln_CFileXmlWriteArchive_included


