#ifndef ibase_ICommandsProvider_included
#define ibase_ICommandsProvider_included


// ACF includes
#include "istd/IPolymorphic.h"

#include "ibase/ICommand.h"


namespace ibase
{


class ICommandsProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Get list of menu commands.
		This commands will be integrated in global menu system undependent of actual selected view.
		For support of normal pull down menu, depth of this tree structure should be at least 3.
	*/
	virtual const IHierarchicalCommand* GetCommands() const = 0;
};


} // namespace ibase


#endif // !ibase_ICommandsProvider_included


