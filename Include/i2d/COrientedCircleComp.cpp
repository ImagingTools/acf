#include "i2d/COrientedCircleComp.h"


namespace i2d
{		


// protected methods

// reimplemented (icomp::CComponentBase)

void COrientedCircleComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_centerXAttrPtr.IsValid());
	I_ASSERT(m_centerYAttrPtr.IsValid());
	I_ASSERT(m_radiusAttrPtr.IsValid());

	SetPosition(i2d::CVector2d(*m_centerXAttrPtr, *m_centerXAttrPtr));
	SetRadius(*m_radiusAttrPtr);
	SetOrientedOutside(*m_isOrientedOutsideAttrPtr);
}


} // namespace i2d


