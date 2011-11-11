#include "i2d/CAnnulusComp.h"


namespace i2d
{		


// protected methods

// reimplemented (icomp::CComponentBase)

void CAnnulusComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_centerXAttrPtr.IsValid());
	I_ASSERT(m_centerYAttrPtr.IsValid());
	I_ASSERT(m_innerRadiusAttrPtr.IsValid());
	I_ASSERT(m_outerRadiusAttrPtr.IsValid());

	SetPosition(i2d::CVector2d(*m_centerXAttrPtr, *m_centerYAttrPtr));
	SetOuterRadius(*m_outerRadiusAttrPtr);
	SetInnerRadius(*m_innerRadiusAttrPtr);
}


} // namespace i2d


