#ifndef iview_CSimpleShapeFactoryComp_included
#define iview_CSimpleShapeFactoryComp_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "iview/IShapeFactory.h"


namespace iview
{


/**
	Simple shape factory for common 2D-objects
*/
class CSimpleShapeFactoryComp:
			public icomp::CComponentBase,
			virtual public IShapeFactory
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSimpleShapeFactoryComp);
		I_REGISTER_INTERFACE(IShapeFactory);
		I_ASSIGN(m_createEditableAttrPtr, "ShapesEditable", "If true, the created shapes will be editable", true, true);
	I_END_COMPONENT;

	// reimplemented (IShapeFactory)
	virtual IShape* CreateShape(const i2d::IObject2d& object, bool connectToModel = false) const;

protected:
	IShape* CreateShapeInstance(const i2d::IObject2d& object) const;

private:
	I_ATTR(bool, m_createEditableAttrPtr);
};


} // namespace iview


#endif // !iview_CSimpleShapeFactoryComp_included


