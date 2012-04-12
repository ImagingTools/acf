#ifndef iser_CXmlFileWriteArchive_included
#define iser_CXmlFileWriteArchive_included


// Qt includes
#include <QtCore/QFile>

// ACF includes
#include "iser/CXmlWriteArchiveBase.h"
#include "iser/CFileArchiveInfo.h"


namespace iser
{


/**
	Simple implementation of archive for writing in XML format.
	This implementation doesn't use any external parser.
	It creates \c counter attribute indicating number of subtags for each mutli tag node.
	XML file in this format are needed by \c iser::CXmlFileReadArchive.

	\ingroup Persistence
*/
class CXmlFileWriteArchive:
			public CXmlWriteArchiveBase,
			public CFileArchiveInfo
{
public:
	typedef CXmlWriteArchiveBase BaseClass;
	typedef CFileArchiveInfo BaseClass2;

	explicit CXmlFileWriteArchive(
				const QString& filePath,
				const IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const CArchiveTag& rootTag = s_acfRootTag);
	virtual ~CXmlFileWriteArchive();

protected:
	// reimplemented (iser::CXmlWriteArchiveBase)
	virtual bool WriteString(const QByteArray& value);

private:
	QTextStream m_stream;
	QFile m_file;
};


} // namespace iser


#endif // !iser_CXmlFileWriteArchive_included


