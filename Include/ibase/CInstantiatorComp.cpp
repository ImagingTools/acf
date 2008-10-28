#include "ibase/CInstantiatorComp.h"


namespace ibase
{


// reimplemented (icomp::IComponent)

void CInstantiatorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int elementsCount = m_instancesAttrPtr.GetCount();
	for (int i = 0; i < elementsCount; ++i){
		m_instancesAttrPtr[i];
	}

	m_freeComponentCompPtr.IsValid();
}


} // namespace ibase


