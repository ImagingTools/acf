#ifndef idoc_ICommandsProvider_included
#define idoc_ICommandsProvider_included


#include "istd/IPolymorphic.h"

#include "idoc/ICommand.h"


namespace idoc
{


class ICommandsProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Get list of menu commands.
		This commands will be integrated in global menu system undependent of actual selected view.
		For support of normal pull down menu, depth of this tree structure should be at least 3.
	*/
	virtual const idoc::IHierarchicalCommand* GetCommands() const = 0;
};


} // namespace idoc


#endif // !idoc_ICommandsProvider_included


