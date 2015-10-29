#ifndef ifile_CSimpleXmlFileReadArchive_included
#define ifile_CSimpleXmlFileReadArchive_included


// Qt includes
#include <QtCore/QFile>

// ACF includes
#include "iser/CXmlStreamReadArchiveBase.h"
#include "ifile/CFileArchiveInfo.h"


namespace ifile
{


/**
	Simple implementation of archive reading from XML file.
	This implementation doesn't use any external parser.
	It uses only single pass and is very fast, but it doesn't support of tags skipping and
	it needs \c counter attribute indicating number of subtags for each mutli tag node.
	XML file in this format can be created using \c ifile::CSimpleXmlFileWriteArchive.

	\ingroup Persistence
*/
class CSimpleXmlFileReadArchive:
			public iser::CXmlStreamReadArchiveBase,
			public CFileArchiveInfo
{
public:
	typedef iser::CXmlStreamReadArchiveBase BaseClass;
	typedef CFileArchiveInfo BaseClass2;

	explicit CSimpleXmlFileReadArchive(const QString& filePath, bool serializeHeader = true, const iser::CArchiveTag& rootTag = s_acfRootTag);
	virtual ~CSimpleXmlFileReadArchive();

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


} // namespace ifile


#endif // !ifile_CSimpleXmlFileReadArchive_included


