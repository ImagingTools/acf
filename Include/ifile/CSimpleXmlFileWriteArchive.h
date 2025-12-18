#pragma once


// Qt includes
#include <QtCore/QFile>

// ACF includes
#include <iser/CXmlWriteArchiveBase.h>
#include <ifile/CFileArchiveInfo.h>


namespace ifile
{


/**
	Simple implementation of archive for writing in XML format.
	This implementation doesn't use any external parser.
	It creates \c counter attribute indicating number of subtags for each mutli tag node.
	XML file in this format are needed by \c ifile::CSimpleXmlFileReadArchive.

	\ingroup Persistence
*/
class CSimpleXmlFileWriteArchive:
			public iser::CXmlWriteArchiveBase,
			public CFileArchiveInfo
{
public:
	typedef iser::CXmlWriteArchiveBase BaseClass;
	typedef CFileArchiveInfo BaseClass2;

	explicit CSimpleXmlFileWriteArchive(
				const QString& filePath,
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
	virtual ~CSimpleXmlFileWriteArchive();

protected:
	// reimplemented (iser::CXmlWriteArchiveBase)
	virtual bool WriteString(const QByteArray& value) override;

private:
	QTextStream m_stream;
	QFile m_file;
};


} // namespace ifile


