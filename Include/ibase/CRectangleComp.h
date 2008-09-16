#ifndef ibase_CRectangleComp_included
#define ibase_CRectangleComp_included


#include "i2d/CRectangle.h"

#include "icomp/CComponentBase.h"


namespace ibase
{		


class CRectangleComp: public icomp::CComponentBase, public i2d::CRectangle
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CRectangleComp);
		I_REGISTER_INTERFACE(IObject2d);
		I_REGISTER_INTERFACE(IObject2d);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_leftAttrPtr, "X", "X-Position of the rectangle", true, 0);
		I_ASSIGN(m_topAttrPtr, "Y", "X-Position of the rectangle", true, 0);
		I_ASSIGN(m_widthAttrPtr, "Width", "Width of the rectangle", true, 0);
		I_ASSIGN(m_heightAttrPtr, "Height", "XHeight of the rectangle", true, 0);
	I_END_COMPONENT;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_ATTR(double, m_leftAttrPtr);
	I_ATTR(double, m_topAttrPtr);
	I_ATTR(double, m_widthAttrPtr);
	I_ATTR(double, m_heightAttrPtr);
};


} // namespace ibase


#endif // !ibase_CRectangleComp_included