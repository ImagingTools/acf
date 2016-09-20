#ifndef ibase_ICommandsProvider_included
#define ibase_ICommandsProvider_included


// ACF includes
#include <istd/IChangeable.h>
#include <ibase/ICommand.h>


namespace ibase
{

	
/**
	Interface for a provider of the heriarchical commands.
*/
class ICommandsProvider: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		CF_COMMANDS = 0x62b7e8
	};

	/**
		Get list of menu commands.
		These commands will be integrated in global menu system, independent from actual selected view.
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


