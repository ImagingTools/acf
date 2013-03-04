#ifndef iview_IMoveable_included
#define iview_IMoveable_included


#include "istd/IPolymorphic.h"

#include "i2d/CVector2d.h"


namespace iview
{


/**
	Common interface for all, you can drag.
*/
class IDraggable: virtual public istd::IPolymorphic
{
public:
	/** 
		Called before dragging is begin.
		It set also reference value for dragging position.
	*/
	virtual void BeginDrag(const istd::CIndex2d& reference) = 0;

	/**
		Set new drag position.
		It must be enclosed using BeginDrag() and EndDrag() methods.
	*/
	virtual void SetDragPosition(const istd::CIndex2d& position) = 0;
	
	/**
		Called after dragging.
	*/
	virtual void EndDrag() = 0;
	
	/**
		Check if drag is enabled.
	*/
	virtual bool IsDraggable() const = 0;
};


} // namespace iview


#endif // !iview_IMoveable_included


