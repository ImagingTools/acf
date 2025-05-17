#pragma once


// ACF includes
#include <istd/IPolymorphic.h>


namespace iview
{


/**
	Update state of edit mode buttons.
	Edit mode buttons controls the mode of interaction with objects.
	For example for polyline nodes: select, move, insert, remove.
	\sa iview::ISelectable::EditMode
*/
class IEditModeButtons : virtual public istd::IPolymorphic
{
public:
	/**
	* Called after edit mode got changed.
	*/
	virtual void UpdateEditModeButtons(int mode) = 0;
};


} // namespace iview
