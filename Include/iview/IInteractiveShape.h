#ifndef iview_IInteractiveShape_included
#define iview_IInteractiveShape_included


#include "iview/IShape.h"
#include "iview/IMouseActionObserver.h"


namespace iview
{


class IShapeView;


/**
	Common interface for all display console shapes which can interacts with user.
*/
class IInteractiveShape: virtual public IShape, virtual public IMouseActionObserver
{
public:
	/**
		Check, if this shape is selected.
	*/
	virtual bool IsSelected() const = 0;
	
	/**
		Inform that this object was selected or deselected.
		\param	selectFlag	if \c true - selection, else deselection.
	*/
	virtual void SetSelected(bool selectFlag = true) = 0;
};


} // namespace iview


#endif // !iview_IInteractiveShape_included


