#pragma once


// ACF includes
#include <istd/IPolymorphic.h>
#include <ibase/ICommandsProvider.h>


namespace ibase
{

	
/**
	Interface for a displayer of the hierarchical commands.
*/
class ICommandsDisplayer : virtual public istd::IPolymorphic
{
public:
	virtual void ShowCommands(const ibase::ICommandsProvider* commandsProvider) = 0;
};


} // namespace ibase




