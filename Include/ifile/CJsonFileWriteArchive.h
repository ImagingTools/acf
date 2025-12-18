#pragma once


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QBuffer>

// ACF includes
#include <iser/CJsonWriteArchiveBase.h>


namespace ifile
{


/**
	Implementation of an ACF Archive serializing to JSON string
*/
class CJsonFileWriteArchive: public iser::CJsonWriteArchiveBase
{
public:
	typedef iser::CJsonWriteArchiveBase BaseClass;

	CJsonFileWriteArchive(
				const QString& filePath,
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
	~CJsonFileWriteArchive();

	bool OpenFile(const QString& filePath);

private:
	QFile m_file;
};


} // namespace ifile


