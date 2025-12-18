#pragma once


// ACF includes
#include <ifile/CFileWriteArchive.h>

// QSF includes
#include <ifile/CSimpleEncoder.h>


namespace ifile
{


class CFileWriteSecureArchive:
			public ifile::CFileWriteArchive,
			protected ifile::CSimpleEncoder
{
public:
	typedef ifile::CFileWriteArchive BaseClass;

	explicit CFileWriteSecureArchive(
				const QString& filePath,
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool supportTagSkipping = true,
				bool serializeHeader = true);

	// reimplemented (ifile::CFileWriteArchive)
	virtual bool ProcessData(void* data, int size) override;
};


} // namespace ifile


