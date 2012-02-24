#ifndef iser_IFileArchiveInfo_included
#define iser_IFileArchiveInfo_included


// Qt includes
#include <QString>


// ACF includes
#include "istd/IPolymorphic.h"


namespace iser
{


/**
	Simple informative interface allowing to get access to current processed file by file archives.
	To check the file name from you \c Serialize methods simply cast your iser::IArchive to this interface.
	File-based archives should implement this interface.
*/
class IFileArchiveInfo: virtual public istd::IPolymorphic
{
public:
	virtual const QString& GetCurrentFilePath() const = 0;
};


} // namespace iser


#endif // !iser_IFileArchiveInfo_included


