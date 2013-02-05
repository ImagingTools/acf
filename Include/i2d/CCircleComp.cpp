#include "i2d/CCircleComp.h"


namespace i2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CCircleComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	Q_ASSERT(m_centerXAttrPtr.IsValid());
	Q_ASSERT(m_centerYAttrPtr.IsValid());
	Q_ASSERT(m_radiusAttrPtr.IsValid());

	SetPosition(i2d::CVector2d(*m_centerXAttrPtr, *m_centerXAttrPtr));
	SetRadius(*m_radiusAttrPtr);
}


} // namespace i2d


