#include <ibase/CInstantiatorComp.h>


namespace ibase
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CInstantiatorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int elementsCount = m_instancesAttrPtr.GetCount();
	for (int i = 0; i < elementsCount; ++i){
		m_instancesAttrPtr[i];
	}
}


} // namespace ibase


