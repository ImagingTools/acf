#include "i2d/CAnnulusSegmentComp.h"


namespace i2d
{		

// protected methods

// reimplemented (icomp::CComponentBase)

void CAnnulusSegmentComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_centerXAttrPtr.IsValid());
	I_ASSERT(m_centerYAttrPtr.IsValid());
	I_ASSERT(m_innerRadiusAttrPtr.IsValid());
	I_ASSERT(m_outerRadiusAttrPtr.IsValid());
	I_ASSERT(m_beginAngleAttrPtr.IsValid());
	I_ASSERT(m_endAngleAttrPtr.IsValid());


	SetPosition(i2d::CVector2d(*m_centerXAttrPtr, *m_centerXAttrPtr));
	SetOuterRadius(*m_outerRadiusAttrPtr);
	SetInnerRadius(*m_innerRadiusAttrPtr);
	SetBeginAngle(imath::GetRadianFromDegree(*m_beginAngleAttrPtr));
	SetEndAngle(imath::GetRadianFromDegree(*m_endAngleAttrPtr));
}


} // namespace i2d


