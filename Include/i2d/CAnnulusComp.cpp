#include "i2d/CAnnulusComp.h"


namespace i2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CAnnulusComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	Q_ASSERT(m_centerXAttrPtr.IsValid());
	Q_ASSERT(m_centerYAttrPtr.IsValid());
	Q_ASSERT(m_innerRadiusAttrPtr.IsValid());
	Q_ASSERT(m_outerRadiusAttrPtr.IsValid());

	SetPosition(i2d::CVector2d(*m_centerXAttrPtr, *m_centerYAttrPtr));
	SetOuterRadius(*m_outerRadiusAttrPtr);
	SetInnerRadius(*m_innerRadiusAttrPtr);
}


} // namespace i2d


