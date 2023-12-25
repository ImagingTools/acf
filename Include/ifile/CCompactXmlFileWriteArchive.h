#pragma once


// Qt includes
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtCore/QFile>

// ACF includes
#include <ifile/CFileArchiveInfo.h>
#include <iser/CCompactXmlWriteArchiveBase.h>


namespace ifile
{


/**
	Qt-based implementation of archive for writing in compact XML format.

	\ingroup Persistence
*/
class CCompactXmlFileWriteArchive:
			public iser::CCompactXmlWriteArchiveBase,
			public ifile::CFileArchiveInfo
{
public:
	typedef iser::CCompactXmlWriteArchiveBase BaseClass;
	typedef ifile::CFileArchiveInfo BaseClass2;

	/**
		Constructor initializing archive to open file immediatelly.
	*/
	CCompactXmlFileWriteArchive(
				const QString& filePath,
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
	~CCompactXmlFileWriteArchive();

	bool OpenFile(const QString& filePath);

	bool Flush();

protected:
	// reimplemented (iser::CTextWriteArchiveBase)
	virtual bool WriteTextNode(const QByteArray& text) override;

private:
	QFile m_file;
};


} // namespace ifile


