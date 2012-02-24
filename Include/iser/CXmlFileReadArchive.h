#ifndef iser_CXmlFileReadArchive_included
#define iser_CXmlFileReadArchive_included


// STL includes
#include <fstream>


// ACF includes
#include "iser/TXmlStreamReadArchiveBase.h"
#include "iser/CFileArchiveInfo.h"


namespace iser
{


/**
	Simple implementation of archive reading from XML file.
	This imlementation doesn't use any external parser.
	It uses only single pass and is very fast, but it doesn't support of tags skipping and
	it needs \c counter attribute indicating number of subtags for each mutli tag node.
	XML file in this format can be created using \c iser::CXmlFileWriteArchive.

	\ingroup Persistence
*/
class CXmlFileReadArchive:
			public TXmlStreamReadArchiveBase<std::ifstream>,
			public CFileArchiveInfo
{
public:
	typedef TXmlStreamReadArchiveBase<std::ifstream> BaseClass;
	typedef CFileArchiveInfo BaseClass2;

	explicit CXmlFileReadArchive(const QString& filePath, bool serializeHeader = true, const CArchiveTag& rootTag = s_acfRootTag);

protected:
	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				MessageCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const;
};


} // namespace iser


#endif // !iser_CXmlFileReadArchive_included


