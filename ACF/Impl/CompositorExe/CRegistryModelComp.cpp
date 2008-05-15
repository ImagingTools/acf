#include "CRegistryModelComp.h"


// public methods

// reimplemented (icomp::IComponent)

void CRegistryModelComp::OnComponentCreated()
{
	if (m_staticInfoCompPtr.IsValid()){
		SetComponentStaticInfo(m_staticInfoCompPtr.GetPtr());
	}
}

