#ifndef iview_IMouseActionObserver_included
#define iview_IMouseActionObserver_included


// Qt includes
#include <qnamespace.h>


// ACF includes
#include <istd/IPolymorphic.h>

#include <iview/iview.h>


namespace iview
{


/**
	Common interface for all shapes which are mouse events receiver.
*/
class IMouseActionObserver: virtual public istd::IPolymorphic
{
public:
	/**
		It is called, when mouse button is pushed down or up on this object.
		\param	position	-	position of mouse pointer on client area (picture coords).
		\param	downFlag	-	true, if mouse button is pushed down, false if pushed up.
	 *		\return	true	if this shape should get focus to catch all mouse movements.
	*/
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) = 0;

	/**
		It is called, when mouse is moved.
		Only objects with focus receive this event.
		\param	position	-	position of mouse pointer on client area (picture coords).
		\return	true	if this shape should hold focus to catch longer all mouse movements.
	*/
	virtual bool OnMouseMove(istd::CIndex2d position) = 0;
};


} // namespace iview


#endif // !iview_IMouseActionObserver_included


