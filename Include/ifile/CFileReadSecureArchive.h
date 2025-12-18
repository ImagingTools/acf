// ACF includes
#include <ifile/CFileReadArchive.h>

// QSF includes
#include <ifile/CSimpleEncoder.h>


namespace ifile
{


class CFileReadSecureArchive:
			public ifile::CFileReadArchive,
			protected CSimpleEncoder
{
public:
	typedef ifile::CFileReadArchive BaseClass;

	explicit CFileReadSecureArchive(
				const QString& filePath,
				bool supportTagSkipping = true,
				bool serializeHeader = true);

	// reimplemented (ifile::CFileReadArchive)
	virtual bool ProcessData(void* data, int size) override;
};


} // namespace ifile


