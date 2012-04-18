#ifndef iser_CXmlFileReadArchive_included
#define iser_CXmlFileReadArchive_included


// Qt includes
#include <QtCore/QFile>

// ACF includes
#include "iser/CXmlStreamReadArchiveBase.h"
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
			public CXmlStreamReadArchiveBase,
			public CFileArchiveInfo
{
public:
	typedef CXmlStreamReadArchiveBase BaseClass;
	typedef CFileArchiveInfo BaseClass2;

	explicit CXmlFileReadArchive(const QString& filePath, bool serializeHeader = true, const CArchiveTag& rootTag = s_acfRootTag);
	virtual ~CXmlFileReadArchive();

protected:
	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const;

private:
	QFile m_file;
};


} // namespace iser


#endif // !iser_CXmlFileReadArchive_included


