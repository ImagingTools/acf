#ifndef ibase_ICommandsProvider_included
#define ibase_ICommandsProvider_included


// ACF includes
#include "istd/IChangeable.h"

#include "ibase/ICommand.h"


namespace ibase
{


class ICommandsProvider: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		CF_COMMANDS = 1 << 14
	};

	/**
		Get list of menu commands.
		This commands will be integrated in global menu system undependent of actual selected view.
		For support of normal pull down menu, depth of this tree structure should be at least 3.
	*/
	virtual const IHierarchicalCommand* GetCommands() const;
};


inline const IHierarchicalCommand* ICommandsProvider::GetCommands() const
{
	return NULL;
}


} // namespace ibase


#endif // !ibase_ICommandsProvider_included


