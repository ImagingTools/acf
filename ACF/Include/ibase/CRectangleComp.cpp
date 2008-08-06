#include "ibase/CRectangleComp.h"


namespace ibase
{		

	
// reimplemented (icomp::IComponent)

void CRectangleComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_leftAttrPtr.IsValid());
	I_ASSERT(m_topAttrPtr.IsValid());
	I_ASSERT(m_widthAttrPtr.IsValid());
	I_ASSERT(m_heightAttrPtr.IsValid());

	SetLeft(*m_leftAttrPtr);
	SetTop(*m_topAttrPtr);
	SetRight(*m_leftAttrPtr + *m_widthAttrPtr);
	SetBottom(*m_topAttrPtr + *m_heightAttrPtr);
}


} // namespace ibase


