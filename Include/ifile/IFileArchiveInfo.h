#ifndef ifile_IFileArchiveInfo_included
#define ifile_IFileArchiveInfo_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IPolymorphic.h>


namespace ifile
{


/**
	Simple informative interface allowing to get access to current processed file by file archives.
	To check the file name from you \c Serialize methods simply cast your iser::IArchive to this interface.
	File-based archives should implement this interface.

	\ingroup Persistence
*/
class IFileArchiveInfo: virtual public istd::IPolymorphic
{
public:
	virtual const QString& GetCurrentFilePath() const = 0;
};


} // namespace ifile


#endif // !ifile_IFileArchiveInfo_included


