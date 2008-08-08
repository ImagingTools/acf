#include "icomp/CCompositeComponentContext.h"


#include "icomp/IRegistry.h"
#include "icomp/IRegistriesManager.h"


namespace icomp
{


CCompositeComponentContext::CCompositeComponentContext(
			const IRegistryElement* elementPtr,
			const IRegistry* registryPtr,
			const IRegistriesManager* registriesManagerPtr,
			const CCompositeComponentContext* parentPtr)
:	BaseClass(elementPtr, parentPtr),
	m_registry(*registryPtr),
	m_registriesManager(*registriesManagerPtr)
{
	I_ASSERT(registryPtr != NULL);
}


}//namespace icomp


