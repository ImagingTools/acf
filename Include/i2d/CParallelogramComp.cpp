#include <i2d/CParallelogramComp.h>


namespace i2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CParallelogramComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultObjectProviderCompPtr.IsValid()){
		return;
	}

	Q_ASSERT(m_xAttrPtr.IsValid());
	Q_ASSERT(m_yAttrPtr.IsValid());
	Q_ASSERT(m_widthAttrPtr.IsValid());
	Q_ASSERT(m_heightAttrPtr.IsValid());

	SetTranslation(i2d::CVector2d(*m_xAttrPtr, *m_yAttrPtr));
	SetWidth(*m_widthAttrPtr);
	SetHeight(*m_heightAttrPtr);
}


} // namespace i2d


