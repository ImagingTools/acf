#include "i2d/CPolygonComp.h"


namespace i2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CPolygonComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	Q_ASSERT(m_xAttrPtr.IsValid());
	Q_ASSERT(m_yAttrPtr.IsValid());

	int count = qMin(m_xAttrPtr.GetCount(), m_yAttrPtr.GetCount());
	for (int i = 0; i < count; i++){
		BaseClass2::InsertNode(i2d::CVector2d(m_xAttrPtr[i], m_yAttrPtr[i]));
	}
}


} // namespace i2d
