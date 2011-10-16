#ifndef i2d_CAnnulusSegmentComp_included
#define i2d_CAnnulusSegmentComp_included


#include "i2d/CAnnulusSegment.h"

#include "icomp/CComponentBase.h"


namespace i2d
{		


/**
	Implementation of a annulus as a component.
	It gives the possibility to define a annulus segment model via component attributes.
*/
class CAnnulusSegmentComp: public icomp::CComponentBase, public i2d::CAnnulusSegment
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CAnnulusSegmentComp);
		I_REGISTER_INTERFACE(IObject2d);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_centerXAttrPtr, "X", "X-Position of the annulus center", true, 0);
		I_ASSIGN(m_centerYAttrPtr, "Y", "X-Position of the annulus center", true, 0);
		I_ASSIGN(m_innerRadiusAttrPtr, "InnerRadius", "Inner radius of the annulus", true, 0);
		I_ASSIGN(m_outerRadiusAttrPtr, "OuterRadius", "Outer radius of the annulus", true, 0);
		I_ASSIGN(m_beginAngleAttrPtr, "BeginAngle", "Begin angle (in degree) of the annulus segment", true, 0);
		I_ASSIGN(m_endAngleAttrPtr, "EndAngle", "End angle (in degree) of the annulus segement", true, 360);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(double, m_centerXAttrPtr);
	I_ATTR(double, m_centerYAttrPtr);
	I_ATTR(double, m_innerRadiusAttrPtr);
	I_ATTR(double, m_outerRadiusAttrPtr);
	I_ATTR(double, m_beginAngleAttrPtr);
	I_ATTR(double, m_endAngleAttrPtr);
};


} // namespace i2d


#endif // !i2d_CAnnulusSegmentComp_included


