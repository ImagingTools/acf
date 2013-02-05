#include "i2d/CPosition2dComp.h"


namespace i2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CPosition2dComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	Q_ASSERT(m_xAttrPtr.IsValid());
	Q_ASSERT(m_yAttrPtr.IsValid());

	SetPosition(i2d::CVector2d(*m_xAttrPtr, *m_yAttrPtr));
}


} // namespace i2d


