#include "i2d/CAnnulusSegmentComp.h"


namespace i2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CAnnulusSegmentComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	Q_ASSERT(m_centerXAttrPtr.IsValid());
	Q_ASSERT(m_centerYAttrPtr.IsValid());
	Q_ASSERT(m_innerRadiusAttrPtr.IsValid());
	Q_ASSERT(m_outerRadiusAttrPtr.IsValid());
	Q_ASSERT(m_beginAngleAttrPtr.IsValid());
	Q_ASSERT(m_endAngleAttrPtr.IsValid());


	SetPosition(i2d::CVector2d(*m_centerXAttrPtr, *m_centerXAttrPtr));
	SetOuterRadius(*m_outerRadiusAttrPtr);
	SetInnerRadius(*m_innerRadiusAttrPtr);
	SetBeginAngle(imath::GetRadianFromDegree(*m_beginAngleAttrPtr));
	SetEndAngle(imath::GetRadianFromDegree(*m_endAngleAttrPtr));
}


} // namespace i2d


