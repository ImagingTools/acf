#ifndef iview_IShapeFactory_included
#define iview_IShapeFactory_included


// ACF includes
#include "i2d/IObject2d.h"

#include "iview/IShape.h"


namespace iview
{


/**
	Interface for creation of visualization shapes of some geometrical object.
*/
class IShapeFactory: virtual public istd::IPolymorphic
{
public:
	/**
		Create a graphical representation of the given 2D-object \c object.
		If \connectModel is enabled, the created shape will be connected to the object's data model.
		If connection has failed, then no shape will be returned.
		The method returns \c NULL if the shape could not be created.
		\param	objectPtr		Object for which the shape will be created.
								If it is NULL, this will be anonymous.
								It is declared as \c const, but for attachement to model this \c const will be removed.
		\param	connectToModel	If this flag is enabled, the shape will be attached to model of object using
								observer/observable mechanism.
	*/
	virtual IShape* CreateShape(const i2d::IObject2d* objectPtr, bool connectToModel = false) const = 0;
};


} // namespace iview


#endif // !iview_IShapeFactory_included


