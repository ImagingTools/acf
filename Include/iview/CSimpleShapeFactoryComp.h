#ifndef iview_CSimpleShapeFactoryComp_included
#define iview_CSimpleShapeFactoryComp_included


// ACF includes
#include "icomp/CComponentBase.h"
#include "iview/IShapeFactory.h"
#include "iview/IColorSchema.h"
#include "iview/CInteractiveShapeBase.h"


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
		I_ASSIGN(m_useInteractiveShapesAttrPtr, "UseInteractiveShapes", "If true, the created shapes will be editable", true, true);
		I_ASSIGN(m_colorSchemaCompPtr, "ShapeColorSchema", "Color schema used by displayed shape", false, "ShapeColorSchema");
	I_END_COMPONENT;

	// reimplemented (iview::IShapeFactory)
	virtual IShape* CreateShape(const i2d::IObject2d& object, bool connectToModel = false) const;

protected:
	virtual CInteractiveShapeBase* CreateShapeInstance(const i2d::IObject2d& object) const;

protected:
	I_ATTR(bool, m_useInteractiveShapesAttrPtr);
	I_REF(iview::IColorSchema, m_colorSchemaCompPtr);
};


} // namespace iview


#endif // !iview_CSimpleShapeFactoryComp_included


