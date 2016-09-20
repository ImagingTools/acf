#ifndef ifile_CCompressedXmlFileWriteArchive_included
#define ifile_CCompressedXmlFileWriteArchive_included


#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtCore/QFile>
#include <QtCore/QBuffer>

// ACF includes
#include <ifile/CFileArchiveInfo.h>
#include <iser/CCompactXmlWriteArchiveBase.h>


namespace ifile
{


/**
	Qt-based implementation of archive for writing in compact XML format.

	\ingroup Persistence
*/
class CCompressedXmlFileWriteArchive:
			public iser::CCompactXmlWriteArchiveBase,
			public ifile::CFileArchiveInfo
{
public:
	typedef iser::CCompactXmlWriteArchiveBase BaseClass;
	typedef ifile::CFileArchiveInfo BaseClass2;

	/**
		Constructor initializing archive to open file immediatelly.
	*/
	CCompressedXmlFileWriteArchive(
				const QString& filePath,
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
	~CCompressedXmlFileWriteArchive();

	bool OpenFile(const QString& filePath);

	bool Flush();

private:
	QBuffer m_buffer;
	QFile m_file;
};


} // namespace ifile


#endif // !ifile_CCompressedXmlFileWriteArchive_included


