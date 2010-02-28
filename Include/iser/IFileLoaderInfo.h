#ifndef iser_IFileLoaderInfo_included
#define iser_IFileLoaderInfo_included


// ACF includes
#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "iser/iser.h"


namespace iser
{


/**
	Provide some information about file loader state.

	\ingroup Persistence
*/
class IFileLoaderInfo: virtual public istd::IPolymorphic
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
	virtual istd::CString GetLastFilePath(OperationType operationType = OT_UNKNOWN, PathType pathType = PT_COMPLETE) const = 0;
};


} // namespace iser


#endif // !iser_IFileLoaderInfo_included


