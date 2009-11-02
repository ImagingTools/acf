#ifndef ibase_IFolderMonitor_included
#define ibase_IFolderMonitor_included


#include "ibase/ibase.h"

#include "istd/IChangeable.h"
#include "istd/CString.h"



namespace ibase
{


/**
	Interface for a folder monitor.
	Observers of this interface are notified about all changes made in the monitored folder such as removing or adding files,
	changes in file contents or in file attributes.
*/
class IFolderMonitor: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		FilesAdded = 0x1000,
		FilesRemoved = 0x2000,
		FilesModified = 0x4000,
		FilesAttributeChanged = 0x8000
	};

	/**
		Get the list of changed files according to the given flags
		\sa ChangeFlags
	*/
	virtual istd::CStringList GetChangedFileItems(int changeFlags) const = 0;
};


} // namespace ibase


#endif // !ibase_IFolderMonitor_included
