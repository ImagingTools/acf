#ifndef ifile_IFilePersistenceInfo_included
#define ifile_IFilePersistenceInfo_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IPolymorphic.h>


namespace ifile
{


/**
	Provide some information about file loader state.

	\ingroup Persistence
*/
class IFilePersistenceInfo: virtual public istd::IPolymorphic
{
public:
	enum OperationType
	{
		OT_UNKNOWN,
		OT_LOAD,
		OT_SAVE
	};

	enum PathType
	{
		PT_COMPLETE,
		OT_FILENAME,
		OT_DIR
	};

	/**
		Returns the last opened file name.
	*/
	virtual QString GetLastFilePath(OperationType operationType = OT_UNKNOWN, PathType pathType = PT_COMPLETE) const = 0;
};


} // namespace ifile


#endif // !ifile_IFilePersistenceInfo_included


