#include <i2d/CRectangleComp.h>


namespace i2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CRectangleComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	Q_ASSERT(m_leftAttrPtr.IsValid());
	Q_ASSERT(m_topAttrPtr.IsValid());
	Q_ASSERT(m_widthAttrPtr.IsValid());
	Q_ASSERT(m_heightAttrPtr.IsValid());

	SetLeft(*m_leftAttrPtr);
	SetTop(*m_topAttrPtr);
	SetRight(*m_leftAttrPtr + *m_widthAttrPtr);
	SetBottom(*m_topAttrPtr + *m_heightAttrPtr);
}


} // namespace i2d


