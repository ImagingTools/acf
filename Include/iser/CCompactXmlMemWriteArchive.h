#ifndef iser_CCompactXmlMemWriteArchive_included
#define iser_CCompactXmlMemWriteArchive_included


#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtCore/QFile>

// ACF includes
#include "iser/CCompactXmlWriteArchiveBase.h"


namespace iser
{


/**
	Qt-based implementation of archive for writing in XML format.
	Please note that it doesn't create \c counter attribute needed by \c ifile::CSimpleXmlFileReadArchive.

	\ingroup Persistence
*/
class CCompactXmlMemWriteArchive:
			public CCompactXmlWriteArchiveBase
{
public:
	typedef CCompactXmlWriteArchiveBase BaseClass;

	/**
		Constructor initializing archive to work with memory.
		Use \c GetString() to get the created XML buffer.
	*/
	CCompactXmlMemWriteArchive(
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

	/**
		Close the archive and get the created XML string.
	*/
	virtual QByteArray GetString() const;
};


} // namespace iser


#endif // !iser_CCompactXmlMemWriteArchive_included


