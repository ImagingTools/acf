#ifndef iview_IShapeObserver_included
#define iview_IShapeObserver_included


#include "istd/IPolymorphic.h"


namespace iview
{


class IShape;


/**
	Common interface for all objects informed about shapes state changes.
	Each shape has a pointer to one IShapeObserver object.
*/
class IShapeObserver: virtual public istd::IPolymorphic
{
public:
	/**
		Invalidate a shape.
		When you call Update, all invalidated shapes will be repainted.
		\return	true, if it was possible to invalidate this shape.
	*/
	virtual void OnChangeShape(IShape* shapePtr) = 0;

	/**
		Disconnect shape object from view.
		This shape had to be connected using ConnectShape.
		\return	true, if it was possible to disconnect this shape.
	*/
	virtual bool DisconnectShape(IShape* shapePtr) = 0;
};


} // namespace iview


#endif // !iview_IShapeObserver_included


